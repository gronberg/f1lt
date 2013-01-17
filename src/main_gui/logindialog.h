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



#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkProxy>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QString getEmail();
    QString getPasswd();
    QNetworkProxy getProxy();
    bool proxyEnabled();
    int exec(QString email="", QString passwd="", QNetworkProxy proxy = QNetworkProxy(), bool proxyEnabled = false);
    
private slots:

    void on_proxyCheckBox_toggled(bool checked);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
