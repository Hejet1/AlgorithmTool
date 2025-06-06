/********************************************************************************
** Form generated from reading UI file 'algorithmparam.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALGORITHMPARAM_H
#define UI_ALGORITHMPARAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <customgraphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_AlgorithmParam
{
public:
    QTableWidget *tableWidget;
    QPushButton *RunProcedureButton;
    QPushButton *SaveParameterButton;
    QPushButton *LoadAlgorithmButton;
    CustomGraphicsView *ImageGraphicsView;

    void setupUi(QDialog *AlgorithmParam)
    {
        if (AlgorithmParam->objectName().isEmpty())
            AlgorithmParam->setObjectName(QString::fromUtf8("AlgorithmParam"));
        AlgorithmParam->resize(972, 528);
        tableWidget = new QTableWidget(AlgorithmParam);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(490, 20, 451, 461));
        RunProcedureButton = new QPushButton(AlgorithmParam);
        RunProcedureButton->setObjectName(QString::fromUtf8("RunProcedureButton"));
        RunProcedureButton->setGeometry(QRect(190, 480, 111, 31));
        SaveParameterButton = new QPushButton(AlgorithmParam);
        SaveParameterButton->setObjectName(QString::fromUtf8("SaveParameterButton"));
        SaveParameterButton->setGeometry(QRect(320, 480, 111, 31));
        LoadAlgorithmButton = new QPushButton(AlgorithmParam);
        LoadAlgorithmButton->setObjectName(QString::fromUtf8("LoadAlgorithmButton"));
        LoadAlgorithmButton->setGeometry(QRect(60, 480, 111, 31));
        ImageGraphicsView = new CustomGraphicsView(AlgorithmParam);
        ImageGraphicsView->setObjectName(QString::fromUtf8("ImageGraphicsView"));
        ImageGraphicsView->setGeometry(QRect(30, 20, 431, 441));
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        ImageGraphicsView->setBackgroundBrush(brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::NoBrush);
        ImageGraphicsView->setForegroundBrush(brush1);

        retranslateUi(AlgorithmParam);

        QMetaObject::connectSlotsByName(AlgorithmParam);
    } // setupUi

    void retranslateUi(QDialog *AlgorithmParam)
    {
        AlgorithmParam->setWindowTitle(QCoreApplication::translate("AlgorithmParam", "Dialog", nullptr));
        RunProcedureButton->setText(QCoreApplication::translate("AlgorithmParam", "Run", nullptr));
        SaveParameterButton->setText(QCoreApplication::translate("AlgorithmParam", "SaveParams", nullptr));
        LoadAlgorithmButton->setText(QCoreApplication::translate("AlgorithmParam", "LoadProgram", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlgorithmParam: public Ui_AlgorithmParam {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALGORITHMPARAM_H
