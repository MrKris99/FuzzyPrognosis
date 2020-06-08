#include "FuzzyModel.h"

double fuzzyPrognosisMath::FuzzyModel::GetInput(size_t nVar)
{
    return m_input.at(nVar);
}

void fuzzyPrognosisMath::FuzzyModel::SetInput(const fuzzyPrognosisMath::InputVector& input)
{
    m_input = input;
    CalculateValues();
}
