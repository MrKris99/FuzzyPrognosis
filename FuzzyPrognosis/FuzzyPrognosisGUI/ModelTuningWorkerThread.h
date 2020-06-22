#pragma once
#include <QThread>
#include <FuzzyDoc.h>

class ModelTuningWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit ModelTuningWorkerThread(document::FuzzyDoc& doc, QObject* parent = nullptr);

signals:
    void TuningFinished();

protected:
    void run() override;

private:
    document::FuzzyDoc& m_doc;
};
