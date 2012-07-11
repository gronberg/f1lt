/********************************************************************************
** Form generated from reading UI file 'ltwindow.ui'
**
** Created: Wed Jul 11 13:33:12 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LTWINDOW_H
#define UI_LTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "driverdatawidget.h"
#include "eventstatuswidget.h"
#include "lttablewidget.h"
#include "nosessionboardwidget.h"
#include "sessiondatawidget.h"
#include "weatherchartswidget.h"

QT_BEGIN_NAMESPACE

class Ui_LTWindow
{
public:
    QAction *actionHead_to_head;
    QAction *actionConnect;
    QAction *actionRecord;
    QAction *actionPreferences;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionOpen;
    QAction *actionLap_time_comparison;
    QAction *actionStop_recording;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    NoSessionBoardWidget *messageBoardWidget;
    EventStatusWidget *eventStatusWidget;
    QSplitter *splitter;
    LTTableWidget *tableWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    DriverDataWidget *driverDataWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    SessionDataWidget *sessionDataWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    WeatherChartsWidget *weatherChartsWidget;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LTWindow)
    {
        if (LTWindow->objectName().isEmpty())
            LTWindow->setObjectName(QString::fromUtf8("LTWindow"));
        LTWindow->resize(1249, 828);
        LTWindow->setContextMenuPolicy(Qt::PreventContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        LTWindow->setWindowIcon(icon);
        actionHead_to_head = new QAction(LTWindow);
        actionHead_to_head->setObjectName(QString::fromUtf8("actionHead_to_head"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ui_icons/head2head-s.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHead_to_head->setIcon(icon1);
        actionConnect = new QAction(LTWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ui_icons/login.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon2);
        actionRecord = new QAction(LTWindow);
        actionRecord->setObjectName(QString::fromUtf8("actionRecord"));
        actionRecord->setEnabled(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/ui_icons/record.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRecord->setIcon(icon3);
        actionPreferences = new QAction(LTWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ui_icons/preferences.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/ui_icons/preferences.png"), QSize(), QIcon::Normal, QIcon::On);
        actionPreferences->setIcon(icon4);
        actionExit = new QAction(LTWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/ui_icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon5);
        actionAbout = new QAction(LTWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout_Qt = new QAction(LTWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionOpen = new QAction(LTWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/ui_icons/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon6);
        actionLap_time_comparison = new QAction(LTWindow);
        actionLap_time_comparison->setObjectName(QString::fromUtf8("actionLap_time_comparison"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/ui_icons/ltc-s.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLap_time_comparison->setIcon(icon7);
        actionStop_recording = new QAction(LTWindow);
        actionStop_recording->setObjectName(QString::fromUtf8("actionStop_recording"));
        actionStop_recording->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/ui_icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop_recording->setIcon(icon8);
        centralWidget = new QWidget(LTWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(5, 5, 5, 5);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        messageBoardWidget = new NoSessionBoardWidget(centralWidget);
        messageBoardWidget->setObjectName(QString::fromUtf8("messageBoardWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(messageBoardWidget->sizePolicy().hasHeightForWidth());
        messageBoardWidget->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(messageBoardWidget);

        eventStatusWidget = new EventStatusWidget(centralWidget);
        eventStatusWidget->setObjectName(QString::fromUtf8("eventStatusWidget"));
        eventStatusWidget->setEnabled(true);
        eventStatusWidget->setAutoFillBackground(true);

        verticalLayout_4->addWidget(eventStatusWidget);

        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setEnabled(true);
        splitter->setMinimumSize(QSize(400, 0));
        splitter->setMaximumSize(QSize(16777215, 16777215));
        splitter->setFrameShape(QFrame::NoFrame);
        splitter->setFrameShadow(QFrame::Plain);
        splitter->setLineWidth(1);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        tableWidget = new LTTableWidget(splitter);
        if (tableWidget->columnCount() < 11)
            tableWidget->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(20, 20, 20, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush2(QColor(73, 73, 73, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush2);
        QBrush brush3(QColor(27, 27, 27, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        QBrush brush4(QColor(130, 130, 130, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        QBrush brush5(QColor(240, 240, 240, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        QBrush brush6(QColor(175, 175, 175, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        tableWidget->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        tableWidget->setFont(font);
        tableWidget->setStyleSheet(QString::fromUtf8(""));
        tableWidget->setFrameShape(QFrame::StyledPanel);
        tableWidget->setFrameShadow(QFrame::Plain);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setIconSize(QSize(200, 50));
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setShowGrid(false);
        splitter->addWidget(tableWidget);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(false);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(verticalLayoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(15, 15));
        tabWidget->setMovable(false);
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setContentsMargins(4, 4, 4, 4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        driverDataWidget = new DriverDataWidget(tab_2);
        driverDataWidget->setObjectName(QString::fromUtf8("driverDataWidget"));

        verticalLayout_2->addWidget(driverDataWidget);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(4, 4, 4, 4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        sessionDataWidget = new SessionDataWidget(tab);
        sessionDataWidget->setObjectName(QString::fromUtf8("sessionDataWidget"));

        verticalLayout_3->addWidget(sessionDataWidget);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(2, 2, 2, 2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        weatherChartsWidget = new WeatherChartsWidget(tab_3);
        weatherChartsWidget->setObjectName(QString::fromUtf8("weatherChartsWidget"));

        verticalLayout_6->addWidget(weatherChartsWidget);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout = new QVBoxLayout(tab_4);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(tab_4);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QPalette palette1;
        QBrush brush7(QColor(220, 220, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush8(QColor(159, 159, 159, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush6);
        textEdit->setPalette(palette1);
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        textEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(textEdit);

        tabWidget->addTab(tab_4, QString());

        verticalLayout_5->addWidget(tabWidget);

        splitter->addWidget(verticalLayoutWidget);

        verticalLayout_4->addWidget(splitter);

        verticalLayout_4->setStretch(2, 1);
        LTWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LTWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1249, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        LTWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LTWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
        mainToolBar->setMovable(false);
        mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
        mainToolBar->setFloatable(false);
        LTWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LTWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setMaximumSize(QSize(16777215, 5));
        LTWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionHead_to_head);
        menuEdit->addAction(actionLap_time_comparison);
        menuEdit->addSeparator();
        menuEdit->addAction(actionRecord);
        menuEdit->addAction(actionStop_recording);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionConnect);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionHead_to_head);
        mainToolBar->addAction(actionLap_time_comparison);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPreferences);
        mainToolBar->addAction(actionExit);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRecord);
        mainToolBar->addAction(actionStop_recording);

        retranslateUi(LTWindow);
        QObject::connect(actionExit, SIGNAL(triggered(bool)), LTWindow, SLOT(close()));

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(LTWindow);
    } // setupUi

    void retranslateUi(QMainWindow *LTWindow)
    {
        LTWindow->setWindowTitle(QApplication::translate("LTWindow", "F1LT", 0, QApplication::UnicodeUTF8));
        actionHead_to_head->setText(QApplication::translate("LTWindow", "Head to head", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("LTWindow", "Login", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConnect->setToolTip(QApplication::translate("LTWindow", "Login", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRecord->setText(QApplication::translate("LTWindow", "Record", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("LTWindow", "Preferences", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("LTWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("LTWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("LTWindow", "About", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("LTWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("LTWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionLap_time_comparison->setText(QApplication::translate("LTWindow", "Lap time comparison", 0, QApplication::UnicodeUTF8));
        actionStop_recording->setText(QApplication::translate("LTWindow", "Stop recording", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("LTWindow", "P", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem1->setText(QApplication::translate("LTWindow", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem2->setText(QApplication::translate("LTWindow", "Gap", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem3->setText(QApplication::translate("LTWindow", "Interval", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem4->setText(QApplication::translate("LTWindow", "Sector 1", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem5->setText(QApplication::translate("LTWindow", "Sector 2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem6->setText(QApplication::translate("LTWindow", "Sector 3", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(10);
        ___qtablewidgetitem7->setText(QApplication::translate("LTWindow", "Pit", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("LTWindow", "Driver data", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("LTWindow", "Session data", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("LTWindow", "Weather charts", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("LTWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Tahoma'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("LTWindow", "Commentary", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("LTWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("LTWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("LTWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LTWindow: public Ui_LTWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LTWINDOW_H
