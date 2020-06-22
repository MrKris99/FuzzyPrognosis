#include "TermsTableWidget.h"

TermsTableWidget::TermsTableWidget(fuzzyPrognosisMath::LinguisticTerms* terms, bool readOnly, QWidget* parent)
    : QWidget(parent)
    , m_termsModel(new TermsTableModel(terms, readOnly, this))
{
    m_ui.setupUi(this);
    m_ui.tableView->setModel(m_termsModel.get());
    m_ui.tableView->horizontalHeader()->setStretchLastSection(true);
    m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui.btnAddTerm->setText(tr("Create term"));
    if (readOnly)
    {
        m_ui.btnAddTerm->hide();
    }
    else
    {
        connect(m_ui.btnAddTerm, &QPushButton::released, [this]()
        {
            m_termsModel->insertRows(m_termsModel->rowCount(), 1);
        });
    }
}
