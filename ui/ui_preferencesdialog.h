/********************************************************************************
** Form generated from reading UI file 'preferencesdialog.ui'
**
** Created: Sat Mar 9 13:50:06 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *commentaryFontButton;
    QPushButton *mainFontButton;
    QLabel *label;
    QLabel *label_2;
    QCheckBox *ltCheckBox;
    QCheckBox *autoConnectBox;
    QCheckBox *autoRecordBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *autoStopRecordBox;
    QSpinBox *autoStopRecordSpinBox;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *autoSaveRecordBox;
    QSpinBox *autoSaveRecordSpinBox;
    QLabel *label_10;
    QCheckBox *thumbnailsCheckBox;
    QCheckBox *trackerBox;
    QSpacerItem *verticalSpacer_2;
    QWidget *Co;
    QGridLayout *gridLayout_3;
    QToolButton *colorCyanButton;
    QToolButton *colorWhiteButton;
    QPushButton *pushButton_6;
    QPushButton *pushButton_3;
    QLabel *label_15;
    QLabel *label_14;
    QPushButton *pushButton_2;
    QLabel *label_9;
    QPushButton *pushButton_5;
    QToolButton *colorGreenButton;
    QPushButton *pushButton_4;
    QLabel *label_13;
    QLabel *label_16;
    QLabel *label_11;
    QPushButton *pushButton;
    QToolButton *colorRedButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *colorYellowButton;
    QToolButton *colorVioletButton;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_7;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_8;
    QSpacerItem *horizontalSpacer_6;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *proxyCheckBox;
    QGroupBox *proxyBox;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QSpinBox *proxyPortEdit;
    QLabel *label_8;
    QComboBox *proxyTypeBox;
    QLineEdit *proxyHostEdit;
    QLineEdit *proxyUserEdit;
    QLineEdit *proxyPasswordEdit;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName(QString::fromUtf8("PreferencesDialog"));
        PreferencesDialog->resize(447, 389);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        PreferencesDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(PreferencesDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(PreferencesDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        commentaryFontButton = new QPushButton(groupBox);
        commentaryFontButton->setObjectName(QString::fromUtf8("commentaryFontButton"));

        gridLayout->addWidget(commentaryFontButton, 2, 1, 1, 1);

        mainFontButton = new QPushButton(groupBox);
        mainFontButton->setObjectName(QString::fromUtf8("mainFontButton"));

        gridLayout->addWidget(mainFontButton, 0, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox);

        ltCheckBox = new QCheckBox(tab);
        ltCheckBox->setObjectName(QString::fromUtf8("ltCheckBox"));
        ltCheckBox->setChecked(true);

        verticalLayout_4->addWidget(ltCheckBox);

        autoConnectBox = new QCheckBox(tab);
        autoConnectBox->setObjectName(QString::fromUtf8("autoConnectBox"));

        verticalLayout_4->addWidget(autoConnectBox);

        autoRecordBox = new QCheckBox(tab);
        autoRecordBox->setObjectName(QString::fromUtf8("autoRecordBox"));

        verticalLayout_4->addWidget(autoRecordBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        autoStopRecordBox = new QCheckBox(tab);
        autoStopRecordBox->setObjectName(QString::fromUtf8("autoStopRecordBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(autoStopRecordBox->sizePolicy().hasHeightForWidth());
        autoStopRecordBox->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(autoStopRecordBox);

        autoStopRecordSpinBox = new QSpinBox(tab);
        autoStopRecordSpinBox->setObjectName(QString::fromUtf8("autoStopRecordSpinBox"));
        autoStopRecordSpinBox->setEnabled(false);
        autoStopRecordSpinBox->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(autoStopRecordSpinBox);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        autoSaveRecordBox = new QCheckBox(tab);
        autoSaveRecordBox->setObjectName(QString::fromUtf8("autoSaveRecordBox"));

        horizontalLayout_4->addWidget(autoSaveRecordBox);

        autoSaveRecordSpinBox = new QSpinBox(tab);
        autoSaveRecordSpinBox->setObjectName(QString::fromUtf8("autoSaveRecordSpinBox"));
        autoSaveRecordSpinBox->setEnabled(false);
        autoSaveRecordSpinBox->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_4->addWidget(autoSaveRecordSpinBox);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_4->addWidget(label_10);


        verticalLayout_4->addLayout(horizontalLayout_4);

        thumbnailsCheckBox = new QCheckBox(tab);
        thumbnailsCheckBox->setObjectName(QString::fromUtf8("thumbnailsCheckBox"));

        verticalLayout_4->addWidget(thumbnailsCheckBox);

        trackerBox = new QCheckBox(tab);
        trackerBox->setObjectName(QString::fromUtf8("trackerBox"));

        verticalLayout_4->addWidget(trackerBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        tabWidget->addTab(tab, QString());
        Co = new QWidget();
        Co->setObjectName(QString::fromUtf8("Co"));
        gridLayout_3 = new QGridLayout(Co);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        colorCyanButton = new QToolButton(Co);
        colorCyanButton->setObjectName(QString::fromUtf8("colorCyanButton"));
        colorCyanButton->setMaximumSize(QSize(24, 24));
        colorCyanButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 255);\n"
"     border-style: solid;\n"
"     border-width: 1px;\n"
"     border-radius: 3px;\n"
"     border-color: rgb(153, 153, 153);\n"
"     padding: 3px;\n"
""));

        gridLayout_3->addWidget(colorCyanButton, 1, 2, 1, 1);

        colorWhiteButton = new QToolButton(Co);
        colorWhiteButton->setObjectName(QString::fromUtf8("colorWhiteButton"));
        colorWhiteButton->setMaximumSize(QSize(24, 24));
        colorWhiteButton->setStyleSheet(QString::fromUtf8("background-color: rgb(218, 218, 218);\n"
"     border-style: solid;\n"
"     border-width: 1px;\n"
"     border-radius: 3px;\n"
"     border-color: rgb(153, 153, 153);\n"
"     padding: 3px;\n"
""));

        gridLayout_3->addWidget(colorWhiteButton, 0, 2, 1, 1);

        pushButton_6 = new QPushButton(Co);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        gridLayout_3->addWidget(pushButton_6, 6, 4, 1, 1);

        pushButton_3 = new QPushButton(Co);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout_3->addWidget(pushButton_3, 2, 4, 1, 1);

        label_15 = new QLabel(Co);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 6, 0, 1, 1);

        label_14 = new QLabel(Co);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_3->addWidget(label_14, 5, 0, 1, 1);

        pushButton_2 = new QPushButton(Co);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout_3->addWidget(pushButton_2, 1, 4, 1, 1);

        label_9 = new QLabel(Co);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 0, 0, 1, 1);

        pushButton_5 = new QPushButton(Co);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        gridLayout_3->addWidget(pushButton_5, 5, 4, 1, 1);

        colorGreenButton = new QToolButton(Co);
        colorGreenButton->setObjectName(QString::fromUtf8("colorGreenButton"));
        colorGreenButton->setMaximumSize(QSize(24, 24));
        colorGreenButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);\n"
"     border-style: solid;\n"
"     border-width: 1px;\n"
"     border-radius: 3px;\n"
"     border-color: rgb(153, 153, 153);\n"
"     padding: 3px;\n"
""));

        gridLayout_3->addWidget(colorGreenButton, 5, 2, 1, 1);

        pushButton_4 = new QPushButton(Co);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        gridLayout_3->addWidget(pushButton_4, 3, 4, 1, 1);

        label_13 = new QLabel(Co);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 3, 0, 1, 1);

        label_16 = new QLabel(Co);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_3->addWidget(label_16, 2, 0, 1, 1);

        label_11 = new QLabel(Co);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 1, 0, 1, 1);

        pushButton = new QPushButton(Co);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_3->addWidget(pushButton, 0, 4, 1, 1);

        colorRedButton = new QToolButton(Co);
        colorRedButton->setObjectName(QString::fromUtf8("colorRedButton"));
        colorRedButton->setMaximumSize(QSize(24, 24));
        colorRedButton->setStyleSheet(QString::fromUtf8("background-color: rgb(231, 31, 31);\n"
"     border-style: solid;\n"
"     border-width: 1px;\n"
"     border-radius: 3px;\n"
"     border-color: rgb(153, 153, 153);\n"
"     padding: 3px;\n"
""));

        gridLayout_3->addWidget(colorRedButton, 3, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        colorYellowButton = new QToolButton(Co);
        colorYellowButton->setObjectName(QString::fromUtf8("colorYellowButton"));
        colorYellowButton->setMaximumSize(QSize(24, 24));
        colorYellowButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0);\n"
"     border-style: solid;\n"
"     border-width: 1px;\n"
"     border-radius: 3px;\n"
"     border-color: rgb(153, 153, 153);\n"
"     padding: 3px;\n"
""));

        gridLayout_3->addWidget(colorYellowButton, 2, 2, 1, 1);

        colorVioletButton = new QToolButton(Co);
        colorVioletButton->setObjectName(QString::fromUtf8("colorVioletButton"));
        colorVioletButton->setMaximumSize(QSize(24, 24));
        colorVioletButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 255);\n"
"     border-style: solid;\n"
"     border-width: 1px;\n"
"     border-radius: 3px;\n"
"     border-color: rgb(153, 153, 153);\n"
"     padding: 3px;\n"
""));

        gridLayout_3->addWidget(colorVioletButton, 6, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_7 = new QPushButton(Co);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

        horizontalLayout_2->addWidget(pushButton_7);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        gridLayout_3->addLayout(horizontalLayout_2, 7, 0, 1, 5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        pushButton_8 = new QPushButton(Co);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));

        horizontalLayout_3->addWidget(pushButton_8);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        gridLayout_3->addLayout(horizontalLayout_3, 8, 0, 1, 5);

        tabWidget->addTab(Co, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        proxyCheckBox = new QCheckBox(tab_2);
        proxyCheckBox->setObjectName(QString::fromUtf8("proxyCheckBox"));

        verticalLayout_3->addWidget(proxyCheckBox);

        proxyBox = new QGroupBox(tab_2);
        proxyBox->setObjectName(QString::fromUtf8("proxyBox"));
        proxyBox->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(proxyBox->sizePolicy().hasHeightForWidth());
        proxyBox->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(proxyBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(proxyBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(proxyBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        label_6 = new QLabel(proxyBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 4, 0, 1, 1);

        label_7 = new QLabel(proxyBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 2, 0, 1, 1);

        proxyPortEdit = new QSpinBox(proxyBox);
        proxyPortEdit->setObjectName(QString::fromUtf8("proxyPortEdit"));
        proxyPortEdit->setMaximum(99999);

        gridLayout_2->addWidget(proxyPortEdit, 2, 1, 1, 1);

        label_8 = new QLabel(proxyBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 2, 2, 1, 1);

        proxyTypeBox = new QComboBox(proxyBox);
        proxyTypeBox->setObjectName(QString::fromUtf8("proxyTypeBox"));

        gridLayout_2->addWidget(proxyTypeBox, 2, 3, 1, 1);

        proxyHostEdit = new QLineEdit(proxyBox);
        proxyHostEdit->setObjectName(QString::fromUtf8("proxyHostEdit"));

        gridLayout_2->addWidget(proxyHostEdit, 1, 1, 1, 3);

        proxyUserEdit = new QLineEdit(proxyBox);
        proxyUserEdit->setObjectName(QString::fromUtf8("proxyUserEdit"));

        gridLayout_2->addWidget(proxyUserEdit, 3, 1, 1, 3);

        proxyPasswordEdit = new QLineEdit(proxyBox);
        proxyPasswordEdit->setObjectName(QString::fromUtf8("proxyPasswordEdit"));
        proxyPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(proxyPasswordEdit, 4, 1, 1, 3);


        verticalLayout_3->addWidget(proxyBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(PreferencesDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(PreferencesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PreferencesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PreferencesDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferencesDialog)
    {
        PreferencesDialog->setWindowTitle(QApplication::translate("PreferencesDialog", "Preferences", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PreferencesDialog", "Fonts", 0, QApplication::UnicodeUTF8));
        commentaryFontButton->setText(QApplication::translate("PreferencesDialog", "Tahoma", 0, QApplication::UnicodeUTF8));
        mainFontButton->setText(QApplication::translate("PreferencesDialog", "Arial", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PreferencesDialog", "Main font:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PreferencesDialog", "Commentary font:", 0, QApplication::UnicodeUTF8));
        ltCheckBox->setText(QApplication::translate("PreferencesDialog", "Show contents during LT window resize", 0, QApplication::UnicodeUTF8));
        autoConnectBox->setText(QApplication::translate("PreferencesDialog", "Connect to LT serwer automatically after application start", 0, QApplication::UnicodeUTF8));
        autoRecordBox->setText(QApplication::translate("PreferencesDialog", "Automatically start recording when session starts", 0, QApplication::UnicodeUTF8));
        autoStopRecordBox->setText(QApplication::translate("PreferencesDialog", "Automatically stop recording ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PreferencesDialog", "minutes after session ends", 0, QApplication::UnicodeUTF8));
        autoSaveRecordBox->setText(QApplication::translate("PreferencesDialog", "Auto-save currently recorded session file every ", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("PreferencesDialog", "minutes", 0, QApplication::UnicodeUTF8));
        thumbnailsCheckBox->setText(QApplication::translate("PreferencesDialog", "Draw car thumbnails", 0, QApplication::UnicodeUTF8));
        trackerBox->setText(QApplication::translate("PreferencesDialog", "Draw drivers classification in driver tracker", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("PreferencesDialog", "General", 0, QApplication::UnicodeUTF8));
        colorCyanButton->setText(QString());
        colorWhiteButton->setText(QString());
        pushButton_6->setText(QApplication::translate("PreferencesDialog", "Reset to default", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("PreferencesDialog", "Reset to default", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("PreferencesDialog", "Lap record color:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("PreferencesDialog", "Personal best time color:", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("PreferencesDialog", "Reset to default", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("PreferencesDialog", "Main color:", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("PreferencesDialog", "Reset to default", 0, QApplication::UnicodeUTF8));
        colorGreenButton->setText(QString());
        pushButton_4->setText(QApplication::translate("PreferencesDialog", "Reset to default", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("PreferencesDialog", "Pit color:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("PreferencesDialog", "Gap color:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("PreferencesDialog", "Position color:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("PreferencesDialog", "Reset to default", 0, QApplication::UnicodeUTF8));
        colorRedButton->setText(QString());
        colorYellowButton->setText(QString());
        colorVioletButton->setText(QString());
        pushButton_7->setText(QApplication::translate("PreferencesDialog", "Reset all to defaults", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("PreferencesDialog", "Driver colors", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Co), QApplication::translate("PreferencesDialog", "Colors", 0, QApplication::UnicodeUTF8));
        proxyCheckBox->setText(QApplication::translate("PreferencesDialog", "Use proxy", 0, QApplication::UnicodeUTF8));
        proxyBox->setTitle(QApplication::translate("PreferencesDialog", "Proxy settings", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PreferencesDialog", "Host:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("PreferencesDialog", "User:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("PreferencesDialog", "Password:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PreferencesDialog", "Port:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PreferencesDialog", "Type:", 0, QApplication::UnicodeUTF8));
        proxyTypeBox->clear();
        proxyTypeBox->insertItems(0, QStringList()
         << QApplication::translate("PreferencesDialog", "HTTP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PreferencesDialog", "Socks5", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("PreferencesDialog", "Network", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
