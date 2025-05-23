#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "run.h"
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
namespace Ui {
class test;
}

class test : public QDialog
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();

private slots:
    void on_Run_clicked();

    void on_LoadImage_clicked();

private:
    Ui::test *ui;
};

#endif // TEST_H
