#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include <list>
#include <halconcpp/HalconCpp.h>
#include <hdevengine/HDevEngineCpp.h>
#include <QTableWidget>
#include <QPushButton>
#include <QProcess>
#include <opencv2/opencv.hpp>
extern "C" {
#include <Python.h>
#include <numpy/arrayobject.h>
}
Q_DECLARE_METATYPE(cv::Mat)
using namespace HalconCpp;
using namespace HDevEngineCpp;
using namespace std;
class Algorithm {
public:
    Algorithm();

    void GetParameter(QString ProgramPath,QString AlgType,std::list<std::string> &CtrlInputParams,std::list<std::string> &IconicInputParams,std::list<std::string> &CtrlOutputParams,std::list<std::string> &IconicOutputParams,QString &ProcName,bool &IsGetSuccess);

    void ExcuteProcedure(QString ProgramPath,HTuple CtrlInputParams,HObject IconicInputParams,HTuple &CtrlOutputParams,HObject &IconicOutputParams,QList<QString> &TypeList,bool &IsSucess);

    void ExcuteProcedure(QString ProgramPath,QList<QString> CtrlInputParamsData,QList<cv::Mat> IconicInputParamsData,QList<QString> &CtrlOutputParamsData,QList<cv::Mat> &IconicOutputParamsData,QList<QString> &TypeList,bool &IsRunSucess);

private:
    QProcess *pythonProcess;
    HDevEngine* MyEngine;
    HDevProgram* MyProgram;
    HDevProcedure* MyProcedure;
    HDevProcedureCall* MyProcedureCall;
};

#endif // ALGORITHM_H
