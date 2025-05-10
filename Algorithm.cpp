#include "Algorithm.h"
using namespace std;
using namespace HalconCpp;
using namespace HDevEngineCpp;

Algorithm::Algorithm() {
    MyEngine = new HDevEngine();
    MyProgram = new HDevProgram();
    MyProcedure = new HDevProcedure();
    MyProcedureCall = new HDevProcedureCall();    
}
int InitializePython()
{
    Py_Initialize();// 初始化Python解释器
    import_array();
    return 0;
}
/// <summary>
/// 获取算子参数
/// </summary>
void Algorithm::GetParameter(QString ProgramPath,QString AlgType,std::list<std::string> &CtrlInputParams,std::list<std::string> &IconicInputParams,std::list<std::string> &CtrlOutputParams,std::list<std::string> &IconicOutputParams,QString &ProcName,bool &IsGetSuccess)
{
    IsGetSuccess = false;
    if(AlgType == "Halcon")
    {
        // 加载程序
        try
        {
            MyProgram->LoadProgram(ProgramPath.toStdString().c_str());
        } catch (HDevEngineException &e) {
            QMessageBox::critical(nullptr,"警告",QString::fromUtf8(e.Message()),QMessageBox::Ok);
            return;
        }

        if (!MyProgram->IsLoaded()) {
            QMessageBox::critical(nullptr,"警告","加载失败",QMessageBox::Ok);
            return;
        }

        // 获取算子名
        HTuple ProcedureNames = MyProgram->GetUsedProcedureNames();
        if (ProcedureNames.Length() == 0) {
            QMessageBox::critical(nullptr,"警告","加载失败",QMessageBox::Ok);
            return;
        }
        ProcName = QString(ProcedureNames[0].S());

        // 加载算子
        MyProcedure->LoadProcedure(*MyProgram, ProcName.toStdString().c_str());

        // 获取输入参数
        int numCtrlInput = MyProcedure->GetInputCtrlParamCount();
        for (int i = 0; i < numCtrlInput; i++) {
            CtrlInputParams.push_back(MyProcedure->GetInputCtrlParamName(i + 1));
        }

        int numIconicInput = MyProcedure->GetInputIconicParamCount();
        for (int i = 0; i < numIconicInput; i++) {
            IconicInputParams.push_back(MyProcedure->GetInputIconicParamName(i + 1));
        }

        // 获取输出参数
        int numCtrlOutput = MyProcedure->GetOutputCtrlParamCount();
        for (int i = 0; i < numCtrlOutput; i++) {
            CtrlOutputParams.push_back(MyProcedure->GetOutputCtrlParamName(i + 1));
        }

        int numIconicOutput = MyProcedure->GetOutputIconicParamCount();
        for (int i = 0; i < numIconicOutput; i++) {
            IconicOutputParams.push_back(MyProcedure->GetOutputIconicParamName(i + 1));
        }
    }
    else if(AlgType == "Python")
    {
        if (!Py_IsInitialized()) {
            InitializePython();// 初始化Python解释器
        }
        if (!Py_IsInitialized()) {
            QMessageBox::critical(nullptr, "Error", "Python Initialize Failed");
            return;
        }

        // 获取.py文件所在目录
        QFileInfo fileInfo(ProgramPath);
        QString dirPath = fileInfo.absolutePath();

        // 将目录添加到Python路径

        PyRun_SimpleString( "import sys\n"
                           "sys.stderr = open('python_errors.log', 'w')\n" );        
        PyRun_SimpleString(QString("sys.path.insert(0, '%1')").arg(dirPath).toUtf8().constData());

        // 导入模块（去掉.py后缀）
        QString moduleName = fileInfo.baseName();
        PyObject* pModule = PyImport_ImportModule(moduleName.toUtf8().constData());
        if (!pModule) {
            PyErr_Print();
            QMessageBox::critical(nullptr, "Error", "Can Not Import PythonModule");
            return;
        }

        // 定义需要提取的字典名称列表
        const char* targetDicts[] = {
            "CtrlInputParamDict",
            "CtrlOutputParamDict",
            "IconicInputParamDict",
            "IconicOutputParamDict"
        };

        // 遍历所有目标字典
        for (const char* dictName : targetDicts) {
            PyObject* pDict = PyObject_GetAttrString(pModule, dictName);
            if (!pDict || !PyDict_Check(pDict)) {
                QMessageBox::critical(nullptr, "Error", QString("Can Not Find Dictionary: %1").arg(dictName));
                Py_XDECREF(pDict);
                Py_DECREF(pModule);
                Py_FinalizeEx();
                return;
            }

            // 遍历字典键
            PyObject *key, *value;
            Py_ssize_t pos = 0;
            std::list<std::string>* targetList = nullptr;

            // 确定目标列表
            if (strcmp(dictName, "CtrlInputParamDict") == 0) targetList = &CtrlInputParams;
            else if (strcmp(dictName, "CtrlOutputParamDict") == 0) targetList = &CtrlOutputParams;
            else if (strcmp(dictName, "IconicInputParamDict") == 0) targetList = &IconicInputParams;
            else if (strcmp(dictName, "IconicOutputParamDict") == 0) targetList = &IconicOutputParams;

            // 提取所有键名
            while (PyDict_Next(pDict, &pos, &key, &value)) {
                if (PyUnicode_Check(key)) {
                    const char* keyStr = PyUnicode_AsUTF8(key);
                    targetList->push_back(keyStr);
                }
            }

            Py_DECREF(pDict);
        }

        // 清理资源
        Py_DECREF(pModule);


        // 设置过程名
        ProcName = fileInfo.baseName();
    }
    IsGetSuccess = true;
}
/// <summary>
/// 执行算子
/// </summary>
void Algorithm::ExcuteProcedure(QString ProgramPath,HTuple CtrlInputParamsData,HObject IconicInputParamsData,HTuple &CtrlOutputParamsData,HObject &IconicOutputParamsData,QList<QString> &TypeList,bool &IsRunSucess)
{
    //调用dev覆盖
   // MyHDevOperatorImpl op_impl;
   // HDevEngine().SetHDevOperatorImpl(&op_impl);
    //调用算子
    IsRunSucess = false;
    HObject SelectedObj;
    HTuple WindowHandle;
    HDevProgram MyProgram;
    HDevOperatorImplCpp MyHDevOperator;
    try
    {
    MyProgram.LoadProgram(ProgramPath.toStdString().c_str());
    }
    catch (HDevEngineException &e)
    {
        QMessageBox::critical(nullptr,"警告",QString::fromUtf8(e.Message()),QMessageBox::Ok);  
        return ;
    }
    HTuple ProcedureNames = MyProgram.GetUsedProcedureNames();
    QString ProcName = QString(ProcedureNames[0].S());
    HDevProcedure MyProcedure;
    MyProcedure.LoadProcedure(MyProgram, ProcName.toStdString().c_str());
    HDevProcedureCall MyProcedureCall(MyProcedure);
    GenEmptyObj(&IconicOutputParamsData);
    //设置输入参数
    for (int i = 0; i < CtrlInputParamsData.Length(); i++) {
        const char m =CtrlInputParamsData[i].Type();
        if(m == 4)
        {
            OpenWindow(0,0,1024,1024,0,"invisible","",&WindowHandle);
            MyProcedureCall.SetInputCtrlParamTuple(i+1,WindowHandle);
        }
        else
        {
            MyProcedureCall.SetInputCtrlParamTuple(i+1,CtrlInputParamsData[i]);
        }

    }

    for (int i = 0; i < IconicInputParamsData.CountObj(); i++) {
        SelectObj(IconicInputParamsData,&SelectedObj,i+1);
        MyProcedureCall.SetInputIconicParamObject(i+1,SelectedObj);
    }

    //获取输出控制参数类型
    for (int i = 0; i < MyProcedure.GetOutputCtrlParamCount(); i++) {
        QString Type = QString(MyProcedure.GetOutputCtrlParamInfo(i+1,"default_type").S());
        TypeList.append(Type);
    }

    //执行
    try
    {
    MyProcedureCall.Execute();
    }
    catch (HDevEngineException &e)
    {
        QMessageBox::critical(nullptr,"警告",QString::fromUtf8(e.Message()),QMessageBox::Ok);
        return ;
    }

    //获取输出参数
    for (int i = 0; i < MyProcedure.GetOutputCtrlParamCount(); i++)
    {
        CtrlOutputParamsData.Append(MyProcedureCall.GetOutputCtrlParamTuple(i+1));
    }
    for (int i = 0; i < MyProcedure.GetOutputIconicParamCount(); i++)
    {
        ConcatObj(IconicOutputParamsData,MyProcedureCall.GetOutputIconicParamObject(i+1),&IconicOutputParamsData);
    }
    IsRunSucess = true;

};

