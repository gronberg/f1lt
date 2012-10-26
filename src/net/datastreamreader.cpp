#include "datastreamreader.h"
#include <cmath>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

DataStreamReader::DataStreamReader(QObject *parent) :
    QObject(parent), host("live-timing.formula1.com"), port(4321), eventData(EventData::getInstance())
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
    connect(&parser, SIGNAL(eventDataChanged()), this, SIGNAL(eventDataChanged()));
    connect(&parser, SIGNAL(driverDataChanged(int)), this, SIGNAL(driverDataChanged(int)));
    connect(&parser, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(&parser, SIGNAL(requestKeyFrame(uint)), &httpReader, SLOT(obtainKeyFrame(uint)));
    connect(&parser, SIGNAL(requestDecryptionKey(uint)), &httpReader, SLOT(obtainDecryptionKey(uint)));
}

void DataStreamReader::connectToLTServer(QString email, QString passwd)
{
    eventData.frame = 0;
//    httpReader.authorize("http://"+host, email, passwd);
    qDebug() << "authorizing";
    emit tryAuthorize();
    httpReader.authorize("http://"+host, "http://formula1.com", email, passwd);
}

void DataStreamReader::disconnectFromLTServer()
{
    socketReader.disconnectFromHost();
}

void DataStreamReader::onCookieReceived(QString cookie)
{    
    eventData.cookie = cookie;
    socketReader.openStream(host, port);
//    socketReader.openStream("localhost", 6666);
//    socketReader.openStream("192.168.1.2", 6666);
//    eventData.key = 2976363859;
//    eventData.key = 2462388168;     //bahrain quali
//    eventData.key = 3875488254; //fp1
//    eventData.key = 2488580439; //qual
//        eventData.key = 2438680630; //race
    //socketReader.start();    
    qDebug() << "opened";
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
