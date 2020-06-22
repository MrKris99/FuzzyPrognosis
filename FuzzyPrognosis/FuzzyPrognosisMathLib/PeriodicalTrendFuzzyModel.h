#pragma once
#include "TrendPrognosisFuzzyModel.h"
#include "PeriodDependencies.h"
#include "LinguisticTerms.h"

namespace fuzzyPrognosisMath
{
    struct RAlgParams
    {
        double II;
        int KS;
        double  SX;
        double SG;
        double ALP;
        double H;
        int NH;
        double Q1;
        double Q2;
        int IS;

    };

    class PeriodicalTrendFuzzyModel
    {
    public:
        std::vector<BelongingFunctionParameter*> m_belongingFunctionParameters;
        std::vector<RuleWeightParameter*> m_ruleWeightParameters;
        std::vector<PeriodDependencies> m_dependencies;
        std::vector<TrendPrognosisFuzzyModel*> m_models;
        std::vector<double> m_realTrend;
        std::vector<double> m_prognosisTrend;
        size_t m_nFirstFullPeriodItem;
        LinguisticTerms* m_pTerms;
        size_t m_nPeriod;

        explicit PeriodicalTrendFuzzyModel(size_t nPeriod = 1);
        void AutoGenerateRules();
        size_t GetRulesCount();

        void SetTerms(LinguisticTerms* pTerms);
        void AddRealDataItem(double item);
        void SetRealDataItem(size_t nItem, double dblValue);
        size_t GetRealTrendLength() const;
        PeriodDependencies& Dependency(size_t i);
        void AutoGenerateRules(size_t nPeriodItem);

        double GetParam(size_t param);
        size_t GetParamsCount();
        size_t Tune(RAlgParams params);
        double CalculateDerived(size_t param,double dblDelta);
        double СalculateE();
        void Precalc();
        int GetParamNum(size_t param);
        int GetParamType(size_t param);

        size_t GetPrognosisTrendLength() const;
        double GetPrognosisTrendItem(size_t nIndex) const;
        void CalculatePrognosisTrend();

        double GetRealDataItem(size_t nItem) const;
        size_t GetFirstFullPeriodRealItem() const;
        void SetFirstFullPeriodItem(size_t nItem);
        void SetPrognisisTrendLength(size_t nLength);
        void SetRealTrendLength(size_t nLength);
        size_t GetPeriod() const;
        void SetPeriod(size_t nPeriod);

    private:
        void GetGradient(double* G, double delta);
        double GetInputItem(size_t nIndex);
        void IncParam(size_t param, double delta);
        IFuzzyModelParameter* GetParameter(size_t nParameter);
        double CalculateModelforItem(size_t item, std::vector<double>& input);
        void PrepareInputVector(size_t nItem, std::vector<double>& input);
        size_t GetFirstFullPeriodPrognosisItem() const;
        size_t GetItemsBeforeFirstFullPeriod() const;
        size_t GetPeriodItemVarOffset(size_t nItem, size_t nVar);
    };
}
