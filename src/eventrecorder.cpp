#include "eventrecorder.h"
#include <QDataStream>
#include <QFile>


EventRecorder::EventRecorder(QObject *parent) :
    QObject(parent), eventData(EventData::getInstance()), elapsedSeconds(0)
{
}

void EventRecorder::startRecording()
{
    //prepare everyting for record, clear old records and store the LTTeam and LTEvent data
    ltTeamList = LTData::ltTeams;
    ltEvent = eventData.eventInfo;

    packets.clear();
    elapsedSeconds = 0;

    gatherInitialData();    

//    eventDataList.clear();
//    lapDataList.clear();

//    for (int i = 0; i < ltTeamList.size(); ++i)
//    {
//        //we've got 2 drivers in every team so we store the place to hold lap datas
//        lapDataList.append(QList<LapData>());
//        lapDataList.append(QList<LapData>());
//    }
}

/*Before start of the recording, we have to gather all initial data, that will be needed to correctly startup replaying the event. This includes:
  - event type
  - track status
  - remaining time
  - car position update and history, driver numbers and names
  - weather data
  - speed records
  - fastest lap data
  - commentary
  */
void EventRecorder::gatherInitialData()
{
    Packet packet;

    gatherSysData();
    gatherDriverData();

    //commentary
    packet.carID = 0;
    packet.type = LTData::SYS_COMMENTARY;
    packet.data = 0;
    packet.longData.append("  ");   //append 2 empty bytes
    packet.longData.append(eventData.commentary);
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));

    ++elapsedSeconds;
}

void EventRecorder::gatherSysData()
{
    Packet packet;

    //event type
    packet.carID = 0;
    packet.type = LTData::SYS_EVENT_ID;
    packet.data = eventData.eventType;
    packet.length = 0;      //we don't need a number for the decryption key
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //track status
    packet.carID = 0;
    packet.type = LTData::SYS_TRACK_STATUS;
    packet.data = 1;
    packet.longData.append((char)(eventData.flagStatus) + '0');
    packet.length = 1;
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //remaining time
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_SESSION_CLOCK;
    packet.longData.append(eventData.remainingTime.toString("h:mm:ss"));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();


    //weather data
    //track temp
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_TRACK_TEMP;
    packet.longData.append(QString::number(eventData.trackTemp));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //air temp
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_AIR_TEMP;
    packet.longData.append(QString::number(eventData.airTemp));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //wind speed
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_WIND_SPEED;
    packet.longData.append(QString::number(eventData.windSpeed));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //humidity
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_HUMIDITY;
    packet.longData.append(QString::number(eventData.humidity));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //pressure
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_PRESSURE;
    packet.longData.append(QString::number(eventData.pressure));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //wind dir.
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_WIND_DIRECTION;
    packet.longData.append(QString::number(eventData.windDirection));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //wet track
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_WET_TRACK;
    packet.longData.append(QString::number(eventData.wetdry));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //speed records
    //sector 1
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::SPEED_SECTOR1));
    for (int i = 0; i < 12; ++i)
    {
        QString name = LTData::getDriverShortName(eventData.sec1Speed[i++]);
        QString speed = eventData.sec1Speed[i];
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //sector 2
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::SPEED_SECTOR2));
    for (int i = 0; i < 12; ++i)
    {
        QString name = LTData::getDriverShortName(eventData.sec2Speed[i++]);
        QString speed = eventData.sec2Speed[i];
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //sector 3
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::SPEED_SECTOR3));
    for (int i = 0; i < 12; ++i)
    {
        QString name = LTData::getDriverShortName(eventData.sec3Speed[i++]);
        QString speed = eventData.sec3Speed[i];
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //speed trap
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::SPEED_TRAP));
    for (int i = 0; i < 12; ++i)
    {
        QString name = LTData::getDriverShortName(eventData.speedTrap[i++]);
        QString speed = eventData.speedTrap[i];
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //fastest lap data
    //FL_CAR
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::FL_CAR));
    packet.longData.append(QString::number(eventData.FLNumber));
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //FL_DRIVER
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::FL_DRIVER));
    packet.longData.append(eventData.FLDriver.toUpper());
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //FL_LAP
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::FL_LAP));
    packet.longData.append(QString::number(eventData.FLLap));
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //FL_TIME
    packet.carID = 0;
    packet.type = LTData::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTData::FL_LAP));
    packet.longData.append(eventData.FLTime);
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();
}

