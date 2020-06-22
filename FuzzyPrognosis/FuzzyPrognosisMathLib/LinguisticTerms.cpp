#include <algorithm>
#include "LinguisticTerms.h"


void fuzzyPrognosisMath::LinguisticTerms::AddTerm(const std::string &strName, fuzzyPrognosisMath::BelongingFunction* pBF)
{
    m_names.push_back(strName);
    m_functions.push_back(pBF);
}

size_t fuzzyPrognosisMath::LinguisticTerms::GetTermFor(double x) const
{
    size_t nTerm = 0;
    double dblBelongMeas = 0.0;
    for(size_t term = 0; term < GetTermsCount(); term++)
    {
        if (GetTermBelongingFunction(term)->GetBelogningMeasure(x) > dblBelongMeas)
        {
             dblBelongMeas = GetTermBelongingFunction(term)->GetBelogningMeasure(x);
             nTerm = term;
        }
    }
    return nTerm;
}

size_t fuzzyPrognosisMath::LinguisticTerms::GetTermsCount() const
{
    return m_functions.size();
}

fuzzyPrognosisMath::BelongingFunction* fuzzyPrognosisMath::LinguisticTerms::GetTermBelongingFunction(size_t nTerm) const
{
    return m_functions.at(nTerm);
}

void fuzzyPrognosisMath::LinguisticTerms::SetTermName(size_t nTerm, const std::string& termName)
{
    m_names.at(nTerm) = termName;
}

void fuzzyPrognosisMath::LinguisticTerms::GetTermName(size_t nTerm, std::string& termName) const
{
    termName = m_names.at(nTerm);
}

double fuzzyPrognosisMath::LinguisticTerms::GetTermParam(size_t nTerm, size_t nParam) const
{
    return m_functions.at(nTerm)->GetParam(nParam);
}

void fuzzyPrognosisMath::LinguisticTerms::Clear()
{
    m_names.clear();
    m_functions.clear();
}
