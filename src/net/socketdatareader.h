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


#ifndef SocketDataReader_H
#define SocketDataReader_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTimer>

class SocketDataReader : public QObject
{
    Q_OBJECT

public:
    void runS();
    SocketDataReader(QObject *parent=0);
    ~SocketDataReader();
    void openStream();

    void connectToHost();
    void disconnectFromHost();

    void wakeUpServer();

signals:
    void streamOpened();
    void packetObtained(const QByteArray &);
    void error(QAbstractSocket::SocketError);

public slots:
    void connected();    
    void readyRead();
    void timeout();
    void connectionError(QAbstractSocket::SocketError);
    void reconnect();

private:
    QTcpSocket *socket;
    QByteArray data;

    QTimer *timer;
    QTimer *reconnectTimer;

    bool connectionOpened;

    int tryReconnect;

    long bytes;
};

#endif // SocketDataReader_H
