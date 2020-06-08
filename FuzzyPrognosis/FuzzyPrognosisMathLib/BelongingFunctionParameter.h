#pragma once
#include "IFuzzyModelParameter.h"
#include "BelongingFunction.h"

namespace fuzzyPrognosisMath
{
    class BelongingFunctionParameter : public IFuzzyModelParameter
    {
    public:
        BelongingFunctionParameter(BelongingFunction* function, size_t paramIndex);
        void SetValue(double value) override;
        double GetValue() const override;

    private:
        const size_t m_paramIndex;
        BelongingFunction* const m_function;
    };
}
