#include "FuzzyModel.h"

fuzzyPrognosisMath::FuzzyModel::FuzzyModel(size_t nVarCount)
    : m_input(nVarCount)
{
}

double fuzzyPrognosisMath::FuzzyModel::GetInput(size_t nVar)
{
    return m_input.at(nVar);
}

void fuzzyPrognosisMath::FuzzyModel::SetInput(const fuzzyPrognosisMath::InputVector& input)
{
    m_input = input;
    CalculateValues();
}
