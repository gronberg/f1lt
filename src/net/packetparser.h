#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QByteArray>
#include <QObject>
#include <QList>

#include "../core/eventdata.h"

class PacketParser;

struct Packet
{
    int length;
    int type;
    int carID;
    int data;

    bool encrypted;

    QByteArray longData;
};

class PacketDecrypter
{
public:
    PacketDecrypter() : salt(0x55555555), key(0) { }

    void resetDecryption()
    {
        salt = 0x55555555;        
    }

    void decrypt(QByteArray &buf)
    {
        for (int i = 0; i < buf.size(); ++i)
        {
            salt = ((salt >> 1) ^ (salt & 0x01 ? key : 0));
            unsigned char c = buf[i];
            c ^= (salt & 0xff);
            buf[i] = c;
        }
    }
    bool checkDecryption(QString stream);

    friend class PacketParser;
private:
    unsigned int salt;
    unsigned int key;
};

class PacketParser : public QObject
{
    Q_OBJECT

public:
    PacketParser(QObject *parent = 0);
    void parseStreamBlock(const QByteArray &streamData);

    bool parsePacket(const QByteArray&, Packet &packet, int &pos);
    void parseCarPacket(Packet &packet, bool emitSignal = true);
    void parseSystemPacket(Packet &packet, bool emitSignal = true);

    void handleRaceEvent(const Packet &packet);
    void handleQualiEvent(const Packet &packet);
    void handlePracticeEvent(const Packet &packet);

    bool isParsing() { return parsing; }
    void append(const QByteArray &buf)
    {
        streamData.append(buf);
    }

    void clearEncryptedPackets()
    {
        encryptedPackets.clear();
    }
    void clearData()
    {
        decrypter.key = 0;
        eventData.clear();
    }

public slots:
     void parsePackets(const QList<Packet> &);
     void decryptionKeyObtained(unsigned int key);
     void keyFrameObtained(const QByteArray &buf);
     void streamBlockObtained(const QByteArray &buf);

signals:
     void packetParsed(const Packet&);
     void eventDataChanged();
     void driverDataChanged(int id);
     void dataChanged();

     void noLiveSession(bool, QString);
     void sessionStarted();

     void requestKeyFrame(unsigned int);
     void requestDecryptionKey(unsigned int);

private:
    bool parsing;
    int packetNo;
    QByteArray streamData;

    QList<Packet> encryptedPackets;

    PacketDecrypter decrypter;
    EventData &eventData;

    void savePacket(const QByteArray &buf);

    bool noSession;

};

#endif // PACKETPARSER_H
