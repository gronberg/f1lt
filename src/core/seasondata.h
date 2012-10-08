#ifndef SEASONDATA_H
#define SEASONDATA_H

#include <QMap>
#include <QPixmap>
#include <QString>
#include <QTime>

#include "f1ltcore.h"
#include "ltpackets.h"

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

//this class contains all the basic informations about the season, like quali and fp lengths, etc.
class SeasonData
{
public:
    static SeasonData &getInstance()
    {
        static SeasonData instance;
        return instance;
    }
    bool loadSeasonFile();

    QPixmap getCarImg(int no);
    int getFPNumber();
    int getEventNo(QDate);
    int getFPLength();
    int getFPLength(int fp);
    int getQualiLength(int q);

    LTEvent getEvent(int);
    LTEvent getEvent(QDate);
    LTEvent getCurrentEvent();
    LTEvent getNextEvent();

    QTime correctFPTime(QTime time);
    QTime correctQualiTime(QTime time, int qualiPeriod);

    QString getDriverName(QString);
    QString getDriverLastName(QString);
    QString getDriverShortName(QString);
    QString getDriverNameFromShort(QString);
    QString getEventNameFromShort(QString);
    int getDriverNo(QString name);
    QString getTeamName(int);
    QStringList getDriversList();
    QStringList getDriversListShort();

    QColor getColor(LTPackets::Colors color) { return colors[color]; }

    QList<LTTeam> &getTeams() { return ltTeams; }
    void setTeams(QList<LTTeam> teams) { ltTeams = teams; }
    QList<LTEvent> &getEvents() { return ltEvents; }

    int timeToMins(QTime time);
    int timeToSecs(QTime time);

    void fillEventNamesMap();

private:
    SeasonData();
    SeasonData(const SeasonData &) { }
    int season;
    int fpLengths[3];
    int qualiLengths[3];

    QList<LTTeam> ltTeams;
    QList<LTEvent> ltEvents;

    int baseEventId;
    int baseEventInc;

    QList<QColor> colors;

    QMap<QString, QString> eventNamesMap;
};

#endif // SEASONDATA_H
