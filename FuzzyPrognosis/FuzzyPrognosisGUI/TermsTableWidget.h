#pragma once
#include <QWidget>
#include "LinguisticTerms.h"
#include "TermsTableModel.h"
#include "ui_TermsTableWidget.h"

namespace Ui {
class TermsTableWidget;
}

class TermsTableWidget : public QWidget
{
    Q_OBJECT

public:
    TermsTableWidget(fuzzyPrognosisMath::LinguisticTerms* terms, bool readOnly, QWidget *parent = nullptr);

private:
    Ui::TermsTableWidget m_ui;
    std::unique_ptr<TermsTableModel> m_termsModel;
};

