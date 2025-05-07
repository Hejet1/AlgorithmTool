#include "algorithmparam.h"
#include "ui_algorithmparam.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>
#include <Algorithm.h>
#include <halconcpp/HalconCpp.h>
#include <QVBoxLayout>
#include <QMessageBox>
#include <toolbox.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

Algorithm Alg;
using namespace HalconCpp;
AlgorithmParam::AlgorithmParam(QWidget *parent)
    : QDialog(parent), ui(new Ui::AlgorithmParam) {
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 初始化表格结构
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"ParamName", "Value"});
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    Disp = new QHalconWindow(this);
    Disp->setMinimumSize(50,50);
    QVBoxLayout *DispVBox = new QVBoxLayout();
    DispVBox->addWidget(Disp,1);
    DispVBox->addSpacing(8);
    ui->DispWidget->setLayout(DispVBox);
}
AlgorithmParam::~AlgorithmParam() {
    delete ui;
}
void AlgorithmParam::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);
    Disp->close();
   // Disp->GetHalconBuffer()->SetPart(-1,-1,-1,-1);
}
/// <summary>
/// 动态生成参数表格
/// </summary>
void AlgorithmParam::populateParameters(QString ProgramPath,QString ProcName,std::list<std::string> CtrlInputParams,std::list<std::string> IconicInputParams,std::list<std::string> CtrlOutputParams,std::list<std::string> IconicOutputParams) {


    m_CtrlOutputParams = CtrlOutputParams;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    m_ctrlInputLineEdits.clear();
    ImagePathLineEdits.clear();

    addProcNameRow("ProcedureName", ProcName);
    addProgramPathRow("ProgramPath", ProgramPath);

    // 控制输入参数
    addSectionHeader("CtrlInputParams");
    CtrlInputParamsCount = 0;
    for (std::string paramName : CtrlInputParams) {
        CtrlInputParamsCount = CtrlInputParamsCount+1;
        addControlInputRow(QString::fromStdString(paramName));
    }

    // 图像输入参数
    addSectionHeader("IconicInputParams");
    for (std::string paramName : IconicInputParams) {
        addImageInputRow(QString::fromStdString(paramName));
    }

    // 控制输出参数
    addSectionHeader("CtrlOutputParams");
    for (std::string paramName : CtrlOutputParams) {
        addControlOutputRow(QString::fromStdString(paramName));
    }

    // 图像输出参数
    addSectionHeader("IconicOutputParams");
    for (std::string paramName : IconicOutputParams) {
        addImageOutputRow(QString::fromStdString(paramName));
    }
}
/// <summary>
/// 添加分类标题行
/// </summary>
void AlgorithmParam::addSectionHeader(const QString &title) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    QTableWidgetItem *headerItem = new QTableWidgetItem(title);
    headerItem->setBackground(QColor(240, 240, 240));
    headerItem->setFlags(headerItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(row, 0, headerItem);
    ui->tableWidget->setSpan(row, 0, 1, 2);
}
/// <summary>
/// 添加算子名称行
/// </summary>
void AlgorithmParam::addProcNameRow(const QString &label, const QString &ProcName) {
    ui->tableWidget->insertRow(0);

    QTableWidgetItem *labelItem = new QTableWidgetItem(label);
    labelItem->setFlags(labelItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(0, 0, labelItem);

    QLineEdit *pathLineEdit = new QLineEdit();
    pathLineEdit->setText(ProcName);
    pathLineEdit->setReadOnly(true);
    pathLineEdit->setStyleSheet("QLineEdit { border: none; }");
    ui->tableWidget->setCellWidget(0, 1, pathLineEdit);
}
/// <summary>
/// 添加算法路径行
/// </summary>
void AlgorithmParam::addProgramPathRow(const QString &label, const QString &ProgramPath) {

    ui->tableWidget->insertRow(0);

    QTableWidgetItem *labelItem = new QTableWidgetItem(label);
    labelItem->setFlags(labelItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(0, 0, labelItem);

    QLineEdit *pathLineEdit = new QLineEdit();
    pathLineEdit->setText(ProgramPath);
    pathLineEdit->setReadOnly(true);
    pathLineEdit->setStyleSheet("QLineEdit { border: none; }");
    ui->tableWidget->setCellWidget(0, 1, pathLineEdit);
}
/// <summary>
/// 添加控制输入行
/// </summary>
void AlgorithmParam::addControlInputRow(const QString &name) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(row, 0, nameItem);

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setStyleSheet("QLineEdit { border: none; }");
    lineEdit->setPlaceholderText("Input");

    ui->tableWidget->setCellWidget(row, 1, lineEdit);
    m_ctrlInputLineEdits.append(lineEdit);
}
/// <summary>
/// 添加图像输入行
/// </summary>
void AlgorithmParam::addImageInputRow(const QString &name) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(row, 0, nameItem);

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    QLineEdit *pathEdit = new QLineEdit();
    pathEdit->setReadOnly(true);
    pathEdit->setStyleSheet("QLineEdit { border: none; }");
    pathEdit->setPlaceholderText("ImagePath");

    QPushButton *button = new QPushButton("SelectImage");
    layout->addWidget(pathEdit);
    layout->addWidget(button);

    widget->setLayout(layout);
    ui->tableWidget->setCellWidget(row, 1, widget);

    // 图片选择按钮点击信号
    connect(button, &QPushButton::clicked, [this, pathEdit, name]() {
        QString filePath = QFileDialog::getOpenFileName(
            this, "选择图片", "", "图片文件 (*.png *.jpg *.bmp)"
            );
        if (!filePath.isEmpty()) {
            pathEdit->setText(filePath);
            loadAndDisplayImage(filePath);
        }
    });
    ImagePathLineEdits.append(pathEdit);
}
/// <summary>
/// 添加控制输出行
/// </summary>
void AlgorithmParam::addControlOutputRow(const QString &name) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(row, 0, nameItem);

    QLineEdit *lineEdit = new QLineEdit("Output");
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lineEdit->setStyleSheet("QLineEdit { border: none; }");
    ui->tableWidget->setCellWidget(row, 1, lineEdit);
}
/// <summary>
/// 添加图像输出行
/// </summary>
void AlgorithmParam::addImageOutputRow(const QString &name) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(row, 0, nameItem);
}
/// <summary>
/// 显示图像
/// </summary>
void AlgorithmParam::loadAndDisplayImage(const QString &path) {
    try {
        ReadImage(&Image,path.toStdString().c_str());
        GetImageSize(Image,&hvImageWidth,&hvImageHeight);
        int Height = hvImageHeight[0].I();
        int Width = hvImageWidth[0].I();
        Disp->GetHalconBuffer()->SetPart(0,0,Height-1,Width-1);
        Disp->GetHalconBuffer()->DispObj(Image);
        Disp->GetHalconBuffer()->FlushBuffer();
    } catch (HalconCpp::HException &e) {
        QMessageBox::critical(this, "Error", "ImageLoadFail" + QString(e.ErrorMessage().Text()));
    }
}
/// <summary>
/// 更新运行后的控制输出行参数
/// </summary>
void AlgorithmParam::updateControlOutputResult(const QString &name, const QString &result) {
    // 遍历所有行，查找名称匹配的项，匹配上则在对应行显示结果
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *nameItem = ui->tableWidget->item(row, 0);
        if (nameItem && nameItem->text() == name) {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row, 1));
            if (lineEdit) {
                lineEdit->setText(result);
                break;
            }
        }
    }
}
/// <summary>
/// 从Json加载已保存的参数
/// </summary>
void AlgorithmParam::loadSavedParameters() {
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
        if (obj["uuid"].toString() != m_uuid) continue;

        // 1. 读取基本参数
        ProcName = obj["ProcedureName"].toString();
        ProgramPath = obj["ProgramPath"].toString();
        ProcType = obj["Type"].toString();
        // 2. 读取参数列表（保持原有逻辑）
        CtrlInputParams.clear();
        QJsonArray ctrlInputNames = obj["ctrlInputParams"].toArray();
        for (const QJsonValue &param : ctrlInputNames) {
            QJsonObject paramObj = param.toObject();
            CtrlInputParams.push_back(paramObj["name"].toString().toStdString());
        }

        IconicInputParams.clear();
        QJsonArray iconicInputNames = obj["iconicInputParams"].toArray();
        for (const QJsonValue &param : iconicInputNames) {
            IconicInputParams.push_back(param.toString().toStdString());
        }

        CtrlOutputParams.clear();
        QJsonArray ctrlOutputNames = obj["ctrlOutputParams"].toArray();
        for (const QJsonValue &param : ctrlOutputNames) {
            CtrlOutputParams.push_back(param.toString().toStdString());
        }

        IconicOutputParams.clear();
        QJsonArray iconicOutputNames = obj["iconicOutputParams"].toArray();
        for (const QJsonValue &param : iconicOutputNames) {
            IconicOutputParams.push_back(param.toString().toStdString());
        }

        // 3. 读取保存的控制输入参数值（新增逻辑）
        QMap<QString, QString> ctrlInputValues;
        QJsonArray savedCtrlInputs = obj["ctrlInputParams"].toArray();
        for (const QJsonValue &paramValue : savedCtrlInputs) {
            QJsonObject paramObj = paramValue.toObject();
            ctrlInputValues[paramObj["name"].toString()] = paramObj["value"].toString();
        }
        // 4. 生成表格
        populateParameters(ProgramPath, ProcName, CtrlInputParams, IconicInputParams, CtrlOutputParams, IconicOutputParams);

        // 5. 填充保存的值（新增逻辑）
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QTableWidgetItem *nameItem = ui->tableWidget->item(row, 0);
            if (!nameItem) continue; // 跳过分类标题行

            QString paramName = nameItem->text();
            if (ctrlInputValues.contains(paramName)) {
                QLineEdit *lineEdit = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row, 1));
                if (lineEdit) {
                    lineEdit->setText(ctrlInputValues[paramName]);
                }
            }
        }

        break; // 找到对应UUID后退出

    }
    file.close();
}
/// <summary>
/// 将获取的uuid,Type保存在常量中
/// </summary>
void AlgorithmParam::setAlgoinfoInfo(const QString &uuid,const QString &Type)
{
    m_uuid = uuid;
    m_Type = Type;
    //每次发生uuid传输就加载一次保存的参数
    loadSavedParameters();
}
/// <summary>
/// 加载算子
/// </summary>
void AlgorithmParam::on_LoadAlgorithmButton_clicked()
{
    QString ProgramPathCache = ProgramPath;
    ProgramPath = QFileDialog::getOpenFileName(this, "SelectAlgorithm", "", "*.hdev;*.py");
    if (ProgramPath.isEmpty())
    {
        ProgramPath = ProgramPathCache;
        return;
    }

    // 每次获取之前将常量清除，避免表格生成出错
    CtrlInputParams.clear();
    IconicInputParams.clear();
    CtrlOutputParams.clear();
    IconicOutputParams.clear();

    // 调用参数获取方法
    Alg.GetParameter(ProgramPath,m_Type,CtrlInputParams, IconicInputParams, CtrlOutputParams, IconicOutputParams, ProcName,IsGetSuccess);

    // 将算法路径传回toolbox界面对应uuid的文本框中
    emit ProgramPathLoaded(ProgramPath, m_uuid);

    // 调用动态表格生成方法
    populateParameters(ProgramPath,ProcName, CtrlInputParams, IconicInputParams, CtrlOutputParams, IconicOutputParams);
}
/// <summary>
/// 保存参数
/// </summary>
void AlgorithmParam::on_SaveParameterButton_clicked()
{
    if (m_uuid.isEmpty() || ProcName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No parameters to save.");
        return;
    }

    QJsonObject paramObj;
    paramObj["uuid"] = m_uuid;
    paramObj["Type"] = m_Type;
    paramObj["ProcedureName"] = ProcName;
    paramObj["ProgramPath"] = ProgramPath;

    // 1. 保存控制输入参数的值（新增逻辑）
    QJsonArray ctrlInputArray;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *nameItem = ui->tableWidget->item(row, 0);
        if (!nameItem) continue; // 跳过分类标题行（无item的行）

        QString paramName = nameItem->text();
        // 检查是否为控制输入参数
        bool isCtrlInputParam = false;
        for (const auto& param : CtrlInputParams) {
            if (paramName == QString::fromStdString(param)) {
                isCtrlInputParam = true;
                break;
            }
        }

        if (isCtrlInputParam) {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row, 1));
            if (lineEdit) {
                QJsonObject paramValueObj;
                paramValueObj["name"] = paramName;
                paramValueObj["value"] = lineEdit->text();
                ctrlInputArray.append(paramValueObj);
            }
        }
    }
    paramObj["ctrlInputParams"] = ctrlInputArray;

    // 2. 其他参数类型（保持原有逻辑）
    QJsonArray iconicInputArray, ctrlOutputArray, iconicOutputArray;
    for (const auto& param : IconicInputParams) {
        iconicInputArray.append(QString::fromStdString(param));
    }
    for (const auto& param : CtrlOutputParams) {
        ctrlOutputArray.append(QString::fromStdString(param));
    }
    for (const auto& param : IconicOutputParams) {
        iconicOutputArray.append(QString::fromStdString(param));
    }

    paramObj["iconicInputParams"] = iconicInputArray;
    paramObj["ctrlOutputParams"] = ctrlOutputArray;
    paramObj["iconicOutputParams"] = iconicOutputArray;

    // 3. 写入文件
    QFile file;
    QString filePath = "D:/QtAlgorithm/Algorithm/algorithmparams.json";
    file.setFileName(filePath);

    // 读取现有数据
    QJsonDocument doc;
    QJsonObject rootObj;
    if (file.open(QIODevice::ReadOnly)) {
        doc = QJsonDocument::fromJson(file.readAll());
        rootObj = doc.object();
        file.close();
    }
    // 创建新数组，只保留不同名的参数
    QJsonArray paramsArray = rootObj["algorithmParams"].toArray();
    QJsonArray newParamsArray;
    for (const auto& param : paramsArray) {
        QJsonObject obj = param.toObject();
        // 过滤掉同名的旧记录
        if (obj["ProcedureName"].toString() != ProcName) {
            newParamsArray.append(param);
        }
    }

    // 添加新参数
    newParamsArray.append(paramObj);
    rootObj["algorithmParams"] = newParamsArray;

    // 写入文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        doc.setObject(rootObj);
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(this, "Success", "Parameters saved.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to save parameters.");
    }
}
/// <summary>
/// 运行算子
/// </summary>
void AlgorithmParam::on_RunProcedureButton_clicked()
{
    if(m_Type == "Halcon")
    {
        QList<QString> TypeList;
        HTuple CtrlInputParamsData;
        HObject IconicInputParamsData,ImageIn,SelectedObj,Num;
        HTuple CtrlOutputParamsData;
        HObject IconicOutputParamsData;
        std::list<QString> imgpath;
        GenEmptyObj(&IconicInputParamsData);

        // 遍历输入的所有参数
        for (QLineEdit *lineEdit : m_ctrlInputLineEdits)
        {
            QString text = lineEdit->text();
            bool isDouble = false;
            double value = text.toDouble(&isDouble);
            if (isDouble)
            {
                CtrlInputParamsData.Append(value);
            }
            else
            {
                HTuple textstring = lineEdit->text().toStdString().c_str();
                CtrlInputParamsData.Append(textstring);
            }
        }

        // 遍历选择的图像路径并添加到list imgpath中
        for (QLineEdit *pathEdit : ImagePathLineEdits)
        {
            try
            {
                QString text = pathEdit->text();
                imgpath.push_back(text);
            } catch (exception &e)
            {
                QMessageBox::critical(nullptr,"Warning"," Table Initialize Failed",QMessageBox::Ok);
                return;
            }
        }

        // 遍历list imgpath 读取图像并将所有图像加载入同一个Obj容器中
        for (QString path : imgpath)
        {
            if(path == "")
            {
                QMessageBox::critical(nullptr,"Warning"," NO IconicInputParamsData Send",QMessageBox::Ok);
                return;
            }
            else
            {
                ReadImage(&ImageIn,path.toStdString().c_str());
                ConcatObj(IconicInputParamsData,ImageIn,&IconicInputParamsData);
            }
        }


        // 调用算子执行方法
        if(ProgramPath.isEmpty())
        {
            QMessageBox::critical(nullptr,"Warning"," NO ProgramPath Send",QMessageBox::Ok);
            return;
        }
        else if(CtrlInputParamsCount!=0 && CtrlInputParamsData.Length() == 0)
        {
            QMessageBox::critical(nullptr,"Warning"," NO CtrlInputParamsData Send",QMessageBox::Ok);
            return;
        }
        else
        {
            Alg.ExcuteProcedure(ProgramPath,CtrlInputParamsData,IconicInputParamsData,CtrlOutputParamsData,IconicOutputParamsData,TypeList,IsRunSuccess);
        }

        if(IsRunSuccess)
        {
            // 获取输出图片并显示再界面上
            for (int i = 0; i < IconicOutputParamsData.CountObj(); i++) {
                SelectObj(IconicOutputParamsData,&SelectedObj,i+1);
                WriteImage(SelectedObj,"jpg",0,"D:/QtAlgorithm/Algorithm/outimage.jpg");
                loadAndDisplayImage("D:/QtAlgorithm/Algorithm/outimage.jpg");
            }
            // 获取输出控制参数数值类型为real，字符串类型为string
            int i1 = 0;
            for (const std::string& paramName : m_CtrlOutputParams)
            {
                if(TypeList[i1] == "string")
                {
                    QString CtrlOutputParam = QString(CtrlOutputParamsData[i1].S());
                    updateControlOutputResult(QString::fromStdString(paramName),CtrlOutputParam);
                    i1=i1+1;
                }
                else if(TypeList[i1] == "real")
                {
                    QString CtrlOutputParam = QString::number(CtrlOutputParamsData[i1].D());
                    updateControlOutputResult(QString::fromStdString(paramName),CtrlOutputParam);
                    i1=i1+1;
                }
            }
        }
        else
        {
            // 获取输出控制参数数值类型为real，字符串类型为string
            int i1 = 0;
            for (const std::string& paramName : m_CtrlOutputParams)
            {
                if(TypeList[i1] == "string")
                {
                    QString CtrlOutputParam = "Error";
                    updateControlOutputResult(QString::fromStdString(paramName),CtrlOutputParam);
                    i1=i1+1;
                }
                else if(TypeList[i1] == "real")
                {
                    QString CtrlOutputParam = "Error";
                    updateControlOutputResult(QString::fromStdString(paramName),CtrlOutputParam);
                    i1=i1+1;
                }
            }
            return;
        }
    }
    else if(m_Type == "Python")
    {
        QList<QString> TypeList;
        QList<QString> CtrlInputParamsData;
        QList<cv::Mat> IconicInputParamsData;
        QList<QString> CtrlOutputParamsData;
        QList<cv::Mat> IconicOutputParamsData;
        QList<QString>imgpath;
        cv::Mat ImageIn;
        // 遍历输入的所有参数
        for (QLineEdit *lineEdit : m_ctrlInputLineEdits)
        {
            QString text = lineEdit->text();
            CtrlInputParamsData.push_back(text);

        }

        // 遍历选择的图像路径并添加到list imgpath中
        for (QLineEdit *pathEdit : ImagePathLineEdits)
        {
            try
            {
                QString text = pathEdit->text();
                imgpath.push_back(text);
            } catch (exception &e)
            {
                QMessageBox::critical(nullptr,"Warning"," Table Initialize Failed",QMessageBox::Ok);
                return;
            }
        }
        //遍历list imgpath 读取图像并将所有图像加载入同一个Obj容器中
        for (QString path : imgpath)
        {
            if(path == "")
            {
                QMessageBox::critical(nullptr,"Warning"," NO IconicInputParamsData Send",QMessageBox::Ok);
                return;
            }
            else
            {
                ImageIn = cv::imread(path.toStdString().c_str(),cv::IMREAD_COLOR_BGR);
                IconicInputParamsData.push_back(ImageIn);
            }
        }
        // 调用算子执行方法
        if(ProgramPath.isEmpty())
        {
            QMessageBox::critical(nullptr,"Warning"," NO ProgramPath Send",QMessageBox::Ok);
            return;
        }
        else if(CtrlInputParamsCount!=0 && CtrlInputParamsData.size() == 0)
        {
            QMessageBox::critical(nullptr,"Warning"," NO CtrlInputParamsData Send",QMessageBox::Ok);
            return;
        }
        else
        {
            Alg.ExcuteProcedure(ProgramPath,CtrlInputParamsData,IconicInputParamsData,CtrlOutputParamsData,IconicOutputParamsData,TypeList,IsRunSuccess);
        }

        if(IsRunSuccess)
        {
            // 获取输出图片并显示再界面上
            for (int i = 0; i < IconicOutputParamsData.size(); i++) {
                cv::imwrite("D:/QtAlgorithm/Algorithm/outimage.jpg",IconicOutputParamsData[i]);
                loadAndDisplayImage("D:/QtAlgorithm/Algorithm/outimage.jpg");

                // 获取输出控制参数数值类型为real，字符串类型为string
                int i1 = 0;
                for (const std::string& paramName : m_CtrlOutputParams)
                {
                    updateControlOutputResult(QString::fromStdString(paramName),CtrlOutputParamsData[i1]);
                    i1=i1+1;
                }
            }
        }
        else
        {
            // 获取输出控制参数数值类型为real，字符串类型为string
            for (const std::string& paramName : m_CtrlOutputParams)
            {
                updateControlOutputResult(QString::fromStdString(paramName),"Error");
            }
            return;
        }
    }




}
