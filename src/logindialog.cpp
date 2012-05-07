#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

int LoginDialog::exec(QString email, QString passwd)
{
    ui->emailEdit->setText(email);
    ui->passwordEdit->setText(passwd);

    return QDialog::exec();
}

QString LoginDialog::getEmail()
{
    return ui->emailEdit->text();
}

QString LoginDialog::getPasswd()
{
    return ui->passwordEdit->text();
}
