#pragma once
#include <QAbstractTableModel>
#include "PeriodicalTrendFuzzyModel.h"

class TrendTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TrendTableModel(fuzzyPrognosisMath::PeriodicalTrendFuzzyModel* fuzzyModel, bool withPrognosis, QObject* parent = nullptr);
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role) override;
    int	rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int	columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    const bool m_withPrognosis;
    fuzzyPrognosisMath::PeriodicalTrendFuzzyModel* const m_fuzzyModel;
};

