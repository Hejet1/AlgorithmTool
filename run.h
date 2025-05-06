#ifndef RUN_H
#define RUN_H
#include "Algorithm.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <opencv2/opencv.hpp>
class Run
{
public:
    Run();
    void run (QString ProcName,cv::Mat Image);

private:
    QString m_ProcName,m_ProgramPath,m_ProcType;

};

#endif // RUN_H
