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



#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "driverdata.h"

class DataStreamReader;
class EventData;
class PacketParser;
class SessionRecords;
class Weather;

/*!
 * \brief The WeatherData class stores single value of weather data, including informations when this value was stored (lap, session time).
 */
class WeatherData
{
public:
    WeatherData() : value(0.0), lap(0), sessionTime(), qPeriod(1) { }

    double getValue() const { return value; }
    int getLap() const { return lap; }
    const QTime &getSessionTime() const { return sessionTime; }
    int getQualiPeriod() const { return qPeriod; }

    friend class Weather;
    friend class EventData;
    friend class PackerParser;

private:
    double value;
    int lap;
    QTime sessionTime;
    int qPeriod;
};

/*!
 * \brief The Weather class stores all weather data that is gathered during a session, including air and track temperatures, wind speed and direction, pressure, humidity and wether the track is dry or wet.
 */
class Weather
{
public:

    const WeatherData &getAirTemp()        const { return currentWeather[0]; }
    const WeatherData &getTrackTemp()      const { return currentWeather[1]; }
    const WeatherData &getWindSpeed()      const { return currentWeather[2]; }
    const WeatherData &getPressure()       const { return currentWeather[3]; }
    const WeatherData &getHumidity()       const { return currentWeather[4]; }
    const WeatherData &getWetDry()         const { return currentWeather[5]; }
    const WeatherData &getWindDirection()  const { return currentWeather[6]; }

    void setAirTemp(double val)         { currentWeather[0].value = val; }
    void setTrackTemp(double val)       { currentWeather[1].value = val; }
    void setWindSpeed(double val)       { currentWeather[2].value = val; }
    void setPressure(double val)        { currentWeather[3].value = val; }
    void setHumidity(double val)        { currentWeather[4].value = val; }
    void setWetDry(double val)          { currentWeather[5].value = val; }
    void setWindDirection(double val)   { currentWeather[6].value = val; }

    QList<WeatherData> getWeatherData(int idx) const
    {
        if (idx >= 0 && idx < 7)
            return weatherData[idx];

        return weatherData[0];
    }

    int getSize(int idx) const
    {
        if (idx >= 0 && idx < 7)
            return weatherData[idx].size();

        return 0;
    }

    void saveWeatherData(const EventData &);


    friend class EventData;
    friend class PackerParser;
private:
    //this list records the weather from whole session
    QList<WeatherData> weatherData[7];	//0 air temp, 1 track temp, 2 wind speed, 3 pressure, 4 humidity, 5 wet-dry, 6 wind direction

    //this list stores the current weather data
    WeatherData currentWeather[7];
};

/*!
 * \brief The SectorRecordData class stores session record from a single sector or a lap time, including driver name and number, lap time and when the record was stored (lap number, session time).
 */
class SectorRecordData
{
public:

    const QString &getDriverName()  const { return driver; }
    int getNumber()                 const { return number; }
    const LapTime &getTime()        const { return lapTime; }
    int getLapNumber()              const { return lapNum; }
    const QTime &getSessionTime()   const { return sessionTime; }
    int getQualiPeriod()            const { return qPeriod; }

    friend class EventData;
    friend class PacketParser;
    friend class SessionRecords;
private:
    QString driver;
    int number;
    LapTime lapTime;
    int lapNum;
    QTime sessionTime;
    int qPeriod;
};

/*!
 * \brief The SpeedRecordData class stores single speed record data (driver name and speed).
 */
class SpeedRecordData
{    
public:
    const QString &getDriverName() const { return driver; }
    double getSpeed()   const { return speed; }

    friend class EventData;
    friend class PacketParser;
    friend class SessionRecords;
private:
    QString driver;
    double speed;
};

/*!
 * \brief The SessionRecords class stores session records, including sector records, fastest lap, and speed records.
 */
class SessionRecords
{
public:
    SpeedRecordData getSectorSpeed(int sector, int idx) const
    {
        if (sector >= 1 && sector <= 3 &&
                idx >= 0 && idx < 6)
            return secSpeed[sector-1][idx];

        return SpeedRecordData();
    }

    SpeedRecordData getSpeedTrap(int idx) const
    {
        if (idx >= 0 && idx < 6)
            return speedTrap[idx];

        return SpeedRecordData();
    }

    const SectorRecordData &getFastestLap() const { return fastestLap; }

    SectorRecordData getSectorRecord(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return secRecord[idx-1];

        return SectorRecordData();
    }

    LapTime getQualiBestTime(int period) const
    {
        if (period >= 1 && period <= 3)
            return qualiRecords[period-1];

        return LapTime();
    }

    friend class EventData;
    friend class PacketParser;
private:
    SpeedRecordData secSpeed[3][6];
    SpeedRecordData speedTrap[6];

    SectorRecordData fastestLap;
    SectorRecordData secRecord[3];

    LapTime qualiRecords[3];
};