void EventRecorder::gatherDriverData()
{
    Packet packet;
    //car position update
    for (int i = 0; i < eventData.driversData.size(); ++i)
    {
        packet.type = LTData::CAR_POSITION_UPDATE;
        packet.carID = eventData.driversData[i].carID;
        packet.length = 0;
        packet.data = eventData.driversData[i].pos;
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();
    }

    //car position history
    for (int i = 0; i < eventData.driversData.size(); ++i)
    {
        packet.type = LTData::CAR_POSITION_HISTORY;
        packet.carID = eventData.driversData[i].carID;
        packet.data = 0;
        packet.length = eventData.driversData[i].posHistory.size();

        for (int j = 0; j < packet.length; ++j)
            packet.longData.append((char)(eventData.driversData[i].posHistory[j]));

        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();
    }

    //other car data
    for (int i = 0; i < eventData.driversData.size(); ++i)
    {
        //position
        packet.type = LTData::RACE_POSITION;
        packet.carID = eventData.driversData[i].carID;
        packet.data = eventData.driversData[i].colorData[LTData::RACE_POSITION];
        packet.longData.append(QString::number(eventData.driversData[i].pos));
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();


        //number
        packet.type = LTData::RACE_NUMBER;
        packet.carID = eventData.driversData[i].carID;
        packet.data = eventData.driversData[i].colorData[LTData::RACE_NUMBER];
        packet.longData.append(QString::number(eventData.driversData[i].number));
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();

        //car driver
        packet.type = LTData::RACE_DRIVER;
        packet.carID = eventData.driversData[i].carID;
        packet.data = eventData.driversData[i].colorData[LTData::RACE_DRIVER];
        packet.longData.append(eventData.driversData[i].driver.toUpper());
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();

        if (eventData.eventType == LTData::RACE_EVENT)
        {
            //race gap
            packet.type = LTData::RACE_GAP;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_GAP];
            packet.longData.append(eventData.driversData[i].lastLap.gap);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race gap
            packet.type = LTData::RACE_INTERVAL;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_INTERVAL];
            packet.longData.append(eventData.driversData[i].lastLap.interval);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race lap time
            packet.type = LTData::RACE_LAP_TIME;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_LAP_TIME];
            packet.longData.append(eventData.driversData[i].lastLap.lapTime.toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race sector 1
            packet.type = LTData::RACE_SECTOR_1;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_SECTOR_1];
            packet.longData.append(eventData.driversData[i].lastLap.sector1);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race sector 2
            packet.type = LTData::RACE_SECTOR_2;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_SECTOR_2];
            packet.longData.append(eventData.driversData[i].lastLap.sector2);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race sector 3
            packet.type = LTData::RACE_SECTOR_3;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_SECTOR_3];
            packet.longData.append(eventData.driversData[i].lastLap.sector3);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race num pits
            packet.type = LTData::RACE_NUM_PITS;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::RACE_NUM_PITS];
            packet.longData.append(QString::number(eventData.driversData[i].numPits));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();
        }
        else if (eventData.eventType == LTData::QUALI_EVENT)
        {
            //quali 1
            packet.type = LTData::QUALI_PERIOD_1;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_PERIOD_1];
            packet.longData.append(eventData.driversData[i].q1.toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali 2
            packet.type = LTData::QUALI_PERIOD_2;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_PERIOD_2];
            packet.longData.append(eventData.driversData[i].q2.toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali 3
            packet.type = LTData::QUALI_PERIOD_3;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_PERIOD_3];
            packet.longData.append(eventData.driversData[i].q3.toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali sector 1
            packet.type = LTData::QUALI_SECTOR_1;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_SECTOR_1];
            packet.longData.append(eventData.driversData[i].lastLap.sector1);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali sector 2
            packet.type = LTData::QUALI_SECTOR_2;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_SECTOR_2];
            packet.longData.append(eventData.driversData[i].lastLap.sector2);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali sector 3
            packet.type = LTData::QUALI_SECTOR_3;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_SECTOR_3];
            packet.longData.append(eventData.driversData[i].lastLap.sector3);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali lap
            packet.type = LTData::QUALI_LAP;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::QUALI_LAP];
            packet.longData.append(QString::number(eventData.driversData[i].lastLap.numLap));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();
        }
        else if (eventData.eventType == LTData::PRACTICE_EVENT)
        {
            //practice best
            packet.type = LTData::PRACTICE_BEST;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::PRACTICE_BEST];
            packet.longData.append(eventData.driversData[i].lastLap.lapTime.toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice gap
            packet.type = LTData::PRACTICE_GAP;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::PRACTICE_GAP];
            packet.longData.append(eventData.driversData[i].lastLap.gap);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice sector 1
            packet.type = LTData::PRACTICE_SECTOR_1;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::PRACTICE_SECTOR_1];
            packet.longData.append(eventData.driversData[i].lastLap.sector1);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice sector 2
            packet.type = LTData::PRACTICE_SECTOR_2;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::PRACTICE_SECTOR_2];
            packet.longData.append(eventData.driversData[i].lastLap.sector2);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice sector 3
            packet.type = LTData::PRACTICE_SECTOR_3;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::PRACTICE_SECTOR_3];
            packet.longData.append(eventData.driversData[i].lastLap.sector3);
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice lap
            packet.type = LTData::PRACTICE_LAP;
            packet.carID = eventData.driversData[i].carID;
            packet.data = eventData.driversData[i].colorData[LTData::PRACTICE_LAP];
            packet.longData.append(QString::number(eventData.driversData[i].lastLap.numLap));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();
        }
    }
}

