#pragma once
#include "FuzzyModel.h"
#include "LinguisticTerms.h"
#include "IFuzzyModelParameter.h"
#include "RuleWeightParameter.h"
#include "BelongingFunctionParameter.h"

namespace fuzzyPrognosisMath
{
    class TrendPrognosisFuzzyModel : public FuzzyModel
    {
    public:
        TrendPrognosisFuzzyModel(size_t nVarCount);
        IFuzzyModelParameter* GetParameter(size_t nParameter) const override;
        size_t GetParametersCount() const override;
        double GetOutput() const override;

        void AddRule(FuzzyRule* pRule);
        FuzzyRule* GetFuzzyRule(size_t rule) const;
        size_t GetRulesCount() const;
        bool HasRule(FuzzyRule* pRule) const;
        void DeleteAllRules();

        void SetTerms(LinguisticTerms* pTerms);
        size_t GetTermFor(double x) const;
        LinguisticTerms* GetLinguisticTerms();

    protected:
        void CalculateValues() override;

    private:
        double GetRuleTruth(FuzzyRule* pRule);

    private:
        double m_output;

    public:
        LinguisticTerms* m_pTerms = nullptr;
        std::vector<FuzzyRule*> m_rules;
        std::vector<double> m_outputTerms;
        std::vector<BelongingFunctionParameter*> m_belongingFunctionParameters;
        std::vector<RuleWeightParameter*> m_ruleWeightParameters;
    };
}

