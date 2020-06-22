#pragma once
#include <string>
#include <json/json.h>
#include "PeriodicalTrendFuzzyModel.h"

namespace document
{
    class FuzzyDoc
    {
    public:
        FuzzyDoc();
        void SerializeModel(const std::string& filepath) const;
        void DeserializeModel(const std::string& filepath);
        void SaveWeightsAfter();
        void SetWeightBefore();
        void SetWeightAfter();
        void Tune();

    public:
        bool m_isTuned;
        fuzzyPrognosisMath::LinguisticTerms m_terms;
        fuzzyPrognosisMath::LinguisticTerms m_termsAfter;
        fuzzyPrognosisMath::PeriodicalTrendFuzzyModel m_model;
        fuzzyPrognosisMath::RAlgParams m_rAlgParams;
        std::vector<double> m_ruleWeightsAfter;

    private:
        void SerializeRealItems(Json::Value& model, const std::string& key) const;
        void SerializeDependencies(Json::Value& model, const std::vector<fuzzyPrognosisMath::PeriodDependencies>& deps,
                                   const std::string& key) const;
        void SerializeTerms(Json::Value& model, const fuzzyPrognosisMath::LinguisticTerms& terms,
                            const std::string& key) const;
        void SerializeSubmodels(Json::Value& model, const std::vector<fuzzyPrognosisMath::TrendPrognosisFuzzyModel*>& submodels,
                                const std::string& key) const;
        void SerializeWeightsAfterTune(Json::Value& model, const std::vector<double>& weights, const std::string& key) const;
    };
}
