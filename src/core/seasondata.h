#ifndef SEASONDATA_H
#define SEASONDATA_H

#include <QMap>
#include <QPixmap>
#include <QString>
#include <QTime>
#include <QVector>

#include "f1ltcore.h"
#include "imagesfactory.h"
#include "ltpackets.h"

class DriverData;

//Two additional stuctures that hold the basic info about all teams and events. Data is loaded from the file.
struct LTTeam
{
    //used for sorting
    bool operator < (const LTTeam &lt) const
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

struct LTTrackCoordinates
{
    int indexes[3];     //index of S1, S2 and pit out
    QList<QPoint> coordinates;
    QString name;
    int year;
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

    LTTrackCoordinates trackCoordinates;
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
    bool loadTrackDataFile();

    QPixmap getCarImg(int no);    
    int getEventNo(QDate);
    int getFPLength();
    int getFPLength(int fp);
    int getQualiLength(int q);

    LTEvent getEvent(int);
    const LTEvent &getEvent(const QDate&) const;
    const LTEvent &getCurrentEvent() const;
    const LTEvent &getNextEvent() const;

    QTime correctFPTime(const QTime &time);
    QTime correctQualiTime(const QTime &time, int qualiPeriod);

    void setTrackCoordinates(LTEvent &event);
    QString getDriverName(QString &name);
    QString getDriverLastName(const QString &name);
    QString getDriverShortName(const QString &name);
    QString getDriverNameFromShort(const QString &name);
    QString getEventNameFromShort(const QString &shortName);

    int getDriverNo(const QString &name);
    QString getTeamName(int no);
    QString getTeamName(const QString &driver);

    QStringList getDriversList();
    QStringList getDriversListShort();

    QColor getColor(LTPackets::Colors color) { return colors[color]; }
    QColor getDefaultColor(LTPackets::Colors color) { return defaultColors[color]; }
    void setColor(LTPackets::Colors colorCode, QColor color)
    {
        colors[colorCode] = color;
    }
    void setDefaultColor(LTPackets::Colors colorCode)
    {
        colors[colorCode] = defaultColors[colorCode];
    }
    void setAllDefaultColors()
    {
        colors = defaultColors;
    }

    QColor getCarColor(int no);

    QVector<LTTeam> &getTeams() { return ltTeams; }
    void setTeams(const QVector<LTTeam> &teams) { ltTeams = teams; }
    QVector<LTEvent> &getEvents() { return ltEvents; }

    int timeToMins(const QTime &time);
    int timeToSecs(const QTime &time);

    void fillEventNamesMap();

    CarThumbnailsFactory &getCarThumbnailsFactory() { return carThumbnailsFactory; }
    HelmetsFactory &getHelmetsFactory() { return helmetsFactory; }

private:
    SeasonData();
    SeasonData(const SeasonData &) { }
    int season;
    int fpLengths[3];
    int qualiLengths[3];

    QVector<LTTeam> ltTeams;
    QVector<LTEvent> ltEvents;
    QVector<LTTrackCoordinates> ltTrackCoordinates;

    int baseEventId;
    int baseEventInc;

    QList<QColor> colors;
    QList<QColor> defaultColors;
    QList<QColor> driverColors;

    QMap<QString, QString> eventNamesMap;
    CarThumbnailsFactory carThumbnailsFactory;
    HelmetsFactory helmetsFactory;
};

#endif // SEASONDATA_H
