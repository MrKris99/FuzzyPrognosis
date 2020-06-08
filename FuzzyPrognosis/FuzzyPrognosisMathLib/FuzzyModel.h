#pragma once
#include <vector>
#include "IFuzzyModelParameter.h"

namespace fuzzyPrognosisMath
{
    using InputVector = std::vector<double>;
    class FuzzyModel
    {
    public:
        virtual ~FuzzyModel(){};
        double GetInput(size_t nVar);
        void SetInput(const InputVector& input);
        virtual size_t GetParametersCount() const = 0;
        virtual IFuzzyModelParameter* GetParameter(size_t nParameter) const = 0;
        virtual double GetOutput() const = 0;
        virtual double GetOutputTerm(size_t nTerm) const = 0;
        virtual void CalculateValues() = 0;

    private:
        InputVector m_input;
    };
}
