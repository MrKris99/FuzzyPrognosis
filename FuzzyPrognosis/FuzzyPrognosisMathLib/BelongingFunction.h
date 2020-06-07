#pragma once
#include <vector>

namespace fuzzyPrognosisMath
{
    class BelongingFunction
    {
    public:
        BelongingFunction(size_t nParams);
        virtual ~BelongingFunction(){}
        void SetParam(size_t nIndex, double dblParam);
        double GetParam(size_t nIndex) const;
        size_t GetParamsCount() const;

        virtual double GetDerivedByParam(size_t param, double x) const = 0;
        virtual double GetBelogningMeasure(double x) const = 0;
        virtual double GetMaxValue() const = 0;

    private:
        std::vector<double> m_params;
    };
}
