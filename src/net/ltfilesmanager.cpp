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


#include "ltfilesmanager.h"

#include <QByteArray>
#include <QDebug>

#include "networksettings.h"

LTFilesManager::LTFilesManager(QObject *parent) :
    QObject(parent)
{
}

void LTFilesManager::getLTList()
{
    req = QNetworkRequest(NetworkSettings::getInstance().getLTDataList());
    req.setRawHeader("User-Agent","f1lt");

    if (NetworkSettings::getInstance().usingProxy())
        manager.setProxy(NetworkSettings::getInstance().getProxy());
    else
        manager.setProxy(QNetworkProxy::NoProxy);

    reply = manager.get(req);

    connect(reply, SIGNAL(finished()), this, SLOT(finishedLTList()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

void LTFilesManager::getLTFile(QString ltDataFile)
{
    req = QNetworkRequest(NetworkSettings::getInstance().getLTDataUrl() + ltDataFile);
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.get(req);

    connect(reply, SIGNAL(finished()), this, SLOT(finishedLTFile()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));
}

void LTFilesManager::finishedLTList()
{
    QString buf = reply->readAll().constData();
    QStringList ltList = buf.split("<br>");

    disconnect(reply, SIGNAL(finished()), this, SLOT(finishedLTFile()));
    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    disconnect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));

    emit ltListObtained(ltList);
}

void LTFilesManager::finishedLTFile()
{
    QByteArray buf = reply->readAll();

    disconnect(reply, SIGNAL(finished()), this, SLOT(finishedLTList()));
    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));

    emit ltFileObtained(buf);            
}

void LTFilesManager::cancel()
{
    if (reply)
    {
        reply->close();
        manager.deleteResource(req);
    }
}
