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

#ifndef UPDATECHECKERDIALOG_H
#define UPDATECHECKERDIALOG_H

#include <QAbstractButton>
#include <QDialog>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QSettings>

namespace Ui {
class UpdatesCheckerDialog;
}

class UpdatesCheckerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdatesCheckerDialog(QWidget *parent = 0);
    ~UpdatesCheckerDialog();

    void checkForUpdates();
    bool newVersionIsAvailable(QString version);

    void loadSettings(QSettings &settings);
    void saveSettings(QSettings &settings);

public slots:
    void finished();
    void error(QNetworkReply::NetworkError);
    void show(bool check = true);

signals:
    void newVersionAvailable();
    
private slots:
    void on_buttonBox_clicked(QAbstractButton *);

private:
    Ui::UpdatesCheckerDialog *ui;

    QNetworkAccessManager manager;
    QNetworkRequest req;
    QNetworkReply *reply;
};

#endif // UPDATECHECKERDIALOG_H
