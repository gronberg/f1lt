#include "datastreamreader.h"
#include <cmath>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

DataStreamReader::DataStreamReader(QObject *parent) :
    QObject(parent), host("live-timing.formula1.com"), port(4321), eventData(EventData::getInstance())
{    
    eventData.key = 0;    
    eventData.frame = 0;
    eventData.remainingTime = QTime();
    eventData.lapsCompleted = 0;

    eventData.flagStatus = LTData::GREEN_FLAG;

    eventData.trackTemp = 0;
    eventData.airTemp = 0;
    eventData.windSpeed = 0;
    eventData.humidity = 0;
    eventData.pressure = 0;
    eventData.windDirection = 0;

    parsing = false;

    for (int i = 0; i < LTData::ltTeams.size(); ++i)
    {
        eventData.driversData.append(DriverData());
        eventData.driversData.append(DriverData());
    }

    connect(&httpReader, SIGNAL(cookieRecieved(QString)), this, SLOT(onCookieReceived(QString)));
    connect(&httpReader, SIGNAL(keyFrameObtained(QByteArray)), this, SLOT(onKeyFrameObtained(QByteArray)));
    connect(&httpReader, SIGNAL(decryptionKeyObtained(uint)), this, SLOT(onDecryptionKeyObtained(uint)));
    connect(&httpReader, SIGNAL(authorizationError()), this, SIGNAL(authorizationError()));
    connect(&httpReader, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));


    connect(&socketReader, SIGNAL(packetObtained(QByteArray)), this, SLOT(onStreamBlockObtained(QByteArray)));
    connect(&socketReader,  SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)));

    packetNo = 0;
}

void DataStreamReader::connectToLTServer(QString email, QString passwd)
{
    eventData.frame = 0;
//    httpReader.authorize("http://"+host, email, passwd);
    httpReader.authorize("http://"+host, "http://formula1.com", email, passwd);
//    onCookieReceived("cookie");

//    start();
}

void DataStreamReader::disconnectFromLTServer()
{
    socketReader.disconnectFromHost();
}

//void DataStreamReader::run()
//{
//    exec();
//}

void DataStreamReader::parseStreamBlock()
{
    Packet packet;
    int pos = 0;

//    std::cout<<"encrypted="<<encryptedPackets.size()<<std::endl;

    parsing = true;
    while(parsePacket(streamData, packet, pos))
    {
        //socketReader.wakeUpServer();
        if (packet.encrypted && !eventData.key)
        {
            packet.longData.clear();
            continue;
        }

        if(packet.carID)
        {
//            qDebug("handle car packet!");
            parseCarPacket(packet);            
        }
        else
        {
//            qDebug("handle system packet!");
            parseSystemPacket(packet);

        }
        emit packetParsed(packet);
        packet.longData.clear();
    }
    parsing = false;
//    socketReader.wakeUpServer();
    //socketReader.obtainData();
}

bool DataStreamReader::parsePacket(const QByteArray &buf, Packet &packet, int &pos)
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

    packet.type = LTData::getPacketType(pbuf);
    packet.carID = LTData::getCarPacket(pbuf);    

    bool decryptPacket = false;

    if (packet.carID)
    {
        switch(packet.type)
        {
            case LTData::CAR_POSITION_UPDATE:
                packet.length = LTData::getSpecialPacketLength(pbuf);
                packet.data = LTData::getSpecialPacketData(pbuf);
                break;

            case LTData::CAR_POSITION_HISTORY:
                packet.length = LTData::getLongPacketLength(pbuf);
                packet.data = LTData::getLongPacketData(pbuf);
                decryptPacket = true;
                break;

            default:
                packet.length = LTData::getShortPacketLength(pbuf);
                packet.data = LTData::getShortPacketData(pbuf);
                decryptPacket = true;
                break;
        }
    }
    else
    {
        switch(packet.type)
        {
            case LTData::SYS_EVENT_ID:
            case LTData::SYS_KEY_FRAME:
                packet.length = LTData::getShortPacketLength(pbuf);
                packet.data = LTData::getShortPacketData(pbuf);
                break;

            case LTData::SYS_TIMESTAMP:
                packet.length = 2;
                packet.data = 0;
                decryptPacket = true;
                break;

            case LTData::SYS_WEATHER:
            case LTData::SYS_TRACK_STATUS:
                packet.length = LTData::getShortPacketLength(pbuf);
                packet.data = LTData::getShortPacketData(pbuf);
                decryptPacket = true;
                break;

            case LTData::SYS_COMMENTARY:
            case LTData::SYS_NOTICE:
            case LTData::SYS_SPEED:
                packet.length = LTData::getLongPacketLength(pbuf);
                packet.data = LTData::getLongPacketData(pbuf);
                decryptPacket = true;
                break;

            case LTData::SYS_COPYRIGHT:
                packet.length = LTData::getLongPacketLength(pbuf);
                packet.data = LTData::getLongPacketData(pbuf);
                break;

            case LTData::SYS_VALID_MARKER:
            case LTData::SYS_REFRESH_RATE:
                packet.length = LTData::getShortPacketLength(pbuf);
                packet.data = LTData::getShortPacketData(pbuf);
                break;

            default:
                packet.length = 0;
                packet.data = 0;
                break;
        }
    }    

