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


#ifndef HttpDataReader_H
#define HttpDataReader_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpDataReader : public QObject
{
    Q_OBJECT
public:
    explicit HttpDataReader(QObject *parent = 0);
    ~HttpDataReader();

    void authorize();

    QString getCookie() { return cookie; }
    bool parseCookie(QString header);
    
signals:
    void cookieRecieved(QString cookie);
    void keyFrameObtained(const QByteArray &);
    void decryptionKeyObtained(unsigned int);
    void authorizationError();
    void error(QNetworkReply::NetworkError);
    
public slots:
    void finished();
    void obtainKeyFrame(unsigned int frame);
    void obtainDecryptionKey(unsigned int event_no);
    

private:
    QNetworkAccessManager manager;
    QNetworkRequest req;
    QNetworkReply *reply;

    QString cookie;
    QByteArray keyFrame;
    unsigned int decryptionKey;

    long bytes;

    int state; //0 - doing nothing, 1 - authorizing, 2 - obtaining key frame, 3 - obtaining decryption key
};

#endif // HttpDataReader_H
