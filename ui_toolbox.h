/********************************************************************************
** Form generated from reading UI file 'toolbox.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLBOX_H
#define UI_TOOLBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_toolbox
{
public:
    QTableWidget *tableWidget;
    QPushButton *AddAlgorithmButton;
    QPushButton *SaveDataButton;

    void setupUi(QDialog *toolbox)
    {
        if (toolbox->objectName().isEmpty())
            toolbox->setObjectName(QString::fromUtf8("toolbox"));
        toolbox->resize(393, 566);
        tableWidget = new QTableWidget(toolbox);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 60, 371, 441));
        AddAlgorithmButton = new QPushButton(toolbox);
        AddAlgorithmButton->setObjectName(QString::fromUtf8("AddAlgorithmButton"));
        AddAlgorithmButton->setGeometry(QRect(10, 20, 101, 31));
        SaveDataButton = new QPushButton(toolbox);
        SaveDataButton->setObjectName(QString::fromUtf8("SaveDataButton"));
        SaveDataButton->setGeometry(QRect(150, 510, 91, 41));

        retranslateUi(toolbox);

        QMetaObject::connectSlotsByName(toolbox);
    } // setupUi

    void retranslateUi(QDialog *toolbox)
    {
        toolbox->setWindowTitle(QCoreApplication::translate("toolbox", "Dialog", nullptr));
        AddAlgorithmButton->setText(QCoreApplication::translate("toolbox", "AddAlgorithm", nullptr));
        SaveDataButton->setText(QCoreApplication::translate("toolbox", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class toolbox: public Ui_toolbox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBOX_H