//    std::cout<<"PACKET: "<<packet.type<<", "<<packet.carID<<", "<<packet.data<<", "<<packet.length<<std::endl;

//    decryptPacket = false;
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
            if (!eventData.key)// || encryptedPackets.size())
                encryptedPackets.push_back(packet);

            else            
                decrypt(packet.longData);
        }
    }

    pbuf_length = 0;
    pbuf.clear();

    return true;
}

bool DataStreamReader::checkDecryption(QString stream)
{
    for (int i = 0; i < stream.size(); ++i)
    {
        unsigned char c = (unsigned char)(stream[i].toAscii());
        if ((int)c != 10 && ((int)c < 32 || (int)c > 126))
            return false;

    }
    return true;
//    int j = str.indexOf(QRegExp("[^\\.?!\\-=:'\" ]&[^A-Z]"));//&[^a-z]&[^0-9]"));
//    if (str != "")
//        qDebug() << "CHECKKER= " << packet.longData << ", " << j;
}

void DataStreamReader::parseCarPacket(Packet &packet, bool emitSignal)
{    
    qDebug()<<"CAR="<<packet.carID<<" "<<packet.type<<" "<<packet.data<<" "<<packet.length<<" "<<packet.longData.size()<<" "<<packet.longData.constData();

    if (packet.carID > eventData.driversData.size() || packet.carID < 1)
    {
//        for (int i = 0; i < packet.longData.size(); ++i)
//            std::cout<<(int)packet.longData[i]<<" ";

//        std::cout<<std::endl;
        return;
    }

    Packet copyPacket = packet;
    if (packet.type != LTData::CAR_POSITION_HISTORY && !checkDecryption(packet.longData))
    {
        packet.longData = "";
        packet.length = 0;
        eventData.frame = 0;
    }

    //return;
    QString s;
    DriverData dd;
    int lap;
    int ibuf;
    bool ok;
    PitData pd;
    switch(packet.type)
    {    
        case LTData::CAR_POSITION_UPDATE:
            dd.carID = packet.carID;
            dd.pos = packet.data;
            dd.numPits = 0;
            dd.colorData[LTData::RACE_POSITION] = LTData::RED;

            for (int i = 0; i < 14; ++i)
                dd.colorData[i] = LTData::DEFAULT;

            if ((packet.carID-1) < eventData.driversData.size() && (packet.carID-1) >= 0 && eventData.driversData[packet.carID - 1].lapData.isEmpty())
            {
                eventData.driversData[packet.carID - 1].carID = packet.carID;
                eventData.driversData[packet.carID - 1].lastLap.carID = packet.carID;
                eventData.driversData[packet.carID - 1].pos = packet.data;
                eventData.driversData[packet.carID - 1].colorData[LTData::RACE_POSITION] = LTData::RED;
            }

            break;

        case LTData::RACE_POSITION:
        //case LTData::PRACTICE_POSITION:
        //case LTData::QUALI_POSITION:
            ibuf = packet.longData.toInt(&ok);
            if (ok && ibuf > 0)
            {
                    eventData.driversData[packet.carID-1].pos = eventData.driversData[packet.carID-1].lastLap.pos = ibuf;
                    eventData.driversData[packet.carID-1].retired = false;
            }
            else
                if ((eventData.driversData[packet.carID-1].lastLap.sector1.toString() == "STOP" ||
                    eventData.driversData[packet.carID-1].lastLap.sector2.toString() == "STOP" ||
                    eventData.driversData[packet.carID-1].lastLap.sector3.toString() == "STOP" ||
                    eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "RETIRED") &&
                    eventData.flagStatus != LTData::RED_FLAG)
                eventData.driversData[packet.carID-1].retired = true;

            //If the packet data isn't a number, probably the decryption has failed. Set the frame number to 0 and wait for another SYS=2 packet to obtain the keyframe again
            if (!ok)
                eventData.frame = 0;

            if ((eventData.eventType == LTData::PRACTICE_EVENT || eventData.eventType == LTData::QUALI_EVENT) && !eventData.driversData[packet.carID-1].lapData.isEmpty())
            {
                eventData.driversData[packet.carID-1].lapData.last().pos = ibuf;

                if (!eventData.driversData[packet.carID-1].posHistory.isEmpty())
                    eventData.driversData[packet.carID-1].posHistory.last() = ibuf;
            }
            eventData.driversData[packet.carID-1].colorData[LTData::RACE_POSITION] = (LTData::Colors)packet.data;
            break;

        case LTData::RACE_NUMBER:
        //case LTData::PRACTICE_NUMBER:
        //case LTData::QUALI_NUMBER:
            ibuf = packet.longData.toInt(&ok);
            if (ok && ibuf > 0)
                eventData.driversData[packet.carID-1].number = ibuf;

            eventData.driversData[packet.carID-1].colorData[LTData::RACE_NUMBER] = (LTData::Colors)packet.data;
            break;

        case LTData::RACE_DRIVER:
        //case LTData::PRACTICE_DRIVER:
        //case LTData::QUALI_DRIVER:
            if (packet.longData != "" && packet.length > 0 && QString(packet.longData).indexOf(QRegExp("[A-Z]")) != -1)//eventData.driversData[packet.carID-1].driver == "")
            {
                s = packet.longData;
                eventData.driversData[packet.carID-1].driver = LTData::getDriverName(s);
            }
            eventData.driversData[packet.carID-1].colorData[LTData::RACE_DRIVER] = (LTData::Colors)packet.data;
            break;

        case LTData::RACE_GAP:
        //case LTData::PRACTICE_BEST:
        //case LTData::QUALI_PERIOD_1:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:
                    if (packet.length > -1)                    
                    {
                        eventData.driversData[packet.carID-1].lastLap.gap = packet.longData;

                        //after the start of the race we don't get the lap time so we have to add the lap here
                        if (eventData.driversData[packet.carID-1].lapData.isEmpty() && eventData.driversData[packet.carID-1].posHistory.size() <= 1)
                        {
                            eventData.driversData[packet.carID-1].lastLap.numLap = 1;
                            eventData.driversData[packet.carID-1].addLap(eventData);
                        }
                    }
                    eventData.driversData[packet.carID-1].colorData[LTData::RACE_GAP] = (LTData::Colors)packet.data;

                    //when driver is in the pits we need to update his position, gap and interval data
                    if (eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "IN PIT")
                        eventData.driversData[packet.carID-1].updateInPit();

                    break;

                case LTData::PRACTICE_EVENT:
                    if (packet.length > 0)
                        eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::PRACTICE_BEST] = (LTData::Colors)packet.data;

                    if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].lastLap.lapTime.isValid() &&
                        eventData.driversData[packet.carID-1].lastLap.lapTime < eventData.FLTime)
                    {
                        eventData.FLTime = eventData.driversData[packet.carID-1].lastLap.lapTime;
                        for (int i = 0; i < eventData.driversData.size(); ++i)
                            eventData.driversData[i].updateGaps(eventData);
                    }
                    break;

                case LTData::QUALI_EVENT:
                    eventData.driversData[packet.carID-1].q1 = LapTime(packet.longData.constData());
                    eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());

                    if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].q1.isValid() &&
                            eventData.driversData[packet.carID-1].q1 < eventData.FLTime)
                    {
                        eventData.FLTime = eventData.driversData[packet.carID-1].q1;
                        for (int i = 0; i < eventData.driversData.size(); ++i)
                            eventData.driversData[i].updateGaps(eventData);
                    }


                    eventData.driversData[packet.carID-1].colorData[LTData::QUALI_PERIOD_1] = (LTData::Colors)packet.data;
                    break;
            }
            break;


        case LTData::RACE_INTERVAL:
        //case LTData::PRACTICE_GAP:
        //case LTData::QUALI_PERIOD_2:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:
                    if (eventData.driversData[packet.carID-1].pos == 1)
                        eventData.lapsCompleted = packet.longData.toInt();                        

                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.interval = packet.longData;

                    if (eventData.driversData[packet.carID-1].pos == 1 &&
                       (eventData.driversData[packet.carID-1].lapData.isEmpty() && eventData.driversData[packet.carID-1].posHistory.size() <= 1))
                        {
                            eventData.driversData[packet.carID-1].lastLap.numLap = 1;
                            eventData.driversData[packet.carID-1].addLap(eventData);
                        }

                    eventData.driversData[packet.carID-1].colorData[LTData::RACE_INTERVAL] = (LTData::Colors)packet.data;

                    //when driver is in the pits we need to update his position, gap and interval data
                    if (eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "IN PIT")
                        eventData.driversData[packet.carID-1].updateInPit();
                    break;

                case LTData::PRACTICE_EVENT:
                    if (packet.length > 0)
                    {
                        eventData.driversData[packet.carID-1].lastLap.gap = packet.longData;
//                        if (!eventData.driversData[packet.carID-1].lapData.isEmpty())
//                            eventData.driversData[packet.carID-1].lapData.last().gap = packet.longData;
                    }
                    eventData.driversData[packet.carID-1].colorData[LTData::PRACTICE_GAP] = (LTData::Colors)packet.data;
                    break;

                case LTData::QUALI_EVENT:
                    eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());
                    eventData.driversData[packet.carID-1].q2 = LapTime(packet.longData.constData());
                    eventData.driversData[packet.carID-1].colorData[LTData::QUALI_PERIOD_2] = (LTData::Colors)packet.data;

                    if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].q2.isValid() &&
                            eventData.driversData[packet.carID-1].q2 < eventData.FLTime)
                    {
                        eventData.FLTime = eventData.driversData[packet.carID-1].q2;
                        for (int i = 0; i < eventData.driversData.size(); ++i)
                            eventData.driversData[i].updateGaps(eventData);
                    }
                    break;
            }
            break;

        case LTData::RACE_LAP_TIME:
        //case LTData::PRACTICE_SECTOR_1:
        //case LTData::QUALI_PERIOD_3:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:
                    if (packet.length > -1)
                    {
                        eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());

                        if (!eventData.driversData[packet.carID-1].lapData.isEmpty() && packet.longData != "OUT")
                            eventData.driversData[packet.carID-1].lastLap.numLap = eventData.driversData[packet.carID-1].lapData.last().numLap + 1;


                        if (eventData.driversData[packet.carID-1].lapData.isEmpty() &&
                            eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "IN PIT" &&
                            eventData.driversData[packet.carID-1].lastLap.numLap < eventData.lapsCompleted)
                            eventData.driversData[packet.carID-1].lastLap.numLap++;

                        eventData.driversData[packet.carID-1].addLap(eventData);
                    }
                    eventData.driversData[packet.carID-1].colorData[LTData::RACE_LAP_TIME] = (LTData::Colors)packet.data;

                    if (eventData.driversData[packet.carID-1].lastLap.lapTime.toString() == "RETIRED")
                        eventData.driversData[packet.carID-1].retired = true;

                    else if (/*eventData.driversData[packet.carID-1].retired &&*/
                             eventData.driversData[packet.carID-1].lastLap.sector1.toString() != "STOP" &&
                             eventData.driversData[packet.carID-1].lastLap.sector2.toString() != "STOP" &&
                             eventData.driversData[packet.carID-1].lastLap.sector3.toString() != "STOP")
                        eventData.driversData[packet.carID-1].retired = false;

                    break;

                case LTData::PRACTICE_EVENT:
                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.sector1 = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::PRACTICE_SECTOR_1] = (LTData::Colors)packet.data;

                    //save the session fastest sector 1 time
                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {
                        if (eventData.driversData[packet.carID-1].lastLap.sector1.toString() != "")
                        {
                            eventData.sec1Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec1Record[1] = packet.longData;

                            eventData.sec1Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                            eventData.sec1Record[3] = eventData.remainingTime.toString("h:mm:ss");
                        }
                    }
                    break;

                case LTData::QUALI_EVENT:
                    eventData.driversData[packet.carID-1].lastLap.lapTime = LapTime(packet.longData.constData());
                    eventData.driversData[packet.carID-1].q3 = LapTime(packet.longData.constData());
                    eventData.driversData[packet.carID-1].colorData[LTData::QUALI_PERIOD_3] = (LTData::Colors)packet.data;

                    if (eventData.driversData[packet.carID-1].pos == 1 && eventData.driversData[packet.carID-1].q3.isValid() &&
                            eventData.driversData[packet.carID-1].q3 < eventData.FLTime)
                    {
                        eventData.FLTime = eventData.driversData[packet.carID-1].q3;
                        for (int i = 0; i < eventData.driversData.size(); ++i)
                            eventData.driversData[i].updateGaps(eventData);
                    }
                    break;
            }
            break;        

        case LTData::RACE_SECTOR_1:
        //case LTData::PRACTICE_SECTOR_2:
        //case LTData::QUALI_SECTOR_1:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:
                case LTData::QUALI_EVENT:
                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.sector1 = LapTime(packet.longData.constData());

                       eventData.driversData[packet.carID-1].colorData[LTData::RACE_SECTOR_1] = (LTData::Colors)packet.data;

                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {                                                  
                        if (eventData.driversData[packet.carID-1].lastLap.sector1.toString() != "" && eventData.driversData[packet.carID-1].lastLap.sector1 <= LapTime(eventData.sec1Record[1]))
                        {
                            eventData.sec1Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec1Record[1] = eventData.driversData[packet.carID-1].lastLap.sector1.toString();

                            if (eventData.eventType == LTData::RACE_EVENT)
                                eventData.sec1Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                            else
                            {
                                eventData.sec1Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                                eventData.sec1Record[3] = eventData.remainingTime.toString("h:mm:ss");
                            }
                        }
                    }

                    if (eventData.driversData[packet.carID-1].lastLap.sector1.toString() == "STOP" && eventData.flagStatus != LTData::RED_FLAG)
                        eventData.driversData[packet.carID-1].retired = true;
                    break;

                case LTData::PRACTICE_EVENT:
                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.sector2 = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::PRACTICE_SECTOR_2] = (LTData::Colors)packet.data;

                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {
                        if (eventData.driversData[packet.carID-1].lastLap.sector2.toString() != "")
                        {
                            eventData.sec2Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec2Record[1] = eventData.driversData[packet.carID-1].lastLap.sector2.toString();

                            eventData.sec2Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                            eventData.sec2Record[3] = eventData.remainingTime.toString("h:mm:ss");
                        }
                    }
            }
            break;

        case LTData::RACE_PIT_LAP_1:
        //case LTData::PRACTICE_SECTOR_3:
        //case LTData::QUALI_SECTOR_2:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:

                    lap = packet.longData.mid(1, packet.longData.size()-1).toInt();

                    if (lap != 0)
                    {
                        QString time;
                        switch (eventData.driversData[packet.carID-1].numPits)
                        {
                            case 0: time = eventData.driversData[packet.carID-1].lastLap.sector3; break;
                            case 1: time = eventData.driversData[packet.carID-1].lastLap.sector2; break;
                            default: time = eventData.driversData[packet.carID-1].lastLap.sector1; break;
                        }

                        PitData pd(time, lap);
                        eventData.driversData[packet.carID-1].addPitStop(pd);
                    }
                    eventData.driversData[packet.carID-1].colorData[LTData::RACE_PIT_LAP_1] = (LTData::Colors)packet.data;
                    break;

                case LTData::PRACTICE_EVENT:                                        
                    if (packet.length > -1)                    
                        eventData.driversData[packet.carID-1].lastLap.sector3 = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::PRACTICE_SECTOR_3] = (LTData::Colors)packet.data;

                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {
                        if (eventData.driversData[packet.carID-1].lastLap.sector3.toString() != "")
                        {
                            eventData.sec3Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec3Record[1] = eventData.driversData[packet.carID-1].lastLap.sector3.toString();

                            eventData.sec3Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                            eventData.sec3Record[3] = eventData.remainingTime.toString("h:mm:ss");
                        }
                    }
                    break;

                case LTData::QUALI_EVENT:
                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.sector2 = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::QUALI_SECTOR_2] =(LTData::Colors) packet.data;

                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {
                        if (eventData.driversData[packet.carID-1].lastLap.sector2.toString() != "" && eventData.driversData[packet.carID-1].lastLap.sector2 <= LapTime(eventData.sec2Record[1]))
                        {
                            eventData.sec2Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec2Record[1] = eventData.driversData[packet.carID-1].lastLap.sector2.toString();

                            eventData.sec2Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                            eventData.sec2Record[3] = eventData.remainingTime.toString("h:mm:ss");
                        }
                    }
                    break;
            }
            break;

        case LTData::RACE_SECTOR_2:
        //case LTData::PRACTICE_LAP:
        //case LTData::QUALI_SECTOR_3:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:
                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.sector2 = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::RACE_SECTOR_2] = (LTData::Colors)packet.data;

                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {
                        if (eventData.driversData[packet.carID-1].lastLap.sector2.toString() != "")
                        {
                            eventData.sec2Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec2Record[1] = eventData.driversData[packet.carID-1].lastLap.sector2.toString();

                            eventData.sec2Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                        }
                    }

                    if (eventData.driversData[packet.carID-1].lastLap.sector2.toString() == "STOP"/* && eventData.flagStatus != LTData::RED_FLAG*/)
                        eventData.driversData[packet.carID-1].retired = true;
                    break;

                case LTData::PRACTICE_EVENT:
