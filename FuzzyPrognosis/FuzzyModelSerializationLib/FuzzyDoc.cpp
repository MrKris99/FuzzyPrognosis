#include <json/json.h>
#include <fstream>
#include "FuzzyDoc.h"

namespace
{
     const std::string g_documentName = "documentName";
     const std::string g_realItems = "realItems";
     const std::string g_dependencies = "dependencies";
     const std::string g_terms = "terms";
     const std::string g_termName = "termName";
     const std::string g_paramB = "paramB";
     const std::string g_paramC = "paramC";
     const std::string g_periodSubmodels = "periodSubmodels";
     const std::string g_outputTerm = "outputTerm";
     const std::string g_varTerms = "varTerms";
     const std::string g_rules = "rules";
     const std::string g_isTuned = "isTuned";
}

void document::FuzzyDoc::SerializeModel(const std::string& filepath) const
{
    Json::Value root;
    root[g_documentName] = "Fuzzy";

    Json::Value model;

    Json::Value realItems(Json::arrayValue);
    for (size_t item = 0; item < m_model.GetRealTrendLength(); ++item)
    {
        realItems.append(Json::Value(m_model.GetRealDataItem(item)));
    }
    model[g_realItems] = realItems;

    Json::Value dependencies(Json::arrayValue);
    for (const auto& dep : m_model.m_dependencies)
    {
        Json::Value dependencyVarOffsets(Json::arrayValue);
        for (size_t var = 0; var < dep.GetVarCount(); ++var)
        {
            dependencyVarOffsets.append(Json::Value(dep.GetVarOffset(var)));
        }
        dependencies.append(dependencyVarOffsets);
    }
    model[g_dependencies] = dependencies;

    Json::Value terms(Json::arrayValue);
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
    }
    model[g_terms] = terms;

    Json::Value periodSubmodels(Json::arrayValue);

    for(const auto model: m_model.m_models)
    {
        Json::Value submodel(Json::objectValue);
        Json::Value rules(Json::arrayValue);
        for (const auto rule : model->m_rules)
        {
            Json::Value ruleObj(Json::objectValue);
            ruleObj[g_outputTerm] = rule->GetOutputTerm();
            Json::Value varTerms(Json::arrayValue);
            for (size_t var = 0; var < rule->GetVariablesCount(); ++var)
            {
                varTerms.append(Json::Value(rule->GetVarTerm(var)));
            }
            ruleObj[g_varTerms] = varTerms;
            rules.append(ruleObj);
        }

        submodel[g_rules] = rules;
        periodSubmodels.append(submodel);
    }

    model[g_periodSubmodels] = periodSubmodels;
    model[g_isTuned] = m_isTuned;

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream(filepath, std::ios::trunc);
    writer->write(root, &outputFileStream);
}

void document::FuzzyDoc::DeserializeModel(const std::string& filepath)
{

}
