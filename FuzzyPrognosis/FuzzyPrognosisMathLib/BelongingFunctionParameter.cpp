#include "BelongingFunctionParameter.h"


fuzzyPrognosisMath::BelongingFunctionParameter::BelongingFunctionParameter(fuzzyPrognosisMath::BelongingFunction *function, size_t paramIndex)
    : m_paramIndex(paramIndex),
      m_function(function)
{
}

void fuzzyPrognosisMath::BelongingFunctionParameter::SetValue(double value)
{
    m_function->SetParam(m_paramIndex, value);
}

double fuzzyPrognosisMath::BelongingFunctionParameter::GetValue() const
{
    return m_function->GetParam(m_paramIndex);
}
