#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "driverdata.h"

struct EventData
{
private:
    EventData();
    EventData(const EventData&) {}
    void operator=(const EventData&) {}

public:

    static EventData &getInstance()
    {
        static EventData ed;
        return ed;
    }

    void clear();

    int getDriverId(QString);
    int getDriverId(int);
    DriverData getDriverData(int);
    unsigned int key;    
    QString cookie;

    LTEvent eventInfo;

    unsigned int frame;
    unsigned int salt;

    LTData::EventType eventType;
    LTData::FlagStatus flagStatus;

    QTime remainingTime;
    int lapsCompleted;

    double airTemp, humidity;
    double windSpeed, windDirection;

    double pressure, trackTemp;

    bool sessionStarted;

    int wetdry;

    QString notice;

    QString commentary;
    QString sec1Speed[12];
    QString sec2Speed[12];
    QString sec3Speed[12];
    QString speedTrap[12];

    int FLNumber;
    QString FLDriver;
    LapTime FLTime;
    int FLLap;

    QString sec1Record[4];
    QString sec2Record[4];
    QString sec3Record[4];

    QList<DriverData> driversData;
    int qualiPeriod;
};

//extern EventData eventData;

#endif // EVENTDATA_H
