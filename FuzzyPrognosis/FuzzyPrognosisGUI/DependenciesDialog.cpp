#include "DependenciesDialog.h"

DependenciesDialog::DependenciesDialog(std::vector<fuzzyPrognosisMath::PeriodDependencies>& deps, QWidget *parent)
    : QDialog(parent)
    , m_deps(deps)
{
    m_ui.setupUi(this);
    m_ui.lblVarCount->setText(tr("j-th dot from i-th period depends on:"));
    m_ui.lblOffsets->setText(tr("Distance to first dependent variable:"));
    SetupComboBoxes();
    connect(m_ui.btnUpdate, &QPushButton::released, [this]()
    {
        this->OnUpdate();
        accept();
    });
}

void DependenciesDialog::SetupComboBoxes()
{
    const uint8_t maxVarCount = 20;
    const uint8_t maxOffset = 20;
    for (uint8_t i = 0; i < maxVarCount; ++i)
    {
        m_ui.cmbVarCount->addItem(QString::number(i + 1));
    }
    for (uint8_t i = 0; i < maxOffset; ++i)
    {
        m_ui.cmbOffsets->addItem(QString::number(i + 1));
    }

    m_ui.cmbVarCount->setCurrentIndex(m_deps[0].GetVarCount() - 1);
    m_ui.cmbOffsets->setCurrentIndex(m_deps[0].GetVarOffset(0) - 1);
}

void DependenciesDialog::OnUpdate()
{
    const size_t offsetBase = m_ui.cmbOffsets->currentIndex() + 1;
    for (auto& dep: m_deps)
    {
        dep.SetVarCount(m_ui.cmbVarCount->currentIndex() + 1);
        for (size_t var = 0; var < dep.GetVarCount(); ++var)
        {
            dep.SetVarOffset(var, offsetBase + var);
        }
    }
}
