#include "BelongingFunction.h"

fuzzyPrognosisMath::BelongingFunction::BelongingFunction(size_t nParams)
    : m_params(nParams)
{
}

size_t fuzzyPrognosisMath::BelongingFunction::GetParamsCount() const
{
    return m_params.size();
}

double fuzzyPrognosisMath::BelongingFunction::GetParam(size_t index) const
{
    return m_params.at(index);
}

void fuzzyPrognosisMath::BelongingFunction::SetParam(size_t index, double param)
{
    m_params.at(index) = param;
}