void Algorithm::ExcuteProcedure(QString ProgramPath,QList<QString> CtrlInputParamsData,QList<cv::Mat> IconicInputParamsData,QList<QString> &CtrlOutputParamsData,QList<cv::Mat> &IconicOutputParamsData,QList<QString> &TypeList,bool &IsRunSucess)
{

    IsRunSucess = false;
    if (!Py_IsInitialized()) {
        InitializePython();// 初始化Python解释器
    }

    qDebug() << "Python interpreter path:" << Py_GetProgramName();
    if (!Py_IsInitialized()) {
        QMessageBox::critical(nullptr, "Error", "Python Initialize Failed");
        return;
    }

    // 获取.py文件所在目录
    QFileInfo fileInfo(ProgramPath);
    QString dirPath = fileInfo.absolutePath();
    QString moduleName = fileInfo.baseName();
    // 将目录添加到Python路径

    PyRun_SimpleString( "import sys\n"
                       "sys.stderr = open('python_errorsrun.log', 'w')\n" );
    PyRun_SimpleString(QString("sys.path.insert(0, '%1')").arg(dirPath).toUtf8().constData());

    // 强制清除模块缓存（关键修改点）
    PyObject* sys = PyImport_ImportModule("sys");
    PyObject* modules = PyObject_GetAttrString(sys, "modules");
    if (PyDict_Contains(modules, PyUnicode_FromString(moduleName.toUtf8()))) {
        PyDict_DelItemString(modules, moduleName.toUtf8().constData());
    }
    Py_DECREF(modules);
    Py_DECREF(sys);
    PyRun_SimpleString("import gc; gc.collect()"); // 强制垃圾回收

    // 导入模块
    PyObject* pModule = PyImport_ImportModule(moduleName.toUtf8().constData());
    if (!pModule) {
        PyErr_Print();
        QMessageBox::critical(nullptr, "Error", "Can Not Import PythonModule");
        return;
    }

    // 获取main函数
    PyObject* pFunc = PyObject_GetAttrString(pModule, "main");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PyErr_Print();
        qDebug() << "找不到main函数";
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        return;
    }


    PyGILState_STATE gstate = PyGILState_Ensure();
    // 准备参数
    int pArgsNum = CtrlInputParamsData.size()+IconicInputParamsData.size();
    PyObject* pArgs = PyTuple_New(pArgsNum);

    // 添加控制参数
    for (int i = 0; i < CtrlInputParamsData.size(); ++i) {
        PyTuple_SetItem(pArgs, i, PyUnicode_FromString(CtrlInputParamsData[i].toUtf8().data()));
    }

    // 添加图像参数（字节流+尺寸信息）
    for (int i = 0; i < IconicInputParamsData.size(); ++i) {
        cv::Mat img = IconicInputParamsData[i];

        // 序列化图像数据
        std::vector<uchar> buffer;
        cv::imencode(".png", img, buffer);

        // 创建Python字节对象
        PyObject* pBytes = PyBytes_FromStringAndSize(
            reinterpret_cast<char*>(buffer.data()), buffer.size());

        // 添加图像元数据
        PyObject* pImageInfo = Py_BuildValue("(iiii)",
                                             img.rows, img.cols, img.type(), buffer.size());

        // 将元数据和字节流打包为元组
        PyObject* pImgTuple = PyTuple_Pack(2, pImageInfo, pBytes);
        PyTuple_SetItem(pArgs, CtrlInputParamsData.size() + i, pImgTuple);
    }
    // 调用函数
    for (int i = 0; i < pArgsNum; ++i) {
        PyObject* item = PyTuple_GetItem(pArgs, i);
        if (!item) {
            qDebug() << "参数" << i << "为空";
            return;
        }
    }
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    if (!pResult) {
        PyErr_Print();
        qDebug() << "函数调用失败";
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        return;
    }

    if (PyDict_Check(pResult)) {
        // 清空输出列表
        CtrlOutputParamsData.clear();
        IconicOutputParamsData.clear();
        TypeList.clear();

        // 解析控制参数
        PyObject* ctrlOut = PyDict_GetItemString(pResult, "CtrlOutputParams");
        if (PyDict_Check(ctrlOut)) {
            PyObject *paramKey, *paramValue;
            Py_ssize_t ctrlPos = 0;

            while (PyDict_Next(ctrlOut, &ctrlPos, &paramKey, &paramValue)) {
                if (PyDict_Check(paramValue)) {
                    // 提取参数值
                    PyObject* valueObj = PyDict_GetItemString(paramValue, "Value");
                    PyObject* typeObj = PyDict_GetItemString(paramValue, "Type");

                    if (valueObj && typeObj) {
                        // 添加类型标记
                        TypeList.append(PyUnicode_AsUTF8(typeObj));
                        CtrlOutputParamsData.append(QString::fromUtf8(PyUnicode_AsUTF8(PyObject_Str(valueObj))));
                    }
                }
            }
        }

        // 解析图像参数
        PyObject* iconicOut = PyDict_GetItemString(pResult, "IconicOutputParams");
        if (PyDict_Check(iconicOut)) {
            PyObject *imgKey, *imgValue;
            Py_ssize_t imgPos = 0;

            while (PyDict_Next(iconicOut, &imgPos, &imgKey, &imgValue)) {
                if (PyDict_Check(imgValue)) {
                    PyObject* imgData = PyDict_GetItemString(imgValue, "Value");
                    PyObject* imgType = PyDict_GetItemString(imgValue, "Type");

                    if (imgData && imgType) {
                        // 添加图像类型标记
                        TypeList.append(PyUnicode_AsUTF8(imgType));

                        // 处理两种图像格式
                        if (PyArray_Check(imgData)) { // numpy数组格式
                            PyArrayObject* npArr = (PyArrayObject*)imgData;
                            int dims[] = {
                                (int)PyArray_DIM(npArr, 0),
                                (int)PyArray_DIM(npArr, 1),
                                PyArray_NDIM(npArr) > 2 ? (int)PyArray_DIM(npArr, 2) : 1
                            };

                            cv::Mat img(dims[0], dims[1], CV_8UC(dims[2]),
                                        PyArray_DATA(npArr));
                            IconicOutputParamsData.append(img.clone());
                        }
                        else if (PyBytes_Check(imgData)) { // 字节流格式
                            char* buffer;
                            Py_ssize_t length;
                            PyBytes_AsStringAndSize(imgData, &buffer, &length);

                            std::vector<uchar> imgDataVec(buffer, buffer + length);
                            cv::Mat img = cv::imdecode(imgDataVec, cv::IMREAD_UNCHANGED);
                            IconicOutputParamsData.append(img);
                        }
                    }
                }
            }
        }
    }
    // 清理资源
    PyGILState_Release(gstate);
    Py_DECREF(pResult);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    // 监控文件变更（新增功能）
    static QFileSystemWatcher watcher;
    if (!watcher.files().contains(ProgramPath)) {
        watcher.addPath(ProgramPath);
        QObject::connect(&watcher, &QFileSystemWatcher::fileChanged, [=](const QString &path){
            QFileInfo fi(path);
            QString mod = fi.baseName();
            PyRun_SimpleString(QString("if '%1' in sys.modules: del sys.modules['%1']")
                                   .arg(mod).toUtf8());
        });
    }
    if(TypeList.size()!= IconicOutputParamsData.size()+CtrlOutputParamsData.size())
    {
        QMessageBox::critical(nullptr, "Error", "Excute Failed");
        return;
    }

    IsRunSucess = true;
};

