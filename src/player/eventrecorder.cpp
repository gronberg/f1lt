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


#include "eventrecorder.h"
#include "ltfilesloader.h"
#include <QDataStream>
#include <QFile>


EventRecorder::EventRecorder(SessionTimer *st, QObject *parent) :
    QObject(parent), sessionTimer(st), eventData(EventData::getInstance()), elapsedSeconds(0), sessionRecorded(false), recordStartTime(0), autoSaveRecord(-1), autoSaveCounter(-1)
{
}

void EventRecorder::startRecording(int delay)
{    
    sessionRecorded = false;
    recordStartTime = QDateTime::currentMSecsSinceEpoch() - delay * 1000;

    //prepare everyting for record, clear old records and store the LTTeam and LTEvent data
    ltTeamList = SeasonData::getInstance().getTeams();
    ltEvent = eventData.getEventInfo();

    packets.clear();
    elapsedSeconds = 0;
    lastSavedTime.first = 0;
    lastSavedTime.second = eventData.getRemainingTime();
    elapsedTimeToStop = -1;

    autoSaveCounter = autoSaveRecord * 60;

    gatherInitialData();
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
    packet.type = LTPackets::SYS_COMMENTARY;
    packet.data = 0;
    packet.longData.append("  ");   //append 2 empty bytes
    packet.longData.append(eventData.getCommentary());
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));

    ++elapsedSeconds;
}

