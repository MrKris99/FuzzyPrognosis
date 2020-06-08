#pragma once
#include "BelongingFunction.h"

namespace fuzzyPrognosisMath
{
    class LinguisticTerms
    {
    public:
        void AddTerm(const std::string& strName, BelongingFunction* pBF);
        size_t GetTermFor(double x) const;
        size_t GetTermsCount() const;
        BelongingFunction* GetTermBelongingFunction(size_t nTerm) const;
        void SetTermName(size_t nTerm, const std::string& termName);
        void GetTermName(size_t nTerm, std::string& termName) const;
        double GetTermParam(size_t nTerm, size_t nParam) const ;
        void Clear();

    private:
        std::vector<BelongingFunction*> m_functions;
        std::vector<std::string> m_names;
    };
}
