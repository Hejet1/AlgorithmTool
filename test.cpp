#include "test.h"
#include "ui_test.h"
Run run;

test::test(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::test)
{
    ui->setupUi(this);
}

test::~test()
{
    delete ui;
}

void test::on_pushButton_clicked()
{
    QList<cv::Mat> imgs,IconicResults;
    QList <QString> CtrlResults;
    cv::Mat img = cv::imread("F:\\HD2423\\2024-11-14-2-50-939.bmp");
    imgs.push_back(img);
    run.run("Predict",imgs,CtrlResults,IconicResults);
}

