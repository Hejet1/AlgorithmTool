/********************************************************************************
** Form generated from reading UI file 'test.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_H
#define UI_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <customgraphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_test
{
public:
    QPushButton *Run;
    QTextEdit *ResultTextEdit;
    QPushButton *LoadImage;
    QTextEdit *ImageTextEdit;
    QLineEdit *ProcNameLineEdit;
    CustomGraphicsView *ImageGraphicsView;

    void setupUi(QDialog *test)
    {
        if (test->objectName().isEmpty())
            test->setObjectName(QString::fromUtf8("test"));
        test->resize(495, 526);
        Run = new QPushButton(test);
        Run->setObjectName(QString::fromUtf8("Run"));
        Run->setGeometry(QRect(390, 130, 75, 23));
        ResultTextEdit = new QTextEdit(test);
        ResultTextEdit->setObjectName(QString::fromUtf8("ResultTextEdit"));
        ResultTextEdit->setGeometry(QRect(20, 90, 341, 71));
        LoadImage = new QPushButton(test);
        LoadImage->setObjectName(QString::fromUtf8("LoadImage"));
        LoadImage->setGeometry(QRect(380, 30, 75, 23));
        ImageTextEdit = new QTextEdit(test);
        ImageTextEdit->setObjectName(QString::fromUtf8("ImageTextEdit"));
        ImageTextEdit->setGeometry(QRect(20, 10, 341, 71));
        ProcNameLineEdit = new QLineEdit(test);
        ProcNameLineEdit->setObjectName(QString::fromUtf8("ProcNameLineEdit"));
        ProcNameLineEdit->setGeometry(QRect(370, 100, 113, 21));
        ImageGraphicsView = new CustomGraphicsView(test);
        ImageGraphicsView->setObjectName(QString::fromUtf8("ImageGraphicsView"));
        ImageGraphicsView->setGeometry(QRect(20, 190, 441, 311));

        retranslateUi(test);

        QMetaObject::connectSlotsByName(test);
    } // setupUi

    void retranslateUi(QDialog *test)
    {
        test->setWindowTitle(QCoreApplication::translate("test", "Dialog", nullptr));
        Run->setText(QCoreApplication::translate("test", "Run", nullptr));
        LoadImage->setText(QCoreApplication::translate("test", "LoadImage", nullptr));
    } // retranslateUi

};

namespace Ui {
    class test: public Ui_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_H
