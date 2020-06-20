#pragma once
#include <string>
#include "PeriodicalTrendFuzzyModel.h"

namespace document
{
    class FuzzyDoc
    {
    public:
        explicit FuzzyDoc();
        void SerializeModel(const std::string& filepath) const;
        void DeserializeModel(const std::string& filepath);

    public:
        bool m_isTuned;
        fuzzyPrognosisMath::LinguisticTerms m_terms;
        fuzzyPrognosisMath::LinguisticTerms m_termsAfter;
        fuzzyPrognosisMath::PeriodicalTrendFuzzyModel m_model;
        fuzzyPrognosisMath::RAlgParams m_rAlgParams;
        std::vector<double> m_ruleWeightsAfter;
    };
}