void EventRecorder::stopRecording()
{
    saveToFile("");
    //store the last eventData
//    eventDataList.append(eventData);
}

void EventRecorder::appendPacket(const Packet &p)
{
    packets.append(QPair<int, Packet>(elapsedSeconds, p));
}

//void EventRecorder::updateEventData(const EventData &ed)
//{
//    //store the EventData object but without its driverData list - we've got here our own list
//    QList<DriverData> tmpList = eventData.driversData;
//    eventData = ed;
//    eventData.driversData = tmpList;
//}

//void EventRecorder::updateDriverData(const DriverData &dd)
//{
//    //check if have this driver in the list, if yes - update it, if no - append it
//    for (int i = 0; i < eventData.driversData.size(); ++i)
//    {
//        if (dd.carID == eventData.driversData[i].carID)
//        {
//            eventData.driversData[i] = dd;
//            storeLapData(dd);
//            eventData.driversData[i].lapData.clear();
//            return;
//        }
//    }
//    eventData.driversData.append(dd);
//    storeLapData(dd);
//    eventData.driversData.last().lapData.clear();
//}

//void EventRecorder::storeLapData(const DriverData &dd)
//{
//    if (dd.carID == -1)
//        return;

//    //ok, we have to copy all driver laps, but without the ones we already have, we only update the last lap

//    //if the current driver lap data list is empty, all his laps are appended
//    bool startAppending = lapDataList[dd.carID-1].isEmpty() ? true : false;
//    for (int i = 0; i < dd.lapData.size(); ++i)
//    {
//        if (startAppending)
//            lapDataList[dd.carID-1].append(dd.lapData[i]);

//        //updating the last lap
//        if (!lapDataList[dd.carID-1].isEmpty() && dd.lapData[i].numLap == lapDataList[dd.carID-1].last().numLap)
//        {
//            lapDataList[dd.carID-1].last() = dd.lapData[i];
//            //from now on we can append the remaning laps
//            startAppending = true;
//        }
//    }
//}

