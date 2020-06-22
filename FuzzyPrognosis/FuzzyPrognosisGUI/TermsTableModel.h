#pragma once
#include <QAbstractTableModel>
#include "LinguisticTerms.h"

class TermsTableModel : public QAbstractTableModel
{
public:
    TermsTableModel(fuzzyPrognosisMath::LinguisticTerms* terms, bool readOnly, QObject* parent);
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role) override;
    int	rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int	columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    const bool m_readOnly;
    fuzzyPrognosisMath::LinguisticTerms* const m_terms;
};

