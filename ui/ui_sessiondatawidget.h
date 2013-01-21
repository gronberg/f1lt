/********************************************************************************
** Form generated from reading UI file 'sessiondatawidget.ui'
**
** Created: Mon Jan 21 21:28:14 2013
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
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
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
    QWidget *widget;
    QVBoxLayout *verticalLayout_7;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_8;
    QLabel *eventNameLabel;
    QLabel *eventPlaceLabel;
    QLabel *eventDateLabel;
    QLabel *eventLapsLabel;
    QSpacerItem *verticalSpacer_4;
    QLabel *eventMapLabel;
    QSpacerItem *verticalSpacer_6;
    QFrame *frame_3;
    QSpacerItem *verticalSpacer_7;
    QLabel *recordsLabel;
    QSpacerItem *verticalSpacer_5;
    QGridLayout *recordsLayout;
    QLabel *label_9;
    QLabel *rRTLabel;
    QLabel *qRTLabel;
    QLabel *rRDLabel;
    QLabel *label_10;
    QLabel *rRDTLabel;
    QLabel *rRYLabel;
    QLabel *qRDTLabel;
    QLabel *qRDLabel;
    QLabel *qRYLabel;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QWidget *infoWidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QFrame *frame;
    QLabel *s3LapLabel;
    QLabel *s2TimeLabel;
    QLabel *s1DriverLabel;
    QLabel *label_3;
    QLabel *s2LapLabel;
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
    QLabel *label_7;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_3;
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
        widget = new QWidget(tab_4);
        widget->setObjectName(QString::fromUtf8("widget"));
        QPalette palette1;
        QBrush brush2(QColor(220, 220, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        QBrush brush3(QColor(130, 130, 130, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        widget->setPalette(palette1);
        widget->setAutoFillBackground(true);
        verticalLayout_7 = new QVBoxLayout(widget);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        scrollArea_2 = new QScrollArea(widget);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setFrameShape(QFrame::NoFrame);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 553, 715));
        verticalLayout_8 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        eventNameLabel = new QLabel(scrollAreaWidgetContents_2);
        eventNameLabel->setObjectName(QString::fromUtf8("eventNameLabel"));
        QPalette palette2;
        QBrush brush4(QColor(255, 255, 0, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        eventNameLabel->setPalette(palette2);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(15);
        font.setBold(true);
        font.setWeight(75);
        eventNameLabel->setFont(font);
        eventNameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(eventNameLabel);

        eventPlaceLabel = new QLabel(scrollAreaWidgetContents_2);
        eventPlaceLabel->setObjectName(QString::fromUtf8("eventPlaceLabel"));
        QPalette palette3;
        QBrush brush5(QColor(0, 255, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        eventPlaceLabel->setPalette(palette3);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        eventPlaceLabel->setFont(font1);
        eventPlaceLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(eventPlaceLabel);

        eventDateLabel = new QLabel(scrollAreaWidgetContents_2);
        eventDateLabel->setObjectName(QString::fromUtf8("eventDateLabel"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        eventDateLabel->setFont(font2);
        eventDateLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(eventDateLabel);

        eventLapsLabel = new QLabel(scrollAreaWidgetContents_2);
        eventLapsLabel->setObjectName(QString::fromUtf8("eventLapsLabel"));
        QPalette palette4;
        QBrush brush6(QColor(0, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        eventLapsLabel->setPalette(palette4);
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(9);
        font3.setBold(true);
        font3.setWeight(75);
        eventLapsLabel->setFont(font3);
        eventLapsLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(eventLapsLabel);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_8->addItem(verticalSpacer_4);

        eventMapLabel = new QLabel(scrollAreaWidgetContents_2);
        eventMapLabel->setObjectName(QString::fromUtf8("eventMapLabel"));
        eventMapLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(eventMapLabel);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_8->addItem(verticalSpacer_6);

        frame_3 = new QFrame(scrollAreaWidgetContents_2);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setStyleSheet(QString::fromUtf8("color: rgba(170, 170, 170, 100);"));
        frame_3->setFrameShape(QFrame::HLine);
        frame_3->setFrameShadow(QFrame::Plain);

        verticalLayout_8->addWidget(frame_3);

        verticalSpacer_7 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_8->addItem(verticalSpacer_7);

        recordsLabel = new QLabel(scrollAreaWidgetContents_2);
        recordsLabel->setObjectName(QString::fromUtf8("recordsLabel"));
        QPalette palette5;
        QBrush brush7(QColor(150, 150, 150, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        recordsLabel->setPalette(palette5);
        recordsLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(recordsLabel);

        verticalSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_8->addItem(verticalSpacer_5);

        recordsLayout = new QGridLayout();
        recordsLayout->setObjectName(QString::fromUtf8("recordsLayout"));
        label_9 = new QLabel(scrollAreaWidgetContents_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_9->setPalette(palette6);

        recordsLayout->addWidget(label_9, 0, 0, 1, 1);

        rRTLabel = new QLabel(scrollAreaWidgetContents_2);
        rRTLabel->setObjectName(QString::fromUtf8("rRTLabel"));
        QPalette palette7;
        QBrush brush8(QColor(255, 0, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        rRTLabel->setPalette(palette7);
        rRTLabel->setAlignment(Qt::AlignCenter);
        rRTLabel->setIndent(10);

        recordsLayout->addWidget(rRTLabel, 1, 1, 1, 1);

        qRTLabel = new QLabel(scrollAreaWidgetContents_2);
        qRTLabel->setObjectName(QString::fromUtf8("qRTLabel"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        qRTLabel->setPalette(palette8);
        qRTLabel->setAlignment(Qt::AlignCenter);
        qRTLabel->setMargin(0);
        qRTLabel->setIndent(10);

        recordsLayout->addWidget(qRTLabel, 0, 1, 1, 1);

        rRDLabel = new QLabel(scrollAreaWidgetContents_2);
        rRDLabel->setObjectName(QString::fromUtf8("rRDLabel"));
        QPalette palette9;
        QBrush brush9(QColor(230, 230, 230, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush9);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush9);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        rRDLabel->setPalette(palette9);

        recordsLayout->addWidget(rRDLabel, 1, 2, 1, 1);

        label_10 = new QLabel(scrollAreaWidgetContents_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_10->setPalette(palette10);

        recordsLayout->addWidget(label_10, 1, 0, 1, 1);

        rRDTLabel = new QLabel(scrollAreaWidgetContents_2);
        rRDTLabel->setObjectName(QString::fromUtf8("rRDTLabel"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush9);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush9);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        rRDTLabel->setPalette(palette11);

        recordsLayout->addWidget(rRDTLabel, 1, 3, 1, 1);

        rRYLabel = new QLabel(scrollAreaWidgetContents_2);
        rRYLabel->setObjectName(QString::fromUtf8("rRYLabel"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        rRYLabel->setPalette(palette12);
        rRYLabel->setIndent(5);

        recordsLayout->addWidget(rRYLabel, 1, 4, 1, 1);

        qRDTLabel = new QLabel(scrollAreaWidgetContents_2);
        qRDTLabel->setObjectName(QString::fromUtf8("qRDTLabel"));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush9);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        qRDTLabel->setPalette(palette13);

        recordsLayout->addWidget(qRDTLabel, 0, 3, 1, 1);

        qRDLabel = new QLabel(scrollAreaWidgetContents_2);
        qRDLabel->setObjectName(QString::fromUtf8("qRDLabel"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush9);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        qRDLabel->setPalette(palette14);

        recordsLayout->addWidget(qRDLabel, 0, 2, 1, 1);

        qRYLabel = new QLabel(scrollAreaWidgetContents_2);
        qRYLabel->setObjectName(QString::fromUtf8("qRYLabel"));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        qRYLabel->setPalette(palette15);
        qRYLabel->setIndent(5);

        recordsLayout->addWidget(qRYLabel, 0, 4, 1, 1);


        verticalLayout_8->addLayout(recordsLayout);

        verticalLayout_8->setStretch(5, 1);
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_7->addWidget(scrollArea_2);


        verticalLayout_5->addWidget(widget);

        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        scrollArea = new QScrollArea(tab_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::Button, brush);
        palette16.setBrush(QPalette::Active, QPalette::Base, brush);
        palette16.setBrush(QPalette::Active, QPalette::Window, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::Window, brush);
        scrollArea->setPalette(palette16);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 260, 262));
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
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        infoWidget->setPalette(palette17);
        gridLayout = new QGridLayout(infoWidget);
        gridLayout->setContentsMargins(4, 4, 4, 4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(4);
        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 10, 0, 1, 1);

        frame = new QFrame(infoWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("color: rgba(170, 170, 170, 100);"));
        frame->setFrameShape(QFrame::HLine);
        frame->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(frame, 9, 0, 1, 4);

        s3LapLabel = new QLabel(infoWidget);
        s3LapLabel->setObjectName(QString::fromUtf8("s3LapLabel"));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3LapLabel->setPalette(palette18);
        s3LapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s3LapLabel, 6, 3, 1, 1);

        s2TimeLabel = new QLabel(infoWidget);
        s2TimeLabel->setObjectName(QString::fromUtf8("s2TimeLabel"));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2TimeLabel->setPalette(palette19);
        s2TimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s2TimeLabel, 4, 1, 1, 1);

        s1DriverLabel = new QLabel(infoWidget);
        s1DriverLabel->setObjectName(QString::fromUtf8("s1DriverLabel"));
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1DriverLabel->setPalette(palette20);

        gridLayout->addWidget(s1DriverLabel, 3, 2, 1, 1);

        label_3 = new QLabel(infoWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        s2LapLabel = new QLabel(infoWidget);
        s2LapLabel->setObjectName(QString::fromUtf8("s2LapLabel"));
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette21.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette21.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2LapLabel->setPalette(palette21);
        s2LapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s2LapLabel, 4, 3, 1, 1);

        s1LapLabel = new QLabel(infoWidget);
        s1LapLabel->setObjectName(QString::fromUtf8("s1LapLabel"));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette22.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette22.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1LapLabel->setPalette(palette22);
        s1LapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s1LapLabel, 3, 3, 1, 1);

        label_2 = new QLabel(infoWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        s3DriverLabel = new QLabel(infoWidget);
        s3DriverLabel->setObjectName(QString::fromUtf8("s3DriverLabel"));
        QPalette palette23;
        palette23.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette23.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette23.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3DriverLabel->setPalette(palette23);

        gridLayout->addWidget(s3DriverLabel, 6, 2, 1, 1);

        flDriverLabel = new QLabel(infoWidget);
        flDriverLabel->setObjectName(QString::fromUtf8("flDriverLabel"));
        QPalette palette24;
        palette24.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette24.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette24.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        flDriverLabel->setPalette(palette24);

        gridLayout->addWidget(flDriverLabel, 2, 2, 1, 1);

        label = new QLabel(infoWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        flTimeLabel = new QLabel(infoWidget);
        flTimeLabel->setObjectName(QString::fromUtf8("flTimeLabel"));
        QPalette palette25;
        palette25.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette25.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette25.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        flTimeLabel->setPalette(palette25);
        flTimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(flTimeLabel, 2, 1, 1, 1);

        s1TimeLabel = new QLabel(infoWidget);
        s1TimeLabel->setObjectName(QString::fromUtf8("s1TimeLabel"));
        QPalette palette26;
        palette26.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette26.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette26.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s1TimeLabel->setPalette(palette26);
        s1TimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s1TimeLabel, 3, 1, 1, 1);

        theoreticalTimeLabel = new QLabel(infoWidget);
        theoreticalTimeLabel->setObjectName(QString::fromUtf8("theoreticalTimeLabel"));
        QPalette palette27;
        palette27.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette27.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette27.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        theoreticalTimeLabel->setPalette(palette27);
        theoreticalTimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(theoreticalTimeLabel, 7, 1, 1, 1);

        s2DriverLabel = new QLabel(infoWidget);
        s2DriverLabel->setObjectName(QString::fromUtf8("s2DriverLabel"));
        QPalette palette28;
        palette28.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette28.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette28.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s2DriverLabel->setPalette(palette28);

        gridLayout->addWidget(s2DriverLabel, 4, 2, 1, 1);

        label_4 = new QLabel(infoWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 6, 0, 1, 1);

        flLapLabel = new QLabel(infoWidget);
        flLapLabel->setObjectName(QString::fromUtf8("flLapLabel"));
        QPalette palette29;
        palette29.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette29.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette29.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        flLapLabel->setPalette(palette29);
        flLapLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(flLapLabel, 2, 3, 1, 1);

        label_5 = new QLabel(infoWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 7, 0, 1, 1);

        s3TimeLabel = new QLabel(infoWidget);
        s3TimeLabel->setObjectName(QString::fromUtf8("s3TimeLabel"));
        QPalette palette30;
        palette30.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette30.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette30.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        s3TimeLabel->setPalette(palette30);
        s3TimeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(s3TimeLabel, 6, 1, 1, 1);

        label_6 = new QLabel(infoWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 11, 0, 1, 4);

        label_7 = new QLabel(infoWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 0, 0, 1, 4);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 8, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 1, 0, 1, 1);


        verticalLayout_6->addWidget(infoWidget);

        fastestLapsTable = new QTableView(scrollAreaWidgetContents);
        fastestLapsTable->setObjectName(QString::fromUtf8("fastestLapsTable"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fastestLapsTable->sizePolicy().hasHeightForWidth());
        fastestLapsTable->setSizePolicy(sizePolicy1);
        QPalette palette31;
        palette31.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush10(QColor(73, 73, 73, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette31.setBrush(QPalette::Active, QPalette::Highlight, brush10);
        QBrush brush11(QColor(30, 30, 30, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette31.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette31.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette31.setBrush(QPalette::Inactive, QPalette::Highlight, brush10);
        palette31.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette31.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        QBrush brush12(QColor(175, 175, 175, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette31.setBrush(QPalette::Disabled, QPalette::Highlight, brush12);
        palette31.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        fastestLapsTable->setPalette(palette31);
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
        QPalette palette32;
        palette32.setBrush(QPalette::Active, QPalette::Base, brush);
        palette32.setBrush(QPalette::Active, QPalette::Highlight, brush10);
        palette32.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette32.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette32.setBrush(QPalette::Inactive, QPalette::Highlight, brush10);
        palette32.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette32.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette32.setBrush(QPalette::Disabled, QPalette::Highlight, brush12);
        palette32.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        speedRecordsTable->setPalette(palette32);
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
        QPalette palette33;
        palette33.setBrush(QPalette::Active, QPalette::Base, brush);
        palette33.setBrush(QPalette::Active, QPalette::Highlight, brush10);
        palette33.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette33.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette33.setBrush(QPalette::Inactive, QPalette::Highlight, brush10);
        palette33.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette33.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette33.setBrush(QPalette::Disabled, QPalette::Highlight, brush12);
        palette33.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        pitStopsTable->setPalette(palette33);
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

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SessionDataWidget);
    } // setupUi

    void retranslateUi(QWidget *SessionDataWidget)
    {
        SessionDataWidget->setWindowTitle(QApplication::translate("SessionDataWidget", "Form", 0, QApplication::UnicodeUTF8));
        eventNameLabel->setText(QApplication::translate("SessionDataWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        eventPlaceLabel->setText(QApplication::translate("SessionDataWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        eventDateLabel->setText(QApplication::translate("SessionDataWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        eventLapsLabel->setText(QApplication::translate("SessionDataWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        eventMapLabel->setText(QApplication::translate("SessionDataWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        recordsLabel->setText(QApplication::translate("SessionDataWidget", "Track records:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("SessionDataWidget", "Quali record:", 0, QApplication::UnicodeUTF8));
        rRTLabel->setText(QApplication::translate("SessionDataWidget", "time", 0, QApplication::UnicodeUTF8));
        qRTLabel->setText(QApplication::translate("SessionDataWidget", "time", 0, QApplication::UnicodeUTF8));
        rRDLabel->setText(QApplication::translate("SessionDataWidget", "driver", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("SessionDataWidget", "Race record:", 0, QApplication::UnicodeUTF8));
        rRDTLabel->setText(QApplication::translate("SessionDataWidget", "team", 0, QApplication::UnicodeUTF8));
        rRYLabel->setText(QApplication::translate("SessionDataWidget", "year", 0, QApplication::UnicodeUTF8));
        qRDTLabel->setText(QApplication::translate("SessionDataWidget", "team", 0, QApplication::UnicodeUTF8));
        qRDLabel->setText(QApplication::translate("SessionDataWidget", "driver", 0, QApplication::UnicodeUTF8));
        qRYLabel->setText(QApplication::translate("SessionDataWidget", "year", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("SessionDataWidget", "Event info", 0, QApplication::UnicodeUTF8));
        s3LapLabel->setText(QString());
        s2TimeLabel->setText(QString());
        s1DriverLabel->setText(QString());
        label_3->setText(QApplication::translate("SessionDataWidget", "Fastest S2:", 0, QApplication::UnicodeUTF8));
        s2LapLabel->setText(QString());
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
        label_7->setText(QApplication::translate("SessionDataWidget", "Session records", 0, QApplication::UnicodeUTF8));
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
