#include "TermsTableModel.h"
#include "GaussBelongingFunction.h"

namespace
{
    enum TermsTableColumn
    {
        Name = 0,
        ParamB = 1,
        ParamC = 2
    };
}

TermsTableModel::TermsTableModel(fuzzyPrognosisMath::LinguisticTerms* terms, bool readOnly, QObject* parent)
    : QAbstractTableModel(parent)
    , m_readOnly(readOnly)
    , m_terms(terms)
{
}

QVariant TermsTableModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch (index.column())
    {
    case Name:
    {
        std::string name;
        m_terms->GetTermName(index.row(), name);
        return QString::fromStdString(name);
    }
    case ParamB:
        return m_terms->GetTermBelongingFunction(index.row())->GetParam(0);
    case ParamC:
        return m_terms->GetTermBelongingFunction(index.row())->GetParam(1);
    default:
       return QVariant();
    }
}

bool TermsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (m_readOnly || role != Qt::EditRole)
    {
        return false;
    }

    switch (index.column())
    {
    case Name:
        m_terms->SetTermName(index.row(), value.toString().toStdString());
        return true;
    case ParamB:
        m_terms->GetTermBelongingFunction(index.row())->SetParam(0, value.toReal());
        return true;
    case ParamC:
        m_terms->GetTermBelongingFunction(index.row())->SetParam(1, value.toReal());
        return true;
    default:
       return false;
    }
}

int TermsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_terms->GetTermsCount();
}

int TermsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return ParamC + 1;
}

QVariant TermsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    {
        return QVariant();
    }

    switch (section)
    {
    case Name:
        return tr("Name");
    case ParamB:
        return tr("Parameter b");
    case ParamC:
        return tr("Parameter c");
    default:
       return QVariant();
    }
}

bool TermsTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count != 1 || m_readOnly)
    {
        return false;
    }
    beginInsertRows(parent, row, row);
    m_terms->AddTerm("Term1", new fuzzyPrognosisMath::GaussBelongingFunction(100, 100));
    endInsertRows();
    return true;
}

Qt::ItemFlags TermsTableModel::flags(const QModelIndex& index) const
{
    if (m_readOnly)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}
