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


#include "socketdatareader.h"
#include <QDebug>

#include "networksettings.h"

SocketDataReader::SocketDataReader(QObject *parent) :
    QObject(parent), connectionOpened(false)
{
    timer = new QTimer(this);
    reconnectTimer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    bytes = 0;
    socket = NULL;
}

SocketDataReader::~SocketDataReader()
{
    qDebug()<<"Bytes received: "<<bytes;
}

void SocketDataReader::openStream()
{
    if (connectionOpened)
        return;

    tryReconnect = 5;

//    if (!isRunning())
//        start();

//    else
//        connectToHost();
    runS();
}

void SocketDataReader::connectToHost()
{    
    if (NetworkSettings::getInstance().usingProxy() && NetworkSettings::getInstance().getProxy().type() == QNetworkProxy::Socks5Proxy)
        socket->setProxy(NetworkSettings::getInstance().getProxy());
    else
        socket->setProxy(QNetworkProxy::NoProxy);

    socket->connectToHost(NetworkSettings::getInstance().getSocketHost(), NetworkSettings::getInstance().getSocketPort(), QIODevice::ReadWrite);
    qDebug() << "connected";
    socket->waitForReadyRead();
    qDebug() << "ready";
}

void SocketDataReader::disconnectFromHost()
{
    if (socket && socket->isOpen())
        socket->disconnectFromHost();

    timer->stop();
    connectionOpened = false;

    if (socket)
    {
        QObject::disconnect(socket, SIGNAL(connected()), this, SLOT(connected()));
        QObject::disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
        QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    if (reconnectTimer)
        QObject::disconnect(reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnect()));
}

void SocketDataReader::wakeUpServer()
{    
    char wbuf[1];
    wbuf[0] = 0x10;
    socket->write(wbuf, sizeof(wbuf));
}

void SocketDataReader::runS()
{
    socket = new QTcpSocket();

    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnect()));

    connectionOpened = false;

    connectToHost();
}

void SocketDataReader::connected()
{
    connectionOpened = true;

    if (reconnectTimer->isActive())
	{
		reconnectTimer->stop();
	}

    timer->start(1000);

    tryReconnect = 5;

    emit streamOpened();
}

void SocketDataReader::readyRead()
{
    QByteArray pac = socket->readAll();

    bytes += pac.size();

    emit packetObtained(pac);
}

void SocketDataReader::timeout()
{
    wakeUpServer();
}

void SocketDataReader::connectionError(QAbstractSocket::SocketError err)
{
    timer->stop();
    if (tryReconnect == 5)
    {
    	reconnect();
    }

    else if (tryReconnect == 0)
    {
    	if (reconnectTimer->isActive())
			reconnectTimer->stop();

        emit error(err);
    }
}

void SocketDataReader::reconnect()
{
	if (tryReconnect == 5)
	{
		--tryReconnect;
		reconnectTimer->start(500);
	}
	else if (tryReconnect > 0)
	{
		if (socket && socket->isOpen())
			socket->disconnectFromHost();

		connectionOpened = false;
		--tryReconnect;

		connectToHost();
	}
	else if (reconnectTimer->isActive())
		reconnectTimer->stop();
}


