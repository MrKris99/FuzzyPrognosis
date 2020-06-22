#include "ModelTuningWorkerThread.h"

ModelTuningWorkerThread::ModelTuningWorkerThread(document::FuzzyDoc &doc, QObject *parent)
    : QThread(parent),
      m_doc(doc)
{
}

void ModelTuningWorkerThread::run()
{
    m_doc.Tune();
    emit TuningFinished();
}