//                    if (packet.length > 0)
                    ibuf = packet.longData.toInt(&ok);
                    if (ok && ibuf > 0)
                        eventData.driversData[packet.carID-1].lastLap.numLap = ibuf;

                    eventData.driversData[packet.carID-1].addLap(eventData);
                    eventData.driversData[packet.carID-1].colorData[LTData::PRACTICE_LAP] = (LTData::Colors)packet.data;
                    break;

                case LTData::QUALI_EVENT:
                    if (packet.length > -1)
                        eventData.driversData[packet.carID-1].lastLap.sector3 = LapTime(packet.longData.constData());

                    eventData.driversData[packet.carID-1].colorData[LTData::QUALI_SECTOR_3] = (LTData::Colors)packet.data;

                    if ((LTData::Colors)packet.data == LTData::VIOLET)
                    {
                        if (eventData.driversData[packet.carID-1].lastLap.sector3.toString() != "" && eventData.driversData[packet.carID-1].lastLap.sector3 <= LapTime(eventData.sec3Record[1]))
                        {
                            eventData.sec3Record[0] = eventData.driversData[packet.carID-1].driver;
                            eventData.sec3Record[1] = eventData.driversData[packet.carID-1].lastLap.sector3.toString();

                            eventData.sec3Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap+1);
                            eventData.sec3Record[3] = eventData.remainingTime.toString("h:mm:ss");
                        }
                    }
                    break;
            }
            break;

        case LTData::RACE_PIT_LAP_2:
        //case LTData::QUALI_LAP:
            switch (eventData.eventType)
            {
                case LTData::RACE_EVENT:
                    lap = packet.longData.mid(1, packet.longData.size()-1).toInt();

                    if (lap != 0)
                    {
                        QString time;
                        switch (eventData.driversData[packet.carID-1].numPits)
                        {
                            case 1: time = eventData.driversData[packet.carID-1].lastLap.sector3; break;
                            default: time = eventData.driversData[packet.carID-1].lastLap.sector2; break;
                        }

                        PitData pd(time, lap);
                        eventData.driversData[packet.carID-1].addPitStop(pd);
                    }

                    eventData.driversData[packet.carID-1].colorData[LTData::RACE_PIT_LAP_2] = (LTData::Colors)packet.data;
                    break;

                case LTData::QUALI_EVENT:
                    eventData.driversData[packet.carID-1].lastLap.numLap = packet.longData.toInt();
                    eventData.driversData[packet.carID-1].addLap(eventData);
                    eventData.driversData[packet.carID-1].colorData[LTData::QUALI_LAP] = (LTData::Colors)packet.data;
                    break;
            }
            break;

        case LTData::RACE_SECTOR_3:

            if (packet.length > -1)
                eventData.driversData[packet.carID-1].lastLap.sector3 = LapTime(packet.longData.constData());

            if ((LTData::Colors)packet.data == LTData::VIOLET)
            {
                if (eventData.driversData[packet.carID-1].lastLap.sector3.toString() != "")
                {
                    eventData.sec3Record[0] = eventData.driversData[packet.carID-1].driver;
                    eventData.sec3Record[1] = eventData.driversData[packet.carID-1].lastLap.sector3.toString();

                    eventData.sec3Record[2] = QString("%1").arg(eventData.driversData[packet.carID-1].lastLap.numLap);//.driversData[packet.carID-1].lastLap.numLap);
                }
            }
            //sector 3 time is sent after the lap time, therefore we have to update recently added lap
            eventData.driversData[packet.carID-1].updateLastLap();
            eventData.driversData[packet.carID-1].colorData[LTData::RACE_SECTOR_3] = (LTData::Colors)packet.data;

            if (eventData.driversData[packet.carID-1].lastLap.sector3.toString() == "STOP" && eventData.flagStatus != LTData::RED_FLAG)
                eventData.driversData[packet.carID-1].retired = true;
            break;

        case LTData::RACE_PIT_LAP_3:
            lap = packet.longData.mid(1, packet.longData.size()-1).toInt();

            pd = PitData(eventData.driversData[packet.carID-1].lastLap.sector3, lap);
            eventData.driversData[packet.carID-1].addPitStop(pd);

            eventData.driversData[packet.carID-1].colorData[LTData::RACE_PIT_LAP_3] = (LTData::Colors)packet.data;
            break;

        case LTData::RACE_NUM_PITS:
            eventData.driversData[packet.carID-1].numPits = packet.longData.toInt();
            eventData.driversData[packet.carID-1].colorData[LTData::RACE_NUM_PITS] = (LTData::Colors)packet.data;
            break;

        case LTData::CAR_POSITION_HISTORY:
            if (packet.length - 1 > eventData.lapsCompleted)
                eventData.lapsCompleted = copyPacket.length - 1;

            if (eventData.driversData[packet.carID-1].lapData.isEmpty())
                eventData.driversData[packet.carID-1].lastLap.numLap = copyPacket.length - 1;

