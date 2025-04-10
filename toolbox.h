#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
namespace Ui {
class toolbox;
}

class toolbox : public QDialog {
    Q_OBJECT

public:
    explicit toolbox(QString Type,QWidget *parent = nullptr);
    ~toolbox();
    void updateProgramPath(const QString &ProgramPath, const QString &uuid);
    void saveTableDataToJson();
    void loadTableDataFromJson();

private slots:

    void on_AddAlgorithmButton_clicked();

    void on_SaveDataButton_clicked();

private:
    Ui::toolbox *ui;
    QMap<QString, QLineEdit*> m_uuidToLineEdit;
    QString m_Type;

protected:

    void resizeEvent(QResizeEvent *event) override;
};
