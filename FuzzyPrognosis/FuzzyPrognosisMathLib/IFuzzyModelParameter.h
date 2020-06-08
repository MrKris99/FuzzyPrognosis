#pragma once

namespace fuzzyPrognosisMath
{
    class IFuzzyModelParameter
    {
    public:
        virtual ~IFuzzyModelParameter(){};
        virtual void SetValue(double value) = 0;
        virtual double GetValue() const = 0;
    };
}
