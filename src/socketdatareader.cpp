#include "socketdatareader.h"
#include <QDebug>

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

void SocketDataReader::openStream(QString host, int port)
{
    if (host == this->host && connectionOpened)
        return;

    tryReconnect = 5;

    this->host = host;
    this->port = port;

//    if (!isRunning())
//        start();

//    else
//        connectToHost();
    runS();
}

void SocketDataReader::connectToHost()
{
    socket->connectToHost(host, port, QIODevice::ReadWrite);
    socket->waitForReadyRead();
}

void SocketDataReader::disconnectFromHost()
{
    if (socket && socket->isOpen())
        socket->disconnectFromHost();

    timer->stop();
    connectionOpened = false;

    QObject::disconnect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
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

    //socket->moveToThread(this->thread());
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnect()));

    connectionOpened = false;

//    std::cout<<"host="<<host.toStdString()<<", port="<<port<<std::endl;

    connectToHost();

    //socket->moveToThread(this);

//    exec();
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
//    connectionOpened = true;

    emit packetObtained(pac);
}

void SocketDataReader::timeout()
{
    wakeUpServer();
//    timer->start(900);
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


