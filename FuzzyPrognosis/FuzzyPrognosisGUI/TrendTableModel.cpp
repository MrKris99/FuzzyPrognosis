#include "TrendTableModel.h"
#include <QDebug>

namespace
{
    enum TrendTableColumn
    {
        ItemNum = 0,
        RealTrend = 1,
        PrognosisTrend = 2
    };
}

TrendTableModel::TrendTableModel(fuzzyPrognosisMath::PeriodicalTrendFuzzyModel* fuzzyModel, bool withPrognosis, QObject* parent)
    : QAbstractTableModel(parent)
    , m_withPrognosis(withPrognosis)
    , m_fuzzyModel(fuzzyModel)
{
}

QVariant TrendTableModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (index.column() == ItemNum)
    {
        return index.row() + 1;
    }

    if (index.column() == RealTrend)
    {
        if (m_fuzzyModel->GetRealTrendLength() > index.row())
        {
            return m_fuzzyModel->GetRealDataItem(index.row());
        }

        return QVariant();
    }

    return m_fuzzyModel->GetPrognosisTrendItem(index.row());
}

bool TrendTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole || m_withPrognosis || !value.canConvert<double>())
    {
        return false;
    }

    m_fuzzyModel->SetRealDataItem(index.row(), value.value<double>());
    emit dataChanged(index, index);
    return true;
}

int TrendTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_withPrognosis ? m_fuzzyModel->GetPrognosisTrendLength() : m_fuzzyModel->GetRealTrendLength();
}

int TrendTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return m_withPrognosis ? (PrognosisTrend + 1) : (RealTrend + 1);
}

QVariant TrendTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch (section)
    {
    case ItemNum:
        return tr("N of dot");
    case RealTrend:
        return tr("Real trend");
    case PrognosisTrend:
        return tr("Progsosis");
    default:
        return QVariant();
    }
}

bool TrendTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if (count != 1 || m_withPrognosis)
    {
        return false;
    }

    beginInsertRows(parent, row, row);
    m_fuzzyModel->AddRealDataItem(0);
    endInsertRows();
    return true;
}

Qt::ItemFlags TrendTableModel::flags(const QModelIndex& index) const
{
    if (m_withPrognosis || index.column() == ItemNum)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}
