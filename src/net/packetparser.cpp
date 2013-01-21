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


#include "packetparser.h"

#include <QDebug>
#include "packetbuffer.h"

#include "../core/trackrecords.h"


bool PacketDecrypter::checkDecryption(QString stream)
{
    for (int i = 0; i < stream.size(); ++i)
    {
        unsigned char c = (unsigned char)(stream[i].toAscii());
        if ((int)c != 10 && ((int)c < 32 || (int)c > 126))
            return false;

    }
    return true;
}

//=============================================================

PacketParser::PacketParser(QObject *parent) : QObject(parent), parsing(false), packetNo(0), eventData(EventData::getInstance()), noSession(false)
{
    packetBuffer = new PacketBuffer(this, this);
}

void PacketParser::setDelay(int, int delay)
{
    packetBuffer->setDelay(delay);
}

void PacketParser::parseStreamBlock(const QByteArray &data)
{
    streamData = data;
    Packet packet;
    int pos = 0;

    parsing = true;
    while(parsePacket(streamData, packet, pos))
    {
        if (packet.encrypted && !eventData.key)
        {
            packet.longData.clear();
            continue;
        }

        //after every SYS_KEY_FRAME packet decryption have to be reset
        if(!packet.carID && (packet.type == LTPackets::SYS_KEY_FRAME || packet.type == LTPackets::SYS_EVENT_ID))
            decrypter.resetDecryption();

        if (packetBuffer->hasToBeBuffered())
        {
            packetBuffer->addPacket(packet);
        }
        else
            parseBufferedPackets(qMakePair(packet, QDateTime::currentMSecsSinceEpoch()));

//        Packet copyPacket = packet;




//            parseCarPacket(packet);

//        else
//            parseSystemPacket(packet);

//        emit packetParsed(copyPacket);
        packet.longData.clear();
    }
    parsing = false;
}

bool PacketParser::parsePacket(const QByteArray &buf, Packet &packet, int &pos)
{
    static QByteArray pbuf;
    static int pbuf_length = 0;

    if (pbuf_length < 2)
    {
        int data_length = std::min(buf.size()-pos, 2 - pbuf_length);

        pbuf.append(buf.mid(pos, data_length));

        pbuf_length += data_length;
        pos += data_length;

        if (pbuf_length < 2)
            return 0;
    }

    if (pos == buf.size())
        return false;

    packet.type = LTPackets::getPacketType(pbuf);
    packet.carID = LTPackets::getCarPacket(pbuf);

    bool decryptPacket = false;

    if (packet.carID)
    {
        switch(packet.type)
        {
            case LTPackets::CAR_POSITION_UPDATE:
                packet.length = LTPackets::getSpecialPacketLength(pbuf);
                packet.data = LTPackets::getSpecialPacketData(pbuf);
                break;

            case LTPackets::CAR_POSITION_HISTORY:
                packet.length = LTPackets::getLongPacketLength(pbuf);
                packet.data = LTPackets::getLongPacketData(pbuf);
                decryptPacket = true;
                break;

            default:
                packet.length = LTPackets::getShortPacketLength(pbuf);
                packet.data = LTPackets::getShortPacketData(pbuf);
                decryptPacket = true;
                break;
        }
    }
    else
    {
        switch(packet.type)
        {
            case LTPackets::SYS_EVENT_ID:
            case LTPackets::SYS_KEY_FRAME:
                packet.length = LTPackets::getShortPacketLength(pbuf);
                packet.data = LTPackets::getShortPacketData(pbuf);
                break;

            case LTPackets::SYS_TIMESTAMP:
                packet.length = 2;
                packet.data = 0;
                decryptPacket = true;
                break;

            case LTPackets::SYS_WEATHER:
            case LTPackets::SYS_TRACK_STATUS:
                packet.length = LTPackets::getShortPacketLength(pbuf);
                packet.data = LTPackets::getShortPacketData(pbuf);
                decryptPacket = true;
                break;

            case LTPackets::SYS_COMMENTARY:
            case LTPackets::SYS_NOTICE:
            case LTPackets::SYS_SPEED:
                packet.length = LTPackets::getLongPacketLength(pbuf);
                packet.data = LTPackets::getLongPacketData(pbuf);
                decryptPacket = true;
                break;

            case LTPackets::SYS_COPYRIGHT:
                packet.length = LTPackets::getLongPacketLength(pbuf);
                packet.data = LTPackets::getLongPacketData(pbuf);
                break;

            case LTPackets::SYS_VALID_MARKER:
            case LTPackets::SYS_REFRESH_RATE:
                packet.length = 0;//LTPackets::getShortPacketLength(pbuf);
                packet.data = 0;//LTPackets::getShortPacketData(pbuf);
                break;

            default:
                packet.length = 0;
                packet.data = 0;
                break;
        }
    }

    decryptPacket = false;
    packet.encrypted = decryptPacket;

    if (packet.length > 0)
    {
        int data_length = std::min(buf.size() - pos, (packet.length+2) - pbuf_length);

        pbuf.append(buf.mid(pos, data_length));

        pbuf_length += data_length;
        pos += data_length;

        if (pbuf_length < (packet.length + 2))
            return false;
    }

    if (packet.length > 0)
    {
        packet.longData.append(pbuf.mid(2, packet.length));

        if (decryptPacket)// && eventData.frame)
        {
//            qDebug() << " KEY=" << decrypter.key;
            if (!decrypter.key)// || encryptedPackets.size())
                encryptedPackets.push_back(packet);

            else
                decrypter.decrypt(packet.longData);
        }
    }

    pbuf_length = 0;
    pbuf.clear();

    return true;
}



