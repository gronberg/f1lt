/********************************************************************************
** Form generated from reading UI file 'sessiondatawidget.ui'
**
** Created: Wed Jul 11 16:02:22 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SESSIONDATAWIDGET_H
#define UI_SESSIONDATAWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SessionDataWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *tableWidget_5;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget_3;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_2;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *tableWidget_4;

    void setupUi(QWidget *SessionDataWidget)
    {
        if (SessionDataWidget->objectName().isEmpty())
            SessionDataWidget->setObjectName(QString::fromUtf8("SessionDataWidget"));
        SessionDataWidget->resize(575, 568);
        verticalLayout = new QVBoxLayout(SessionDataWidget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(SessionDataWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QPalette palette;
        QBrush brush(QColor(20, 20, 20, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(240, 240, 240, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        tabWidget->setPalette(palette);
        tabWidget->setMovable(false);
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_5 = new QVBoxLayout(tab_4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tableWidget_5 = new QTableWidget(tab_4);
        if (tableWidget_5->columnCount() < 1)
            tableWidget_5->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableWidget_5->setObjectName(QString::fromUtf8("tableWidget_5"));
        QPalette palette1;
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush3(QColor(130, 130, 130, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        tableWidget_5->setPalette(palette1);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        tableWidget_5->setFont(font);
        tableWidget_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_5->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget_5->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_5->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_5->setShowGrid(false);
        tableWidget_5->horizontalHeader()->setVisible(false);
        tableWidget_5->verticalHeader()->setVisible(false);

        verticalLayout_5->addWidget(tableWidget_5);

        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableWidget_3 = new QTableWidget(tab_2);
        if (tableWidget_3->columnCount() < 8)
            tableWidget_3->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(5, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(6, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(7, __qtablewidgetitem8);
        tableWidget_3->setObjectName(QString::fromUtf8("tableWidget_3"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush4(QColor(73, 73, 73, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        QBrush brush5(QColor(175, 175, 175, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        tableWidget_3->setPalette(palette2);
        tableWidget_3->setFont(font);
        tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_3->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget_3->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_3->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_3->setShowGrid(false);
        tableWidget_3->horizontalHeader()->setVisible(false);
        tableWidget_3->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(tableWidget_3);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
#ifndef Q_OS_MAC
        verticalLayout_2->setContentsMargins(9, 9, 9, 9);
#endif
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget_2 = new QTableWidget(tab);
        if (tableWidget_2->columnCount() < 5)
            tableWidget_2->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem13);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Highlight, brush4);
        QBrush brush6(QColor(27, 27, 27, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Highlight, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush6);
        tableWidget_2->setPalette(palette3);
        tableWidget_2->setFont(font);
        tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_2->setAlternatingRowColors(true);
        tableWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_2->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_2->setShowGrid(false);
        tableWidget_2->horizontalHeader()->setVisible(false);
        tableWidget_2->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(tableWidget_2);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        tableWidget_4 = new QTableWidget(tab_3);
        if (tableWidget_4->columnCount() < 6)
            tableWidget_4->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(1, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(2, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(3, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(4, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(5, __qtablewidgetitem19);
        tableWidget_4->setObjectName(QString::fromUtf8("tableWidget_4"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Highlight, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Highlight, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        tableWidget_4->setPalette(palette4);
        tableWidget_4->setFont(font);
        tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_4->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget_4->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_4->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_4->setShowGrid(false);
        tableWidget_4->horizontalHeader()->setVisible(false);
        tableWidget_4->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(tableWidget_4);

        tabWidget->addTab(tab_3, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(SessionDataWidget);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(SessionDataWidget);
    } // setupUi

    void retranslateUi(QWidget *SessionDataWidget)
    {
        SessionDataWidget->setWindowTitle(QApplication::translate("SessionDataWidget", "Form", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("SessionDataWidget", "Event info", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_3->horizontalHeaderItem(5);
        ___qtablewidgetitem->setText(QApplication::translate("SessionDataWidget", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SessionDataWidget", "Fastest laps", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SessionDataWidget", "Speed records", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("SessionDataWidget", "Fastest pit stops", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SessionDataWidget: public Ui_SessionDataWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SESSIONDATAWIDGET_H
