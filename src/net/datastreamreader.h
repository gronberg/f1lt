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



#ifndef DATASTREAMREADER_H
#define DATASTREAMREADER_H

#include <QObject>
#include <QFile>
#include <QList>

#include <iostream>

#include "../core/eventdata.h"
#include "httpdatareader.h"
#include "packetparser.h"
#include "socketdatareader.h"

//#include <httpconnection.h>



class DataStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit DataStreamReader(QObject *parent = 0);

    void connectToLTServer();
    void disconnectFromLTServer();

    bool parsePacket(const QByteArray&, Packet &packet, int &pos);
    void parseCarPacket(Packet &packet, bool emitSignal = true);
    void parseSystemPacket(Packet &packet, bool emitSignal = true);


//    void run();

signals:
    void streamOpened();
    void tryAuthorize();
    void authorized(QString);

    void sessionStarted();
    void authorizationError();
    void error(QAbstractSocket::SocketError);
    void error(QNetworkReply::NetworkError);

    void noLiveSession(bool, QString);

    void packetParsed(const Packet&);
    void packetParsed(const QPair<Packet, qint64>&);
    void eventDataChanged(const DataUpdates &);
    void driverDataChanged(int id, const DataUpdates &);
    void dataChanged(const DataUpdates &);
    
public slots:
    //void connected();
    //void error(QAbstractSocket::SocketError);
    void clearData();    

    void onCookieReceived(QString);

    void parsePackets(const QVector<Packet> &);
    void setDelay(int prevDelay, int delay);

private:
    SocketDataReader socketReader;
    HttpDataReader httpReader;
    PacketParser parser;  


    EventData &eventData;   
};

#endif // DATASTREAMREADER_H
