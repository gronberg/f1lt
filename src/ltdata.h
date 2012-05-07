#ifndef LTDATA_H
#define LTDATA_H

#include <QByteArray>
#include <QCoreApplication>
#include <QDate>
#include <QList>
#include <QPixmap>
#include <QTime>
#include <iostream>
#include <cmath>
#include <QMap>
#include <QStringList>


#include "f1ltcore.h"
//#include "driverdata.h"
//#include "eventdata.h"

//Two additional stuctures that hold the basic info about all teams and events. Data is loaded from the file.
struct LTTeam
{
    //used for sorting
    bool operator < (const LTTeam lt) const
    {
        return driver1No < lt.driver1No;
    }

    QString teamName;

    QString driver1Name;
    QString driver1ShortName;
    int driver1No;

    QString driver2Name;
    QString driver2ShortName;
    int driver2No;

    QPixmap carImg;
};

struct LTEvent
{
    LTEvent() : eventNo(0), laps(0), trackImg(100,100) { }
    int eventNo;
    QString eventName;
    QString eventShortName;
    QString eventPlace;
    int laps;
    QDate fpDate;
    QDate raceDate;

    QPixmap trackImg;

    bool operator<(const LTEvent &event) const
    {
        return fpDate < event.fpDate;
    }
};

//This class defines all the packet types that server sends to us. Code below is highly based on the live-f1 source code by Dave Pusey.
class LTData
{
public:
    LTData();

    enum CarPacket
    {
        CAR_POSITION_UPDATE	= 0,
        CAR_POSITION_HISTORY = 15
    };

    enum RacePacket
    {
        RACE_POSITION = 1,
        RACE_NUMBER,
        RACE_DRIVER,
        RACE_GAP,
        RACE_INTERVAL,
        RACE_LAP_TIME,
        RACE_SECTOR_1,
        RACE_PIT_LAP_1,
        RACE_SECTOR_2,
        RACE_PIT_LAP_2,
        RACE_SECTOR_3,
        RACE_PIT_LAP_3,
        RACE_NUM_PITS
    };

    enum PracticePacket
    {
        PRACTICE_POSITION	= 1,
        PRACTICE_NUMBER,
        PRACTICE_DRIVER,
        PRACTICE_BEST,
        PRACTICE_GAP,
        PRACTICE_SECTOR_1,
        PRACTICE_SECTOR_2,
        PRACTICE_SECTOR_3,
        PRACTICE_LAP
    };

    enum QualifyingPacket
    {
        QUALI_POSITION= 1,
        QUALI_NUMBER,
        QUALI_DRIVER,
        QUALI_PERIOD_1,
        QUALI_PERIOD_2,
        QUALI_PERIOD_3,
        QUALI_SECTOR_1,
        QUALI_SECTOR_2,
        QUALI_SECTOR_3,
        QUALI_LAP
    };


    enum SystemPacket
    {
        SYS_EVENT_ID		= 1,
        SYS_KEY_FRAME       = 2,
        SYS_VALID_MARKER    = 3,
        SYS_COMMENTARY      = 4,
        SYS_REFRESH_RATE    = 5,
        SYS_NOTICE          = 6,
        SYS_TIMESTAMP       = 7,
        SYS_WEATHER         = 9,
        SYS_SPEED           = 10,
        SYS_TRACK_STATUS    = 11,
        SYS_COPYRIGHT       = 12
    };

    enum WeatherPacket
    {
        WEATHER_SESSION_CLOCK	= 0,
        WEATHER_TRACK_TEMP,
        WEATHER_AIR_TEMP,
        WEATHER_WET_TRACK,
        WEATHER_WIND_SPEED,
        WEATHER_HUMIDITY,
        WEATHER_PRESSURE,
        WEATHER_WIND_DIRECTION
    };

    enum SpeedPacket
    {
        SPEED_SECTOR1 = 1,
        SPEED_SECTOR2,
        SPEED_SECTOR3,
        SPEED_TRAP,
        FL_CAR,
        FL_DRIVER,
        FL_TIME,
        FL_LAP
    };

    enum EventType
    {
        RACE_EVENT = 1,
        PRACTICE_EVENT,
        QUALI_EVENT
    };

    enum FlagStatus
    {
        GREEN_FLAG = 1,
        YELLOW_FLAG,
        SAFETY_CAR_STANDBY,
        SAFETY_CAR_DEPLOYED,
        RED_FLAG,
    };

    enum Colors
    {
        DEFAULT,
        WHITE,
        PIT,
        GREEN,
        VIOLET,
        CYAN,
        YELLOW,
        RED,
        BACKGROUND,
        BACKGROUND2
    };

    static int getPacketType(const QByteArray &buf)
    {
        unsigned char arr[2] = {(unsigned char)(buf[0]), (unsigned char)(buf[1])};
        return (arr[0] >> 5) | ((arr[1] & 0x01) << 3);
    }
    static int getCarPacket(const QByteArray &buf)
    {
        unsigned char arr[2] = {(unsigned char)(buf[0]), (unsigned char)(buf[1])};
        return arr[0] & 0x1f;
    }
    static int getLongPacketData(const QByteArray &)
    {
        return 0;
    }
    static int getShortPacketData(const QByteArray &buf)
    {
        unsigned char arr[2] = {(unsigned char)(buf[0]), (unsigned char)(buf[1])};
        return (arr[1] & 0x0e) >> 1;
    }
    static int getSpecialPacketData(const QByteArray &buf)
    {
        unsigned char arr[2] = {(unsigned char)(buf[0]), (unsigned char)(buf[1])};
        return arr[1] >> 1;
    }
    static int getLongPacketLength(const QByteArray &buf)
    {
        unsigned char arr[2] = {(unsigned char)(buf[0]), (unsigned char)(buf[1])};
        return arr[1] >> 1;
    }
    static int getShortPacketLength(const QByteArray &buf)
    {
        unsigned char arr[2] = {(unsigned char)(buf[0]), (unsigned char)(buf[1])};
        return (arr[1] & 0xf0) == 0xf0 ? -1 : (arr[1] >> 4);
    }
    static int getSpecialPacketLength(const QByteArray &)
    {
        return 0;
    }

    static bool loadLTData();
    static void init();
    static QPixmap getCarImg(int no);
    static QString getDriverName(QString);
    static QString getDriverShortName(QString);
    static QString getDriverNameFromShort(QString);
    static QString getTeamName(int);
    static QStringList getDriversList();
    static LTEvent getEvent(int);
    static LTEvent getEvent(QDate);
    static int getEventNo(QDate);
    static int getDriverNo(QString);

    static QList<LTTeam> ltTeams;
    static QList<LTEvent> ltEvents;
    static int season;

    static QList<QColor> colors;
};

#endif // LTDATA_H
