#pragma once
#include "IFuzzyModelParameter.h"
#include "FuzzyRule.h"

namespace fuzzyPrognosisMath
{
    class RuleWeightParameter : public IFuzzyModelParameter
    {
    public:
        explicit RuleWeightParameter(FuzzyRule* rule);
        void SetValue(double value) override;
        double GetValue() const override;

    private:
        FuzzyRule* const m_rule;
    };
}
