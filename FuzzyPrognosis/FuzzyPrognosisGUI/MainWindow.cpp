#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "MainWindow.h"
#include "TrendTableWidget.h"
#include "TermsTableWidget.h"
#include "DependenciesDialog.h"
#include "ModelTuningWorkerThread.h"


namespace
{
    enum StackWidgetIndex
    {
        RealTrend = 0,
        TermsBefore = 1
    };
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);
    CreateMenus();
    InitStackedWidget();
    m_ui.stackedWidget->setCurrentIndex(RealTrend);
}

void MainWindow::CreateMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_fileMenu->addAction(tr("New"), [this]()
    {
        this->NewDocument();
    });
    m_fileMenu->addAction(tr("Open"), [this]()
    {
        this->OpenDocument();
    });
    m_fileMenu->addAction(tr("Save as..."), [this]()
    {
        this->SaveDocument();
    });

    m_trendMenu = menuBar()->addMenu(tr("Trend"));
    m_trendMenu->addAction(tr("Show real trend"), [this]()
    {
        m_ui.stackedWidget->setCurrentIndex(RealTrend);
    });
    m_trendMenu->addAction(tr("Show real trend chart"), [this]()
    {
        this->ShowRealTrendChart();
    });
    m_trendMenu->addAction(tr("Period..."), [this]()
    {
        this->ShowPeriodDialog();
    });
    m_trendMenu->addAction(tr("Dependencies..."), [this]()
    {
        this->ShowDependenciesDialog();
    });

    m_termsMenu = menuBar()->addMenu(tr("Terms"));
    m_termsMenu->addAction(tr("Terms before tune"), [this]()
    {
        m_ui.stackedWidget->setCurrentIndex(TermsBefore);
    });

    m_tuneMenu = menuBar()->addMenu(tr("Tune"));
    m_tuneMenu->addAction(tr("Do tune"), [this]()
    {
        QDialog* dialog = new QDialog(this, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        dialog->setWindowTitle(tr("Tuning model..."));
        ModelTuningWorkerThread* worker = new ModelTuningWorkerThread(m_doc, this);
        connect(worker, &ModelTuningWorkerThread::TuningFinished, dialog, &QDialog::close);
        worker->start();
        dialog->exec();
        worker->deleteLater();
    });
    m_tuneMenu->addAction(tr("Prognosis chart after tune"), [this]()
    {
        this->ShowPrognosisChartAfterTune();
    });
}

void MainWindow::ShowRealTrendChart()
{
    using namespace QtCharts;
    QSplineSeries *series = new QSplineSeries;
    for (size_t item = 0; item < m_doc.m_model.GetRealTrendLength(); ++item)
    {
        series->append(item + 1, m_doc.m_model.GetRealDataItem(item));
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle(tr("Real trend"));
    chart->createDefaultAxes();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(this->size());
    chartView->show();
}

void MainWindow::ShowPeriodDialog()
{
    bool result = false;
    const int newPeriod = QInputDialog::getInt(this, tr("Change model period"), tr("Period:"),
        m_doc.m_model.GetPeriod(), 4, 30, 1, &result);

    if (result)
    {
        m_doc.m_model.SetPeriod(newPeriod);
        m_doc.m_model.AutoGenerateRules();
        m_doc.m_isTuned = false;
    }
}

void MainWindow::ShowDependenciesDialog()
{
    DependenciesDialog* dialog = new DependenciesDialog(m_doc.m_model.m_dependencies, this);
    dialog->setFixedSize(dialog->size());
    if(dialog->exec() == QDialog::Accepted)
    {
        m_doc.m_model.AutoGenerateRules();
        m_doc.m_isTuned = false;
    }
}

void MainWindow::ShowPrognosisChartAfterTune()
{
    auto& model = m_doc.m_model;
    model.SetTerms(&m_doc.m_termsAfter);
    m_doc.SetWeightAfter();
    model.SetPrognisisTrendLength(model.GetRealTrendLength() + model.GetPeriod());
    model.CalculatePrognosisTrend();

    using namespace QtCharts;
    QSplineSeries* seriesReal = new QSplineSeries;
    seriesReal->setName(tr("Real trend"));
    for (size_t item = 0; item < model.GetRealTrendLength(); ++item)
    {
        seriesReal->append(item + 1, model.GetRealDataItem(item));
    }

    QSplineSeries* seriesPrognosis = new QSplineSeries;
    seriesPrognosis->setName(tr("Prognosis"));
    for (size_t item = 0; item < model.GetPrognosisTrendLength(); ++item)
    {
        seriesPrognosis->append(item + 1, model.GetPrognosisTrendItem(item));
    }

    model.SetTerms(&m_doc.m_terms);
    m_doc.SetWeightBefore();

    QChart* chart = new QChart();
    chart->addSeries(seriesPrognosis);
    chart->addSeries(seriesReal);
    chart->setTitle(tr("Prognosis"));
    chart->createDefaultAxes();
    chart->axisY()->setMin(0);
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(this->size());
    chartView->show();
}

void MainWindow::InitStackedWidget()
{
    m_ui.stackedWidget->insertWidget(RealTrend, new TrendTableWidget(&m_doc.m_model, false, m_ui.stackedWidget));
    m_ui.stackedWidget->insertWidget(TermsBefore, new TermsTableWidget(&m_doc.m_terms, false, m_ui.stackedWidget));
}

void MainWindow::ShutdownStackedWidget()
{
    for(int i = 0; i < m_ui.stackedWidget->count(); ++i)
    {
        QWidget* widget = m_ui.stackedWidget->widget(i);
        m_ui.stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
}

void MainWindow::NewDocument()
{
    ShutdownStackedWidget();
    m_doc = document::FuzzyDoc();
    InitStackedWidget();
    m_ui.stackedWidget->setCurrentIndex(RealTrend);
}

void MainWindow::OpenDocument()
{
    ShutdownStackedWidget();
    QString path = QFileDialog::getOpenFileName(this, tr("Choose file with your saved model"));
    if (path.isEmpty())
    {
        return;
    }

    try
    {
        m_doc = document::FuzzyDoc();
        m_doc.DeserializeModel(path.toStdString());
    }
    catch (const std::exception& ex)
    {
        m_doc = document::FuzzyDoc();
        QMessageBox::critical(this, tr("Parsing error!"),
                              tr("Encountered unproper model serialization. Please check that the specified file is correct."));
    }

    InitStackedWidget();
    m_ui.stackedWidget->setCurrentIndex(RealTrend);
}

void MainWindow::SaveDocument()
{
    ShutdownStackedWidget();
    QString path = QFileDialog::getSaveFileName(this, tr("Choose file to save your model"));
    if (path.isEmpty())
    {
        return;
    }

    try
    {
        m_doc.SerializeModel(path.toStdString());
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(this, tr("Parsing error!"),
                              tr("Cannot save. Please check that the specified file is correct."));
    }
}
