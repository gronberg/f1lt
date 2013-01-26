/********************************************************************************
** Form generated from reading UI file 'headtoheaddialog.ui'
**
** Created: Sat Jan 26 20:30:45 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEADTOHEADDIALOG_H
#define UI_HEADTOHEADDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HeadToHeadDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QComboBox *comboBox1;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *gapChartTableWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *chartsTableWidget;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *posChartTableWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *HeadToHeadDialog)
    {
        if (HeadToHeadDialog->objectName().isEmpty())
            HeadToHeadDialog->setObjectName(QString::fromUtf8("HeadToHeadDialog"));
        HeadToHeadDialog->setWindowModality(Qt::NonModal);
        HeadToHeadDialog->resize(660, 631);
        HeadToHeadDialog->setMinimumSize(QSize(400, 0));
        HeadToHeadDialog->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        HeadToHeadDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(HeadToHeadDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, -1, 0);
        comboBox1 = new QComboBox(HeadToHeadDialog);
        comboBox1->setObjectName(QString::fromUtf8("comboBox1"));

        gridLayout->addWidget(comboBox1, 1, 0, 1, 1);

        label = new QLabel(HeadToHeadDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(HeadToHeadDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        comboBox2 = new QComboBox(HeadToHeadDialog);
        comboBox2->setObjectName(QString::fromUtf8("comboBox2"));

        gridLayout->addWidget(comboBox2, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        tabWidget = new QTabWidget(HeadToHeadDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 12)
            tableWidget->setColumnCount(12);
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
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QPalette palette;
        QBrush brush(QColor(230, 230, 230, 255));
        brush.setStyle(Qt::SolidPattern);
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
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        QBrush brush4(QColor(130, 130, 130, 255));
        brush4.setStyle(Qt::SolidPattern);
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
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        tableWidget->setFont(font);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
        tableWidget->setShowGrid(false);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(tableWidget);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        gapChartTableWidget = new QTableWidget(tab_3);
        if (gapChartTableWidget->columnCount() < 2)
            gapChartTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        gapChartTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        gapChartTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        gapChartTableWidget->setObjectName(QString::fromUtf8("gapChartTableWidget"));
        QPalette palette1;
        QBrush brush7(QColor(255, 255, 255, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        gapChartTableWidget->setPalette(palette1);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        gapChartTableWidget->setFont(font1);
        gapChartTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        gapChartTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        gapChartTableWidget->setIconSize(QSize(300, 300));
        gapChartTableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        gapChartTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        gapChartTableWidget->setShowGrid(false);
        gapChartTableWidget->horizontalHeader()->setVisible(false);
        gapChartTableWidget->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(gapChartTableWidget);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        chartsTableWidget = new QTableWidget(tab_2);
        if (chartsTableWidget->columnCount() < 2)
            chartsTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem15);
        chartsTableWidget->setObjectName(QString::fromUtf8("chartsTableWidget"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        chartsTableWidget->setPalette(palette2);
        chartsTableWidget->setFont(font1);
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
        verticalLayout_5 = new QVBoxLayout(tab_4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        posChartTableWidget = new QTableWidget(tab_4);
        if (posChartTableWidget->columnCount() < 2)
            posChartTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        posChartTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        posChartTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem17);
        posChartTableWidget->setObjectName(QString::fromUtf8("posChartTableWidget"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        posChartTableWidget->setPalette(palette3);
        posChartTableWidget->setFont(font1);
        posChartTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        posChartTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        posChartTableWidget->setIconSize(QSize(300, 300));
        posChartTableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        posChartTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        posChartTableWidget->setShowGrid(false);
        posChartTableWidget->horizontalHeader()->setVisible(false);
        posChartTableWidget->verticalHeader()->setVisible(false);

        verticalLayout_5->addWidget(posChartTableWidget);

        tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 8, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(HeadToHeadDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(HeadToHeadDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(HeadToHeadDialog);
    } // setupUi

    void retranslateUi(QDialog *HeadToHeadDialog)
    {
        HeadToHeadDialog->setWindowTitle(QApplication::translate("HeadToHeadDialog", "Head to head", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HeadToHeadDialog", "Driver 1", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("HeadToHeadDialog", "Driver 2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem->setText(QApplication::translate("HeadToHeadDialog", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("HeadToHeadDialog", "Laps", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = gapChartTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("HeadToHeadDialog", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("HeadToHeadDialog", "Gap chart", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = chartsTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("HeadToHeadDialog", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("HeadToHeadDialog", "Lap time chart", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = posChartTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("HeadToHeadDialog", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("HeadToHeadDialog", "Position chart", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("HeadToHeadDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HeadToHeadDialog: public Ui_HeadToHeadDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEADTOHEADDIALOG_H