//            eventData.driversData[packet.carID-1].posHistory.clear();
            for (int i = eventData.driversData[packet.carID-1].posHistory.size(); i < copyPacket.longData.size(); ++i)
                eventData.driversData[packet.carID-1].posHistory.append((int)copyPacket.longData[i]);

            break;

         default:
//            std::cout<<"CAR DEFAULT!! "<<packet.type<<" "<<packet.carID<<" "<<packet.data<<std::endl;
            break;
    }
    eventData.driversData[packet.carID-1].carID = packet.carID;

    if (emitSignal)
        emit driverDataChanged(packet.carID);
}

void DataStreamReader::parseSystemPacket(Packet &packet, bool emitSignal)
{
    if (packet.type != LTData::SYS_COMMENTARY )//&& packet.type != LTData::SYS_TIMESTAMP)
        qDebug()<<"SYS="<<packet.type<<" "<<packet.carID<<" "<<packet.data<<" "<<packet.length<<" "<<packet.longData.constData();

    unsigned int number, i;
    unsigned char packetLongData[129];
    QString s, format;
    QTime time;
    int ibuf;
    double dbuf;
    bool ok;

    int j = 0;
    int cnt;
    Packet copyPacket = packet;
    memcpy(packetLongData, packet.longData.constData(), 128);

    if (packet.type != LTData::SYS_COMMENTARY && packet.type != LTData::SYS_KEY_FRAME && !checkDecryption(packet.longData))
    {
        packet.longData = "";
        packet.length = 0;
        eventData.frame = 0;
    }

    switch(packet.type)
    {
        case LTData::SYS_EVENT_ID:
            number = 0;

            //just before the start of the session server sends a strange packet with the current data instead of the number of decryption key that we have to obtain
            number = copyPacket.longData.right(copyPacket.longData.size()-1).toInt(&ok);

            eventData.key = 0;

            if (ok)
                httpReader.obtainDecryptionKey(number);
            else
                eventData.frame = 0;

            if (eventData.eventType != 0 && eventData.eventType != (LTData::EventType)packet.data)
                eventData.clear();

            eventData.eventInfo = LTData::getEvent(/*QDate::fromString("12.06.2011", "dd.MM.yyyy"));//*/QDate::currentDate());//(int)(packet.longData[0]);
            eventData.eventType = (LTData::EventType)copyPacket.data;
            eventData.lapsCompleted = 0;

            resetDecryption();
            break;

        case LTData::SYS_KEY_FRAME:
            number = 0;
            i = packet.length;

            while(i)
            {
                number <<= 8;
                number |= packetLongData[--i];
            }            
             resetDecryption();

             if (!eventData.frame) // || decryption_failure
             {
//                eventData.frame = number;
//                httpReader.obtainKeyFrame(number);


//                onDecryptionKeyObtained(3585657959);  //?
//                onDecryptionKeyObtained(2488580439);  //qual
//                 onDecryptionKeyObtained(2438680630);  //race
//                 onDecryptionKeyObtained(3875488254); //fp1

//                onDecryptionKeyObtained(2991040326);    //bahrain fp2
//                onDecryptionKeyObtained(2976363859);    //bahrain fp3
//                onDecryptionKeyObtained(2462388168);    //bahrain quali
//                  onDecryptionKeyObtained(2942703366);      //bahrain race

//                 onDecryptionKeyObtained(3710001497);       //malaysia race
                onDecryptionKeyObtained(2922444379);      //spain race
//                httpReader.obtainKeyFrame(53);

             }
             else
                 eventData.frame = number;

             break;

        case LTData::SYS_WEATHER:
            switch (packet.data)
            {                
                case LTData::WEATHER_SESSION_CLOCK:
                    s = packet.longData;
                    cnt = s.count(':');
                    format = (cnt == 2) ? "h:mm:ss" : "mm:ss";
                    time = QTime::fromString(s, format);
                    if (time.isValid())
                        eventData.remainingTime = time;

                    //session actually starts when we get the 59 seconds mark (i.e. Q1 starts when the time is 19:59)
                    j = eventData.remainingTime.second();

                    if (j != 0 && (eventData.eventType != LTData::RACE_EVENT ||
                        (eventData.eventType == LTData::RACE_EVENT && eventData.lapsCompleted < eventData.eventInfo.laps)))
                        eventData.sessionStarted = true;

                    emit sessionStarted();

                break;
                case LTData::WEATHER_TRACK_TEMP:
                    number = 0;
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                        eventData.trackTemp = dbuf;
                    break;

                case LTData::WEATHER_AIR_TEMP:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                        eventData.airTemp = dbuf;
                    break;

                case LTData::WEATHER_WIND_SPEED:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                        eventData.windSpeed = dbuf;
                    break;

                case LTData::WEATHER_HUMIDITY:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                        eventData.humidity = dbuf;
                    break;

                case LTData::WEATHER_PRESSURE:
                    number = 0;
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                        eventData.pressure = dbuf;

                    break;

                case LTData::WEATHER_WIND_DIRECTION:
                    dbuf = packet.longData.toDouble(&ok);
                    if (ok)
                        eventData.windDirection = dbuf;
                    break;

                case LTData::WEATHER_WET_TRACK:
                    ibuf = packet.longData.toInt(&ok);
                    if (ok)
                        eventData.wetdry = ibuf;
                    break;

                default:
                    break;
            }
            break;
        case LTData::SYS_TRACK_STATUS:
        qDebug() << "SYS_TRACK_STATUS=" << packetLongData[0] << ", " << packet.data;
            switch (packet.data)
            {
                case 1:
                    eventData.flagStatus = LTData::FlagStatus(packetLongData[0] - '0');
                    break;
            }
            break;
        case LTData::SYS_COPYRIGHT:
            break;
        case LTData::SYS_NOTICE:
            eventData.notice = packet.longData;
//            std::cout<<"SYS_NOTICE: "<<packet.longData.constData()<<std::endl;
            break;
        case LTData::SYS_SPEED:
            switch((int)copyPacket.longData[0])
            {
                case LTData::SPEED_SECTOR1:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 12; i+=8)
                    {
                        eventData.sec1Speed[j++] = LTData::getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sec1Speed[j++] = copyPacket.longData.mid(i+4, 3);
                    }
                    break;
                case LTData::SPEED_SECTOR2:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 12; i+=8)
                    {
                        eventData.sec2Speed[j++] = LTData::getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sec2Speed[j++] = copyPacket.longData.mid(i+4, 3);
                    }
                    break;
                case LTData::SPEED_SECTOR3:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 12; i+=8)
                    {
                        eventData.sec3Speed[j++] = LTData::getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.sec3Speed[j++] = copyPacket.longData.mid(i+4, 3);
                    }
                    break;
                case LTData::SPEED_TRAP:
                    for (int i = 1, j=0; i < copyPacket.longData.size() && j < 12; i+=8)
                    {
                        eventData.speedTrap[j++] = LTData::getDriverNameFromShort(copyPacket.longData.mid(i, 3));
                        eventData.speedTrap[j++] = copyPacket.longData.mid(i+4, 3);
                    }
                    break;
                case LTData::FL_CAR:
                    eventData.FLNumber = copyPacket.longData.mid(1, copyPacket.longData.size()-1).toInt();
                    break;

                case LTData::FL_DRIVER:
                    s = copyPacket.longData.mid(1, copyPacket.longData.size()-1);
                    eventData.FLDriver = LTData::getDriverName(s);// s.left(4) + s.right(s.size()-4).toLower();
                    break;                    

                case LTData::FL_LAP:
                    eventData.FLLap = copyPacket.longData.mid(1, copyPacket.longData.size()-1).toInt();
                    if (eventData.FLNumber > 0)
                    {
                        for (int i = 0; i < eventData.driversData.size(); ++i)
                        {
                            if (eventData.driversData[i].number == eventData.FLNumber)
                            {
                                eventData.driversData[i].setFastestLap(eventData.FLTime, eventData.FLLap);
                                break;
                            }
                        }
                    }
                    break;

                case LTData::FL_TIME:
                    if (eventData.eventType == LTData::RACE_EVENT)
                        eventData.FLTime = LapTime(copyPacket.longData.mid(1, copyPacket.longData.size()-1).constData());

                    break;
            }

            break;
        case LTData::SYS_COMMENTARY:        
            s = copyPacket.longData.mid(2, copyPacket.longData.size()-2);
