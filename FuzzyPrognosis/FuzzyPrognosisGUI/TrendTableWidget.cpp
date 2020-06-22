#include "TrendTableWidget.h"
#include <QDebug>

TrendTableWidget::TrendTableWidget(fuzzyPrognosisMath::PeriodicalTrendFuzzyModel* fuzzyModel, bool withPrognosis, QWidget *parent)
    : QWidget(parent)
    , m_tableModel(new TrendTableModel(fuzzyModel, withPrognosis, this))
{
    m_ui.setupUi(this);
    m_ui.tableView->setModel(m_tableModel.get());
    m_ui.tableView->horizontalHeader()->setStretchLastSection(true);
    m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui.newDataInput->setPlaceholderText(tr("Input new data here..."));
    m_ui.btnAddData->setText(tr("Add"));
    if (withPrognosis)
    {
        m_ui.newDataInput->hide();
        m_ui.btnAddData->hide();
    }
    else
    {
        connect(m_ui.btnAddData, &QPushButton::released, this, &TrendTableWidget::AddDataToRealTrend);
    }
}

void TrendTableWidget::AddDataToRealTrend()
{
    QString input = m_ui.newDataInput->text();
    bool successfulConvertion = false;
    const double inputValue = input.toDouble(&successfulConvertion);
    if (!successfulConvertion)
    {
        return;
    }

    const int lastRow = m_tableModel->rowCount();
    m_tableModel->insertRows(lastRow, 1);
    m_tableModel->setData(m_tableModel->index(lastRow, 0), QVariant(inputValue), Qt::EditRole);
}
