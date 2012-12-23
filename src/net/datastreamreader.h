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

    void connectToLTServer(QString email, QString passwd);
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
    void eventDataChanged();
    void driverDataChanged(int id);
    void dataChanged();    
    
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

    QString host;
    int port;    


    EventData &eventData;   
};

#endif // DATASTREAMREADER_H
