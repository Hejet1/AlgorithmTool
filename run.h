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
    void run (QString ProcName,QList<cv::Mat> Image,QList <QString> &CtrlResults,QList <cv::Mat> &IconicResults);

private:
    QString m_ProcName,m_ProgramPath,m_ProcType;
    std::list<std::string> CtrlInputParams;
    QList <QString> TypeList;
    bool IsRunSucess;

};

#endif // RUN_H
