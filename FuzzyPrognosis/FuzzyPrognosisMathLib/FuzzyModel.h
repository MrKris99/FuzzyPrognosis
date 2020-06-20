#pragma once
#include <vector>
#include "IFuzzyModelParameter.h"

namespace fuzzyPrognosisMath
{
    using InputVector = std::vector<double>;
    class FuzzyModel
    {
    public:
        FuzzyModel(size_t nVarCount);
        virtual ~FuzzyModel(){};
        double GetInput(size_t nVar);
        void SetInput(const InputVector& input);
        virtual size_t GetParametersCount() const = 0;
        virtual IFuzzyModelParameter* GetParameter(size_t nParameter) const = 0;
        virtual double GetOutput() const = 0;

   protected:
        virtual void CalculateValues() = 0;

    private:
        InputVector m_input;
    };
}
