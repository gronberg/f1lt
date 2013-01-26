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


#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QByteArray>
#include <QObject>
#include <QList>

#include "../core/eventdata.h"


class PacketParser;
class PacketBuffer;

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

    void clearBuffer();

public slots:
     void parsePackets(const QVector<Packet> &);
     void parseBufferedPackets(const QVector< QPair<Packet, qint64> > &);
     void parseBufferedPackets(const QPair<Packet, qint64> &);
     void decryptionKeyObtained(unsigned int key);
     void keyFrameObtained(const QByteArray &buf);
     void streamBlockObtained(const QByteArray &buf);

     void setDelay(int prevDelay, int delay);

signals:
     void packetParsed(const Packet&);
     void packetParsed(const QPair<Packet, qint64>&);
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

    PacketBuffer *packetBuffer;

};

#endif // PACKETPARSER_H
