#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "driverdata.h"

class DataStreamReader;
class EventData;
class PacketParser;
class SessionRecords;
class Weather;

class WeatherData
{
public:
    WeatherData() : value(0.0), lap(0), sessionTime(), qPeriod(1) { }

    double getValue() const { return value; }
    int getLap() const { return lap; }
    QTime getSessionTime() const { return sessionTime; }
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

class Weather
{
public:

    WeatherData getAirTemp()        const { return currentWeather[0]; }
    WeatherData getTrackTemp()      const { return currentWeather[1]; }
    WeatherData getWindSpeed()      const { return currentWeather[2]; }
    WeatherData getPressure()       const { return currentWeather[3]; }
    WeatherData getHumidity()       const { return currentWeather[4]; }
    WeatherData getWetDry()         const { return currentWeather[5]; }
    WeatherData getWindDirection()  const { return currentWeather[6]; }

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

class SectorRecordData
{
public:

    QString getDriverName()     const { return driver; }
    int getNumber()         const { return number; }
    LapTime getTime()       const { return lapTime; }
    int getLapNumber()      const { return lapNum; }
    QTime getSessionTime()  const { return sessionTime; }
    int getQualiPeriod()    const { return qPeriod; }

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

class SpeedRecordData
{    
public:
    QString getDriverName() const { return driver; }
    double getSpeed()   const { return speed; }

    friend class EventData;
    friend class PacketParser;
    friend class SessionRecords;
private:
    QString driver;
    double speed;
};

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

    SectorRecordData getFastestLap() const { return fastestLap; }

    SectorRecordData getSectorRecord(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return secRecord[idx-1];
    }

    friend class EventData;
    friend class PacketParser;
private:
    SpeedRecordData secSpeed[3][6];
    SpeedRecordData speedTrap[6];

    SectorRecordData fastestLap;
    SectorRecordData secRecord[3];
};

class EventData
{
public:

    static EventData &getInstance()
    {
        static EventData ed;

        if (ed.driversData.isEmpty())
        {
            for (int i = 0; i < SeasonData::getInstance().getTeams().size(); ++i)
            {
                ed.driversData.append(DriverData());
                ed.driversData.append(DriverData());
            }
        }

        return ed;
    } 

    void clear();    

    int getDriverId(QString) const;
    int getDriverId(int no) const;
    DriverData getDriverData(int no) const;
    DriverData getDriverDataByPos(int pos) const;
    DriverData getDriverDataById(int id) const;
    QString calculateInterval(DriverData d1, DriverData d2, int lap) const;

    LTEvent getEventInfo()              const { return eventInfo; }
    void setEventInfo(LTEvent ev)       { eventInfo = ev; }
    int getEventId()                    const { return eventId; }
    LTPackets::EventType getEventType()    const { return eventType; }
    LTPackets::FlagStatus getFlagStatus()  const { return flagStatus; }

    QTime getRemainingTime()            const { return remainingTime; }
    void setRemainingTime(QTime t)       { remainingTime = t; }
    int getCompletedLaps()              const { return lapsCompleted; }
    Weather getWeather()                const { return weather; }
    void saveWeather()                  { weather.saveWeatherData(*this); }

    bool isSessionStarted()             const { return sessionStarted; }
    void setSessionStarted(bool st)     { sessionStarted = st; }

    QString getCommentary()             const { return commentary; }
    int getQualiPeriod()                const { return qualiPeriod; }

    SessionRecords getSessionRecords()  const { return sessionRecords; }

    QList<DriverData> &getDriversData()        { return driversData; }

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

    QString commentary;

    SessionRecords sessionRecords;

    QList<DriverData> driversData;
    int qualiPeriod;           
};

//extern EventData eventData;

#endif // EVENTDATA_H
