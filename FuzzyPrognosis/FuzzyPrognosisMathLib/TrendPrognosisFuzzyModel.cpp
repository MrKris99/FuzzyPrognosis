#include <algorithm>
#include <numeric>
#include "TrendPrognosisFuzzyModel.h"

fuzzyPrognosisMath::TrendPrognosisFuzzyModel::TrendPrognosisFuzzyModel(size_t nVarCount)
    : FuzzyModel(nVarCount)
    , m_pTerms(nullptr)
{

}

fuzzyPrognosisMath::IFuzzyModelParameter *fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetParameter(size_t nParameter) const
{
    if(nParameter < m_belongingFunctionParameters.size())
    {
        return m_belongingFunctionParameters[nParameter];
    }
    else
    {
        return m_ruleWeightParameters[nParameter - m_belongingFunctionParameters.size()];
    }
}

size_t fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetParametersCount() const
{
    return m_pTerms->GetTermsCount() * 2;
}

double fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetOutput() const
{
    return m_output;
}

void fuzzyPrognosisMath::TrendPrognosisFuzzyModel::AddRule(fuzzyPrognosisMath::FuzzyRule* pRule)
{
    m_rules.push_back(pRule);
    m_ruleWeightParameters.push_back(new RuleWeightParameter(pRule));
}

fuzzyPrognosisMath::FuzzyRule* fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetFuzzyRule(size_t rule) const
{
    return m_rules[rule];
}

size_t fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetRulesCount() const
{
    return m_rules.size();
}

bool fuzzyPrognosisMath::TrendPrognosisFuzzyModel::HasRule(fuzzyPrognosisMath::FuzzyRule* pRule) const
{
    return std::find_if(m_rules.begin(), m_rules.end(), [pRule](FuzzyRule* val)
    {
        return *pRule == *val;
    }) == m_rules.end();
}

void fuzzyPrognosisMath::TrendPrognosisFuzzyModel::DeleteAllRules()
{
    m_rules.clear();
}

void fuzzyPrognosisMath::TrendPrognosisFuzzyModel::SetTerms(fuzzyPrognosisMath::LinguisticTerms* pTerms)
{
    m_pTerms = pTerms;
    m_outputTerms.resize(m_pTerms->GetTermsCount());
}

size_t fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetTermFor(double x) const
{
    return m_pTerms->GetTermFor(x);
}

fuzzyPrognosisMath::LinguisticTerms* fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetLinguisticTerms()
{
    return m_pTerms;
}

void fuzzyPrognosisMath::TrendPrognosisFuzzyModel::CalculateValues()
{
    m_output = 0;
    std::fill(m_outputTerms.begin(), m_outputTerms.end(), 0.0);

    for(auto rule : m_rules)
    {
        m_outputTerms[rule->GetOutputTerm()] += GetRuleTruth(rule);
    }

    for(size_t term = 0; term < m_outputTerms.size(); term++)
    {
        m_output += m_outputTerms[term] * m_pTerms->GetTermBelongingFunction(term)->GetMaxValue();
    }

    m_output /= std::accumulate(m_outputTerms.begin(), m_outputTerms.end(), 0.0);
}

double fuzzyPrognosisMath::TrendPrognosisFuzzyModel::GetRuleTruth(fuzzyPrognosisMath::FuzzyRule* pRule)
{
    double res = pRule->GetWeight();
    for(size_t var = 0; var < pRule->GetVariablesCount(); var++)
    {
        res *= m_pTerms->GetTermBelongingFunction(pRule->GetVarTerm(var))->GetBelogningMeasure(GetInput(var));
    }
    return res;
}
