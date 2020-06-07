#pragma once
#include "BelongingFunction.h"

namespace fuzzyPrognosisMath
{
    class GaussBelongingFunction : public BelongingFunction
    {
    public:
        GaussBelongingFunction(double b, double c);
        double GetDerivedByParam(size_t param, double x) const override;
        double GetBelogningMeasure(double x) const override;
        double GetMaxValue() const override;
    };
}
