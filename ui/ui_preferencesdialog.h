/********************************************************************************
** Form generated from reading UI file 'preferencesdialog.ui'
**
** Created: Thu Oct 11 14:54:05 2012
**      by: Qt User Interface Compiler version 4.8.2
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
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *commentaryFontButton;
    QPushButton *mainFontButton;
    QLabel *label;
    QLabel *label_2;
    QCheckBox *autoConnectBox;
    QCheckBox *ltCheckBox;
    QCheckBox *autoRecordBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *autoStopRecordBox;
    QSpinBox *autoStopRecordSpinBox;
    QLabel *label_3;
    QCheckBox *thumbnailsCheckBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *revH2HBox;
    QCheckBox *revLTCBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName(QString::fromUtf8("PreferencesDialog"));
        PreferencesDialog->resize(447, 371);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        PreferencesDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(PreferencesDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(PreferencesDialog);
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


        verticalLayout->addWidget(groupBox);

        autoConnectBox = new QCheckBox(PreferencesDialog);
        autoConnectBox->setObjectName(QString::fromUtf8("autoConnectBox"));

        verticalLayout->addWidget(autoConnectBox);

        ltCheckBox = new QCheckBox(PreferencesDialog);
        ltCheckBox->setObjectName(QString::fromUtf8("ltCheckBox"));
        ltCheckBox->setChecked(true);

        verticalLayout->addWidget(ltCheckBox);

        autoRecordBox = new QCheckBox(PreferencesDialog);
        autoRecordBox->setObjectName(QString::fromUtf8("autoRecordBox"));

        verticalLayout->addWidget(autoRecordBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        autoStopRecordBox = new QCheckBox(PreferencesDialog);
        autoStopRecordBox->setObjectName(QString::fromUtf8("autoStopRecordBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(autoStopRecordBox->sizePolicy().hasHeightForWidth());
        autoStopRecordBox->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(autoStopRecordBox);

        autoStopRecordSpinBox = new QSpinBox(PreferencesDialog);
        autoStopRecordSpinBox->setObjectName(QString::fromUtf8("autoStopRecordSpinBox"));
        autoStopRecordSpinBox->setEnabled(false);
        autoStopRecordSpinBox->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(autoStopRecordSpinBox);

        label_3 = new QLabel(PreferencesDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout);

        thumbnailsCheckBox = new QCheckBox(PreferencesDialog);
        thumbnailsCheckBox->setObjectName(QString::fromUtf8("thumbnailsCheckBox"));

        verticalLayout->addWidget(thumbnailsCheckBox);

        groupBox_2 = new QGroupBox(PreferencesDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        revH2HBox = new QCheckBox(groupBox_2);
        revH2HBox->setObjectName(QString::fromUtf8("revH2HBox"));

        verticalLayout_2->addWidget(revH2HBox);

        revLTCBox = new QCheckBox(groupBox_2);
        revLTCBox->setObjectName(QString::fromUtf8("revLTCBox"));

        verticalLayout_2->addWidget(revLTCBox);


        verticalLayout->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(PreferencesDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(PreferencesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PreferencesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PreferencesDialog, SLOT(reject()));

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
        autoConnectBox->setText(QApplication::translate("PreferencesDialog", "Connect to LT serwer automatically after application start", 0, QApplication::UnicodeUTF8));
        ltCheckBox->setText(QApplication::translate("PreferencesDialog", "Show contents during LT window resize", 0, QApplication::UnicodeUTF8));
        autoRecordBox->setText(QApplication::translate("PreferencesDialog", "Automatically start recording when session starts", 0, QApplication::UnicodeUTF8));
        autoStopRecordBox->setText(QApplication::translate("PreferencesDialog", "Automatically stop recording ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PreferencesDialog", "minutes after session ends", 0, QApplication::UnicodeUTF8));
        thumbnailsCheckBox->setText(QApplication::translate("PreferencesDialog", "Draw car thumbnails", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("PreferencesDialog", "Print laps in reverse order", 0, QApplication::UnicodeUTF8));
        revH2HBox->setText(QApplication::translate("PreferencesDialog", "In head to head", 0, QApplication::UnicodeUTF8));
        revLTCBox->setText(QApplication::translate("PreferencesDialog", "In lap time comparison", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
