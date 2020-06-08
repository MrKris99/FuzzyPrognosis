#pragma once
#include <vector>

namespace fuzzyPrognosisMath
{
    class FuzzyRule
    {
    public:
        FuzzyRule(size_t nVariables, size_t nOutputTerm, double weight = 1.0);
        void SetOutputTerm(size_t nTerm);
        size_t GetOutputTerm() const;
        void SetVarTerm(size_t nVar, size_t nTerm);
        size_t GetVarTerm(size_t nVar) const;
        void SetWeight(double weight);
        double GetWeight() const;
        size_t GetVariablesCount() const;
        bool operator==(const FuzzyRule& rule) const;

    private:
        double m_weight;
        std::vector<size_t> m_terms;
        size_t m_nOutputTerm;
    };
}
