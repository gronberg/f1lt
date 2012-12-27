/********************************************************************************
** Form generated from reading UI file 'sessiondatawidget.ui'
**
** Created: Thu Dec 27 22:24:47 2012
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SESSIONDATAWIDGET_H
#define UI_SESSIONDATAWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
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
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QWidget *infoWidget;
    QGridLayout *gridLayout;
    QLabel *s2TimeLabel;
    QLabel *s1DriverLabel;
    QLabel *label_3;
    QLabel *s2LapLabel;
    QLabel *s3LapLabel;
    QLabel *s1LapLabel;
    QLabel *label_2;
    QLabel *s3DriverLabel;
    QLabel *flDriverLabel;
    QLabel *label;
    QLabel *flTimeLabel;
    QLabel *s1TimeLabel;
    QLabel *theoreticalTimeLabel;
    QLabel *s2DriverLabel;
    QLabel *label_4;
    QLabel *flLapLabel;
    QLabel *label_5;
    QLabel *s3TimeLabel;
    QLabel *label_6;
    QTableView *fastestLapsTable;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTableView *speedRecordsTable;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableView *pitStopsTable;

    void setupUi(QWidget *SessionDataWidget)
    {
        if (SessionDataWidget->objectName().isEmpty())
            SessionDataWidget->setObjectName(QString::fromUtf8("SessionDataWidget"));
        SessionDataWidget->resize(575, 762);
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
        scrollArea = new QScrollArea(tab_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Button, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush);
        scrollArea->setPalette(palette2);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 551, 713));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setContentsMargins(2, 2, 2, 2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        infoWidget = new QWidget(scrollAreaWidgetContents);
        infoWidget->setObjectName(QString::fromUtf8("infoWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoWidget->sizePolicy().hasHeightForWidth());
        infoWidget->setSizePolicy(sizePolicy);
        infoWidget->setMinimumSize(QSize(0, 0));
        QPalette palette3;
        QBrush brush4(QColor(150, 150, 150, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        infoWidget->setPalette(palette3);
        gridLayout = new QGridLayout(infoWidget);
        gridLayout->setContentsMargins(4, 4, 4, 4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(4);
        s2TimeLabel = new QLabel(infoWidget);
        s2TimeLabel->setObjectName(QString::fromUtf8("s2TimeLabel"));
        QPalette palette4;
        QBrush brush5(QColor(255, 0, 255, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2TimeLabel->setPalette(palette4);
        s2TimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s2TimeLabel, 2, 1, 1, 1);

        s1DriverLabel = new QLabel(infoWidget);
        s1DriverLabel->setObjectName(QString::fromUtf8("s1DriverLabel"));
        QPalette palette5;
        QBrush brush6(QColor(220, 220, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1DriverLabel->setPalette(palette5);

        gridLayout->addWidget(s1DriverLabel, 1, 2, 1, 1);

        label_3 = new QLabel(infoWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        s2LapLabel = new QLabel(infoWidget);
        s2LapLabel->setObjectName(QString::fromUtf8("s2LapLabel"));
        QPalette palette6;
        QBrush brush7(QColor(255, 255, 0, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2LapLabel->setPalette(palette6);
        s2LapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s2LapLabel, 2, 3, 1, 1);

        s3LapLabel = new QLabel(infoWidget);
        s3LapLabel->setObjectName(QString::fromUtf8("s3LapLabel"));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3LapLabel->setPalette(palette7);
        s3LapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s3LapLabel, 4, 3, 1, 1);

        s1LapLabel = new QLabel(infoWidget);
        s1LapLabel->setObjectName(QString::fromUtf8("s1LapLabel"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1LapLabel->setPalette(palette8);
        s1LapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s1LapLabel, 1, 3, 1, 1);

        label_2 = new QLabel(infoWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        s3DriverLabel = new QLabel(infoWidget);
        s3DriverLabel->setObjectName(QString::fromUtf8("s3DriverLabel"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3DriverLabel->setPalette(palette9);

        gridLayout->addWidget(s3DriverLabel, 4, 2, 1, 1);

        flDriverLabel = new QLabel(infoWidget);
        flDriverLabel->setObjectName(QString::fromUtf8("flDriverLabel"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        flDriverLabel->setPalette(palette10);

        gridLayout->addWidget(flDriverLabel, 0, 2, 1, 1);

        label = new QLabel(infoWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        flTimeLabel = new QLabel(infoWidget);
        flTimeLabel->setObjectName(QString::fromUtf8("flTimeLabel"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        flTimeLabel->setPalette(palette11);
        flTimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(flTimeLabel, 0, 1, 1, 1);

        s1TimeLabel = new QLabel(infoWidget);
        s1TimeLabel->setObjectName(QString::fromUtf8("s1TimeLabel"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1TimeLabel->setPalette(palette12);
        s1TimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s1TimeLabel, 1, 1, 1, 1);

        theoreticalTimeLabel = new QLabel(infoWidget);
        theoreticalTimeLabel->setObjectName(QString::fromUtf8("theoreticalTimeLabel"));
        QPalette palette13;
        QBrush brush8(QColor(0, 255, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        theoreticalTimeLabel->setPalette(palette13);
        theoreticalTimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(theoreticalTimeLabel, 5, 1, 1, 1);

        s2DriverLabel = new QLabel(infoWidget);
        s2DriverLabel->setObjectName(QString::fromUtf8("s2DriverLabel"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2DriverLabel->setPalette(palette14);

        gridLayout->addWidget(s2DriverLabel, 2, 2, 1, 1);

        label_4 = new QLabel(infoWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        flLapLabel = new QLabel(infoWidget);
        flLapLabel->setObjectName(QString::fromUtf8("flLapLabel"));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        flLapLabel->setPalette(palette15);
        flLapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(flLapLabel, 0, 3, 1, 1);

        label_5 = new QLabel(infoWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        s3TimeLabel = new QLabel(infoWidget);
        s3TimeLabel->setObjectName(QString::fromUtf8("s3TimeLabel"));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3TimeLabel->setPalette(palette16);
        s3TimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s3TimeLabel, 4, 1, 1, 1);

        label_6 = new QLabel(infoWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 6, 0, 1, 4);


        verticalLayout_6->addWidget(infoWidget);

        fastestLapsTable = new QTableView(scrollAreaWidgetContents);
        fastestLapsTable->setObjectName(QString::fromUtf8("fastestLapsTable"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fastestLapsTable->sizePolicy().hasHeightForWidth());
        fastestLapsTable->setSizePolicy(sizePolicy1);
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush9(QColor(73, 73, 73, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette17.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        QBrush brush10(QColor(30, 30, 30, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette17.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette17.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        QBrush brush11(QColor(175, 175, 175, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette17.setBrush(QPalette::Disabled, QPalette::Highlight, brush11);
        palette17.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        fastestLapsTable->setPalette(palette17);
        fastestLapsTable->setFrameShape(QFrame::NoFrame);
        fastestLapsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        fastestLapsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        fastestLapsTable->setAutoScroll(true);
        fastestLapsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fastestLapsTable->setAlternatingRowColors(true);
        fastestLapsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        fastestLapsTable->setShowGrid(false);
        fastestLapsTable->horizontalHeader()->setVisible(false);
        fastestLapsTable->verticalHeader()->setVisible(false);

        verticalLayout_6->addWidget(fastestLapsTable);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        speedRecordsTable = new QTableView(tab);
        speedRecordsTable->setObjectName(QString::fromUtf8("speedRecordsTable"));
        sizePolicy1.setHeightForWidth(speedRecordsTable->sizePolicy().hasHeightForWidth());
        speedRecordsTable->setSizePolicy(sizePolicy1);
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::Base, brush);
        palette18.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        palette18.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        palette18.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette18.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        palette18.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette18.setBrush(QPalette::Disabled, QPalette::Highlight, brush11);
        palette18.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        speedRecordsTable->setPalette(palette18);
        speedRecordsTable->setFrameShape(QFrame::NoFrame);
        speedRecordsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        speedRecordsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        speedRecordsTable->setAutoScroll(true);
        speedRecordsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        speedRecordsTable->setAlternatingRowColors(true);
        speedRecordsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        speedRecordsTable->setShowGrid(false);
        speedRecordsTable->horizontalHeader()->setVisible(false);
        speedRecordsTable->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(speedRecordsTable);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pitStopsTable = new QTableView(tab_3);
        pitStopsTable->setObjectName(QString::fromUtf8("pitStopsTable"));
        sizePolicy1.setHeightForWidth(pitStopsTable->sizePolicy().hasHeightForWidth());
        pitStopsTable->setSizePolicy(sizePolicy1);
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::Base, brush);
        palette19.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        palette19.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        palette19.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette19.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        palette19.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette19.setBrush(QPalette::Disabled, QPalette::Highlight, brush11);
        palette19.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        pitStopsTable->setPalette(palette19);
        pitStopsTable->setFrameShape(QFrame::NoFrame);
        pitStopsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        pitStopsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pitStopsTable->setAutoScroll(true);
        pitStopsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        pitStopsTable->setAlternatingRowColors(true);
        pitStopsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        pitStopsTable->setShowGrid(false);
        pitStopsTable->horizontalHeader()->setVisible(false);
        pitStopsTable->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(pitStopsTable);

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
        s2TimeLabel->setText(QString());
        s1DriverLabel->setText(QString());
        label_3->setText(QApplication::translate("SessionDataWidget", "Fastest S2:", 0, QApplication::UnicodeUTF8));
        s2LapLabel->setText(QString());
        s3LapLabel->setText(QString());
        s1LapLabel->setText(QString());
        label_2->setText(QApplication::translate("SessionDataWidget", "Fastest S1:", 0, QApplication::UnicodeUTF8));
        s3DriverLabel->setText(QString());
        flDriverLabel->setText(QString());
        label->setText(QApplication::translate("SessionDataWidget", "Fastest lap:", 0, QApplication::UnicodeUTF8));
        flTimeLabel->setText(QString());
        s1TimeLabel->setText(QString());
        theoreticalTimeLabel->setText(QString());
        s2DriverLabel->setText(QString());
        label_4->setText(QApplication::translate("SessionDataWidget", "Fastest S3:", 0, QApplication::UnicodeUTF8));
        flLapLabel->setText(QString());
        label_5->setText(QApplication::translate("SessionDataWidget", "Approximate:", 0, QApplication::UnicodeUTF8));
        s3TimeLabel->setText(QString());
        label_6->setText(QApplication::translate("SessionDataWidget", "Best laps", 0, QApplication::UnicodeUTF8));
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
