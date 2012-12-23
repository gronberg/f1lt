#include "packetbuffer.h"

#include "packetparser.h"
#include <QDebug>

PacketBuffer::PacketBuffer(PacketParser *parser, QObject *parent) :
    packetParser(parser), QObject(parent), delay(0)
{
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void PacketBuffer::addPacket(Packet &packet)
{
    if (delay == 0)
        packetParser->parseBufferedPackets(qMakePair(packet, QDateTime::currentMSecsSinceEpoch()));

    else
        packetsQueue.enqueue(qMakePair(packet, QDateTime::currentMSecsSinceEpoch()));
}

bool PacketBuffer::hasToBeBuffered()
{
    if (delay == 0)
    {
        timeout();
        return false;
    }
    return true;
}

void PacketBuffer::timeout()
{
    qint64 currTime = QDateTime::currentMSecsSinceEpoch() - delay * 1000;
    QVector< QPair<Packet, qint64> > packetsToHandle;
    while (!packetsQueue.isEmpty())
    {
//        qDebug() << currTime/1000 << packetsQueue.head().second/1000;
        if (packetsQueue.head().second  <= currTime)
            packetsToHandle.append(packetsQueue.dequeue());

        else
            break;
    }
    packetParser->parseBufferedPackets(packetsToHandle);
}

void PacketBuffer::setDelay(int del)
{
//    if (!packetsQueue.isEmpty())
//        qDebug() << "BEFORE" << delay <<(QDateTime::currentMSecsSinceEpoch() - delay * 1000)/1000 << packetsQueue.head().second/1000;
    delay = del;
    if (delay != 0 && !timer->isActive())
        timer->start(100);

    else if (delay == 0 && timer->isActive())
    {
        timer->stop();
        timeout();
    }

//    qDebug() << "DELAY=" << del << packetsQueue.size();
}
