#pragma once
#include <QMainWindow>
#include "ui_MainWindow.h"
#include "FuzzyDoc.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    void CreateMenus();
    void ShowRealTrendChart();
    void ShowPeriodDialog();
    void ShowDependenciesDialog();
    void ShowPrognosisChartAfterTune();
    void InitStackedWidget();
    void ShutdownStackedWidget();
    void NewDocument();
    void OpenDocument();
    void SaveDocument();

private:
    Ui::MainWindow m_ui;
    document::FuzzyDoc m_doc;
    QMenu* m_fileMenu;
    QMenu* m_trendMenu;
    QMenu* m_termsMenu;
    QMenu* m_rulesMenu;
    QMenu* m_tuneMenu;
    QMenu* m_prognosisMenu;

    // file actions
    QAction* m_actNew;
    QAction* m_actOpen;
    QAction* m_actSave;
    QAction* m_actSaveAs;

    // trend actions
    QAction* m_actShowTrendData;
    QAction* m_actShowTrendView;
    QAction* m_actShowPeriodDialog;
    QAction* m_actShowDepsDialog;

    // terms actions
    QAction* m_actTermsBeforeTune;
    QAction* m_actTermsViewBeforeTune;
    QAction* m_actTermsAfterTune;
    QAction* m_actTermsViewAfterTune;

    // rules actions
    QAction* m_actShowRules;
    QAction* m_actGenerateRules;

    // tune actions
    QAction* m_actDoTune;
    QAction* m_actRAlgParamsDialog;

    // prognosis actions
    QAction* m_actPrognosisBeforeTune;
    QAction* m_actPrognosisViewBeforeTune;
    QAction* m_actPrognosisAfterTune;
    QAction* m_actPrognosisViewAfterTune;
    QAction* m_actShowDeviation;
};
