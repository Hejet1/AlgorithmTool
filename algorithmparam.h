#include <QDialog>
#include <halconcpp/HalconCpp.h>
#include <qhalconwindow.h>
#include <QLineEdit>
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
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

using namespace std;

namespace Ui {
class AlgorithmParam;
}

class AlgorithmParam : public QDialog {
    Q_OBJECT

public:
    explicit AlgorithmParam(QWidget *parent = nullptr);
    ~AlgorithmParam();

    // 填充参数表格
    void populateParameters(QString ProgramPath,QString ProcName,std::list<std::string> CtrlInputParams, std::list<std::string> IconicInputParams,
                            std::list<std::string> CtrlOutputParams,std::list<std::string> IconicOutputParams);
    void addSectionHeader(const QString &title);
    void addControlInputRow(const QString &name);
    void addImageInputRow(const QString &name);
    void addControlOutputRow(const QString &name);
    void addImageOutputRow(const QString &name);
    void loadAndDisplayImage(const QString &path);
    void addProcNameRow(const QString &label, const QString &ProcName);
    void updateControlOutputResult(const QString &name, const QString &result);
    void setAlgoinfoInfo(const QString &uuid,const QString &Type);
    void loadSavedParameters();
    void addProgramPathRow(const QString &label, const QString &ProgramPath);
signals:
    void ProgramPathLoaded(const QString &ProgramPath, const QString &uuid);

private slots:

    void on_RunProcedureButton_clicked();

    void on_LoadAlgorithmButton_clicked();

    void on_SaveParameterButton_clicked();

private:
    QString m_uuid;
    QString m_Type;
    Ui::AlgorithmParam *ui;
    QHalconWindow *Disp;
    HalconCpp::HTuple m_hvHandle,hvImageHeight,hvImageWidth;
    HalconCpp::HObject Image;
    QList<QLineEdit*> m_ctrlInputLineEdits;
    QList<QLineEdit*> ImagePathLineEdits;
    int CtrlInputParamsCount; //输入控制参数计数，用来校验参数加载是否与表一致
    int IconicInputParamsCount;
    QString ProgramPath;
    std::list<std::string> m_CtrlOutputParams;
    std::list<std::string> CtrlInputParams;
    std::list<std::string> IconicInputParams;
    std::list<std::string> CtrlOutputParams;
    std::list<std::string> IconicOutputParams;
    std::list<std::string> m_storedCtrlInputs;
    std::list<std::string> m_storedIconicInputs;
    std::list<std::string> m_storedCtrlOutputs;
    std::list<std::string> m_storedIconicOutputs;
    QString ProcName,ProcType;
    bool IsRunSuccess,IsGetSuccess;

protected:

    void closeEvent(QCloseEvent *event) override;

};