void PacketParser::parseCarPacket(Packet &packet, bool emitSignal)
{
    if (noSession)
    {
        eventData.frame = 0;
        noSession = false;
        emit noLiveSession(false, "");
    }

    qDebug()<<"CAR="<<packet.carID<<" "<<packet.type<<" "<<packet.data<<" "<<packet.length<<" "<<packet.longData.size()<<" "<<packet.longData.constData();

    if (packet.carID > eventData.driversData.size() || packet.carID < 1)
    {
//        for (int i = 0; i < packet.longData.size(); ++i)
//            std::cout<<(int)packet.longData[i]<<" ";

//        std::cout<<std::endl;
        return;
    }

    Packet copyPacket = packet;
    if (packet.type != LTPackets::CAR_POSITION_HISTORY && !decrypter.checkDecryption(packet.longData))
    {
        packet.longData = "";
        packet.length = 0;
//        eventData.frame = 0;
    }

    //return;
    QString s;
//    DriverData dd;
    int ibuf;
    bool ok;
    PitData pd;
    switch(packet.type)
    {
        case LTPackets::CAR_POSITION_UPDATE:
//            dd.carID = packet.carID;
//            dd.pos = packet.data;
//            dd.numPits = 0;
//            dd.colorData.positionColor() = LTPackets::RED;

//            dd.colorData = ColorData();

            if ((packet.carID-1) < eventData.driversData.size() && (packet.carID-1) >= 0 && eventData.driversData[packet.carID - 1].lapData.isEmpty())
            {
                eventData.driversData[packet.carID - 1].carID = packet.carID;
                eventData.driversData[packet.carID - 1].lastLap.carID = packet.carID;
                eventData.driversData[packet.carID - 1].pos = packet.data;
                eventData.driversData[packet.carID - 1].colorData.positionColor() = LTPackets::RED;
            }

            break;

        case LTPackets::RACE_POSITION:
        //case LTPackets::PRACTICE_POSITION:
        //case LTPackets::QUALI_POSITION:
            ibuf = packet.longData.toInt(&ok);
            if (ok && ibuf > 0)
            {
                    eventData.driversData[packet.carID-1].pos = eventData.driversData[packet.carID-1].lastLap.pos = ibuf;
                    eventData.driversData[packet.carID-1].retired = false;

                    if ((eventData.eventType == LTPackets::PRACTICE_EVENT || eventData.eventType == LTPackets::QUALI_EVENT) &&
                            !eventData.driversData[packet.carID-1].lapData.isEmpty())
                    {
                        eventData.driversData[packet.carID-1].lapData.last().pos = ibuf;

                        if (!eventData.driversData[packet.carID-1].posHistory.isEmpty())
                            eventData.driversData[packet.carID-1].posHistory.last() = ibuf;

                    }
            }
//            else
                if ((eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString() == "STOP" ||
                    eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString() == "STOP" ||
                    eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString() == "STOP" ||
                    eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "RETIRED") &&
                    eventData.flagStatus != LTPackets::RED_FLAG)
                eventData.driversData[packet.carID-1].retired = true;

            //If the packet data isn't a number, probably the decryption has failed. Set the frame number to 0 and wait for another SYS=2 packet to obtain the keyframe again
//            if (!ok)
//                eventData.frame = 0;

            eventData.driversData[packet.carID-1].colorData.positionColor() = (LTPackets::Colors)packet.data;
            break;

        case LTPackets::RACE_NUMBER:
        //case LTPackets::PRACTICE_NUMBER:
        //case LTPackets::QUALI_NUMBER:
            ibuf = packet.longData.toInt(&ok);
            if (ok && ibuf > 0)
                eventData.driversData[packet.carID-1].number = ibuf;

            eventData.driversData[packet.carID-1].colorData.numberColor() = (LTPackets::Colors)packet.data;
            eventData.driversData[packet.carID-1].updatePitStatus((LTPackets::Colors)packet.data, eventData);
            break;

        case LTPackets::RACE_DRIVER:
        //case LTPackets::PRACTICE_DRIVER:
        //case LTPackets::QUALI_DRIVER:
            if (packet.longData != "" /*&& packet.length > 0*/ && QString(packet.longData).indexOf(QRegExp("[A-Z]")) != -1)//eventData.driversData[packet.carID-1].driver == "")
            {
                s = packet.longData;
                eventData.driversData[packet.carID-1].driver = SeasonData::getInstance().getDriverName(s);

                //for every driver this code is executed only once since drivers list is sent only at the beginnig of the session
                //after whole list is updated, SeasonData singleton will contain fresh list of the main drivers that take a part in this session
                SeasonData::getInstance().updateTeamList(eventData.driversData[packet.carID-1]);

                //driver names are usually sent sorted, so after we got last driver we can reload helmets
                if (packet.carID == eventData.driversData.size())
                    ImagesFactory::getInstance().getHelmetsFactory().reloadHelmets();
            }
            eventData.driversData[packet.carID-1].colorData.driverColor() = (LTPackets::Colors)packet.data;
            break;

        case LTPackets::CAR_POSITION_HISTORY:
            if (packet.length - 1 > eventData.lapsCompleted)
                eventData.lapsCompleted = copyPacket.length - 1;

            if (eventData.driversData[packet.carID-1].lapData.isEmpty())
                eventData.driversData[packet.carID-1].lastLap.lapNum = copyPacket.length - 1;

            eventData.driversData[packet.carID-1].posHistory.clear();
            for (int i = eventData.driversData[packet.carID-1].posHistory.size(); i < copyPacket.longData.size(); ++i)
                eventData.driversData[packet.carID-1].posHistory.append((int)copyPacket.longData[i]);

            //during the race carID is always equal to grid position
            if (eventData.eventType == LTPackets::RACE_EVENT && !eventData.driversData[packet.carID-1].posHistory.isEmpty())
                eventData.driversData[packet.carID-1].posHistory[0] = packet.carID;

            break;

        default:
            switch (eventData.eventType)
            {
                case LTPackets::RACE_EVENT: handleRaceEvent(packet); break;
                case LTPackets::QUALI_EVENT: handleQualiEvent(packet); break;
                case LTPackets::PRACTICE_EVENT: handlePracticeEvent(packet); break;
            }
    }
    eventData.driversData[packet.carID-1].carID = packet.carID;

    if (emitSignal)
        emit driverDataChanged(packet.carID);
}

