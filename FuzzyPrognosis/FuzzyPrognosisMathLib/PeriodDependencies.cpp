#include <algorithm>
#include "PeriodDependencies.h"

fuzzyPrognosisMath::PeriodDependencies::PeriodDependencies(size_t nVarCount)
    : m_varConvertions(nVarCount)
{
}

void fuzzyPrognosisMath::PeriodDependencies::SetVarCount(size_t nVarCount)
{
    m_varConvertions.resize(nVarCount);
    for(size_t i = 0; i < nVarCount; i++)
    {
        m_varConvertions[i] = i + 1;
    }
}

size_t fuzzyPrognosisMath::PeriodDependencies::GetVarCount() const
{
    return m_varConvertions.size();
}

size_t fuzzyPrognosisMath::PeriodDependencies::GetVarOffset(size_t nVar) const
{
    return m_varConvertions.at(nVar);
}

void fuzzyPrognosisMath::PeriodDependencies::SetVarOffset(size_t nVar, size_t nOffset)
{
    m_varConvertions.at(nVar) = nOffset;
}

size_t fuzzyPrognosisMath::PeriodDependencies::GetMaxOffset() const
{
    return *std::max_element(m_varConvertions.begin(), m_varConvertions.end());
}