void EventRecorder::gatherSysData()
{
    Packet packet;

    //event type
    packet.carID = 0;
    packet.type = LTPackets::SYS_EVENT_ID;
    packet.data = eventData.getEventType();
    packet.length = 0;      //we don't need a number for the decryption key
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //track status
    packet.carID = 0;
    packet.type = LTPackets::SYS_TRACK_STATUS;
    packet.data = 1;
    packet.longData.append((char)(eventData.getFlagStatus()) + '0');
    packet.length = 1;
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //remaining time
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_SESSION_CLOCK;
    packet.longData.append(eventData.getRemainingTime().toString("h:mm:ss"));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();


    //weather data
    //track temp
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_TRACK_TEMP;
    packet.longData.append(QString::number(eventData.getWeather().getTrackTemp().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //air temp
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_AIR_TEMP;
    packet.longData.append(QString::number(eventData.getWeather().getAirTemp().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //wind speed
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_WIND_SPEED;
    packet.longData.append(QString::number(eventData.getWeather().getWindSpeed().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //humidity
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_HUMIDITY;
    packet.longData.append(QString::number(eventData.getWeather().getHumidity().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //pressure
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_PRESSURE;
    packet.longData.append(QString::number(eventData.getWeather().getPressure().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //wind dir.
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_WIND_DIRECTION;
    packet.longData.append(QString::number(eventData.getWeather().getWindDirection().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //wet track
    packet.carID = 0;
    packet.type = LTPackets::SYS_WEATHER;
    packet.data = LTPackets::WEATHER_WET_TRACK;
    packet.longData.append(QString::number(eventData.getWeather().getWetDry().getValue()));
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //speed records
    //sector 1
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::SPEED_SECTOR1));
    for (int i = 0; i < 6; ++i)
    {
        QString name = SeasonData::getInstance().getDriverShortName(eventData.getSessionRecords().getSectorSpeed(1, i).getDriverName());
        QString speed = QString::number(eventData.getSessionRecords().getSectorSpeed(1, i).getSpeed());
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //sector 2
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::SPEED_SECTOR2));
    for (int i = 0; i < 6; ++i)
    {
        QString name = SeasonData::getInstance().getDriverShortName(eventData.getSessionRecords().getSectorSpeed(2, i).getDriverName());
        QString speed = QString::number(eventData.getSessionRecords().getSectorSpeed(2, i).getSpeed());
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //sector 3
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::SPEED_SECTOR3));
    for (int i = 0; i < 6; ++i)
    {
        QString name = SeasonData::getInstance().getDriverShortName(eventData.getSessionRecords().getSectorSpeed(3, i).getDriverName());
        QString speed = QString::number(eventData.getSessionRecords().getSectorSpeed(3, i).getSpeed());
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //speed trap
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::SPEED_TRAP));
    for (int i = 0; i < 6; ++i)
    {
        QString name = SeasonData::getInstance().getDriverShortName(eventData.getSessionRecords().getSpeedTrap(i).getDriverName());
        QString speed = QString::number(eventData.getSessionRecords().getSpeedTrap(i).getSpeed());
        packet.longData.append(name + speed);
    }
    packet.length = packet.longData.size();
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //fastest lap data
    //FL_CAR
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::FL_CAR));
    packet.longData.append(QString::number(eventData.getSessionRecords().getFastestLap().getNumber()));
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //FL_DRIVER
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::FL_DRIVER));
    packet.longData.append(eventData.getSessionRecords().getFastestLap().getDriverName().toUpper());
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //FL_LAP
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::FL_LAP));
    packet.longData.append(QString::number(eventData.getSessionRecords().getFastestLap().getLapNumber()));
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();

    //FL_TIME
    packet.carID = 0;
    packet.type = LTPackets::SYS_SPEED;
    packet.data = 0;
    packet.longData.append(QString::number(LTPackets::FL_LAP));
    packet.longData.append(eventData.getSessionRecords().getFastestLap().getTime());
    packets.append(QPair<int, Packet>(elapsedSeconds, packet));
    packet.longData.clear();
}

void EventRecorder::gatherDriverData()
{
    Packet packet;
    //car position update
    for (int i = 0; i < eventData.getDriversData().size(); ++i)
    {
        packet.type = LTPackets::CAR_POSITION_UPDATE;
        packet.carID = eventData.getDriversData()[i].getCarID();
        packet.length = 0;
        packet.data = eventData.getDriversData()[i].getPosition();
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();
    }

    //car position history
    for (int i = 0; i < eventData.getDriversData().size(); ++i)
    {
        packet.type = LTPackets::CAR_POSITION_HISTORY;
        packet.carID = eventData.getDriversData()[i].getCarID();
        packet.data = 0;
        packet.length = eventData.getDriversData()[i].getPositionHistory().size();

        for (int j = 0; j < packet.length; ++j)
            packet.longData.append((char)(eventData.getDriversData()[i].getPositionHistory()[j]));

        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();
    }

    //other car data
    for (int i = 0; i < eventData.getDriversData().size(); ++i)
    {
        //position
        packet.type = LTPackets::RACE_POSITION;
        packet.carID = eventData.getDriversData()[i].getCarID();
        packet.data = eventData.getDriversData()[i].getColorData().positionColor();
        packet.longData.append(QString::number(eventData.getDriversData()[i].getPosition()));
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();


        //number
        packet.type = LTPackets::RACE_NUMBER;
        packet.carID = eventData.getDriversData()[i].getCarID();
        packet.data = eventData.getDriversData()[i].getColorData().numberColor();
        packet.longData.append(QString::number(eventData.getDriversData()[i].getNumber()));
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();

        //car driver
        packet.type = LTPackets::RACE_DRIVER;
        packet.carID = eventData.getDriversData()[i].getCarID();
        packet.data = eventData.getDriversData()[i].getColorData().driverColor();
        packet.longData.append(eventData.getDriversData()[i].getDriverName().toUpper());
        packets.append(QPair<int, Packet>(elapsedSeconds, packet));
        packet.longData.clear();

        if (eventData.getEventType() == LTPackets::RACE_EVENT)
        {
            //race gap
            packet.type = LTPackets::RACE_GAP;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().gapColor();
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getGap());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race gap
            packet.type = LTPackets::RACE_INTERVAL;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().intervalColor();
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getInterval());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race lap time
            packet.type = LTPackets::RACE_LAP_TIME;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().lapTimeColor();
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getTime().toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race sector 1
            packet.type = LTPackets::RACE_SECTOR_1;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(1);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(1));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race sector 2
            packet.type = LTPackets::RACE_SECTOR_2;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(2);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(2));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race sector 3
            packet.type = LTPackets::RACE_SECTOR_3;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(3);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(3));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //race num pits
            packet.type = LTPackets::RACE_NUM_PITS;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().numPitsColor();
            packet.longData.append(QString::number(eventData.getDriversData()[i].getNumPits()));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();
        }
        else if (eventData.getEventType() == LTPackets::QUALI_EVENT)
        {
            //quali 1
            packet.type = LTPackets::QUALI_PERIOD_1;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().qualiTimeColor(1);
            packet.longData.append(eventData.getDriversData()[i].getQualiTime(1).toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali 2
            packet.type = LTPackets::QUALI_PERIOD_2;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().qualiTimeColor(2);
            packet.longData.append(eventData.getDriversData()[i].getQualiTime(2).toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali 3
            packet.type = LTPackets::QUALI_PERIOD_3;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().qualiTimeColor(3);
            packet.longData.append(eventData.getDriversData()[i].getQualiTime(3).toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali sector 1
            packet.type = LTPackets::QUALI_SECTOR_1;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(1);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(1));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali sector 2
            packet.type = LTPackets::QUALI_SECTOR_2;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(2);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(2));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali sector 3
            packet.type = LTPackets::QUALI_SECTOR_3;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(3);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(3));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //quali lap
            packet.type = LTPackets::QUALI_LAP;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().numLapsColor();
            packet.longData.append(QString::number(eventData.getDriversData()[i].getLastLap().getLapNumber()));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();
        }
        else if (eventData.getEventType() == LTPackets::PRACTICE_EVENT)
        {
            //practice best
            packet.type = LTPackets::PRACTICE_BEST;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().lapTimeColor();
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getTime().toString());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice gap
            packet.type = LTPackets::PRACTICE_GAP;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().gapColor();
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getGap());
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice sector 1
            packet.type = LTPackets::PRACTICE_SECTOR_1;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(1);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(1));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice sector 2
            packet.type = LTPackets::PRACTICE_SECTOR_2;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(2);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(2));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice sector 3
            packet.type = LTPackets::PRACTICE_SECTOR_3;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().sectorColor(3);
            packet.longData.append(eventData.getDriversData()[i].getLastLap().getSectorTime(3));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();

            //practice lap
            packet.type = LTPackets::PRACTICE_LAP;
            packet.carID = eventData.getDriversData()[i].getCarID();
            packet.data = eventData.getDriversData()[i].getColorData().numLapsColor();
            packet.longData.append(QString::number(eventData.getDriversData()[i].getLastLap().getLapNumber()));
            packets.append(QPair<int, Packet>(elapsedSeconds, packet));
            packet.longData.clear();
        }
    }
}

