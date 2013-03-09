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


#include "packetbuffer.h"

#include "packetparser.h"
#include <QDebug>

PacketBuffer::PacketBuffer(PacketParser *parser, QObject *parent) :
    QObject(parent), delay(0), packetParser(parser)
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
        if (packetsQueue.head().second  <= currTime)
            packetsToHandle.append(packetsQueue.dequeue());

        else
            break;
    }
    packetParser->parseBufferedPackets(packetsToHandle);
}

void PacketBuffer::setDelay(int del)
{
    delay = del;
    if (delay != 0 && !timer->isActive())
        timer->start(100);

    else if (delay == 0 && timer->isActive())
    {
        timer->stop();
        timeout();
    }
}
