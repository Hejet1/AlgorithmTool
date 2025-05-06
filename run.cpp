#include "run.h"
Run::Run() {}

void Run::run (QString ProcName,cv::Mat Image)
{
    QFile file;
    QString filePath = "D:/QtAlgorithm/Algorithm/algorithmparams.json";
    file.setFileName(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No saved parameter file found";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject rootObj = doc.object();
    QJsonArray paramsArray = rootObj["algorithmParams"].toArray();

    for (const QJsonValue &value : paramsArray) {
        QJsonObject obj = value.toObject();
        m_ProcName = obj["ProcedureName"].toString();
        if(ProcName == m_ProcName)
        {
        m_ProgramPath = obj["ProgramPath"].toString();
        m_ProcType = obj["Type"].toString();
        }
    }
}
