#pragma once
#include <QDialog>
#include "ui_DependenciesDialog.h"
#include "PeriodDependencies.h"

class DependenciesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DependenciesDialog(std::vector<fuzzyPrognosisMath::PeriodDependencies>& deps, QWidget *parent = nullptr);

private:
    void SetupComboBoxes();
    void OnUpdate();

private:
    Ui::DependenciesDialog m_ui;
    std::vector<fuzzyPrognosisMath::PeriodDependencies>& m_deps;
};
