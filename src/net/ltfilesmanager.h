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


#ifndef LTFILESMANAGER_H
#define LTFILESMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>

#include <QObject>

class LTFilesManager : public QObject
{
    Q_OBJECT
public:
    explicit LTFilesManager(QObject *parent = 0);
    
signals:
    void ltListObtained(QStringList);
    void ltFileObtained(QByteArray);
    void downloadProgress ( qint64 bytesReceived, qint64 bytesTotal );
    void error ( QNetworkReply::NetworkError code );
    
public slots:
    void getLTList();
    void getLTFile(QString);

    void cancel();

    void finishedLTList();
    void finishedLTFile();    

private:
    QNetworkAccessManager manager;
    QNetworkRequest req;
    QNetworkReply *reply;
};

#endif // LTFILESMANAGER_H
