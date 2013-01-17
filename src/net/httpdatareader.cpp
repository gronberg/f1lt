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


#include "httpdatareader.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "networksettings.h"

HttpDataReader::HttpDataReader(QObject *parent) :
    QObject(parent), state(0)
{
    bytes = 0;
}

HttpDataReader::~HttpDataReader()
{
    qDebug()<<"HTTP bytes received = "<<bytes;
}

void HttpDataReader::authorize()
{
    QString authHost = NetworkSettings::getInstance().getHttpLoginUrl() + "";
    QString content("application/x-www-form-urlencoded");

    QByteArray bArray(QString("email="+NetworkSettings::getInstance().getUser() + "&password=" +
                              NetworkSettings::getInstance().getPassword()).toAscii());

    if (NetworkSettings::getInstance().usingProxy())
        manager.setProxy(NetworkSettings::getInstance().getProxy());
    else
        manager.setProxy(QNetworkProxy::NoProxy);

    req = QNetworkRequest(authHost);
    req.setHeader(QNetworkRequest::ContentTypeHeader, content.toAscii());
    req.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(bArray.size()).toString());
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.post(req, bArray);

    state = 1;

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpDataReader::obtainKeyFrame(unsigned int frame)
{
    if (state != 0)
        return;


    QString keyhost = NetworkSettings::getInstance().getHttpKeyFrameUrl();
    if (frame > 0)
    {
        QString k = QString("%1").arg(frame);

        int size = k.size();
        for (int i = 0; i < 5 - size; ++i)
        {
            k = "0" + k;
        }

        keyhost += "_" + k;
    }

    keyhost += ".bin";

    req = QNetworkRequest(keyhost);
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.get(req);

    state = 2;

    qDebug(keyhost.toStdString().c_str());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpDataReader::obtainDecryptionKey(unsigned int event_no)
{
    QString keyhost = NetworkSettings::getInstance().getHttpDecryptionKeyUrl() + QString("%1").arg(event_no) + ".asp?auth=" + cookie;


    req = QNetworkRequest(keyhost);
    req.setRawHeader("User-Agent","f1lt");

    reply = manager.get(req);

    state = 3;

    qDebug(keyhost.toStdString().c_str());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpDataReader::finished()
{
    if (state == 1)
    {
        QString v = qvariant_cast<QString>(reply->rawHeader("Set-Cookie"));

        state = 0;
        if (parseCookie(v))
            emit cookieRecieved(cookie);        
    }
    else if (state == 2)
    {
        keyFrame = reply->readAll();
        bytes += keyFrame.size();

        state = 0;

        emit keyFrameObtained(keyFrame);
    }
    else if (state == 3)
    {
        QByteArray buf = reply->readAll();
        decryptionKey = 0;

        bytes += buf.size();

        for (int i = 0; i < buf.size(); ++i)
        {
            if ((buf[i] >= '0') && (buf[i] <= '9'))
                decryptionKey = (decryptionKey << 4) | (buf[i] - '0');

            else if ((buf[i] >= 'a') && (buf[i] <= 'f'))
                decryptionKey = (decryptionKey << 4) | (buf[i] - 'a' + 10);

            else if ((buf[i] >= 'A') && (buf[i] <= 'F'))
                decryptionKey = (decryptionKey << 4) | (buf[i] - 'A' + 10);

            else break;
        }
        state = 0;

        emit decryptionKeyObtained(decryptionKey);
    }
}

bool HttpDataReader::parseCookie(QString header)
{
    int idx = header.indexOf("USER=");
    if (idx == -1)
    {
        emit authorizationError();
        return false;
    }

    idx += 5;

    int idx2 = header.indexOf(";");
    cookie = header.right(header.length() - idx).left(idx2-idx);

    return true;
}