//            qDebug() << "COMMENTARY!!!!!!! " << (int)(copyPacket.longData[0]) << " " << (int)(copyPacket.longData[1]) << " " << s;
//            eventData.commentary.append(copyPacket.longData.mid(2, copyPacket.longData.size()-2));
            j = 0;            
            j = s.indexOf("’");
            while (j != -1)
            {
                s.replace(j, 3, "'");
                j = s.indexOf("’", j);
            }
            if (checkDecryption(s))
            {
                eventData.commentary.append(s);

                //second byte of the commentary tells wether we need to append new line
                if ((int)(copyPacket.longData[1]))
                    eventData.commentary.append("\n\n");

//                j = eventData.commentary.indexOf(QRegExp("[\\.?!][A-Z]|[\\.?! ][0-9]{1,2}:[0-9]{1,2}"));
//                while (j != -1)
//                {
//                    qDebug() << "COMMENTARY!!!!!!!";
//                    eventData.commentary.insert(j+1, "\n\n");
//                    j = eventData.commentary.indexOf(QRegExp("[\\.?!][A-Z]|[\\.?! ][0-9]{1,2}:[0-9]{1,2}"), j+2);
//                }
            }


        case LTData::SYS_TIMESTAMP:
            break;
        default:
            //dd = packet.longData.toDouble();
            break;
    }
    if (emitSignal)
        emit eventDataChanged();
}

