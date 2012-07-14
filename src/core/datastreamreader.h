#ifndef DATASTREAMREADER_H
#define DATASTREAMREADER_H

#include <QObject>
#include <QFile>
#include <QList>

#include <iostream>

#include "eventdata.h"
#include "../net/httpdatareader.h"
#include "../net/socketdatareader.h"

//#include <httpconnection.h>

struct Packet
{
    int length;
    int type;
    int carID;
    int data;

    bool encrypted;

    QByteArray longData;
};

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

    void resetDecryption()
    {
        eventData.salt = 0x55555555;
    }

    void decrypt(QByteArray &buf)
    {
        for (int i = 0; i < buf.size(); ++i)
        {
            eventData.salt = ((eventData.salt >> 1) ^ (eventData.salt & 0x01 ? eventData.key : 0));
            unsigned char c = buf[i];
            c ^= (eventData.salt & 0xff);
            buf[i] = c;
        }
    }
    bool checkDecryption(QString stream);

//    void run();

signals:
    void streamOpened();
    void authorized(QString);

    void eventDataChanged();
    void driverDataChanged(int id);
    void dataChanged();

    void sessionStarted();
    void authorizationError();
    void error(QAbstractSocket::SocketError);
    void error(QNetworkReply::NetworkError);
    void packetParsed(const Packet&);
    void noLiveSession(bool, QString);
    
public slots:
    //void connected();
    //void error(QAbstractSocket::SocketError);
    void parseStreamBlock();
    void clearData();
    void parseEPPackets(const QList<Packet> &);

    void onStreamBlockObtained(const QByteArray&);    
    void onCookieReceived(QString);
    void onKeyFrameObtained(QByteArray);
    void onDecryptionKeyObtained(unsigned int key);

private:
    SocketDataReader socketReader;
    HttpDataReader httpReader;

    QString host;
    int port;

    QList<Packet> encryptedPackets;

    QByteArray streamData;

    bool parsing;

    int packetNo;

    void savePacket(const QByteArray &buf);
    EventData &eventData;

    bool noSession;
};

#endif // DATASTREAMREADER_H
