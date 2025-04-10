#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <halconcpp/HalconCpp.h>
#include <QMainWindow>
#include <qhalconwindow.h>
#include <QTableWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_2_clicked();

    void on_PythonAlgorithmTool_clicked();

private:
    Ui::MainWindow *ui;
    HalconCpp::HTuple m_hvHandle,hvImageHeight,hvImageWidth;
    HalconCpp::HObject m_hImg;
    QHalconWindow *Disp;
    int CtrlParamsNum;



};
#endif // MAINWINDOW_H
