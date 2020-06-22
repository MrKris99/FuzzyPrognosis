#pragma once
#include <QWidget>
#include "ui_TrendTableWidget.h"
#include "PeriodicalTrendFuzzyModel.h"
#include <TrendTableModel.h>

class TrendTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrendTableWidget(fuzzyPrognosisMath::PeriodicalTrendFuzzyModel* fuzzyModel, bool withPrognosis, QWidget *parent = nullptr);

private slots:
    void AddDataToRealTrend();

private:
    Ui::TrendTableWidget m_ui;
    std::unique_ptr<TrendTableModel> m_tableModel;
};