void PacketParser::handleRaceEvent(const Packet &packet)
{
    int lap = 0;
    PitData pd;
    switch (packet.type)
    {
        case LTPackets::RACE_GAP:

            if (packet.length > -1)
            {
                eventData.driversData[packet.carID-1].lastLap.gap = packet.longData;

                //after the start of the race we don't get the lap time so we have to add the lap here
                if (eventData.driversData[packet.carID-1].lapData.isEmpty() && eventData.driversData[packet.carID-1].posHistory.size() <= 1)
                {
                    eventData.driversData[packet.carID-1].lastLap.lapNum = 1;
                    eventData.driversData[packet.carID-1].addLap(eventData);
                }
            }
            eventData.driversData[packet.carID-1].colorData.gapColor() = (LTPackets::Colors)packet.data;

            //when driver is in the pits we need to update his position, gap and interval data
            if (eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "IN PIT")
                eventData.driversData[packet.carID-1].updateInPit();


            break;


        case LTPackets::RACE_INTERVAL:

            if (eventData.driversData[packet.carID-1].pos == 1)
            {
                eventData.lapsCompleted = packet.longData.toInt();
                eventData.saveWeather();
            }

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.interval = packet.longData;

            if (eventData.driversData[packet.carID-1].pos == 1 &&
               (eventData.driversData[packet.carID-1].lapData.isEmpty() && eventData.driversData[packet.carID-1].posHistory.size() <= 1))
                {
                    eventData.driversData[packet.carID-1].lastLap.lapNum = 1;
                    eventData.driversData[packet.carID-1].addLap(eventData);
                }

            eventData.driversData[packet.carID-1].colorData.intervalColor() = (LTPackets::Colors)packet.data;

            //when driver is in the pits we need to update his position, gap and interval data
            if (eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "IN PIT")
                eventData.driversData[packet.carID-1].updateInPit();

            break;

        case LTPackets::RACE_LAP_TIME:

            if (packet.length > -1)
            {
                eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());

                if (!eventData.driversData[packet.carID-1].lapData.isEmpty() && packet.longData != "OUT")
                    eventData.driversData[packet.carID-1].lastLap.lapNum = eventData.driversData[packet.carID-1].lapData.last().lapNum + 1;


                if (eventData.driversData[packet.carID-1].lapData.isEmpty() &&
                    eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "IN PIT" &&
                    eventData.driversData[packet.carID-1].lastLap.lapNum < eventData.lapsCompleted)
                    eventData.driversData[packet.carID-1].lastLap.lapNum++;

                eventData.driversData[packet.carID-1].addLap(eventData);
            }
            eventData.driversData[packet.carID-1].colorData.lapTimeColor() = (LTPackets::Colors)packet.data;

            if (eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "RETIRED")
                eventData.driversData[packet.carID-1].retired = true;

            else if (/*eventData.driversData[packet.carID-1].retired &&*/
                     eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString() != "STOP" &&
                     eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString() != "STOP" &&
                     eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString() != "STOP")
                eventData.driversData[packet.carID-1].retired = false;


            break;

        case LTPackets::RACE_SECTOR_1:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[0] = LapTime(packet.longData.constData());

               eventData.driversData[packet.carID-1].colorData.sectorColor(1) = (LTPackets::Colors)packet.data;

            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString() != "")
                {
                    eventData.sessionRecords.secRecord[0].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[0].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString();
                    eventData.sessionRecords.secRecord[0].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[0].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum + 1;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }

            if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString() == "STOP" && eventData.flagStatus != LTPackets::RED_FLAG)
                eventData.driversData[packet.carID-1].retired = true;

            break;

        case LTPackets::RACE_PIT_LAP_1:

            lap = packet.longData.mid(1, packet.longData.size()-1).toInt();

            if (lap != 0)
            {
                QString time;
                switch (eventData.driversData[packet.carID-1].numPits)
                {
                    case 0: time = eventData.driversData[packet.carID-1].lastLap.sectorTimes[2]; break;
                    case 1: time = eventData.driversData[packet.carID-1].lastLap.sectorTimes[1]; break;
                    default: time = eventData.driversData[packet.carID-1].lastLap.sectorTimes[0]; break;
                }

                PitData pd(time, lap);
                eventData.driversData[packet.carID-1].addPitStop(pd);
            }
            eventData.driversData[packet.carID-1].colorData.pitColor(1) = (LTPackets::Colors)packet.data;

            break;

        case LTPackets::RACE_SECTOR_2:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[1] = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.sectorColor(2) = (LTPackets::Colors)packet.data;

            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString() != "")
                {
                    eventData.sessionRecords.secRecord[1].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[1].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString();
                    eventData.sessionRecords.secRecord[1].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[1].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum+1;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }

            if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString() == "STOP"/* && eventData.flagStatus != LTPackets::RED_FLAG*/)
                eventData.driversData[packet.carID-1].retired = true;

            break;

        case LTPackets::RACE_PIT_LAP_2:

            lap = packet.longData.mid(1, packet.longData.size()-1).toInt();

            if (lap != 0)
            {
                QString time;
                switch (eventData.driversData[packet.carID-1].numPits)
                {
                    case 1: time = eventData.driversData[packet.carID-1].lastLap.sectorTimes[2]; break;
                    default: time = eventData.driversData[packet.carID-1].lastLap.sectorTimes[1]; break;
                }

                PitData pd(time, lap);
                eventData.driversData[packet.carID-1].addPitStop(pd);
            }

            eventData.driversData[packet.carID-1].colorData.pitColor(2) = (LTPackets::Colors)packet.data;

            break;

        case LTPackets::RACE_SECTOR_3:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[2] = LapTime(packet.longData.constData());

            //sector 3 time is sent after the lap time, therefore we have to update recently added lap
            eventData.driversData[packet.carID-1].updateLastLap();
            eventData.driversData[packet.carID-1].colorData.sectorColor(3) = (LTPackets::Colors)packet.data;

            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET && eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString() != "")
            {
                eventData.sessionRecords.secRecord[2].driver = eventData.driversData[packet.carID-1].driver;
                eventData.sessionRecords.secRecord[2].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString();
                eventData.sessionRecords.secRecord[2].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;//.driversData[packet.carID-1].lastLap.numLap);
                eventData.sessionRecords.secRecord[2].number = eventData.driversData[packet.carID-1].number;

                TrackRecords::getInstance().gatherSessionRecords();
            }


            if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString() == "STOP" && eventData.flagStatus != LTPackets::RED_FLAG)
                eventData.driversData[packet.carID-1].retired = true;
            break;

        case LTPackets::RACE_PIT_LAP_3:

            lap = packet.longData.mid(1, packet.longData.size()-1).toInt();

            pd = PitData(eventData.driversData[packet.carID-1].lastLap.sectorTimes[2], lap);
            eventData.driversData[packet.carID-1].addPitStop(pd);

            eventData.driversData[packet.carID-1].colorData.pitColor(3)= (LTPackets::Colors)packet.data;
            break;

        case LTPackets::RACE_NUM_PITS:

            eventData.driversData[packet.carID-1].numPits = packet.longData.toInt();
            eventData.driversData[packet.carID-1].colorData.numPitsColor() = (LTPackets::Colors)packet.data;
            break;

         default:
            break;
    }
}

