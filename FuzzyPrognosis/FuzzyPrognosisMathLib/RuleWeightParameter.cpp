#include "RuleWeightParameter.h"

fuzzyPrognosisMath::RuleWeightParameter::RuleWeightParameter(fuzzyPrognosisMath::FuzzyRule* rule)
    : m_rule(rule)
{
}

void fuzzyPrognosisMath::RuleWeightParameter::SetValue(double value)
{
    m_rule->SetWeight(value);
}

double fuzzyPrognosisMath::RuleWeightParameter::GetValue() const
{
    return m_rule->GetWeight();
}