void DataStreamReader::onDecryptionKeyObtained(unsigned int key)
{    
    eventData.key = key;   

   if (key == 0)
       return;

//   QFile file(QString("packets/key.dat"));
//   if (file.open(QIODevice::WriteOnly))
//       file.write(QString::number(key).toAscii());

   qDebug() << "KEY=" << key;

    for (int i = 0; i < encryptedPackets.size(); ++i)
    {
        decrypt(encryptedPackets[i].longData);

        if(encryptedPackets[i].carID)        
            parseCarPacket(encryptedPackets[i]);

        else
            parseSystemPacket(encryptedPackets[i]);        
    }
    encryptedPackets.clear();
}

void DataStreamReader::onCookieReceived(QString cookie)
{
//    socketReader.openStream(host, port);
//    socketReader.openStream("localhost", 6666);
    socketReader.openStream("192.168.1.2", 6666);
//    eventData.key = 2976363859;
//    eventData.key = 2462388168;     //bahrain quali
//    eventData.key = 3875488254; //fp1
//    eventData.key = 2488580439; //qual
//        eventData.key = 2438680630; //race
    //socketReader.start();
    eventData.cookie = cookie;
    emit authorized(cookie);
}

void DataStreamReader::onStreamBlockObtained(const QByteArray &buf)
{
    savePacket(buf);
    if (parsing)    
    {
        qDebug() << "PARSING";
        streamData.append(buf);
    }

    else
    {
        streamData = buf;
        parseStreamBlock();
    }
}

