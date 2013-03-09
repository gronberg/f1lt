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
#include "sessiondefaults.h"
#include "trackmapscoordinates.h"

class DriverData;


/*!
 * \brief The LTDriver struct stores basic informations about a driver loaded from the season.dat file.
 */
struct LTDriver
{
    QString name;
    QString shortName;
    int no;
    QPixmap helmet;

    bool mainDriver;    //during a session team contains 2 main drivers, however it can have more than 2 saved in the file (test drivers, reserved, etc.)

    bool operator==(const LTDriver &drv) const
    {
        return name == drv.name;
    }

    bool operator < (const LTDriver &drv) const
    {
        return no < drv.no;
    }
};

/*!
 * \brief The LTTeam struct stores informations about a team loaded from the season.dat file.
 */
struct LTTeam
{    
    QString teamName;
    QVector<LTDriver> drivers;

    QPixmap carImg;

    //used for sorting
    bool operator < (const LTTeam &lt) const
    {
        if (!drivers.isEmpty() && !lt.drivers.isEmpty())
            return drivers.first().no < lt.drivers.first().no;

        if (lt.drivers.isEmpty())
            return true;

        return false;
    }

    bool operator==(const LTTeam &team) const
    {
        return teamName == team.teamName;
    }
};


/*!
 * \brief The LTEvent struct stores informations about an event loaded from the season.dat file.
 */
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

    bool operator==(const LTEvent &event) const
    {
        if (eventNo == event.eventNo &&
            eventName == event.eventName &&
            event.fpDate == event.fpDate &&
            event.raceDate == event.raceDate)
            return true;

        return false;
    }        

//    LTTrackCoordinates trackCoordinates;
};


/*!
 * \brief The SeasonData class stores all basic informations loaded from season.dat file plus it gives access to session defaults and track map coordinates.
 * This is a singleton.
 */
class SeasonData : public QObject
{
    Q_OBJECT
public:
    static SeasonData &getInstance()
    {
        static SeasonData instance;
        return instance;
    }

    bool loadSeasonFile();
    bool loadSeasonData(int season);
    void loadSeasonData(QDataStream &stream);
    void checkSeasonData();

    void updateTeamList(const QVector<LTTeam> &teams);
    void updateTeamList(const DriverData &dd);


    QPixmap getCarImg(int no);    
    int getEventNo(QDate);


    LTEvent getEvent(int);
    const LTEvent &getEvent(const QDate&) const;
    const LTEvent &getCurrentEvent() const;
    const LTEvent &getNextEvent() const;


    QString getDriverName(QString &name);
    QString getDriverLastName(const QString &name);
    QString getDriverShortName(const QString &name);
    QString getDriverNameFromShort(const QString &name);
    QString getEventNameFromShort(const QString &shortName);

    int getDriverNo(const QString &name);
    QString getTeamName(int no);
    QString getTeamName(const QString &driver);

    QList<LTDriver> getMainDrivers(const LTTeam &team);

    QStringList getDriversList();
    QStringList getDriversListShort();


    const SessionDefaults &getSessionDefaults() const
    {
        return sessionDefaults;
    }

    const TrackMapsCoordinates &getTrackMapsCoordinates() const
    {
        return trackMapsCoordinates;
    }

    QVector<LTTeam> &getTeams() { return ltTeams; }
    QVector<LTTeam> getTeamsFromCurrentSession();
    QVector<LTEvent> &getEvents() { return ltEvents; }   

    void getTrackMap(LTEvent &ev);

    void fillEventNamesMap();

signals:
    void seasonDataChanged();

private:
    SeasonData();
    SeasonData(const SeasonData &) : QObject() { }
    int season;

    QMap<int, int> seasonOffsets;        //only one season data from season.dat file will be kept in the memory during runtime, this map holds offsets of all seasons in the file
    QVector<LTTeam> ltTeams;
    QVector<LTEvent> ltEvents;

    SessionDefaults sessionDefaults;
    TrackMapsCoordinates trackMapsCoordinates;
    QMap<QString, QString> eventNamesMap;    
};

#endif // SEASONDATA_H

