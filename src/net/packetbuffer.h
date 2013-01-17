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
