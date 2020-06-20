#include "PeriodicalTrendFuzzyModel.h"


fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::PeriodicalTrendFuzzyModel(size_t nPeriod)
    : m_nFirstFullPeriodItem(0),
      m_pTerms(nullptr)
{
    SetPeriod(nPeriod);
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::AutoGenerateRules()
{
    for(size_t i = 0; i < m_nPeriod;i++)
    {
        AutoGenerateRules(i);
    }
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetRulesCount()
{
    size_t rules = 0;
    for(size_t i = 0; i < m_models.size(); i++)
    {
        rules += m_models[i]->GetRulesCount();
    }
    return rules;
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::SetTerms(fuzzyPrognosisMath::LinguisticTerms *pTerms)
{
    m_pTerms = pTerms;
    for(auto model : m_models)
    {
        model->SetTerms(m_pTerms);
    }
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::AddRealDataItem(double item)
{
    m_realTrend.push_back(item);
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetRealTrendLength() const
{
    return m_realTrend.size();
}

fuzzyPrognosisMath::PeriodDependencies& fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::Dependency(size_t i)
{
    return m_dependencies.at(i);
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::AutoGenerateRules(size_t nPeriodItem)
{
    m_models[nPeriodItem]->DeleteAllRules();
    size_t nVarCount = m_dependencies[nPeriodItem].GetVarCount();
    for(size_t i = m_dependencies[nPeriodItem].GetMaxOffset(); i < m_realTrend.size();i++)
    {
        if((i % m_nPeriod) != nPeriodItem)
        {
            return;
        }

        FuzzyRule* pNewRule = new FuzzyRule(nVarCount, m_models[nPeriodItem]->GetTermFor(GetRealDataItem(i)));
        for(size_t var = 0; var < nVarCount; var++)
        {
            size_t term = m_models[nPeriodItem]->GetTermFor(GetRealDataItem(i - GetPeriodItemVarOffset(nPeriodItem,var)));
            pNewRule->SetVarTerm(var,term);
        }

        m_models[nPeriodItem]->AddRule(pNewRule);
    }
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetParam(size_t param)
{
    return GetParamater(param)->GetValue();
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetParamsCount()
{
    return m_pTerms->GetTermsCount() * 2;
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::Tune(fuzzyPrognosisMath::RAlgParams params)
{

    Precalc();

    int K = 0;
    int N = GetParamsCount();
    double* G = new double[N];
    double* G1 = new double[N];
    double* G2 = new double[N];
    double* Z = new double[N];
    double* HH = new double[N*N];

    double H1 = params.II * params.H;
    double SX1 = params.SX * params.SX;
    double SG1 = params.SG * params.SG;
    double W = 1 / (params.ALP * params.ALP - 1);

    int i,j;

m100:
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++) HH[i* N + j] = 0;
        HH[i * N + i] = 1;
    }
    GetGradient(G,params.SG);

    double D = 0;

    for(i = 0; i < N; i++) D += G[i] * G[i];

    if(D < SG1){
        delete[] G;
        delete[] G1;
        delete[] G2;
        delete[] Z;
        delete[] HH;
        return K;
    }

    for(i = 0; i < N; i++){
        G1[i] = G[i];
        Z[i] = GetParam(i);
    }

#define ZERO 0.0000001
//ОБЩИЙ ШАГ АЛГОРИТМА
m101:
    K++;
//ПРЕОБРАЗРВАНИЕ МАТРИЦЫ НН
    for(i = 0; i < N; i++) G2[i] = G1[i] - G[i];
    D = 0;
    for(i = 0; i < N; i++) D += G2[i] * G2[i];
    if(fabs(D) > ZERO){//IF (D - 1.E-18) 32,32,33
        for(i = 0; i < N; i++){
            G1[i] = 0;
            for(j = 0; j < N; j++) G1[i] += HH[i * N + j] * G2[j];
        }
        D = 0;
        for(i = 0; i < N; i++) D += G2[i] * G1[i];//36
        D = W / D;
        for(i = 0; i < N; i++){//DO 37 i = 1,N
            double D1 = D * G1[i];
            for(j = 0; j < N; j++) HH[i * N + j] += D1 * G1[j]; //38
        }//37

    }//32
    for(i = 0; i < N; i++) G1[i] = G[i];
//ВЫЧИСЛЕНИЕ НАПРАВЛНИЯ ДВИЖЕНИЯ
    //fun(N,X,G);
    for(i = 0; i < N; i++){
        G[i] = 0;
        for(j = 0; j < N; j++) G[i] += HH[i * N +j] * G1[j];
    }//40
    D = 0;
    for(i = 0; i < N; i++) D += G1[i] * G[i];//42
    if(fabs(D) < ZERO){
        D = 0;
        for(i = 0; i < N ; i++) D += (GetParam(i) - Z[i]) * (GetParam(i) - Z[i]);//46
        D = sqrt(D);
        H1 = params.II * D / params.NH;
        K--;
        goto m100;
    }
    D = sqrt(D);
    for(i = 0; i < N; i++) G[i] /= D;
    for(i = 0; i < N; i++) Z[i] = GetParam(i);
//АЛГОРИТМ ОДНОМЕРНОГО СПУСКА ПО НАРПАВЛЕНИЮ G
    int K1 = 0;
    int K2 = 100;
    //fun(N,X,G2);
    // = H * II ;
    do{//103
        for(i = 0; i < N; i++) IncParam(i,H1 * G[i]);
        //fun(N,X,G2);
        GetGradient(G2,SG1);
        K1 ++;
        if(params.IS != 0 && K1 > K2){
            delete[] G;
            delete[] G1;
            delete[] G2;
            delete[] Z;
            delete[] HH;
            return -1;
        }
        if(K1 >= params.NH) H1 *= params.Q2;//IF(K1.LT.NH) GO TO 801
        D = 0;//801
        for(i = 0; i < N; i++) D +=G[i] * G2[i];
    }while(D > 0);//IF(D.GT.0) Go TO 103
    if(K1 <= 1) H1 *= params.Q1;
    for(i = 0; i < N; i++) G[i] = G2[i];
//ПРОВЕРКА КРИТЕРИЯ ОСТАНОВА

    if( K < params.KS){
        D = 0.0;
        double D2 = 0;
        for(i =0; i < N; i++){
            D += (GetParam(i) - Z[i]) * (GetParam(i) - Z[i]);
            D2 += G[i] * G[i];
        }
        if(D >= SX1 ){
//КОРРЕКТИРОВКА МАТРИЦЫ НН
            if(fabs(H1) < 1.0E+10){
                D = sqrt(D);
                H1 = params.II*D / params.NH;
                goto m100;
            }else{
                goto m101;
            }
        }

    }
    delete[] G;
    delete[] G1;
    delete[] G2;
    delete[] Z;
    delete[] HH;
    return K;
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::CalculateDerived(size_t param, double dblDelta)
{
    dblDelta /= 10.0;
    IFuzzyModelParameter* pParam = GetParamater(param);
    const double savedVal = pParam->GetValue();

    if (GetParamType(param) == 2)
    {
        if (savedVal > 1.0)
        {
            return 0;
        }
        else
        {
            if (savedVal < 0.0)
            {
                return 0;
            }
        }
    }

    pParam->SetValue(savedVal - dblDelta);
    CalculatePrognosisTrend();
    double y1 = СalculateE();

    pParam->SetValue(savedVal + dblDelta);
    CalculatePrognosisTrend();
    double y2 = СalculateE();

    pParam->SetValue(savedVal);

    return (y2 - y1) / (2.0 * dblDelta);
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::СalculateE()
{
    double squareSum = 0.0;
    for(size_t i = 0; i < m_realTrend.size(); i++)
    {
        squareSum += std::pow(m_realTrend[i] - m_prognosisTrend[i], 2);
    }

    return (1.00 / GetParamsCount()) * sqrt(squareSum);
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::Precalc()
{
    m_belongingFunctionParameters.clear();
    m_ruleWeightParameters.clear();

    for(size_t term = 0; term < m_pTerms->GetTermsCount(); term++)
    {
        for(size_t param = 0; param < m_pTerms->GetTermBelongingFunction(term)->GetParamsCount(); param++)
        {
            m_belongingFunctionParameters.push_back(new BelongingFunctionParameter(m_pTerms->GetTermBelongingFunction(term), param));
        }
    }

    for(size_t i = 0; i < m_models.size(); i++)
    {
        for(size_t j = 0; j < m_models[i]->GetRulesCount(); j++)
        {
            m_ruleWeightParameters.push_back(new RuleWeightParameter(m_models[i]->m_rules[j]));
        }
    }

    SetPrognisisTrendLength(m_realTrend.size());
}

int fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetParamNum(size_t param)
{
    if (param >= m_pTerms->GetTermsCount() * 2)
    {
        return param - m_pTerms->GetTermsCount() * 2;
    }
    else
    {
        return param / 2;
    }
}

int fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetParamType(size_t param)
{
    if(param >= m_pTerms->GetTermsCount() * 2)
    {
        return 2;
    }
    else
    {
        return param % 2;
    }
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetPrognosisTrendLength()
{
    return m_prognosisTrend.size();
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetPrognosisTrendItem(size_t nItem)
{
    if(nItem < GetFirstFullPeriodPrognosisItem())
    {
        return m_realTrend[nItem];
    }
    else
    {
        return m_prognosisTrend[nItem];
    }
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::CalculatePrognosisTrend()
{
    std::vector<double> input;
    size_t n = GetFirstFullPeriodPrognosisItem();
    for(size_t item = n; item < m_prognosisTrend.size(); item++)
    {
        PrepareInputVector(item, input);
        m_prognosisTrend[item] = CalculateModelforItem(item, input);
    }
    for(size_t item = 0; item < n; item++)
    {
        m_prognosisTrend[item] = m_realTrend[item];
    }
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetRealDataItem(size_t nItem) const
{
    return m_realTrend[nItem];
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetFirstFullPeriodRealItem()
{
    return m_nFirstFullPeriodItem;
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::SetFirstFullPeriodItem(size_t nItem)
{
    m_nFirstFullPeriodItem = nItem;
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::SetPrognisisTrendLength(size_t nLength)
{
    m_prognosisTrend.resize(nLength);
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::SetRealTrendLength(size_t nLength)
{
    m_realTrend.resize(nLength);
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetPeriod()
{
    return m_nPeriod;
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::SetPeriod(size_t nPeriod)
{
    m_nPeriod = nPeriod;
    m_dependencies.resize(nPeriod);
    m_models.resize(nPeriod);

    for(size_t i = 0; i < nPeriod; i++){
        m_dependencies[i].SetVarCount(4);
        m_models[i] = new TrendPrognosisFuzzyModel(4);
    }

    if (m_pTerms != nullptr)
    {
        SetTerms(m_pTerms);
    }
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetGradient(double* G, double delta)
{
    for(size_t i = 0; i < GetParamsCount(); i++)
    {
        G[i] = CalculateDerived(i, delta);
    }
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetInputItem(size_t nIndex)
{
    if(nIndex < m_realTrend.size())
    {
        return m_realTrend[nIndex];
    }
    else
    {
        return m_prognosisTrend[nIndex];
    }
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::IncParam(size_t param, double delta)
{
    IFuzzyModelParameter* pParam = GetParamater(param);
    pParam->SetValue(pParam->GetValue() + delta);
}

fuzzyPrognosisMath::IFuzzyModelParameter* fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetParamater(size_t nParameter)
{
    if (nParameter < m_belongingFunctionParameters.size())
    {
        return m_belongingFunctionParameters[nParameter];
    }
    else
    {
        return m_ruleWeightParameters[nParameter - m_belongingFunctionParameters.size()];
    }
}

double fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::CalculateModelforItem(size_t item, std::vector<double>& input)
{
    size_t nOffset = item % GetPeriod();
    m_models[nOffset]->SetInput(input);
    return m_models[nOffset]->GetOutput();
}

void fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::PrepareInputVector(size_t nItem, std::vector<double>& input)
{
    size_t nOffset = nItem % GetPeriod();
    input.resize(m_dependencies[nOffset].GetVarCount());
    for(size_t var = 0; var < m_dependencies[nOffset].GetVarCount(); var++)
    {
        input[var] = GetInputItem(nItem - m_dependencies[nOffset].GetVarOffset(var));
    }
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetFirstFullPeriodPrognosisItem()
{
    return ((GetItemsBeforeFirstFullPeriod() - GetFirstFullPeriodRealItem()) / GetPeriod() + 1) * GetPeriod();
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetItemsBeforeFirstFullPeriod()
{
    size_t nRes = 0;
    for(size_t item = 0; item < m_dependencies.size(); item++)
    {
        size_t nMaxOffset = m_dependencies[item].GetMaxOffset();
        if(nMaxOffset > item &&  (nMaxOffset - item > nRes))
        {
            nRes = nMaxOffset - item;
        }
    }

    return nRes;
}

size_t fuzzyPrognosisMath::PeriodicalTrendFuzzyModel::GetPeriodItemVarOffset(size_t nItem, size_t nVar)
{
    return m_dependencies[nItem].GetVarOffset(nVar);
}
