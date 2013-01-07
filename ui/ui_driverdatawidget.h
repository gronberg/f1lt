/********************************************************************************
** Form generated from reading UI file 'driverdatawidget.ui'
**
** Created: Mon Jan 7 20:49:33 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRIVERDATAWIDGET_H
#define UI_DRIVERDATAWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "src/main_gui/driverinfolabel.h"

QT_BEGIN_NAMESPACE

class Ui_DriverDataWidget
{
public:
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_5;
    QVBoxLayout *verticalLayout;
    QWidget *infoWidget;
    QGridLayout *gridLayout;
    QGridLayout *recordsLayout;
    QLabel *label_7;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_12;
    QLabel *tRLabel;
    QLabel *tBLabel;
    QLabel *label_11;
    QLabel *s3RLabel;
    QLabel *s3BLabel;
    QLabel *label_10;
    QLabel *s2RLabel;
    QLabel *s2BLabel;
    QLabel *label_9;
    QLabel *s1RLabel;
    QLabel *s1BLabel;
    QSpacerItem *verticalSpacer;
    QLabel *approxLapLabel;
    QLabel *gridPositionLabel;
    QLabel *lastLapLabel;
    QLabel *gapLabel;
    QLabel *label_4;
    QLabel *gridLabel;
    QLabel *label_5;
    QLabel *bestLapLabel;
    QLabel *label_3;
    QLabel *carImageLabel;
    QFrame *frame;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_6;
    QLabel *numPitsLabel;
    QLabel *pitStopsLabel;
    QLabel *label_2;
    DriverInfoLabel *driverInfoLabel;
    QSpacerItem *verticalSpacer_2;
    QFrame *frame_2;
    QTableView *tableView;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *lapTimeChartTableWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *chartsTableWidget;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_6;
    QTextEdit *textEdit;

    void setupUi(QWidget *DriverDataWidget)
    {
        if (DriverDataWidget->objectName().isEmpty())
            DriverDataWidget->setObjectName(QString::fromUtf8("DriverDataWidget"));
        DriverDataWidget->resize(626, 552);
        DriverDataWidget->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_5 = new QVBoxLayout(DriverDataWidget);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tabWidget = new QTabWidget(DriverDataWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
#ifndef Q_OS_MAC
        verticalLayout_2->setContentsMargins(9, 9, 9, 9);
#endif
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QPalette palette;
        QBrush brush(QColor(204, 204, 205, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush1(QColor(20, 20, 20, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        scrollArea->setPalette(palette);
        scrollArea->setAutoFillBackground(true);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 602, 503));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents_5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 4, 0, 4);
        infoWidget = new QWidget(scrollAreaWidgetContents_5);
        infoWidget->setObjectName(QString::fromUtf8("infoWidget"));
        gridLayout = new QGridLayout(infoWidget);
        gridLayout->setContentsMargins(6, 6, 6, 6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(20);
        gridLayout->setVerticalSpacing(6);
        recordsLayout = new QGridLayout();
        recordsLayout->setContentsMargins(5, 5, 5, 5);
        recordsLayout->setObjectName(QString::fromUtf8("recordsLayout"));
        recordsLayout->setHorizontalSpacing(3);
        recordsLayout->setVerticalSpacing(6);
        label_7 = new QLabel(infoWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QPalette palette1;
        QBrush brush2(QColor(150, 150, 150, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        QBrush brush3(QColor(130, 130, 130, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_7->setPalette(palette1);

        recordsLayout->addWidget(label_7, 2, 0, 1, 1);

        label = new QLabel(infoWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label->setPalette(palette2);
        label->setAlignment(Qt::AlignCenter);

        recordsLayout->addWidget(label, 0, 0, 1, 5);

        label_8 = new QLabel(infoWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_8->setPalette(palette3);

        recordsLayout->addWidget(label_8, 3, 0, 1, 1);

        label_12 = new QLabel(infoWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_12->setPalette(palette4);
        label_12->setAlignment(Qt::AlignCenter);

        recordsLayout->addWidget(label_12, 1, 1, 1, 1);

        tRLabel = new QLabel(infoWidget);
        tRLabel->setObjectName(QString::fromUtf8("tRLabel"));
        QPalette palette5;
        QBrush brush4(QColor(0, 255, 0, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        tRLabel->setPalette(palette5);

        recordsLayout->addWidget(tRLabel, 2, 1, 1, 1);

        tBLabel = new QLabel(infoWidget);
        tBLabel->setObjectName(QString::fromUtf8("tBLabel"));
        QPalette palette6;
        QBrush brush5(QColor(255, 0, 255, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        tBLabel->setPalette(palette6);

        recordsLayout->addWidget(tBLabel, 3, 1, 1, 1);

        label_11 = new QLabel(infoWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_11->setPalette(palette7);
        label_11->setAlignment(Qt::AlignCenter);

        recordsLayout->addWidget(label_11, 1, 4, 1, 1);

        s3RLabel = new QLabel(infoWidget);
        s3RLabel->setObjectName(QString::fromUtf8("s3RLabel"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3RLabel->setPalette(palette8);

        recordsLayout->addWidget(s3RLabel, 2, 4, 1, 1);

        s3BLabel = new QLabel(infoWidget);
        s3BLabel->setObjectName(QString::fromUtf8("s3BLabel"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3BLabel->setPalette(palette9);

        recordsLayout->addWidget(s3BLabel, 3, 4, 1, 1);

        label_10 = new QLabel(infoWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_10->setPalette(palette10);
        label_10->setAlignment(Qt::AlignCenter);

        recordsLayout->addWidget(label_10, 1, 3, 1, 1);

        s2RLabel = new QLabel(infoWidget);
        s2RLabel->setObjectName(QString::fromUtf8("s2RLabel"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2RLabel->setPalette(palette11);

        recordsLayout->addWidget(s2RLabel, 2, 3, 1, 1);

        s2BLabel = new QLabel(infoWidget);
        s2BLabel->setObjectName(QString::fromUtf8("s2BLabel"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2BLabel->setPalette(palette12);

        recordsLayout->addWidget(s2BLabel, 3, 3, 1, 1);

        label_9 = new QLabel(infoWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_9->setPalette(palette13);
        label_9->setAlignment(Qt::AlignCenter);

        recordsLayout->addWidget(label_9, 1, 2, 1, 1);

        s1RLabel = new QLabel(infoWidget);
        s1RLabel->setObjectName(QString::fromUtf8("s1RLabel"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1RLabel->setPalette(palette14);

        recordsLayout->addWidget(s1RLabel, 2, 2, 1, 1);

        s1BLabel = new QLabel(infoWidget);
        s1BLabel->setObjectName(QString::fromUtf8("s1BLabel"));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1BLabel->setPalette(palette15);

        recordsLayout->addWidget(s1BLabel, 3, 2, 1, 1);


        gridLayout->addLayout(recordsLayout, 13, 0, 1, 4);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 11, 0, 1, 1);

        approxLapLabel = new QLabel(infoWidget);
        approxLapLabel->setObjectName(QString::fromUtf8("approxLapLabel"));
        QPalette palette16;
        QBrush brush6(QColor(0, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        approxLapLabel->setPalette(palette16);
        approxLapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(approxLapLabel, 10, 3, 1, 1);

        gridPositionLabel = new QLabel(infoWidget);
        gridPositionLabel->setObjectName(QString::fromUtf8("gridPositionLabel"));
        QPalette palette17;
        QBrush brush7(QColor(220, 220, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        gridPositionLabel->setPalette(palette17);
        gridPositionLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(gridPositionLabel, 7, 1, 1, 1);

        lastLapLabel = new QLabel(infoWidget);
        lastLapLabel->setObjectName(QString::fromUtf8("lastLapLabel"));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        lastLapLabel->setPalette(palette18);
        lastLapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lastLapLabel, 7, 3, 1, 1);

        gapLabel = new QLabel(infoWidget);
        gapLabel->setObjectName(QString::fromUtf8("gapLabel"));
        QPalette palette19;
        QBrush brush8(QColor(255, 255, 0, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        gapLabel->setPalette(palette19);
        gapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(gapLabel, 9, 1, 1, 1);

        label_4 = new QLabel(infoWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_4->setPalette(palette20);

        gridLayout->addWidget(label_4, 9, 2, 1, 1);

        gridLabel = new QLabel(infoWidget);
        gridLabel->setObjectName(QString::fromUtf8("gridLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gridLabel->sizePolicy().hasHeightForWidth());
        gridLabel->setSizePolicy(sizePolicy);
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        gridLabel->setPalette(palette21);

        gridLayout->addWidget(gridLabel, 7, 0, 1, 1);

        label_5 = new QLabel(infoWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_5->setPalette(palette22);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 16, 0, 1, 4);

        bestLapLabel = new QLabel(infoWidget);
        bestLapLabel->setObjectName(QString::fromUtf8("bestLapLabel"));
        QPalette palette23;
        palette23.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette23.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette23.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        bestLapLabel->setPalette(palette23);
        bestLapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(bestLapLabel, 9, 3, 1, 1);

        label_3 = new QLabel(infoWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QPalette palette24;
        palette24.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette24.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette24.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_3->setPalette(palette24);

        gridLayout->addWidget(label_3, 7, 2, 1, 1);

        carImageLabel = new QLabel(infoWidget);
        carImageLabel->setObjectName(QString::fromUtf8("carImageLabel"));
        sizePolicy.setHeightForWidth(carImageLabel->sizePolicy().hasHeightForWidth());
        carImageLabel->setSizePolicy(sizePolicy);
        carImageLabel->setMinimumSize(QSize(0, 80));
        carImageLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(carImageLabel, 4, 0, 1, 4);

        frame = new QFrame(infoWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(0, 0));
        frame->setSizeIncrement(QSize(0, 0));
        frame->setStyleSheet(QString::fromUtf8("color: rgba(170, 170, 170, 100);"));
        frame->setFrameShape(QFrame::HLine);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(1);
        frame->setMidLineWidth(0);
        verticalLayout_7 = new QVBoxLayout(frame);
#ifndef Q_OS_MAC
        verticalLayout_7->setSpacing(6);
#endif
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));

        gridLayout->addWidget(frame, 14, 0, 1, 4);

        label_6 = new QLabel(infoWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QPalette palette25;
        palette25.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette25.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette25.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_6->setPalette(palette25);

        gridLayout->addWidget(label_6, 10, 2, 1, 1);

        numPitsLabel = new QLabel(infoWidget);
        numPitsLabel->setObjectName(QString::fromUtf8("numPitsLabel"));
        QPalette palette26;
        QBrush brush9(QColor(255, 0, 0, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette26.setBrush(QPalette::Active, QPalette::WindowText, brush9);
        palette26.setBrush(QPalette::Inactive, QPalette::WindowText, brush9);
        palette26.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        numPitsLabel->setPalette(palette26);
        numPitsLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(numPitsLabel, 10, 1, 1, 1);

        pitStopsLabel = new QLabel(infoWidget);
        pitStopsLabel->setObjectName(QString::fromUtf8("pitStopsLabel"));
        QPalette palette27;
        palette27.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette27.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette27.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        pitStopsLabel->setPalette(palette27);

        gridLayout->addWidget(pitStopsLabel, 10, 0, 1, 1);

        label_2 = new QLabel(infoWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QPalette palette28;
        palette28.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette28.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette28.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_2->setPalette(palette28);

        gridLayout->addWidget(label_2, 9, 0, 1, 1);

        driverInfoLabel = new DriverInfoLabel(infoWidget);
        driverInfoLabel->setObjectName(QString::fromUtf8("driverInfoLabel"));
        sizePolicy.setHeightForWidth(driverInfoLabel->sizePolicy().hasHeightForWidth());
        driverInfoLabel->setSizePolicy(sizePolicy);
        driverInfoLabel->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(driverInfoLabel, 5, 0, 1, 4);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 15, 0, 1, 1);

        frame_2 = new QFrame(infoWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8("color: rgba(170, 170, 170, 100);"));
        frame_2->setFrameShape(QFrame::HLine);
        frame_2->setFrameShadow(QFrame::Plain);
        frame_2->setLineWidth(1);

        gridLayout->addWidget(frame_2, 12, 0, 1, 4);


        verticalLayout->addWidget(infoWidget);

        tableView = new QTableView(scrollAreaWidgetContents_5);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);
        QPalette palette29;
        palette29.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush10(QColor(73, 73, 73, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette29.setBrush(QPalette::Active, QPalette::Highlight, brush10);
        QBrush brush11(QColor(30, 30, 30, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette29.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette29.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette29.setBrush(QPalette::Inactive, QPalette::Highlight, brush10);
        palette29.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        QBrush brush12(QColor(240, 240, 240, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette29.setBrush(QPalette::Disabled, QPalette::Base, brush12);
        QBrush brush13(QColor(175, 175, 175, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette29.setBrush(QPalette::Disabled, QPalette::Highlight, brush13);
        palette29.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        tableView->setPalette(palette29);
        tableView->setFrameShape(QFrame::NoFrame);
        tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView->setAutoScroll(true);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableView->setShowGrid(false);
        tableView->horizontalHeader()->setVisible(false);
        tableView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableView);

        scrollArea->setWidget(scrollAreaWidgetContents_5);

        verticalLayout_2->addWidget(scrollArea);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab_3->setEnabled(true);
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lapTimeChartTableWidget = new QTableWidget(tab_3);
        if (lapTimeChartTableWidget->columnCount() < 1)
            lapTimeChartTableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        lapTimeChartTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        lapTimeChartTableWidget->setObjectName(QString::fromUtf8("lapTimeChartTableWidget"));
        QPalette palette30;
        QBrush brush14(QColor(255, 255, 255, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette30.setBrush(QPalette::Active, QPalette::WindowText, brush14);
        palette30.setBrush(QPalette::Active, QPalette::Text, brush14);
        palette30.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette30.setBrush(QPalette::Inactive, QPalette::WindowText, brush14);
        palette30.setBrush(QPalette::Inactive, QPalette::Text, brush14);
        palette30.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette30.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette30.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette30.setBrush(QPalette::Disabled, QPalette::Base, brush12);
        lapTimeChartTableWidget->setPalette(palette30);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        lapTimeChartTableWidget->setFont(font);
        lapTimeChartTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lapTimeChartTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        lapTimeChartTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        lapTimeChartTableWidget->setIconSize(QSize(300, 300));
        lapTimeChartTableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        lapTimeChartTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        lapTimeChartTableWidget->setShowGrid(false);
        lapTimeChartTableWidget->horizontalHeader()->setVisible(false);
        lapTimeChartTableWidget->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(lapTimeChartTableWidget);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setEnabled(true);
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        chartsTableWidget = new QTableWidget(tab_2);
        if (chartsTableWidget->columnCount() < 1)
            chartsTableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        chartsTableWidget->setObjectName(QString::fromUtf8("chartsTableWidget"));
        QPalette palette31;
        palette31.setBrush(QPalette::Active, QPalette::WindowText, brush14);
        palette31.setBrush(QPalette::Active, QPalette::Text, brush14);
        palette31.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette31.setBrush(QPalette::Inactive, QPalette::WindowText, brush14);
        palette31.setBrush(QPalette::Inactive, QPalette::Text, brush14);
        palette31.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette31.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette31.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette31.setBrush(QPalette::Disabled, QPalette::Base, brush12);
        chartsTableWidget->setPalette(palette31);
        chartsTableWidget->setFont(font);
        chartsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        chartsTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        chartsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        chartsTableWidget->setIconSize(QSize(300, 300));
        chartsTableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        chartsTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        chartsTableWidget->setShowGrid(false);
        chartsTableWidget->horizontalHeader()->setVisible(false);
        chartsTableWidget->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(chartsTableWidget);

        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_6 = new QVBoxLayout(tab_4);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        textEdit = new QTextEdit(tab_4);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QPalette palette32;
        palette32.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette32.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush15(QColor(159, 159, 159, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette32.setBrush(QPalette::Active, QPalette::Highlight, brush15);
        palette32.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette32.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette32.setBrush(QPalette::Inactive, QPalette::Highlight, brush15);
        palette32.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette32.setBrush(QPalette::Disabled, QPalette::Base, brush12);
        palette32.setBrush(QPalette::Disabled, QPalette::Highlight, brush13);
        textEdit->setPalette(palette32);
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        textEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_6->addWidget(textEdit);

        tabWidget->addTab(tab_4, QString());

        verticalLayout_5->addWidget(tabWidget);


        retranslateUi(DriverDataWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DriverDataWidget);
    } // setupUi

    void retranslateUi(QWidget *DriverDataWidget)
    {
        DriverDataWidget->setWindowTitle(QApplication::translate("DriverDataWidget", "Form", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DriverDataWidget", "Driver:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DriverDataWidget", "Weekend records", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DriverDataWidget", "Best:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("DriverDataWidget", "Time", 0, QApplication::UnicodeUTF8));
        tRLabel->setText(QString());
        tBLabel->setText(QString());
        label_11->setText(QApplication::translate("DriverDataWidget", "S3", 0, QApplication::UnicodeUTF8));
        s3RLabel->setText(QString());
        s3BLabel->setText(QString());
        label_10->setText(QApplication::translate("DriverDataWidget", "S2", 0, QApplication::UnicodeUTF8));
        s2RLabel->setText(QString());
        s2BLabel->setText(QString());
        label_9->setText(QApplication::translate("DriverDataWidget", "S1", 0, QApplication::UnicodeUTF8));
        s1RLabel->setText(QString());
        s1BLabel->setText(QString());
        approxLapLabel->setText(QString());
        gridPositionLabel->setText(QString());
        lastLapLabel->setText(QString());
        gapLabel->setText(QString());
        label_4->setText(QApplication::translate("DriverDataWidget", "Best lap:", 0, QApplication::UnicodeUTF8));
        gridLabel->setText(QApplication::translate("DriverDataWidget", "Grid position:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DriverDataWidget", "Lap history", 0, QApplication::UnicodeUTF8));
        bestLapLabel->setText(QString());
        label_3->setText(QApplication::translate("DriverDataWidget", "Last lap:", 0, QApplication::UnicodeUTF8));
        carImageLabel->setText(QString());
        label_6->setText(QApplication::translate("DriverDataWidget", "Approximate:", 0, QApplication::UnicodeUTF8));
        numPitsLabel->setText(QString());
        pitStopsLabel->setText(QApplication::translate("DriverDataWidget", "Pit stops:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DriverDataWidget", "Gap to leader:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DriverDataWidget", "Lap history", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("DriverDataWidget", "Lap time chart", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DriverDataWidget", "Position && gap charts", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("DriverDataWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Tahoma'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("DriverDataWidget", "Related commentary", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DriverDataWidget: public Ui_DriverDataWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRIVERDATAWIDGET_H