void EventRecorder::timeout()
{
    ++elapsedSeconds;
    //append the system clock packet every second
    Packet packet;
    packet.carID = 0;
    packet.type = LTData::SYS_WEATHER;
    packet.data = LTData::WEATHER_SESSION_CLOCK;
    packet.longData.append(eventData.remainingTime.toString("h:mm:ss"));
    packet.length = packet.longData.size();

    packets.append(QPair<int, Packet>(elapsedSeconds, packet));

    //on every timeout the eventData object is appended into eventDataList and the driverData list is cleared
//    eventDataList.append(eventData);
//    eventData.driversData.clear();
}

void EventRecorder::saveToFile(QString)
{
    if (packets.size() > 0)
    {
        int year = eventData.eventInfo.raceDate.year();
        int no = eventData.eventInfo.eventNo;
        QString shortName = eventData.eventInfo.eventShortName.toLower();

        QString session;
        switch (eventData.eventType)
        {
            case LTData::RACE_EVENT:
                session = "race"; break;

            case LTData::QUALI_EVENT:
                session = "quali"; break;

            default:
                session = "fp1"; break;
        }
        QString sNo = QString::number(no);
        if (no < 10)
            sNo = "0" + QString::number(no);

        QString fName = QString("ltdata/%1-%2-%3-%4.lt").arg(year).arg(sNo).arg(shortName).arg(session);

        //since we have 3 practice session we have to choose the correct session number
        if (session == "fp1" && QFile::exists(fName))
        {
            fName = QString("ltdata/%1-%2-%3-%4.lt").arg(year).arg(sNo).arg(shortName).arg("fp2");

            if (QFile::exists(fName))
                fName = QString("ltdata/%1-%2-%3-%4.lt").arg(year).arg(sNo).arg(shortName).arg("fp3");
        }

        QFile f(fName);
        if (f.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&f);

            const char tab[5] = "F1LT";

            stream << tab;
            //save the teams and events info
            saveLTInfo(stream);
            savePackets(stream);

            //now save events
//            saveEventData(stream);

//            saveLapData(stream);
        }
    }
}

void EventRecorder::saveLTInfo(QDataStream &stream)
{
    int size;// = ltEventList.size();
//    stream << size;

    QString sbuf;
    int ibuf;
    QPixmap pixBuf;
//    for (int i = 0; i < size; ++i)
    {
        ibuf = ltEvent.eventNo;
        stream << ibuf;

        sbuf = ltEvent.eventName;
        stream << sbuf;

        sbuf = ltEvent.eventShortName;
        stream << sbuf;

        sbuf = ltEvent.eventPlace;
        stream << sbuf;

        sbuf = ltEvent.fpDate.toString("dd-MM-yyyy");
        stream << sbuf;

        sbuf = ltEvent.raceDate.toString("dd-MM-yyyy");
        stream << sbuf;

        ibuf = ltEvent.laps;
        stream << ibuf;

        pixBuf = ltEvent.trackImg;
        stream << pixBuf;
    }

    size = ltTeamList.size();
    stream << size;
    for (int i = 0; i < size; ++i)
    {
        sbuf = ltTeamList[i].teamName;
        stream << sbuf;

        sbuf = ltTeamList[i].driver1Name;
        stream << sbuf;

        sbuf = ltTeamList[i].driver1ShortName;
        stream << sbuf;

        ibuf = ltTeamList[i].driver1No;
        stream << ibuf;

        sbuf = ltTeamList[i].driver2Name;
        stream << sbuf;

        sbuf = ltTeamList[i].driver2ShortName;
        stream << sbuf;

        ibuf = ltTeamList[i].driver2No;
        stream << ibuf;

        pixBuf = ltTeamList[i].carImg;
        stream << pixBuf;
    }
}

