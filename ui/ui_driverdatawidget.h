/********************************************************************************
** Form generated from reading UI file 'driverdatawidget.ui'
**
** Created: Fri Aug 31 16:18:36 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRIVERDATAWIDGET_H
#define UI_DRIVERDATAWIDGET_H

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
    QLabel *gridPositionLabel;
    QLabel *gapLabel;
    QLabel *label;
    QLabel *lastLapLabel;
    QLabel *label_4;
    QLabel *gridLabel;
    QLabel *numPitsLabel;
    QLabel *label_5;
    QLabel *bestLapLabel;
    QLabel *currentPositionLabel;
    QLabel *pitStopsLabel;
    QLabel *label_3;
    QLabel *carImageLabel;
    QLabel *label_2;
    QLabel *driverNameLabel;
    QTableView *tableView;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *lapTimeChartTableWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *chartsTableWidget;

    void setupUi(QWidget *DriverDataWidget)
    {
        if (DriverDataWidget->objectName().isEmpty())
            DriverDataWidget->setObjectName(QString::fromUtf8("DriverDataWidget"));
        DriverDataWidget->resize(650, 615);
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
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 626, 566));
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
        gridPositionLabel = new QLabel(infoWidget);
        gridPositionLabel->setObjectName(QString::fromUtf8("gridPositionLabel"));
        QPalette palette1;
        QBrush brush2(QColor(255, 255, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        QBrush brush3(QColor(130, 130, 130, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        gridPositionLabel->setPalette(palette1);
        gridPositionLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(gridPositionLabel, 5, 1, 1, 1);

        gapLabel = new QLabel(infoWidget);
        gapLabel->setObjectName(QString::fromUtf8("gapLabel"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        gapLabel->setPalette(palette2);
        gapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(gapLabel, 8, 1, 1, 1);

        label = new QLabel(infoWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette3;
        QBrush brush4(QColor(150, 150, 150, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label->setPalette(palette3);

        gridLayout->addWidget(label, 7, 0, 1, 1);

        lastLapLabel = new QLabel(infoWidget);
        lastLapLabel->setObjectName(QString::fromUtf8("lastLapLabel"));
        QPalette palette4;
        QBrush brush5(QColor(220, 220, 220, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        lastLapLabel->setPalette(palette4);
        lastLapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lastLapLabel, 5, 3, 1, 1);

        label_4 = new QLabel(infoWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_4->setPalette(palette5);

        gridLayout->addWidget(label_4, 7, 2, 1, 1);

        gridLabel = new QLabel(infoWidget);
        gridLabel->setObjectName(QString::fromUtf8("gridLabel"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        gridLabel->setPalette(palette6);

        gridLayout->addWidget(gridLabel, 5, 0, 1, 1);

        numPitsLabel = new QLabel(infoWidget);
        numPitsLabel->setObjectName(QString::fromUtf8("numPitsLabel"));
        QPalette palette7;
        QBrush brush6(QColor(255, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        numPitsLabel->setPalette(palette7);
        numPitsLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(numPitsLabel, 8, 3, 1, 1);

        label_5 = new QLabel(infoWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_5->setPalette(palette8);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 10, 0, 1, 4);

        bestLapLabel = new QLabel(infoWidget);
        bestLapLabel->setObjectName(QString::fromUtf8("bestLapLabel"));
        QPalette palette9;
        QBrush brush7(QColor(0, 255, 0, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        bestLapLabel->setPalette(palette9);
        bestLapLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(bestLapLabel, 7, 3, 1, 1);

        currentPositionLabel = new QLabel(infoWidget);
        currentPositionLabel->setObjectName(QString::fromUtf8("currentPositionLabel"));
        QPalette palette10;
        QBrush brush8(QColor(0, 255, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        currentPositionLabel->setPalette(palette10);
        currentPositionLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(currentPositionLabel, 7, 1, 1, 1);

        pitStopsLabel = new QLabel(infoWidget);
        pitStopsLabel->setObjectName(QString::fromUtf8("pitStopsLabel"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        pitStopsLabel->setPalette(palette11);

        gridLayout->addWidget(pitStopsLabel, 8, 2, 1, 1);

        label_3 = new QLabel(infoWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_3->setPalette(palette12);

        gridLayout->addWidget(label_3, 5, 2, 1, 1);

        carImageLabel = new QLabel(infoWidget);
        carImageLabel->setObjectName(QString::fromUtf8("carImageLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(carImageLabel->sizePolicy().hasHeightForWidth());
        carImageLabel->setSizePolicy(sizePolicy);
        carImageLabel->setMinimumSize(QSize(0, 80));
        carImageLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(carImageLabel, 2, 0, 1, 4);

        label_2 = new QLabel(infoWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_2->setPalette(palette13);

        gridLayout->addWidget(label_2, 8, 0, 1, 1);

        driverNameLabel = new QLabel(infoWidget);
        driverNameLabel->setObjectName(QString::fromUtf8("driverNameLabel"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        driverNameLabel->setPalette(palette14);
        driverNameLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(driverNameLabel, 1, 0, 1, 4);


        verticalLayout->addWidget(infoWidget);

        tableView = new QTableView(scrollAreaWidgetContents_5);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush9(QColor(73, 73, 73, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette15.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        QBrush brush10(QColor(27, 27, 27, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette15.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette15.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette15.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush11(QColor(240, 240, 240, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        QBrush brush12(QColor(175, 175, 175, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette15.setBrush(QPalette::Disabled, QPalette::Highlight, brush12);
        palette15.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        tableView->setPalette(palette15);
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
        QPalette palette16;
        QBrush brush13(QColor(255, 255, 255, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush13);
        palette16.setBrush(QPalette::Active, QPalette::Text, brush13);
        palette16.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush13);
        palette16.setBrush(QPalette::Inactive, QPalette::Text, brush13);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette16.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        lapTimeChartTableWidget->setPalette(palette16);
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
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush13);
        palette17.setBrush(QPalette::Active, QPalette::Text, brush13);
        palette17.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush13);
        palette17.setBrush(QPalette::Inactive, QPalette::Text, brush13);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette17.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        chartsTableWidget->setPalette(palette17);
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

        verticalLayout_5->addWidget(tabWidget);


        retranslateUi(DriverDataWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DriverDataWidget);
    } // setupUi

    void retranslateUi(QWidget *DriverDataWidget)
    {
        DriverDataWidget->setWindowTitle(QApplication::translate("DriverDataWidget", "Form", 0, QApplication::UnicodeUTF8));
        gridPositionLabel->setText(QString());
        gapLabel->setText(QString());
        label->setText(QApplication::translate("DriverDataWidget", "Current position:", 0, QApplication::UnicodeUTF8));
        lastLapLabel->setText(QString());
        label_4->setText(QApplication::translate("DriverDataWidget", "Best lap:", 0, QApplication::UnicodeUTF8));
        gridLabel->setText(QApplication::translate("DriverDataWidget", "Grid position:", 0, QApplication::UnicodeUTF8));
        numPitsLabel->setText(QString());
        label_5->setText(QApplication::translate("DriverDataWidget", "Lap history", 0, QApplication::UnicodeUTF8));
        bestLapLabel->setText(QString());
        currentPositionLabel->setText(QString());
        pitStopsLabel->setText(QApplication::translate("DriverDataWidget", "Pit stops:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DriverDataWidget", "Last lap:", 0, QApplication::UnicodeUTF8));
        carImageLabel->setText(QString());
        label_2->setText(QApplication::translate("DriverDataWidget", "Gap to leader:", 0, QApplication::UnicodeUTF8));
        driverNameLabel->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DriverDataWidget", "Lap history", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("DriverDataWidget", "Lap time chart", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DriverDataWidget", "Position && gap charts", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DriverDataWidget: public Ui_DriverDataWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRIVERDATAWIDGET_H