void DataStreamReader::onKeyFrameObtained(QByteArray keyFrame)
{
//    std::cout<<"tu powinienem byc tylko raz!! parsing="<<parsing<<", size="<<keyFrame.size()<<std::endl;
    //streamData = keyFrame;
    savePacket(keyFrame);
    resetDecryption();

//    resetDecryption();
    if (parsing)
    {
        streamData.append(keyFrame);
    }
    else
    {
        encryptedPackets.clear();
        streamData = keyFrame;
        //streamData.append(keyFrame);
        parseStreamBlock();
    }
}

void DataStreamReader::savePacket(const QByteArray &buf)
{
//    QFile file(QString("packets/packet_%1.dat").arg(packetNo++));
//    if (file.open(QIODevice::WriteOnly))
//        file.write(buf);
}

void DataStreamReader::clearData()
{
//    eventData.eventInfo.laps = 0;
//    eventData.eventInfo.eventNo = 0;
//    eventData.key = 0;
//    eventData.frame = 0;
//    eventData.remainingTime = QTime();
//    eventData.lapsCompleted = 0;

//    eventData.flagStatus = LTData::GREEN_FLAG;

//    eventData.trackTemp = 0;
//    eventData.airTemp = 0;
//    eventData.windSpeed = 0;
//    eventData.humidity = 0;
//    eventData.pressure = 0;
//    eventData.windDirection = 0;
//    eventData.sessionStarted = false;

//    eventData.commentary = "";
//    eventData.driversData.clear();
    eventData.clear();
}

void DataStreamReader::parseEPPackets(const QList<Packet> &packets)
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
            eventData.eventType = (LTData::EventType)packet.data;
//            eventData.eventInfo = LTData::getEvent(QDate::currentDate());
            qDebug() << "EVENT TYPE=" << eventData.eventType;
            eventData.lapsCompleted = 0;
        }
    }
    emit dataChanged();
}