void PacketParser::handleQualiEvent(const Packet &packet)
{
    switch (packet.type)
    {
        case LTPackets::QUALI_PERIOD_1:
            eventData.driversData[packet.carID-1].qualiTimes[0] = LapTime(packet.longData.constData());
            eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());

            if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].qualiTimes[0].isValid())
            {
                if (eventData.driversData[packet.carID-1].qualiTimes[0] < eventData.sessionRecords.fastestLap.lapTime)
                {
                    eventData.sessionRecords.fastestLap.lapTime = eventData.driversData[packet.carID-1].qualiTimes[0];
                    eventData.sessionRecords.fastestLap.number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.fastestLap.driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.fastestLap.qPeriod = 1;

                    for (int i = 0; i < eventData.driversData.size(); ++i)
                        eventData.driversData[i].updateGaps(eventData);


                    TrackRecords::getInstance().gatherSessionRecords();

                }
                eventData.qualiPeriod = 1;
            }

            //correction of positions of drivers with time worse than 107%
            if (eventData.eventType == LTPackets::QUALI_EVENT && eventData.qualiPeriod == 1)
            {
                for (int i = 0; i < eventData.driversData.size(); ++i)
                {
                    if (eventData.driversData[i].carID != packet.carID)
                        eventData.driversData[i].correctPosition(eventData);
                }
            }


            eventData.driversData[packet.carID-1].colorData.qualiTimeColor(1) = (LTPackets::Colors)packet.data;
            break;


        case LTPackets::QUALI_PERIOD_2:

            eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());            
            eventData.driversData[packet.carID-1].qualiTimes[1] = LapTime(packet.longData.constData());
            eventData.driversData[packet.carID-1].colorData.qualiTimeColor(2) = (LTPackets::Colors)packet.data;

            if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].qualiTimes[1].isValid())
            {
                if (eventData.driversData[packet.carID-1].qualiTimes[1] < eventData.sessionRecords.fastestLap.lapTime)
                {
                    eventData.sessionRecords.fastestLap.lapTime = eventData.driversData[packet.carID-1].qualiTimes[1];
                    eventData.sessionRecords.fastestLap.number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.fastestLap.driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.fastestLap.qPeriod = 2;

                    for (int i = 0; i < eventData.driversData.size(); ++i)
                        eventData.driversData[i].updateGaps(eventData);

                    TrackRecords::getInstance().gatherSessionRecords();
                }
                eventData.qualiPeriod = 2;
            }
            break;

        case LTPackets::QUALI_PERIOD_3:

            eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());
            eventData.driversData[packet.carID-1].qualiTimes[2] = LapTime(packet.longData.constData());
            eventData.driversData[packet.carID-1].colorData.qualiTimeColor(3) = (LTPackets::Colors)packet.data;

            if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].qualiTimes[2].isValid())
            {
                if (eventData.driversData[packet.carID-1].qualiTimes[2] < eventData.sessionRecords.fastestLap.lapTime)
                {
                    eventData.sessionRecords.fastestLap.lapTime = eventData.driversData[packet.carID-1].qualiTimes[2];
                    eventData.sessionRecords.fastestLap.number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.fastestLap.driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.fastestLap.qPeriod = 3;

                    for (int i = 0; i < eventData.driversData.size(); ++i)
                        eventData.driversData[i].updateGaps(eventData);

                    TrackRecords::getInstance().gatherSessionRecords();
                }
                eventData.qualiPeriod = 3;
            }
            break;

        case LTPackets::QUALI_SECTOR_1:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[0] = LapTime(packet.longData.constData());

               eventData.driversData[packet.carID-1].colorData.sectorColor(1) = (LTPackets::Colors)packet.data;

            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString() != "" &&
                        eventData.driversData[packet.carID-1].lastLap.sectorTimes[0] <= LapTime(eventData.sessionRecords.secRecord[0].lapTime))
                {
                    eventData.sessionRecords.secRecord[0].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[0].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString();

                    eventData.sessionRecords.secRecord[0].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;
                    eventData.sessionRecords.secRecord[0].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[0].sessionTime = eventData.remainingTime;
                    eventData.sessionRecords.secRecord[0].qPeriod = eventData.qualiPeriod;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;

        case LTPackets::QUALI_SECTOR_2:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[1] = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.sectorColor(2) =(LTPackets::Colors) packet.data;

            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString() != "" &&
                        eventData.driversData[packet.carID-1].lastLap.sectorTimes[1] <= LapTime(eventData.sessionRecords.secRecord[1].lapTime))
                {
                    eventData.sessionRecords.secRecord[1].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[1].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString();

                    eventData.sessionRecords.secRecord[1].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;
                    eventData.sessionRecords.secRecord[1].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[1].sessionTime = eventData.remainingTime;
                    eventData.sessionRecords.secRecord[1].qPeriod = eventData.qualiPeriod;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;

        case LTPackets::QUALI_SECTOR_3:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[2] = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.sectorColor(3) = (LTPackets::Colors)packet.data;

            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString() != "" &&
                        eventData.driversData[packet.carID-1].lastLap.sectorTimes[2] <= LapTime(eventData.sessionRecords.secRecord[2].lapTime))
                {
                    eventData.sessionRecords.secRecord[2].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[2].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString();

                    eventData.sessionRecords.secRecord[2].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;
                    eventData.sessionRecords.secRecord[2].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[2].sessionTime = eventData.remainingTime;
                    eventData.sessionRecords.secRecord[2].qPeriod = eventData.qualiPeriod;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;

        case LTPackets::QUALI_LAP:

            eventData.driversData[packet.carID-1].lastLap.lapNum = packet.longData.toInt();
            eventData.driversData[packet.carID-1].addLap(eventData);
            eventData.driversData[packet.carID-1].colorData.numLapsColor() = (LTPackets::Colors)packet.data;

            break;


         default:
            break;
    }
}

