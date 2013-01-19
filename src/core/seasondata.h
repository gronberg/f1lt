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



    QVector<LTTeam> &getTeams() { return ltTeams; }
    void setTeams(const QVector<LTTeam> &teams) { ltTeams = teams; }
    QVector<LTEvent> &getEvents() { return ltEvents; }

    int timeToMins(const QTime &time);
    int timeToSecs(const QTime &time);

    void fillEventNamesMap();



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



    QMap<QString, QString> eventNamesMap;    
};

#endif // SEASONDATA_H
