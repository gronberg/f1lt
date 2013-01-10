/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created: Thu Jan 10 18:46:15 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *emailEdit;
    QLabel *label_2;
    QLineEdit *passwordEdit;
    QDialogButtonBox *buttonBox;
    QCheckBox *proxyCheckBox;
    QGroupBox *proxyBox;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_5;
    QSpinBox *proxyPortEdit;
    QLabel *label_7;
    QComboBox *proxyTypeBox;
    QLineEdit *proxyHostEdit;
    QLineEdit *proxyUserEdit;
    QLineEdit *proxyPasswordEdit;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QString::fromUtf8("LoginDialog"));
        LoginDialog->setWindowModality(Qt::ApplicationModal);
        LoginDialog->resize(400, 290);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoginDialog->sizePolicy().hasHeightForWidth());
        LoginDialog->setSizePolicy(sizePolicy);
        LoginDialog->setMinimumSize(QSize(400, 135));
        LoginDialog->setMaximumSize(QSize(400, 290));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui_icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        LoginDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(LoginDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(LoginDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        emailEdit = new QLineEdit(LoginDialog);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));

        gridLayout->addWidget(emailEdit, 0, 1, 1, 1);

        label_2 = new QLabel(LoginDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        passwordEdit = new QLineEdit(LoginDialog);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordEdit, 1, 1, 1, 1);

        buttonBox = new QDialogButtonBox(LoginDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 2);

        proxyCheckBox = new QCheckBox(LoginDialog);
        proxyCheckBox->setObjectName(QString::fromUtf8("proxyCheckBox"));

        gridLayout->addWidget(proxyCheckBox, 2, 0, 1, 1);

        proxyBox = new QGroupBox(LoginDialog);
        proxyBox->setObjectName(QString::fromUtf8("proxyBox"));
        proxyBox->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(proxyBox->sizePolicy().hasHeightForWidth());
        proxyBox->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(proxyBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(proxyBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(proxyBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        label_6 = new QLabel(proxyBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 4, 0, 1, 1);

        label_5 = new QLabel(proxyBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 2, 0, 1, 1);

        proxyPortEdit = new QSpinBox(proxyBox);
        proxyPortEdit->setObjectName(QString::fromUtf8("proxyPortEdit"));
        proxyPortEdit->setMaximum(99999);

        gridLayout_2->addWidget(proxyPortEdit, 2, 1, 1, 1);

        label_7 = new QLabel(proxyBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 2, 2, 1, 1);

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


        gridLayout->addWidget(proxyBox, 3, 0, 1, 2);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(emailEdit);
        label_2->setBuddy(passwordEdit);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(emailEdit, passwordEdit);
        QWidget::setTabOrder(passwordEdit, proxyCheckBox);
        QWidget::setTabOrder(proxyCheckBox, proxyHostEdit);
        QWidget::setTabOrder(proxyHostEdit, proxyPortEdit);
        QWidget::setTabOrder(proxyPortEdit, proxyTypeBox);
        QWidget::setTabOrder(proxyTypeBox, proxyUserEdit);
        QWidget::setTabOrder(proxyUserEdit, proxyPasswordEdit);
        QWidget::setTabOrder(proxyPasswordEdit, buttonBox);

        retranslateUi(LoginDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LoginDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LoginDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Login", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LoginDialog", "E-mail:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        emailEdit->setToolTip(QApplication::translate("LoginDialog", "Use your formula1.com login and password. <br/>If you don't have one, register for free at formula1.com site.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("LoginDialog", "Password:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        passwordEdit->setToolTip(QApplication::translate("LoginDialog", "Use your formula1.com login and password. <br/>If you don't have one, register for free at formula1.com site.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        proxyCheckBox->setText(QApplication::translate("LoginDialog", "Use proxy", 0, QApplication::UnicodeUTF8));
        proxyBox->setTitle(QApplication::translate("LoginDialog", "Proxy settings", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LoginDialog", "Host:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("LoginDialog", "User:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("LoginDialog", "Password:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("LoginDialog", "Port:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("LoginDialog", "Type:", 0, QApplication::UnicodeUTF8));
        proxyTypeBox->clear();
        proxyTypeBox->insertItems(0, QStringList()
         << QApplication::translate("LoginDialog", "HTTP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LoginDialog", "Socks5", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