void PacketParser::handlePracticeEvent(const Packet &packet)
{
    int ibuf;
    bool ok;
    switch (packet.type)
    {
        case LTPackets::PRACTICE_BEST:

            if (packet.length > 0)
                eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.lapTimeColor() = (LTPackets::Colors)packet.data;

            if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].lastLap.lapTime.isValid() &&
                eventData.driversData[packet.carID-1].lastLap.lapTime < eventData.sessionRecords.fastestLap.lapTime)
            {
                eventData.sessionRecords.fastestLap.lapTime = eventData.driversData[packet.carID-1].lastLap.lapTime;
                eventData.sessionRecords.fastestLap.number = eventData.driversData[packet.carID-1].number;
                eventData.sessionRecords.fastestLap.driver = eventData.driversData[packet.carID-1].driver;
                for (int i = 0; i < eventData.driversData.size(); ++i)
                    eventData.driversData[i].updateGaps(eventData);

                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;


        case LTPackets::PRACTICE_GAP:

            if (packet.length > 0)
                eventData.driversData[packet.carID-1].lastLap.gap = packet.longData;

            eventData.driversData[packet.carID-1].colorData.gapColor() = (LTPackets::Colors)packet.data;

            break;

        case LTPackets::PRACTICE_SECTOR_1:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[0] = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.sectorColor(1) = (LTPackets::Colors)packet.data;

            //save the session fastest sector 1 time
            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[0].toString() != "")
                {
                    eventData.sessionRecords.secRecord[0].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[0].lapTime = LapTime(packet.longData.constData());

                    eventData.sessionRecords.secRecord[0].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;
                    eventData.sessionRecords.secRecord[0].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[0].sessionTime = eventData.remainingTime;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;

        case LTPackets::PRACTICE_SECTOR_2:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[1] = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.sectorColor(2) = (LTPackets::Colors)packet.data;

            //save the session fastest sector 2 time
            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[1].toString() != "")
                {
                    eventData.sessionRecords.secRecord[1].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[1].lapTime = LapTime(packet.longData.constData());

                    eventData.sessionRecords.secRecord[1].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;
                    eventData.sessionRecords.secRecord[1].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[1].sessionTime = eventData.remainingTime;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;

        case LTPackets::PRACTICE_SECTOR_3:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sectorTimes[2] = LapTime(packet.longData.constData());

            eventData.driversData[packet.carID-1].colorData.sectorColor(3) = (LTPackets::Colors)packet.data;

            //save the session fastest sector 3 time
            if ((LTPackets::Colors)packet.data == LTPackets::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sectorTimes[2].toString() != "")
                {
                    eventData.sessionRecords.secRecord[2].driver = eventData.driversData[packet.carID-1].driver;
                    eventData.sessionRecords.secRecord[2].lapTime = eventData.driversData[packet.carID-1].lastLap.sectorTimes[2];

                    eventData.sessionRecords.secRecord[2].lapNum = eventData.driversData[packet.carID-1].lastLap.lapNum;
                    eventData.sessionRecords.secRecord[2].number = eventData.driversData[packet.carID-1].number;
                    eventData.sessionRecords.secRecord[2].sessionTime = eventData.remainingTime;
                }
                TrackRecords::getInstance().gatherSessionRecords();
            }
            break;

        case LTPackets::PRACTICE_LAP:

            ibuf = packet.longData.toInt(&ok);
            if (ok && ibuf > 0)
                eventData.driversData[packet.carID-1].lastLap.lapNum = ibuf;

            eventData.driversData[packet.carID-1].addLap(eventData);
            eventData.driversData[packet.carID-1].colorData.numLapsColor() = (LTPackets::Colors)packet.data;

            break;

         default:
            break;
    }
}

