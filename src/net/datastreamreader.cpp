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


#include "datastreamreader.h"
#include <cmath>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include "networksettings.h"

DataStreamReader::DataStreamReader(QObject *parent) :
    QObject(parent), eventData(EventData::getInstance())
{    
    connect(&httpReader, SIGNAL(cookieRecieved(QString)), this, SLOT(onCookieReceived(QString)));
    connect(&httpReader, SIGNAL(keyFrameObtained(QByteArray)), &parser, SLOT(keyFrameObtained(QByteArray)));
    connect(&httpReader, SIGNAL(decryptionKeyObtained(uint)), &parser, SLOT(decryptionKeyObtained(uint)));
    connect(&httpReader, SIGNAL(authorizationError()), this, SIGNAL(authorizationError()));
    connect(&httpReader, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));


    connect(&socketReader, SIGNAL(packetObtained(QByteArray)), &parser, SLOT(streamBlockObtained(QByteArray)));
    connect(&socketReader,  SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)));

    connect(&parser, SIGNAL(noLiveSession(bool,QString)), this, SIGNAL(noLiveSession(bool,QString)));
    connect(&parser, SIGNAL(sessionStarted()), this, SIGNAL(sessionStarted()));
    connect(&parser, SIGNAL(packetParsed(Packet)), this, SIGNAL(packetParsed(Packet)));
    connect(&parser, SIGNAL(packetParsed(QPair<Packet, qint64>)), this, SIGNAL(packetParsed(QPair<Packet, qint64>)));
    connect(&parser, SIGNAL(eventDataChanged(const DataUpdates &)), this, SIGNAL(eventDataChanged(const DataUpdates &)));
    connect(&parser, SIGNAL(driverDataChanged(int, const DataUpdates &)), this, SIGNAL(driverDataChanged(int, const DataUpdates &)));
    connect(&parser, SIGNAL(dataChanged(const DataUpdates &)), this, SIGNAL(dataChanged(const DataUpdates &)));
    connect(&parser, SIGNAL(requestKeyFrame(uint)), &httpReader, SLOT(obtainKeyFrame(uint)));
    connect(&parser, SIGNAL(requestDecryptionKey(uint)), &httpReader, SLOT(obtainDecryptionKey(uint)));
}

void DataStreamReader::setDelay(int prevDelay, int delay)
{
    parser.setDelay(prevDelay, delay);
}

void DataStreamReader::connectToLTServer()
{
    eventData.frame = 0;
    emit tryAuthorize();
    httpReader.authorize();
}

void DataStreamReader::disconnectFromLTServer()
{
    socketReader.disconnectFromHost();
    parser.clearBuffer();
}

void DataStreamReader::onCookieReceived(QString cookie)
{    
    eventData.cookie = cookie;
    socketReader.openStream();
    emit authorized(cookie);
}

void DataStreamReader::clearData()
{
    parser.clearData();
}

void DataStreamReader::parsePackets(const QVector<Packet> &packets)
{
    parser.parsePackets(packets);
}
