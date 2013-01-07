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

int LoginDialog::exec(QString email, QString passwd, QNetworkProxy proxy)
{
    ui->emailEdit->setText(email);
    ui->passwordEdit->setText(passwd);

    ui->proxyHostEdit->setText(proxy.hostName());
    ui->proxyPortEdit->setValue(proxy.port());
    ui->proxyUserEdit->setText(proxy.user());
    ui->proxyPasswordEdit->setText(proxy.password());

    switch (proxy.type())
    {
        case QNetworkProxy::HttpCachingProxy: ui->proxyTypeBox->setCurrentIndex(0); break;
        case QNetworkProxy::FtpCachingProxy: ui->proxyTypeBox->setCurrentIndex(1); break;
        case QNetworkProxy::Socks5Proxy: ui->proxyTypeBox->setCurrentIndex(2); break;
    }

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

QNetworkProxy LoginDialog::getProxy()
{
    QNetworkProxy proxy;

    if (ui->proxyCheckBox->isChecked())
    {
        proxy.setHostName(ui->proxyHostEdit->text());
        proxy.setUser(ui->proxyUserEdit->text());
        proxy.setPassword(ui->proxyPasswordEdit->text());
        proxy.setPort(ui->proxyPortEdit->value());

        switch (ui->proxyTypeBox->currentIndex())
        {
            case 0: proxy.setType(QNetworkProxy::HttpCachingProxy); break;
            case 1: proxy.setType(QNetworkProxy::FtpCachingProxy); break;
            case 2: proxy.setType(QNetworkProxy::Socks5Proxy); break;
        }
    }

    return proxy;
}

void LoginDialog::on_proxyCheckBox_toggled(bool checked)
{
    ui->proxyBox->setEnabled(checked);

//    if (checked)
//        setGeometry(x(), y(), width(), 290);
//    else
//        setGeometry(x(), y(), width(), 135);
}