void PacketParser::parseSystemPacket(Packet &packet, bool emitSignal)
{
//    if (packet.type != LTPackets::SYS_COMMENTARY && packet.type != LTPackets::SYS_TIMESTAMP)
        qDebug()<<"SYS="<<packet.type<<" "<<packet.carID<<" "<<packet.data<<" "<<packet.length<<" "<< ((packet.type != LTPackets::SYS_COMMENTARY) ? packet.longData.constData() : "");

//    qDebug() << "EVENT TYPE=" << eventData.eventType;

    unsigned int number, i;
//    unsigned char packetLongData[129];
    unsigned char uc;
    QString s, format, oldTime;
    QTime time;
    int ibuf;
    double dbuf;
    bool ok;

    int j = 0;
    int cnt;
    Packet copyPacket = packet;

//    if (packet.type != LTPackets::SYS_COMMENTARY && packet.type != LTPackets::SYS_KEY_FRAME && !decrypter.checkDecryption(packet.longData))
//    {
//        packet.longData = "";
//        packet.length = 0;
////        eventData.frame = 0;
//    }

    if (eventData.eventType == LTPackets::QUALI_EVENT && eventData.qualiPeriod == 0)
        eventData.qualiPeriod = 1;

    switch(packet.type)
    {
        case LTPackets::SYS_EVENT_ID:
            number = 0;

            //just before the start of the session server sends a strange packet with the current data instead of the number of decryption key that we have to obtain
            number = copyPacket.longData.right(copyPacket.longData.size()-1).toInt(&ok);
            s = copyPacket.longData.right(copyPacket.longData.size()-1);

            eventData.key = 0;
            decrypter.key = 0;

            if (ok)
            {
                eventData.eventId = number;
                emit requestDecryptionKey(number);
                //httpReader.obtainDecryptionKey(number);
            }
            else
                eventData.frame = 0;

            if (eventData.eventType != 0 && eventData.eventType != (LTPackets::EventType)packet.data)
                eventData.clear();

            if (s.size() > 0 && s[0] == '_')
            {
                eventData.clear();
                noSession = true;
                emit noLiveSession(true, s.right(s.size()-1));
            }

            eventData.eventInfo = SeasonData::getInstance().getEvent(QDate::fromString("18.11.2012", "dd.MM.yyyy"));//QDate::currentDate());//(int)(packet.longData[0]);
            eventData.eventType = (LTPackets::EventType)copyPacket.data;

            eventData.lapsCompleted = 0;



//            decrypter.resetDecryption();
            break;

        case LTPackets::SYS_KEY_FRAME:
            number = 0;
            i = packet.length;

            while(i)
            {
                number <<= 8;
                uc = packet.longData[--i];
                number |= uc;
            }
//            decrypter.resetDecryption();

             if (!eventData.frame || number == 1) // || decryption_failure
             {
                eventData.frame = number;
//                emit requestKeyFrame(number);

//                httpReader.obtainKeyFrame(number);


                 /*onDecryptionKeyObtained(2841044872);*/   //valencia race
//                  decryptionKeyObtained(2971732062);      //valencia qual
//                onDecryptionKeyObtained(3585657959);  //?
//                onDecryptionKeyObtained(2488580439);  //qual
//                 onDecryptionKeyObtained(2438680630);  //race
//                 onDecryptionKeyObtained(3875488254); //fp1

//                onDecryptionKeyObtained(2991040326);    //bahrain fp2
//                onDecryptionKeyObtained(2976363859);    //bahrain fp3
//                onDecryptionKeyObtained(2462388168);    //bahrain quali
//                  onDecryptionKeyObtained(2942703366);      //bahrain race

//                 onDecryptionKeyObtained(3710001497);       //malaysia race
//                onDecryptionKeyObtained(2922444379);      //spain race
//                httpReader.obtainKeyFrame(53);
//                onDecryptionKeyObtained(4246644581);      //gbr race
//                decryptionKeyObtained(3195846070);	//gbr quali
//                 onDecryptionKeyObtained(3397635038);	//gbr fp3
//                 onDecryptionKeyObtained(4071769653);	//gbr fp1

//                resetDecryption();

             }
             else
                 eventData.frame = number;

             break;

        case LTPackets::SYS_WEATHER:
            switch (packet.data)
            {
                case LTPackets::WEATHER_SESSION_CLOCK:
                    s = packet.longData;
                    cnt = s.count(':');
                    oldTime = eventData.remainingTime.toString("mm:ss");
                    format = (cnt == 2) ? "h:mm:ss" : "mm:ss";
                    time = QTime::fromString(s, format);
                    if (time.isValid())
                        eventData.remainingTime = time;

                    if (oldTime == "00:00" && time.minute() != 0 && eventData.eventType == LTPackets::QUALI_EVENT)
                        ++eventData.qualiPeriod;

                    //session actually starts when we get the 59 seconds mark (i.e. Q1 starts when the time is 19:59)
                    j = eventData.remainingTime.second();

                    if (!eventData.sessionStarted && j != 0 && (eventData.eventType != LTPackets::RACE_EVENT ||
                        (eventData.eventType == LTPackets::RACE_EVENT && eventData.lapsCompleted < eventData.eventInfo.laps)))
                    {
                        eventData.sessionStarted = true;
                        eventData.sessionFinished = false;
                    }

                    emit sessionStarted();

                break;
                case LTPackets::WEATHER_TRACK_TEMP:
                    number = 0;
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                    {
                        eventData.weather.setTrackTemp(dbuf);
                        eventData.saveWeather();
                    }
                    break;

                case LTPackets::WEATHER_AIR_TEMP:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                    {
                        eventData.weather.setAirTemp(dbuf);
                        eventData.saveWeather();
                    }
                    break;

                case LTPackets::WEATHER_WIND_SPEED:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                    {
                        eventData.weather.setWindSpeed(dbuf);
                        eventData.saveWeather();
                    }
                    break;

                case LTPackets::WEATHER_HUMIDITY:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                    {
                        eventData.weather.setHumidity(dbuf);
                        eventData.saveWeather();
                    }
                    break;

                case LTPackets::WEATHER_PRESSURE:
                    number = 0;
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                    {
                        eventData.weather.setPressure(dbuf);
                        eventData.saveWeather();
                    }

                    break;

                case LTPackets::WEATHER_WIND_DIRECTION:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                    {
                        eventData.weather.setWindDirection(dbuf);
                        eventData.saveWeather();
                    }
                    break;

                case LTPackets::WEATHER_WET_TRACK:
                    ibuf = packet.longData.toInt(&ok);
                    if (ok)
                    {
                        eventData.weather.setWetDry(ibuf);
                        eventData.saveWeather();
                    }
                    break;

                default:
                    break;
            }
            break;
        case LTPackets::SYS_TRACK_STATUS:
            switch (packet.data)
            {
                case 1:
                    uc = packet.longData[0];
                    eventData.flagStatus = LTPackets::FlagStatus(uc - '0');
                    break;
            }
            break;
        case LTPackets::SYS_COPYRIGHT:
            break;
        case LTPackets::SYS_NOTICE:
            break;
        case LTPackets::SYS_SPEED:
            switch((int)copyPacket.longData[0])
            {
                case LTPackets::SPEED_SECTOR1:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 6; i+=8)
                    {
                        eventData.sessionRecords.secSpeed[0][j].driver = SeasonData::getInstance().getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sessionRecords.secSpeed[0][j++].speed = copyPacket.longData.mid(i+4, 3).toDouble();
                    }
                    break;
                case LTPackets::SPEED_SECTOR2:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 6; i+=8)
                    {
                        eventData.sessionRecords.secSpeed[1][j].driver = SeasonData::getInstance().getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sessionRecords.secSpeed[1][j++].speed = copyPacket.longData.mid(i+4, 3).toDouble();
                    }
                    break;
                case LTPackets::SPEED_SECTOR3:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 6; i+=8)
                    {
                        eventData.sessionRecords.secSpeed[2][j].driver = SeasonData::getInstance().getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sessionRecords.secSpeed[2][j++].speed = copyPacket.longData.mid(i+4, 3).toDouble();
                    }
                    break;
                case LTPackets::SPEED_TRAP:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 6; i+=8)
                    {
                        eventData.sessionRecords.speedTrap[j].driver = SeasonData::getInstance().getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sessionRecords.speedTrap[j++].speed = copyPacket.longData.mid(i+4, 3).toDouble();
                    }
                    break;
                case LTPackets::FL_CAR:
                    eventData.sessionRecords.fastestLap.number = copyPacket.longData.mid(1, copyPacket.longData.size()-1).toInt();
                    break;

                case LTPackets::FL_DRIVER:
                    if (eventData.eventType == LTPackets::RACE_EVENT)
                    {
                        s = copyPacket.longData.mid(1, copyPacket.longData.size()-1);
                        eventData.sessionRecords.fastestLap.driver = SeasonData::getInstance().getDriverName(s);// s.left(4) + s.right(s.size()-4).toLower();
                    }
                    break;

                case LTPackets::FL_LAP:
                    eventData.sessionRecords.fastestLap.lapNum = copyPacket.longData.mid(1, copyPacket.longData.size()-1).toInt();
                    if (eventData.sessionRecords.fastestLap.lapNum > 0)
                    {
                        for (int i = 0; i < eventData.driversData.size(); ++i)
                        {
                            if (eventData.driversData[i].number == eventData.sessionRecords.fastestLap.number)
                            {
                                eventData.driversData[i].setFastestLap(eventData.sessionRecords.fastestLap.lapTime, eventData.sessionRecords.fastestLap.lapNum);
                                break;
                            }
                        }
                    }
                    break;

                case LTPackets::FL_TIME:
                    if (eventData.eventType == LTPackets::RACE_EVENT)
                        eventData.sessionRecords.fastestLap.lapTime = LapTime(copyPacket.longData.mid(1, copyPacket.longData.size()-1).constData());

                    TrackRecords::getInstance().gatherSessionRecords();
                    break;
            }

            break;
        case LTPackets::SYS_COMMENTARY:
            s = copyPacket.longData.mid(2, copyPacket.longData.size()-2);

            if ((int)copyPacket.longData[0] < 32)
            {
                if ((int)(copyPacket.longData[1] & 2) > 0)
                    eventData.commentary.append(s);
                else
                    eventData.commentary.append(QString::fromUtf8(s.toAscii()));
            }
            else
                eventData.commentary.append(s.toLatin1());

            //second byte of the commentary tells wether we need to append new line
            if ((int)(copyPacket.longData[1]))
                eventData.commentary.append("\n\n");


        case LTPackets::SYS_TIMESTAMP:
        {
           uc = copyPacket.longData[1];
           int ts = uc << 8;
           uc = copyPacket.longData[0];
           ts |= uc | 0 << 16;// & 0xff0000;
//           if ((eventData.timeStamp==0 && ts <= 1000) ||
//               (ts >= eventData.timeStamp+75 && ts < eventData.timeStamp+1000))
//           {
////                eventData.saveWeatherData();
//                eventData.timeStamp = ts;
//           }
        }
            break;
        default:
            break;
    }
    if (emitSignal)
        emit eventDataChanged();
}

