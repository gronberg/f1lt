/********************************************************************************
** Form generated from reading UI file 'chartwidget.ui'
**
** Created: Wed Jul 11 16:02:22 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTWIDGET_H
#define UI_CHARTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartWidget
{
public:

    void setupUi(QWidget *ChartWidget)
    {
        if (ChartWidget->objectName().isEmpty())
            ChartWidget->setObjectName(QString::fromUtf8("ChartWidget"));
        ChartWidget->resize(400, 300);

        retranslateUi(ChartWidget);

        QMetaObject::connectSlotsByName(ChartWidget);
    } // setupUi

    void retranslateUi(QWidget *ChartWidget)
    {
        ChartWidget->setWindowTitle(QApplication::translate("ChartWidget", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ChartWidget: public Ui_ChartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTWIDGET_H