/*!
 * \brief The EventData class contains all informations about an event. Access to all drivers data, weather and session records data, event type (practice, quali, race), commentary, etc.
 * is available through this singleton.
 */
class EventData
{
public:

    static EventData &getInstance()
    {
        static EventData ed;

        if (ed.driversData.isEmpty())
        {
            ed.driversData = QVector<DriverData>(SeasonData::getInstance().getTeams().size()*2);
            for (int i = 0; i < ed.driversData.size(); ++i)
            {
                ed.driversData[i] = DriverData();
//                ed.driversData.append(DriverData());
//                ed.driversData.append(DriverData());
            }
        }

        return ed;
    } 

    void clear();    
    void reset();

    int getDriverId(const QString&) const;
    int getDriverId(int no) const;

    DriverData getDriverData(int no) const;
    DriverData *getDriverDataPtr(int no);

    DriverData getDriverDataByPos(int pos) const;
    DriverData *getDriverDataByPosPtr(int pos);

    DriverData getDriverDataById(int id) const;
    DriverData *getDriverDataByIdPtr(int id);

    QString calculateInterval(const DriverData &d1, const DriverData &d2, int lap) const;

    int correctPosition(const LapTime &ld) const;

    const LTEvent &getEventInfo()              const { return eventInfo; }
    void setEventInfo(const LTEvent &ev)
    {
        eventInfo = ev;        
        SeasonData::getInstance().getTrackMap(eventInfo);
    }
    int getEventId()                    const { return eventId; }
    LTPackets::EventType getEventType()    const { return eventType; }
    void setEventType(LTPackets::EventType type)
    {
        eventType = type;
    }

    LTPackets::FlagStatus getFlagStatus()  const { return flagStatus; }

    const QTime &getRemainingTime()            const { return remainingTime; }
    void setRemainingTime(const QTime &t)       { remainingTime = t; }
    int getCompletedLaps()              const { return lapsCompleted; }
    const Weather &getWeather()                const { return weather; }
    void saveWeather()                  { weather.saveWeatherData(*this); }

    bool isSessionStarted()             const { return sessionStarted; }
    void setSessionStarted(bool st)     { sessionStarted = st; }

    bool isSessionFinished()             const { return sessionFinished; }
    void setSessionFinished(bool st)     { sessionFinished = st; }

    bool isQualiBreak()                 const { return qualiBreak; }
    void setQualiBreak(bool qb)          { qualiBreak = qb; }

    const QString &getCommentary()             const { return commentary; }
    int getQualiPeriod()                const { return qualiPeriod; }

    int getFPNumber() const;
    void setFPNumber(int fp)
    {
        fpNumber = fp;
    }

    const SessionRecords &getSessionRecords()  const { return sessionRecords; }

    QVector<DriverData> &getDriversData()        { return driversData; }

    friend class PacketParser;
    friend class DataStreamReader;

private:
    EventData();
    EventData(const EventData&) {}
    void operator=(const EventData&) {}

    unsigned int key;
    unsigned int frame;

    QString cookie;

    LTEvent eventInfo;    

    int eventId;

    LTPackets::EventType eventType;
    LTPackets::FlagStatus flagStatus;

    QTime remainingTime;
    int lapsCompleted;

    Weather weather;

    bool sessionStarted;
    bool sessionFinished;
    bool qualiBreak;

    QString commentary;

    SessionRecords sessionRecords;

    QVector<DriverData> driversData;
    int qualiPeriod;
    int fpNumber;

    int baseEventId;
    int baseEventInc;
};

inline int EventData::getDriverId(const QString &name) const
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].getDriverName() == name)
            return driversData[i].getCarID();
    }
    return -1;
}

inline int EventData::getDriverId(int no) const
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].getNumber() == no)
            return driversData[i].getCarID();
    }
    return -1;
}

inline DriverData EventData::getDriverData(int no) const
{
    int id = getDriverId(no);
    if (id > 0 && id <= driversData.size())
        return driversData[id-1];

    return DriverData();
}

inline DriverData *EventData::getDriverDataPtr(int no)
{
    int id = getDriverId(no);
    if (id > 0 && id <= driversData.size())
        return &driversData[id-1];

    return 0;
}

inline DriverData EventData::getDriverDataByPos(int pos) const
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].getPosition() == pos)
            return driversData[i];
    }
    return DriverData();
}

inline DriverData *EventData::getDriverDataByPosPtr(int pos)
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].getPosition() == pos)
            return &driversData[i];
    }

    return 0;
}

inline DriverData EventData::getDriverDataById(int id) const
{
    if (id > 0 && id <= driversData.size())
        return driversData[id-1];

    return DriverData();
}

inline DriverData *EventData::getDriverDataByIdPtr(int id)
{
    if (id > 0 && id <= driversData.size())
        return &driversData[id-1];

    return 0;
}

//extern EventData eventData;

#endif // EVENTDATA_H
