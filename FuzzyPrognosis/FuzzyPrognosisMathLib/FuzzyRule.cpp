#include "FuzzyRule.h"

fuzzyPrognosisMath::FuzzyRule::FuzzyRule(size_t nVariables, size_t nOutputTerm, double weight)
    : m_weight(weight),
      m_terms(nVariables),
      m_nOutputTerm(nOutputTerm)
{
}

void fuzzyPrognosisMath::FuzzyRule::SetOutputTerm(size_t nTerm)
{
    m_nOutputTerm = nTerm;
}

size_t fuzzyPrognosisMath::FuzzyRule::GetOutputTerm() const
{
    return m_nOutputTerm;
}

void fuzzyPrognosisMath::FuzzyRule::SetVarTerm(size_t nVar, size_t nTerm)
{
    m_terms.at(nVar) = nTerm;
}

size_t fuzzyPrognosisMath::FuzzyRule::GetVarTerm(size_t nVar) const
{
    return m_terms.at(nVar);
}

void fuzzyPrognosisMath::FuzzyRule::SetWeight(double weight)
{
    m_weight = weight;
}

double fuzzyPrognosisMath::FuzzyRule::GetWeight() const
{
    return m_weight;
}

size_t fuzzyPrognosisMath::FuzzyRule::GetVariablesCount() const
{
    return m_terms.size();
}

bool fuzzyPrognosisMath::FuzzyRule::operator==(const fuzzyPrognosisMath::FuzzyRule& rule) const
{
    return m_nOutputTerm == rule.m_nOutputTerm && m_terms == rule.m_terms;
}
