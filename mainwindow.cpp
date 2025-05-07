#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <Algorithm.h>
#include <halconcpp/HalconCpp.h>
#include <QLineEdit>
#include "toolbox.h"
#include "test.h"

using namespace HalconCpp;
using namespace HDevEngineCpp;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qputenv("PATH", "D:\\ProgramData\\Anaconda\\envs\\yolo;" + qgetenv("PATH"));
    qputenv("PYTHONHOME", "D:\\ProgramData\\Anaconda\\envs\\yolo");
    Algorithm();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    toolbox *tool = new toolbox("Halcon",this);
    tool->exec();
}


void MainWindow::on_PythonAlgorithmTool_clicked()
{
    toolbox *tool = new toolbox("Python",this);
    tool->exec();
}


void MainWindow::on_pushButton_clicked()
{
    test *test1 = new test(this);
    test1->exec();
}