void EventRecorder::savePackets(QDataStream &stream)
{
    stream << packets.size();

    for (int i = 0; i < packets.size(); ++i)
    {
        stream << packets[i].first;
        stream << packets[i].second.type;
        stream << packets[i].second.carID;
        stream << packets[i].second.data;
        stream << packets[i].second.length;
        stream << packets[i].second.longData;
    }
}

//void EventRecorder::saveEventData(QDataStream &stream)
//{
//    int size = eventDataList.size();
//    stream << size;

//    QString sbuf;
//    int ibuf;
//    double dbuf;
//    for (int i = 0; i < size; ++i)
//    {
////        ibuf = eventDataList[i].key;
//        stream << ibuf;

////        sbuf = eventDataList[i].cookie;
//        stream << sbuf;

////        ibuf = eventDataList[i].frame;
//        stream << ibuf;

//        ibuf = eventDataList[i].eventNo;
//        stream << ibuf;

////        ibuf = eventDataList[i].salt;
//        stream << ibuf;

//        ibuf = eventDataList[i].eventType;
//        stream << ibuf;

//        ibuf = eventDataList[i].flagStatus;
//        stream << ibuf;

//        sbuf = eventDataList[i].remainingTime.toString("h:mm:ss");
//        stream << sbuf;

//        ibuf = eventDataList[i].lapsCompleted;
//        stream << ibuf;

//        ibuf = eventDataList[i].totalLaps;
//        stream << ibuf;

//        dbuf = eventDataList[i].airTemp;
//        stream << dbuf;

//        dbuf = eventDataList[i].humidity;
//        stream << dbuf;

//        dbuf = eventDataList[i].windSpeed;
//        stream << dbuf;

//        dbuf = eventDataList[i].windDirection;
//        stream << dbuf;

//        dbuf = eventDataList[i].pressure;
//        stream << dbuf;

//        dbuf = eventDataList[i].trackTemp;
//        stream << dbuf;

//        ibuf = eventDataList[i].wetdry;
//        stream << ibuf;

//        sbuf = eventDataList[i].commentary;
//        stream << sbuf;

//        for (int j = 0; j < 12; ++j)
//        {
//            sbuf = eventDataList[i].sec1Speed[j];
//            stream << sbuf;

//            sbuf = eventDataList[i].sec2Speed[j];
//            stream << sbuf;

//            sbuf = eventDataList[i].sec3Speed[j];
//            stream << sbuf;

//            sbuf = eventDataList[i].speedTrap[j];
//            stream << sbuf;
//        }
//        ibuf = eventDataList[i].FLNumber;
//        stream << ibuf;

//        sbuf = eventDataList[i].FLDriver;
//        stream << sbuf;

//        ibuf = eventDataList[i].FLLap;
//        stream << ibuf;

//        sbuf = eventDataList[i].FLTime.toString();
//        stream << sbuf;

//        for (int j = 0; j < 3; ++j)
//        {
//            sbuf = eventDataList[i].sec1Record[j];
//            stream << sbuf;

//            sbuf = eventDataList[i].sec2Record[j];
//            stream << sbuf;

//            sbuf = eventDataList[i].sec3Record[j];
//            stream << sbuf;
//        }

//        ibuf = eventDataList[i].driversData.size();
//        stream << ibuf;

//        for (int j = 0; j < eventDataList[i].driversData.size(); ++j)
//        {
//            ibuf = eventDataList[i].driversData[j].carID; stream << ibuf;
//            sbuf = eventDataList[i].driversData[j].driver; stream << sbuf;
//            ibuf = eventDataList[i].driversData[j].number; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].pos; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].numPits; stream << ibuf;
////            ibuf = eventDataList[i].driversData[j].numPits; stream << ibuf;   //wczytac to 2. raz!
//            ibuf = eventDataList[i].driversData[j].retired; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].releasedFromPits; stream << ibuf;
//            sbuf = eventDataList[i].driversData[j].q1.toString(); stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].q2.toString(); stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].q3.toString(); stream << sbuf;