void PacketParser::parsePackets(const QVector<Packet> &packets)
{
    for (int i = 0; i < packets.size(); ++i)
    {
        Packet packet = packets[i];
        if (packet.carID)
            parseCarPacket(packet, false);

        else if (packet.type > 2)
            parseSystemPacket(packet, false);

        else if (packet.type == 1)
        {
            eventData.eventType = (LTPackets::EventType)packet.data;
//            eventData.eventInfo = LTPackets::getEvent(QDate::currentDate());
            eventData.lapsCompleted = 0;
        }
    }
    emit dataChanged();
}

void PacketParser::parseBufferedPackets(const QVector<QPair<Packet, qint64> > &packets)
{
    bool emitSignal = false;
    for (int i = 0; i < packets.size(); ++i)
    {
        if (i == packets.size() - 1)
            emitSignal = true;

        Packet packet = packets[i].first;
        if (packet.carID)
            parseCarPacket(packet, false);

        else
            parseSystemPacket(packet, false);

        emit packetParsed(packets[i]);
    }
    emit dataChanged();
}

void PacketParser::parseBufferedPackets(const QPair<Packet, qint64> &packetPair)
{
    Packet copyPacket = packetPair.first;
    if (copyPacket.carID)
        parseCarPacket(copyPacket, true);

    else
        parseSystemPacket(copyPacket, true);

    emit packetParsed(packetPair);
}

