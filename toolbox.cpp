#include "toolbox.h"
#include "ui_toolbox.h"
#include <algorithmparam.h>
#include <Algorithm.h>
#include <halconcpp/HalconCpp.h>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QUuid>
#include <QToolButton>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
using namespace HalconCpp;
using namespace HDevEngineCpp;
using namespace std;

toolbox::toolbox(QString Type,QWidget *parent) : QDialog(parent), ui(new Ui::toolbox)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"AlgorothmPatn", "Operation"});
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    // 定义传入算法类型
    m_Type = Type;
    // 启动时加载数据
    loadTableDataFromJson();
}
toolbox::~toolbox() {
    delete ui;
}

/// <summary>
/// 设置列宽
/// </summary>
void toolbox::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    // 计算当前表格可用宽度
    int tableWidth = ui->tableWidget->viewport()->width();
    // 设置列宽比例为 1:1
    ui->tableWidget->setColumnWidth(0, tableWidth * 0.5);
    ui->tableWidget->setColumnWidth(1, tableWidth * 0.5);
}
/// <summary>
/// 更新对应算法路径到界面
/// </summary>
void toolbox::updateProgramPath(const QString &ProgramPath, const QString &uuid) {
    // 通过保存的uuid匹配对应的行
    if (m_uuidToLineEdit.contains(uuid)) {
        QLineEdit *lineEdit = m_uuidToLineEdit[uuid];
        lineEdit->setText(ProgramPath);
    }
}
/// <summary>
/// 将界面表的数据存储到Json
/// </summary>
void toolbox::saveTableDataToJson() {
    QJsonArray jsonArray;
    QFile file;
    // 定义保存对象类型
    if(m_Type == "Halcon")
    {
        file.setFileName("D:/QtAlgorithm/Algorithm/halcon_saved_data.json");
    }
    else if(m_Type == "Python")
    {
        file.setFileName("D:/QtAlgorithm/Algorithm/python_saved_data.json");
    }
    // 遍历 UUID 到 LineEdit 的映射
    for (auto it = m_uuidToLineEdit.begin(); it != m_uuidToLineEdit.end(); ++it)
    {
        const QString& uuid = it.key();
        QLineEdit* lineEdit = it.value();
        // 创建 JSON 对象并添加字段
        QJsonObject jsonObj;
        jsonObj["AlgorithmPath"] = lineEdit->text();
        jsonObj["AlgorithmType"] = m_Type;
        jsonObj["uuid"] = uuid;
        jsonArray.append(jsonObj);
    }
    QJsonDocument jsonDoc(jsonArray);
    // 保存参数
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
        QMessageBox::information(this, "Success", "Parameters saved.");
        qDebug() << "Save Success";
    }

}
/// <summary>
/// 从Json读取参数
/// </summary>
void toolbox::loadTableDataFromJson()
{
    QFile file;
    // 定义保存对象类型
    if(m_Type == "Halcon")
    {
        file.setFileName("D:/QtAlgorithm/Algorithm/halcon_saved_data.json");
    }
    else if(m_Type == "Python")
    {
        file.setFileName("D:/QtAlgorithm/Algorithm/python_saved_data.json");
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No File Saved";
        return;
    }
    //建立Json对象
    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonArray jsonArray = jsonDoc.array();

    ui->tableWidget->setRowCount(0);
    //遍历Json内容
    for (const QJsonValue &value : jsonArray) {
        QJsonObject obj = value.toObject();
        QString AlgorithmPath = obj["AlgorithmPath"].toString();

        // 插入新行
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        // 读取保存的 UUID，此处只能读取uuid，不可新建，否贼会读取错误
        QString uuid = obj["uuid"].toString();

        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setText(AlgorithmPath);
        lineEdit->setReadOnly(true);
        ui->tableWidget->setCellWidget(row, 0, lineEdit);

        //绑定uuid和lineEdit
        m_uuidToLineEdit[uuid] = lineEdit;

        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);

        // Load 按钮
        QToolButton *loadButton = new QToolButton();
        loadButton->setText("Load");
        layout->addWidget(loadButton);
        loadButton->setStyleSheet("QToolButton { padding: 2px 8px; }");

        // Delete 按钮
        QToolButton *deleteButton = new QToolButton();
        deleteButton->setText("Delete");
        layout->addWidget(deleteButton);
        deleteButton->setStyleSheet("QToolButton { padding: 2px 8px; color: red; }");

        widget->setLayout(layout);
        ui->tableWidget->setCellWidget(row, 1, widget);

        // Load按钮点击信号
        connect(loadButton, &QToolButton::clicked, [this, uuid]() {
            AlgorithmParam *dialog = new AlgorithmParam(this);
            // 传递uuid到算子界面
            dialog->setAlgoinfoInfo(uuid,m_Type);
            connect(dialog, &AlgorithmParam::ProgramPathLoaded, this, &toolbox::updateProgramPath);
            dialog->exec();
        });

        // Delete按钮点击信号
        connect(deleteButton, &QToolButton::clicked, [this, uuid, deleteButton]() {
            QWidget *widget = qobject_cast<QWidget*>(deleteButton->parent());
            if (!widget) return;
            for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
                QWidget *cellWidget = ui->tableWidget->cellWidget(row, 1);
                if (cellWidget == widget) {
                    ui->tableWidget->removeRow(row);
                    break;
                }
            }

            // 从映射中移除 uuid
            m_uuidToLineEdit.remove(uuid);
        });
    }

    file.close();
}
/// <summary>
/// 保存数据
/// </summary>
void toolbox::on_SaveDataButton_clicked()
{
    saveTableDataToJson();
}
/// <summary>
/// 添加算法行
/// </summary>
void toolbox::on_AddAlgorithmButton_clicked()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    // 创建uuid，每个uuid对应一行
    QString uuid = QUuid::createUuid().toString();

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setText("new algorithm");
    lineEdit->setReadOnly(true);
    ui->tableWidget->setCellWidget(row, 0, lineEdit);

    // 绑定uuid和lineEdit
    m_uuidToLineEdit[uuid] = lineEdit;

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    // Load 按钮
    QToolButton *loadButton = new QToolButton();
    loadButton->setText("Load");
    layout->addWidget(loadButton);
    loadButton->setStyleSheet("QToolButton { padding: 2px 8px; }");

    // Delete 按钮
    QToolButton *deleteButton = new QToolButton();
    deleteButton->setText("Delete");
    layout->addWidget(deleteButton);
    deleteButton->setStyleSheet("QToolButton { padding: 2px 8px; color: red; }");

    widget->setLayout(layout);
    ui->tableWidget->setCellWidget(row, 1, widget);

    // Load按钮点击信号
    connect(loadButton, &QToolButton::clicked, [this, uuid]()
    {
        AlgorithmParam *dialog = new AlgorithmParam(this);
        // 将算法信息传递到算子界面
        dialog->setAlgoinfoInfo(uuid,m_Type);
        connect(dialog, &AlgorithmParam::ProgramPathLoaded, this, &toolbox::updateProgramPath);
        dialog->exec();
    });

    // Delete按钮点击信号
    connect(deleteButton, &QToolButton::clicked, [this, uuid, deleteButton]() {
        // 删除行
        QWidget *widget = qobject_cast<QWidget*>(deleteButton->parent());
        if (!widget) return;

        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QWidget *cellWidget = ui->tableWidget->cellWidget(row, 1);
            if (cellWidget == widget) {
                ui->tableWidget->removeRow(row);
                break;
            }
        }

        // 从映射中移除 uuid
        m_uuidToLineEdit.remove(uuid);
    });



}