//            ibuf = eventDataList[i].driversData[j].posHistory.size();
//            stream << ibuf;

//            for (int k = 0; k < eventDataList[i].driversData[j].posHistory.size(); ++k)
//            {
//                ibuf = eventDataList[i].driversData[j].posHistory[k];
//                stream << ibuf;
//            }
//            for (int k = 0; k < 14; ++k)
//            {
//                ibuf = eventDataList[i].driversData[j].colorData[k];
//                stream << ibuf;
//            }

//            ibuf = eventDataList[i].driversData[j].pitData.size();
//            stream << ibuf;

//            for (int k = 0; k < eventDataList[i].driversData[j].pitData.size(); ++k)
//            {
//                sbuf = eventDataList[i].driversData[j].pitData[k].pitTime; stream << sbuf;
//                ibuf = eventDataList[i].driversData[j].pitData[k].pitLap; stream << ibuf;
//            }

//            //lastLap & bestLap
//            ibuf = eventDataList[i].driversData[j].lastLap.carID; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].bestLap.carID; stream << ibuf;

////            ibuf = eventDataList[i].driversData[j].lastLap.number; stream << ibuf;
////            ibuf = eventDataList[i].driversData[j].bestLap.number; stream << ibuf;

//            ibuf = eventDataList[i].driversData[j].lastLap.pos; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].bestLap.pos; stream << ibuf;

//            sbuf = eventDataList[i].driversData[j].lastLap.gap; stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].bestLap.gap; stream << sbuf;

//            sbuf = eventDataList[i].driversData[j].lastLap.interval; stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].bestLap.interval; stream << sbuf;

//            sbuf = eventDataList[i].driversData[j].lastLap.lapTime.toString(); stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].bestLap.lapTime.toString(); stream << sbuf;

//            sbuf = eventDataList[i].driversData[j].lastLap.sector1.toString(); stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].bestLap.sector1.toString(); stream << sbuf;

//            sbuf = eventDataList[i].driversData[j].lastLap.sector2.toString(); stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].bestLap.sector2.toString(); stream << sbuf;

//            sbuf = eventDataList[i].driversData[j].lastLap.sector3.toString(); stream << sbuf;
//            sbuf = eventDataList[i].driversData[j].bestLap.sector3.toString(); stream << sbuf;

//            ibuf = eventDataList[i].driversData[j].lastLap.numLap; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].bestLap.numLap; stream << ibuf;

//            ibuf = eventDataList[i].driversData[j].lastLap.scLap; stream << ibuf;
//            ibuf = eventDataList[i].driversData[j].bestLap.scLap; stream << ibuf;
//        }
//    }
//}

//void EventRecorder::saveLapData(QDataStream &stream)
//{
//    int size = lapDataList.size();
//    stream << size;

//    QString sbuf;
//    int ibuf;
//    double dbuf;

//    for (int i = 0; i < size; ++i)
//    {
//        ibuf = lapDataList[i].size();
//        stream << ibuf;

//        for (int j = 0; j < lapDataList[i].size(); ++j)
//        {
//            ibuf = lapDataList[i][j].carID; stream << ibuf;

////            ibuf = lapDataList[i][j].number; stream << ibuf;

//            ibuf = lapDataList[i][j].pos; stream << ibuf;

//            sbuf = lapDataList[i][j].gap; stream << sbuf;

//            sbuf = lapDataList[i][j].interval; stream << sbuf;

//            sbuf = lapDataList[i][j].lapTime.toString(); stream << sbuf;

//            sbuf = lapDataList[i][j].sector1.toString(); stream << sbuf;

//            sbuf = lapDataList[i][j].sector2.toString(); stream << sbuf;

//            sbuf = lapDataList[i][j].sector3.toString(); stream << sbuf;

//            ibuf = lapDataList[i][j].numLap; stream << ibuf;

//            ibuf = lapDataList[i][j].scLap; stream << ibuf;
//        }
//    }
//}