void EventRecorder::stopRecording()
{
    sessionRecorded = true;
    saveToFile("");
}

void EventRecorder::appendPacket(const Packet &p)
{
    packets.append(QPair<int, Packet>(elapsedSeconds, p));    
}

void EventRecorder::appendPacket(const QPair<Packet, qint64> &packet)
{
    elapsedSeconds = round((double(packet.second) - double(recordStartTime)) / 1000.0);    

    if (elapsedSeconds < 0)
    {        
        recordStartTime += elapsedSeconds * 1000;
        elapsedSeconds = 0;
    }

    appendSessionTimer();

    packets.append(QPair<int, Packet>(elapsedSeconds, packet.first));    

    if (packet.first.type == LTPackets::SYS_WEATHER && packet.first.data == LTPackets::WEATHER_SESSION_CLOCK)
    {
        lastSavedTime.first = elapsedSeconds;
        lastSavedTime.second = eventData.getRemainingTime();
    }    
}

void EventRecorder::appendSessionTimer()
{
    if (eventData.getRemainingTime().isNull())
        return;

    int secs = lastSavedTime.second.second();
    int mins = lastSavedTime.second.minute();
    int hours = lastSavedTime.second.hour();

    for (int i = lastSavedTime.first + 1; i <= elapsedSeconds; ++i)
    {
        --secs;
        if (secs < 0)
        {
            --mins;
            secs = 60 + secs;

            if (mins < 0)
            {
                --hours;
                mins = 60 + mins;

                if (hours < 0)
                    hours = mins = secs = 0;

            }
        }

        Packet packet;
        packet.carID = 0;
        packet.type = LTPackets::SYS_WEATHER;
        packet.data = LTPackets::WEATHER_SESSION_CLOCK;
        lastSavedTime.second  = QTime(hours, mins, secs);
        packet.longData.append(lastSavedTime.second.toString("h:mm:ss"));
        packet.length = packet.longData.size();

        packets.append(QPair<int, Packet>(i, packet));        

        lastSavedTime.first = i;
    }
}

