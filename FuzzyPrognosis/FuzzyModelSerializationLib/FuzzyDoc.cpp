#include <json/json.h>
#include <fstream>
#include <iostream>
#include "FuzzyDoc.h"
#include "GaussBelongingFunction.h"

namespace
{
     const std::string g_documentName = "documentName";
     const std::string g_realItems = "realItems";
     const std::string g_dependencies = "dependencies";
     const std::string g_terms = "terms";
     const std::string g_termsAfter = "termsAfter";
     const std::string g_weightsAfter = "weightsAfter";
     const std::string g_termName = "termName";
     const std::string g_paramB = "paramB";
     const std::string g_paramC = "paramC";
     const std::string g_periodSubmodels = "periodSubmodels";
     const std::string g_outputTerm = "outputTerm";
     const std::string g_varTerms = "varTerms";
     const std::string g_rules = "rules";
     const std::string g_isTuned = "isTuned";
     const std::string g_rootModel = "rootModel";

     void DumpModel(fuzzyPrognosisMath::PeriodicalTrendFuzzyModel* model)
     {
         std::cout << "Period: " << model->m_nPeriod << std::endl;

         std::cout << "Terms: " << std::endl;
         for (size_t term = 0; term < model->m_pTerms->GetTermsCount(); ++term)
         {
             std::string name;
             model->m_pTerms->GetTermName(term, name);
             const double paramB = model->m_pTerms->GetTermBelongingFunction(term)->GetParam(0);
             const double paramC = model->m_pTerms->GetTermBelongingFunction(term)->GetParam(1);
             std::cout << name << " " << paramB << " " << paramC << std::endl;
         }

         std::cout << "First full period: " << model->m_nFirstFullPeriodItem << std::endl;

         std::cout << "Real trend: ";
         for (auto val : model->m_realTrend)
         {
             std::cout << val << " ";
         }
         std::cout << std::endl;

         std::cout << "Prognosis trend: ";
         for (auto val : model->m_prognosisTrend)
         {
             std::cout << val << " ";
         }
         std::cout << std::endl;

         std::cout << "Period deps: " << std::endl;
         for (const auto& dep : model->m_dependencies)
         {
             for (auto val : dep.m_varConvertions)
             {
                 std::cout << val << " ";
             }
             std::cout << std::endl;
         }
         std::cout << std::endl;
     }
}

document::FuzzyDoc::FuzzyDoc()
    : m_isTuned(false)
{
    m_model.SetTerms(&m_terms);
    m_rAlgParams.II = -1.0;
    m_rAlgParams.KS = 1000;
    m_rAlgParams.SX = 0.01;
    m_rAlgParams.SG = 0.01;
    m_rAlgParams.ALP = 3.0;
    m_rAlgParams.H = 0.1;
    m_rAlgParams.NH = 3;
    m_rAlgParams.Q1 = 0.9;
    m_rAlgParams.Q2 = 1.1;
    m_rAlgParams.IS = 1;
}

void document::FuzzyDoc::SerializeModel(const std::string& filepath) const
{
    Json::Value root(Json::objectValue);
    root[g_documentName] = "Fuzzy";

    Json::Value model(Json::objectValue);
    SerializeRealItems(model, g_realItems);
    SerializeDependencies(model, m_model.m_dependencies, g_dependencies);
    SerializeTerms(model, m_terms, g_terms);
    SerializeSubmodels(model, m_model.m_models, g_periodSubmodels);
    model[g_isTuned] = m_isTuned;

    if (m_isTuned)
    {
        SerializeTerms(model, m_termsAfter, g_termsAfter);
        SerializeWeightsAfterTune(model, m_ruleWeightsAfter, g_weightsAfter);
    }

    root[g_rootModel] = model;

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream(filepath, std::ios::trunc);
    writer->write(root, &outputFileStream);
}

void document::FuzzyDoc::DeserializeModel(const std::string& filepath)
{
    Json::Value root;
    Json::Reader reader;
    std::ifstream modelFile(filepath);
    bool parsingSuccessful = reader.parse(modelFile, root, false);
    if (!parsingSuccessful )
    {
        throw std::runtime_error("Cannot parse document");
    }

    Json::Value rootModel = root[g_rootModel];
    Json::Value realItems = rootModel[g_realItems];
    for (const auto& item : realItems)
    {
        m_model.AddRealDataItem(item.asDouble());
    }

    Json::Value dependencies = rootModel[g_dependencies];
    m_model.SetPeriod(dependencies.size());
    for (int dep = 0; dep < dependencies.size(); ++dep)
    {
        const size_t varCount = dependencies[dep].size();
        m_model.Dependency(dep).SetVarCount(varCount);
        for (int var = 0; var < varCount; ++var)
        {
            m_model.Dependency(dep).SetVarOffset(var, dependencies[dep][var].asUInt());
        }
    }

    m_terms.Clear();
    Json::Value termsValue = rootModel[g_terms];
    for (const auto& termValue : termsValue)
    {
        const std::string termName = termValue[g_termName].asString();
        const double paramB = termValue[g_paramB].asDouble();
        const double paramC = termValue[g_paramC].asDouble();
        m_terms.AddTerm(termName, new fuzzyPrognosisMath::GaussBelongingFunction(paramB, paramC));
    }

    m_model.SetTerms(&m_terms);

    Json::Value submodels = rootModel[g_periodSubmodels];
    for (int i = 0; i < submodels.size(); ++i)
    {
        m_model.m_models[i]->DeleteAllRules();
        Json::Value rulesValue = submodels[i][g_rules];
        for (const auto& ruleValue: rulesValue)
        {
            const size_t outputTerm = ruleValue[g_outputTerm].asUInt();
            const size_t varCount = ruleValue[g_varTerms].size();
            auto fuzzyRule = new fuzzyPrognosisMath::FuzzyRule(varCount, outputTerm);
            for (int var = 0; var < varCount; ++var)
            {
                fuzzyRule->SetVarTerm(var, ruleValue[g_varTerms][i].asUInt());
            }
            m_model.m_models[i]->AddRule(fuzzyRule);
        }
    }

    m_isTuned = false;
}

