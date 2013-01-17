/*******************************************************************************
 *                                                                             *
 *   F1LT - unofficial Formula 1 live timing application                       *
 *   Copyright (C) 2012-2013  Mariusz Pilarek (pieczaro@gmail.com)             *
 *                                                                             *
 *   This program is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation, either version 3 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                             *
 *******************************************************************************/


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

int LoginDialog::exec(QString email, QString passwd, QNetworkProxy proxy, bool proxyEnabled)
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
        case QNetworkProxy::Socks5Proxy: ui->proxyTypeBox->setCurrentIndex(1); break;
        default: break;
    }

    ui->proxyCheckBox->setChecked(proxyEnabled);

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

//    if (ui->proxyCheckBox->isChecked())
    {
        proxy.setHostName(ui->proxyHostEdit->text());
        proxy.setUser(ui->proxyUserEdit->text());
        proxy.setPassword(ui->proxyPasswordEdit->text());
        proxy.setPort(ui->proxyPortEdit->value());

        switch (ui->proxyTypeBox->currentIndex())
        {
            case 0: proxy.setType(QNetworkProxy::HttpCachingProxy); break;
            case 1: proxy.setType(QNetworkProxy::Socks5Proxy); break;
        }
    }

    return proxy;
}

bool LoginDialog::proxyEnabled()
{
    return ui->proxyCheckBox->isChecked();
}

void LoginDialog::on_proxyCheckBox_toggled(bool checked)
{
    ui->proxyBox->setEnabled(checked);

//    if (checked)
//        setGeometry(x(), y(), width(), 290);
//    else
//        setGeometry(x(), y(), width(), 135);
}