void EventRecorder::timeout()
{
    if (sessionTimer->isSynchronizing())
        return;

    if (autoStopRecord >= 0)
    {
        if((eventData.getRemainingTime().toString("h:mm:ss") == "0:00:00" && eventData.getEventType() == LTPackets::PRACTICE_EVENT) ||
            (eventData.getRemainingTime().toString("h:mm:ss") == "0:00:00" && eventData.getEventType() == LTPackets::QUALI_EVENT && eventData.getQualiPeriod() == 3) ||
            ((eventData.getRemainingTime().toString("h:mm:ss") == "0:00:00" || eventData.getCompletedLaps() == eventData.getEventInfo().laps) && eventData.getEventType() == LTPackets::RACE_EVENT))
    		++elapsedTimeToStop;

		if (elapsedTimeToStop >= (autoStopRecord * 60))
		{
			emit recordingStopped();
			stopRecording();
		}
    }

    if (autoSaveRecord > -1)
    {
        --autoSaveCounter;

        if (autoSaveCounter <= 0)
        {
            saveToFile("");
            autoSaveCounter = autoSaveRecord * 60;
        }
    }
}

void EventRecorder::saveToFile(QString)
{
    if (packets.size() > 0)
    {
        int year = eventData.getEventInfo().raceDate.year();
        int no = eventData.getEventInfo().eventNo;
        QString shortName = eventData.getEventInfo().eventShortName.toLower();

        QString session;
        switch (eventData.getEventType())
        {
            case LTPackets::RACE_EVENT:
                session = "race"; break;

            case LTPackets::QUALI_EVENT:
                session = "quali"; break;

            case LTPackets::PRACTICE_EVENT:
                session = "fp" + QString::number(EventData::getInstance().getFPNumber()); break;

            default: break;
        }
        QString sNo = QString::number(no);
        if (no < 10)
            sNo = "0" + QString::number(no);

        QString fName = QString("%1-%2-%3-%4.lt").arg(year).arg(sNo).arg(shortName).arg(session);

        //since we have 3 practice session we have to choose the correct session number
        if (session == "fp1" && QFile::exists(fName))
        {
            fName = QString("%1-%2-%3-%4.lt").arg(year).arg(sNo).arg(shortName).arg("fp2");

            if (QFile::exists(fName))
                fName = QString("%1-%2-%3-%4.lt").arg(year).arg(sNo).arg(shortName).arg("fp3");
        }
        QDir dir(F1LTCore::ltDataHomeDir());
        if (!dir.exists())
            dir.mkpath(F1LTCore::ltDataHomeDir());

        LTFilesLoader loader;
        loader.saveFile(F1LTCore::ltDataHomeDir()+fName, packets.toVector());
    }
}

