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
    if (NetworkSettings::getInstance().usingProxy())
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


