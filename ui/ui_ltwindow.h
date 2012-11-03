/********************************************************************************
** Form generated from reading UI file 'ltwindow.ui'
**
** Created: Thu Nov 1 13:17:13 2012
**      by: Qt User Interface Compiler version 4.8.3
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
#include "main_gui/driverdatawidget.h"
#include "main_gui/eventstatuswidget.h"
#include "main_gui/ltwidget.h"
#include "main_gui/nosessionboardwidget.h"
#include "main_gui/sessiondatawidget.h"
#include "main_gui/weatherchartswidget.h"

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
    QAction *actionSession_analysis;
    QAction *actionLT_files_data_base;
    QAction *actionFollow_a_driver;
    QAction *actionSession_times;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    NoSessionBoardWidget *messageBoardWidget;
    EventStatusWidget *eventStatusWidget;
    QSplitter *splitter;
    LTWidget *ltWidget;
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
        actionSession_analysis = new QAction(LTWindow);
        actionSession_analysis->setObjectName(QString::fromUtf8("actionSession_analysis"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/ui_icons/sa.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSession_analysis->setIcon(icon9);
        actionLT_files_data_base = new QAction(LTWindow);
        actionLT_files_data_base->setObjectName(QString::fromUtf8("actionLT_files_data_base"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/ui_icons/ltdb.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLT_files_data_base->setIcon(icon10);
        actionFollow_a_driver = new QAction(LTWindow);
        actionFollow_a_driver->setObjectName(QString::fromUtf8("actionFollow_a_driver"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/ui_icons/fad.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFollow_a_driver->setIcon(icon11);
        actionSession_times = new QAction(LTWindow);
        actionSession_times->setObjectName(QString::fromUtf8("actionSession_times"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/ui_icons/st.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSession_times->setIcon(icon12);
        centralWidget = new QWidget(LTWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(4, 4, 4, 4);
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
        splitter->setOrientation(Qt::Horizontal);
        ltWidget = new LTWidget(splitter);
        ltWidget->setObjectName(QString::fromUtf8("ltWidget"));
        splitter->addWidget(ltWidget);
        tabWidget = new QTabWidget(splitter);
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
        QPalette palette;
        QBrush brush(QColor(220, 220, 220, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(20, 20, 20, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush2(QColor(159, 159, 159, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush2);
        QBrush brush3(QColor(130, 130, 130, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        QBrush brush4(QColor(240, 240, 240, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        QBrush brush5(QColor(175, 175, 175, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        textEdit->setPalette(palette);
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        textEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(textEdit);

        tabWidget->addTab(tab_4, QString());
        splitter->addWidget(tabWidget);

        verticalLayout_4->addWidget(splitter);

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
        menuFile->addAction(actionLT_files_data_base);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionSession_analysis);
        menuEdit->addAction(actionSession_times);
        menuEdit->addAction(actionFollow_a_driver);
        menuEdit->addAction(actionHead_to_head);
        menuEdit->addAction(actionLap_time_comparison);
        menuEdit->addSeparator();
        menuEdit->addAction(actionRecord);
        menuEdit->addAction(actionStop_recording);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        mainToolBar->addAction(actionLT_files_data_base);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionConnect);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionFollow_a_driver);
        mainToolBar->addAction(actionHead_to_head);
        mainToolBar->addAction(actionLap_time_comparison);
        mainToolBar->addAction(actionSession_analysis);
        mainToolBar->addAction(actionSession_times);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPreferences);
        mainToolBar->addAction(actionExit);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRecord);
        mainToolBar->addAction(actionStop_recording);

        retranslateUi(LTWindow);
        QObject::connect(actionExit, SIGNAL(triggered(bool)), LTWindow, SLOT(close()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LTWindow);
    } // setupUi

    void retranslateUi(QMainWindow *LTWindow)
    {
        LTWindow->setWindowTitle(QApplication::translate("LTWindow", "F1LT", 0, QApplication::UnicodeUTF8));
        actionHead_to_head->setText(QApplication::translate("LTWindow", "Head to head", 0, QApplication::UnicodeUTF8));
        actionHead_to_head->setShortcut(QApplication::translate("LTWindow", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("LTWindow", "Login", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConnect->setToolTip(QApplication::translate("LTWindow", "Login", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionConnect->setShortcut(QApplication::translate("LTWindow", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        actionRecord->setText(QApplication::translate("LTWindow", "Record", 0, QApplication::UnicodeUTF8));
        actionRecord->setShortcut(QApplication::translate("LTWindow", "Ctrl+Shift+R", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("LTWindow", "Preferences", 0, QApplication::UnicodeUTF8));
        actionPreferences->setShortcut(QApplication::translate("LTWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("LTWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("LTWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("LTWindow", "About", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("LTWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("LTWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("LTWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionLap_time_comparison->setText(QApplication::translate("LTWindow", "Lap time comparison", 0, QApplication::UnicodeUTF8));
        actionLap_time_comparison->setShortcut(QApplication::translate("LTWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
        actionStop_recording->setText(QApplication::translate("LTWindow", "Stop recording", 0, QApplication::UnicodeUTF8));
        actionStop_recording->setShortcut(QApplication::translate("LTWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionSession_analysis->setText(QApplication::translate("LTWindow", "Session analysis", 0, QApplication::UnicodeUTF8));
        actionSession_analysis->setShortcut(QApplication::translate("LTWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionLT_files_data_base->setText(QApplication::translate("LTWindow", "LT files database", 0, QApplication::UnicodeUTF8));
        actionLT_files_data_base->setShortcut(QApplication::translate("LTWindow", "Ctrl+B", 0, QApplication::UnicodeUTF8));
        actionFollow_a_driver->setText(QApplication::translate("LTWindow", "Follow a driver", 0, QApplication::UnicodeUTF8));
        actionFollow_a_driver->setShortcut(QApplication::translate("LTWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionSession_times->setText(QApplication::translate("LTWindow", "Session times", 0, QApplication::UnicodeUTF8));
        actionSession_times->setShortcut(QApplication::translate("LTWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
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