void PacketParser::streamBlockObtained(const QByteArray &buf)
{
    savePacket(buf);
    if (parsing)
        streamData.append(buf);

    else
        parseStreamBlock(buf);
}

void PacketParser::keyFrameObtained(const QByteArray &keyFrame)
{
    //streamData = keyFrame;
    savePacket(keyFrame);

    decrypter.resetDecryption();
    if (parsing)
    {
        streamData.append(keyFrame);
    }
    else
    {
        clearEncryptedPackets();
        parseStreamBlock(keyFrame);
    }
}

void PacketParser::decryptionKeyObtained(unsigned int key)
{
    qDebug() << "GOT KEY" << key << "size="<<encryptedPackets.size() << "old key=" << decrypter.key;
    eventData.key = key;
    decrypter.key = key;

   if (key == 0)
       return;

    for (int i = 0; i < encryptedPackets.size(); ++i)
    {
        decrypter.decrypt(encryptedPackets[i].longData);

        if(encryptedPackets[i].carID)
            parseCarPacket(encryptedPackets[i]);

        else
            parseSystemPacket(encryptedPackets[i]);
    }
    encryptedPackets.clear();
}


void PacketParser::savePacket(const QByteArray &buf)
{
    //QFile file(QString("packets/packet_%1.dat").arg(packetNo++));
    //if (file.open(QIODevice::WriteOnly))
        //file.write(buf);
}
