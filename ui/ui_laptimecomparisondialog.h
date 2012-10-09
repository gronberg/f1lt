/********************************************************************************
** Form generated from reading UI file 'laptimecomparisondialog.ui'
**
** Created: Tue Oct 9 20:44:34 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAPTIMECOMPARISONDIALOG_H
#define UI_LAPTIMECOMPARISONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LapTimeComparisonDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QComboBox *comboBox1;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboBox2;
    QComboBox *comboBox3;
    QComboBox *comboBox4;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *chartsTableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LapTimeComparisonDialog)
    {
        if (LapTimeComparisonDialog->objectName().isEmpty())
            LapTimeComparisonDialog->setObjectName(QString::fromUtf8("LapTimeComparisonDialog"));
        LapTimeComparisonDialog->resize(700, 484);
        LapTimeComparisonDialog->setMinimumSize(QSize(700, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        LapTimeComparisonDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(LapTimeComparisonDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, -1, 0);
        comboBox1 = new QComboBox(LapTimeComparisonDialog);
        comboBox1->setObjectName(QString::fromUtf8("comboBox1"));

        gridLayout->addWidget(comboBox1, 1, 0, 1, 1);

        label = new QLabel(LapTimeComparisonDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(LapTimeComparisonDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(LapTimeComparisonDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(LapTimeComparisonDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        comboBox2 = new QComboBox(LapTimeComparisonDialog);
        comboBox2->setObjectName(QString::fromUtf8("comboBox2"));

        gridLayout->addWidget(comboBox2, 1, 1, 1, 1);

        comboBox3 = new QComboBox(LapTimeComparisonDialog);
        comboBox3->setObjectName(QString::fromUtf8("comboBox3"));

        gridLayout->addWidget(comboBox3, 1, 2, 1, 1);

        comboBox4 = new QComboBox(LapTimeComparisonDialog);
        comboBox4->setObjectName(QString::fromUtf8("comboBox4"));

        gridLayout->addWidget(comboBox4, 1, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        tabWidget = new QTabWidget(LapTimeComparisonDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
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
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        chartsTableWidget = new QTableWidget(tab_2);
        if (chartsTableWidget->columnCount() < 4)
            chartsTableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        chartsTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        chartsTableWidget->setObjectName(QString::fromUtf8("chartsTableWidget"));
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
        chartsTableWidget->setPalette(palette1);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
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

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(LapTimeComparisonDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(LapTimeComparisonDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LapTimeComparisonDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LapTimeComparisonDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LapTimeComparisonDialog);
    } // setupUi

    void retranslateUi(QDialog *LapTimeComparisonDialog)
    {
        LapTimeComparisonDialog->setWindowTitle(QApplication::translate("LapTimeComparisonDialog", "Lap time comparison", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LapTimeComparisonDialog", "Driver 1", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LapTimeComparisonDialog", "Driver 2", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LapTimeComparisonDialog", "Driver 3", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("LapTimeComparisonDialog", "Driver 4", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("LapTimeComparisonDialog", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("LapTimeComparisonDialog", "Laps", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = chartsTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("LapTimeComparisonDialog", "New Column", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("LapTimeComparisonDialog", "Chart", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LapTimeComparisonDialog: public Ui_LapTimeComparisonDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAPTIMECOMPARISONDIALOG_H
