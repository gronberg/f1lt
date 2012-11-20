#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include <QObject>
#include <QPair>
#include <QQueue>
#include <QTimer>

#include "packetparser.h"


class PacketBuffer : public QObject
{
    Q_OBJECT
public:
    explicit PacketBuffer(PacketParser *parser, QObject *parent = 0);

    void addPacket(Packet &);
    bool hasToBeBuffered();
    
signals:
    
public slots:
    void timeout();
    void setDelay(int);

private:
    int delay;
    QTimer *timer;
    QQueue< QPair<Packet, qint64> > packetsQueue;
    PacketParser *packetParser;
    
};

#endif // PACKETBUFFER_H