void document::FuzzyDoc::Tune()
{
    m_termsAfter.Clear();
    for(size_t term = 0; term < m_terms.GetTermsCount(); term++)
    {
        std::string stdstr;
        m_terms.GetTermName(term, stdstr);
        m_termsAfter.AddTerm(stdstr,
             new fuzzyPrognosisMath::GaussBelongingFunction(
                m_terms.GetTermBelongingFunction(term)->GetParam(0),
                m_terms.GetTermBelongingFunction(term)->GetParam(1)));
    }

    m_model.SetTerms(&m_termsAfter);
    DumpModel(&m_model);
    const long n = m_model.Tune(m_rAlgParams);
    m_model.SetPrognisisTrendLength(m_model.GetRealTrendLength() + m_model.GetPeriod());
    m_model.CalculatePrognosisTrend();
    SaveWeightsAfter();
    SetWeightBefore();
    m_model.SetTerms(&m_terms);
    m_isTuned = true;
}

void document::FuzzyDoc::SerializeRealItems(Json::Value& model, const std::string& key) const
{
    Json::Value realItems(Json::arrayValue);
    for (size_t item = 0; item < m_model.GetRealTrendLength(); ++item)
    {
        realItems.append(Json::Value(m_model.GetRealDataItem(item)));
    }
    model[key] = realItems;
}

void document::FuzzyDoc::SerializeDependencies(Json::Value &model, const std::vector<fuzzyPrognosisMath::PeriodDependencies>& deps,
                                               const std::string& key) const
{
    Json::Value dependencies(Json::arrayValue);
    for (const auto& dep : deps)
    {
        Json::Value dependencyVarOffsets(Json::arrayValue);
        for (size_t var = 0; var < dep.GetVarCount(); ++var)
        {
            dependencyVarOffsets.append(Json::Value(dep.GetVarOffset(var)));
        }
        dependencies.append(dependencyVarOffsets);
    }
    model[key] = dependencies;
}

void document::FuzzyDoc::SerializeTerms(Json::Value &model, const fuzzyPrognosisMath::LinguisticTerms& terms, const std::string &key) const
{
    Json::Value termsArray(Json::arrayValue);
    for (size_t term = 0; term < m_terms.GetTermsCount(); ++term)
    {
        Json::Value termObject(Json::objectValue);
        std::string termName;
        m_terms.GetTermName(term, termName);
        const double paramB = m_terms.GetTermParam(term, 0);
        const double paramC = m_terms.GetTermParam(term, 1);
        termObject[g_termName] = termName;
        termObject[g_paramB] = paramB;
        termObject[g_paramC] = paramC;
        termsArray.append(termObject);
    }
    model[key] = termsArray;
}

void document::FuzzyDoc::SerializeSubmodels(Json::Value& model, const std::vector<fuzzyPrognosisMath::TrendPrognosisFuzzyModel*>& submodels, const std::string &key) const
{
    Json::Value periodSubmodels(Json::arrayValue);

    for(const auto modelPtr : submodels)
    {
        Json::Value submodel(Json::objectValue);
        Json::Value rules(Json::arrayValue);
        for(const auto rule : modelPtr->m_rules)
        {
            Json::Value ruleObj(Json::objectValue);
            ruleObj[g_outputTerm] = rule->GetOutputTerm();
            Json::Value varTerms(Json::arrayValue);
            for(size_t var = 0; var < rule->GetVariablesCount(); ++var)
            {
                varTerms.append(Json::Value(rule->GetVarTerm(var)));
            }
            ruleObj[g_varTerms] = varTerms;
            rules.append(ruleObj);
        }

        submodel[g_rules] = rules;
        periodSubmodels.append(submodel);
    }

    model[key] = periodSubmodels;
}

void document::FuzzyDoc::SerializeWeightsAfterTune(Json::Value& model, const std::vector<double>& weights, const std::string& key) const
{
    Json::Value weightsArr(Json::arrayValue);
    for(auto weight: weights)
    {
        weightsArr.append(Json::Value(weight));
    }
    model[key] = weightsArr;
}

void document::FuzzyDoc::SaveWeightsAfter()
{
    m_ruleWeightsAfter.clear();
    for(int i = 0; i < m_model.GetPeriod(); i++)
    {
        for(int j = 0; j < m_model.m_models[i]->GetRulesCount(); j++)
        {
            m_ruleWeightsAfter.push_back(m_model.m_models[i]->m_rules[j]->GetWeight());
        }
    }
}

void document::FuzzyDoc::SetWeightBefore()
{
    for(int i = 0; i < m_model.GetPeriod(); i++)
    {
        for(int j = 0; j < m_model.m_models[i]->GetRulesCount(); j++)
        {
            m_model.m_models[i]->m_rules[j]->SetWeight(1.0);
        }
    }
}

void document::FuzzyDoc::SetWeightAfter()
{
    int rule = 0;
    for(size_t i = 0; i < m_model.GetPeriod(); i++)
    {
        for(int j = 0; j < m_model.m_models[i]->GetRulesCount(); j++)
        {
            m_model.m_models[i]->m_rules[j]->SetWeight(m_ruleWeightsAfter[rule]);
            rule++;
        }
    }
}
