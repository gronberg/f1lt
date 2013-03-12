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


#include "seasondata.h"

#include "ltpackets.h"
#include <QDataStream>
#include <QFile>

#include "colorsmanager.h"
#include "eventdata.h"
#include "trackrecords.h"


SeasonData::SeasonData() : season(2013)
{
    ltTeams.clear();
    fillEventNamesMap();
}

bool SeasonData::loadSeasonFile()
{
    ltTeams.clear();

    season = 0;

    bool ok[3];
    //on startup try to load current seasons data    
    ok[0] = loadSeasonData(QDate::currentDate().year());
    ok[1] = trackMapsCoordinates.loadTrackDataFile();

    ok[2] = TrackRecords::getInstance().loadTrackRecords(F1LTCore::trackRercordsFile());
    return (ok[0] && ok[1] && ok[2]);
}

bool SeasonData::loadSeasonData(int season)
{
    if (this->season != season)
    {
        ltTeams.clear();

        QString fName = F1LTCore::seasonDataFile();
        if (!fName.isNull())
        {
            QFile f(fName);
            if (f.open(QIODevice::ReadOnly))
            {
                QDataStream stream(&f);
                char *tab;
                stream >> tab;  //F1LT2_SD

                QString sbuf(tab);

                delete [] tab;
                if (sbuf != "F1LT2_SD")
                    return false;

                int size;

                stream >> size;

                //first load offsets
                for (int i = 0; i < size; ++i)
                {
                    int season, offset;
                    stream >> season;
                    stream >> offset;

                    seasonOffsets.insert(season, offset);
                }

                //store current position
                int headerPos = f.pos();

                if (!seasonOffsets.contains(season))
                    return false;

                int offset = seasonOffsets[season];
                f.seek(headerPos + offset);

                loadSeasonData(stream);
            }
            else
                return false;
        }
        else
            return false;

        qSort(ltTeams);
        qSort(ltEvents);

        emit seasonDataChanged();        
    }

    return true;
}

void SeasonData::loadSeasonData(QDataStream &stream)
{
    int size;
    stream >> season;
    stream >> size;

    ltEvents.resize(size);
    for (int i = 0; i < size; ++i)
    {
        stream >> ltEvents[i].eventNo;
        stream >> ltEvents[i].eventName;
        stream >> ltEvents[i].eventShortName;
        stream >> ltEvents[i].eventPlace;
        stream >> ltEvents[i].laps;

        QString sbuf;
        stream >> sbuf;
        ltEvents[i].fpDate = QDate::fromString(sbuf, "dd-MM-yyyy");

        stream >> sbuf;
        ltEvents[i].raceDate = QDate::fromString(sbuf, "dd-MM-yyyy");

        stream >> ltEvents[i].trackImg;

    }
    stream >> size;
    ltTeams.resize(size);
    for (int i = 0; i < size; ++i)
    {
        stream >> ltTeams[i].teamName;
        stream >> ltTeams[i].carImg;

        int dsize;
        stream >> dsize;

        ltTeams[i].drivers.resize(dsize);

        for (int j = 0; j < dsize; ++j)
        {
            stream >> ltTeams[i].drivers[j].name;
            stream >> ltTeams[i].drivers[j].shortName;
            stream >> ltTeams[i].drivers[j].no;
            stream >> ltTeams[i].drivers[j].helmet;

            //first 2 drivers are always main by default
            if (j < 2)
                ltTeams[i].drivers[j].mainDriver = true;
            else
                ltTeams[i].drivers[j].mainDriver = false;
        }
    }
}

/*!
 * \brief SeasonData::checkSeasonData - checks if current season is loaded. If current date is < than first race FP1 date - loads last years data
 */
void SeasonData::checkSeasonData()
{
    if (season != QDate::currentDate().year())
        loadSeasonData(QDate::currentDate().year());

    return;

    LTEvent ev = getEvent(1);
    if (ev.eventNo > 0)
    {
        if ((ev.fpDate.year() == QDate::currentDate().year()) &&
            (ev.fpDate.month() == QDate::currentDate().month()) &&
            (ev.fpDate.day() > QDate::currentDate().day()))
        {
                loadSeasonData(QDate::currentDate().year() - 1);
        }
    }
}

void SeasonData::updateTeamList(const QVector<LTTeam> &teams)
{
    for (int i = 0; i < teams.size(); ++i)
    {
        if (!ltTeams.contains(teams[i]))
            ltTeams.append(teams[i]);

        int idx = ltTeams.indexOf(teams[i]);

        if (teams[i].teamName == ltTeams[idx].teamName)
        {
            //clear main drivers
            for (int k = 0; k < ltTeams[idx].drivers.size(); ++k)
            {
                ltTeams[idx].drivers[k].mainDriver = false;
            }

            for (int k = 0; k < teams[i].drivers.size(); ++k)
            {
                int didx = ltTeams[idx].drivers.indexOf(teams[i].drivers[k]);

                qDebug() << "APPEND" << teams[i].drivers[k].name;
                if (didx == -1)
                {
                    ltTeams[idx].drivers.append(teams[i].drivers[k]);
                    ltTeams[idx].drivers.last().mainDriver = true;                    
                }
                else
                {
                    ltTeams[idx].drivers[didx].mainDriver = true;
                }
            }
        }
    }    
    ImagesFactory::getInstance().getHelmetsFactory().reloadHelmets();
}


//this updates team list from eventdata drivers list
void SeasonData::updateTeamList(const DriverData &dd)
{
    for (int j = 0; j < ltTeams.size(); ++j)
    {
        bool foundNumberWithoutName = false, foundAlready = false;

        for (int k = 0; k < ltTeams[j].drivers.size(); ++k)
        {            
            if (ltTeams[j].drivers[k].no == dd.getNumber())
            {
                if (ltTeams[j].drivers[k].name == dd.getDriverName())
                {
                    ltTeams[j].drivers[k].mainDriver = true;
                    foundNumberWithoutName = false;
                    foundAlready = true;
                }
                else
                {
                    ltTeams[j].drivers[k].mainDriver = false;
                    foundNumberWithoutName = true;
                }
            }
        }

        if (foundNumberWithoutName && !foundAlready)
        {
            LTDriver driver;
            driver.name = dd.getDriverName();
            driver.no = dd.getNumber();
            driver.shortName = getDriverShortName(dd.getDriverName());
            driver.mainDriver = true;

            ltTeams[j].drivers.append(driver);
        }
    }    
}


QPixmap SeasonData::getCarImg(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].no == no)
            {
                return ltTeams[i].carImg;
            }
        }
    }
    return QPixmap();
}

QString SeasonData::getDriverName(QString &name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].name.toUpper() == name.toUpper())
                return ltTeams[i].drivers[j].name;

            //size of data that holds driver name is stored on 4 bytes only, and van der Garde name is shortened by server
            if (name.toUpper() == "G. VAN DER GAR" && ltTeams[i].drivers[j].name.toUpper().contains(name.toUpper()))
                return ltTeams[i].drivers[j].name;
        }
    }
    name = name.left(4) + name.right(name.size()-4).toLower();
    int idx = name.indexOf(" ");
    while (idx != -1)
    {
        name.replace(name[idx+1], name[idx+1].toUpper());
        idx = name.indexOf(" ", idx+1);
    }
    return name;
}

QString SeasonData::getDriverLastName(const QString &name)
{
    if (name.size() > 3)
        return name.right(name.size()-3);

    return name;
}

QString SeasonData::getDriverShortName(const QString &name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].name.toUpper() == name.toUpper())
                return ltTeams[i].drivers[j].shortName;
        }
    }
    return name.toUpper().mid(3, 3);
}

QString SeasonData::getDriverNameFromShort(const QString &name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].shortName.toUpper() == name.toUpper())
                return ltTeams[i].drivers[j].name;
        }
    }
    return name.left(1) + name.right(name.size()-1).toLower();
}

QString SeasonData::getTeamName(const QString &driver)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].name.toUpper() == driver.toUpper())
                return ltTeams[i].teamName;
        }
    }
    return QString();
}

QList<LTDriver> SeasonData::getMainDrivers(const LTTeam &team)
{
    int idx = ltTeams.indexOf(team);
    if (idx == -1)
        return QList<LTDriver>();

    QList<LTDriver> drivers;
    for (int i = 0; i < ltTeams[idx].drivers.size(); ++i)
    {
        if (ltTeams[idx].drivers[i].mainDriver == true)
            drivers.append(ltTeams[idx].drivers[i]);

        if (drivers.size() == 2)
            break;
    }

    return drivers;
}

QString SeasonData::getTeamName(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].no == no)
                return ltTeams[i].teamName;
        }
    }
    return QString();
}

LTEvent SeasonData::getEvent(int ev)
{
    for (int i = 0; i < ltEvents.size(); ++i)
    {
        if (ltEvents[i].eventNo == ev)
            return ltEvents[i];
    }
    return LTEvent();
}

int SeasonData::getEventNo(QDate date)
{
    for (int i = 1; i < ltEvents.size(); ++i)
    {
        if (date >= ltEvents[i-1].fpDate && date < ltEvents[i].fpDate)
            return ltEvents[i-1].eventNo;
    }
    return 1;
}

const LTEvent &SeasonData::getEvent(const QDate &date) const
{
    for (int i = 1; i < ltEvents.size(); ++i)
    {
        if (date >= ltEvents[i-1].fpDate && date < ltEvents[i].fpDate)
            return ltEvents[i-1];
    }
    if (date >= ltEvents.last().fpDate && date <= ltEvents.last().raceDate)
        return ltEvents.last();

    return ltEvents[0];
}

const LTEvent &SeasonData::getCurrentEvent()const
{
    return getEvent(QDate::currentDate());
}

const LTEvent &SeasonData::getNextEvent() const
{
    QDate date = QDate::currentDate();
    for (int i = 1; i < ltEvents.size(); ++i)
    {
        if (date > ltEvents[i-1].raceDate && date <= ltEvents[i].raceDate)
            return ltEvents[i];
    }

    return ltEvents[0];
}

int SeasonData::getDriverNo(const QString &name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].name.toUpper() == name.toUpper())
                return ltTeams[i].drivers[j].no;
        }
    }
    return -1;
}

QStringList SeasonData::getDriversList()
{
    QStringList list;
    list.append("");

    for (int i = 0; i < 30; ++i)
    {
        DriverData dd = EventData::getInstance().getDriverData(i);
        if (dd.getCarID() != -1)
        {
            list.append(QString::number(dd.getNumber()) + " " + dd.getDriverName());
        }
    }
    if (list.size() == 1)
    {
        for (int i = 0; i < ltTeams.size(); ++i)
        {
            QList<LTDriver> drivers = getMainDrivers(ltTeams[i]);
            for (int j = 0; j < drivers.size(); ++j)
                list.append(QString::number(drivers[j].no) + " " + drivers[j].name);
        }
    }

    return list;
}

QStringList SeasonData::getDriversListShort()
{
    QStringList list;

    for (int i = 0; i < 30; ++i)
    {
        DriverData dd = EventData::getInstance().getDriverData(i);
        if (dd.getCarID() != -1)
        {
            list.append(QString::number(dd.getNumber()) + " " + SeasonData::getDriverShortName(dd.getDriverName()));
        }
    }
    if (list.isEmpty())
    {
        for (int i = 0; i < ltTeams.size(); ++i)
        {
            QList<LTDriver> drivers = getMainDrivers(ltTeams[i]);
            for (int j = 0; j < drivers.size(); ++j)
                list.append(QString::number(drivers[j].no) + " " + drivers[j].shortName);
        }
    }

    return list;
}

QVector<LTTeam> SeasonData::getTeamsFromCurrentSession()
{
    QVector<LTTeam> teams;

    for (int i = 0; i < ltTeams.size(); ++i)
    {
        LTTeam team;
        team.teamName = ltTeams[i].teamName;

        for (int j = 0; j < ltTeams[i].drivers.size(); ++j)
        {
            if (ltTeams[i].drivers[j].mainDriver)
                team.drivers.append(ltTeams[i].drivers[j]);
        }
        teams.append(team);
    }

    return teams;
}

void SeasonData::getTrackMap(LTEvent &ev)
{
    for (int i = 0; i < ltEvents.size(); ++i)
    {
        if (ltEvents[i] == ev)
        {
            ev.trackImg = ltEvents[i].trackImg;
            return;
        }
    }
}

QString SeasonData::getEventNameFromShort(const QString &shortName)
{
    return eventNamesMap[shortName.toLower()] + " Grand Prix";
//    for (int i = 0; i < ltEvents.size(); ++i)
//    {
//        if (ltEvents[i].eventShortName.toLower() == shortName.toLower())
//            return ltEvents[i].eventName;
//    }
//    return shortName;
}



void SeasonData::fillEventNamesMap()
{
    eventNamesMap.insert("aus", "Australian");
    eventNamesMap.insert("mal", "Malaysian");
    eventNamesMap.insert("chn", "Chinese");
    eventNamesMap.insert("bah", "Bahrain");
    eventNamesMap.insert("spa", "Spanish");
    eventNamesMap.insert("mon", "Monaco");
    eventNamesMap.insert("can", "Canadian");
    eventNamesMap.insert("eur", "European");
    eventNamesMap.insert("val", "European");
    eventNamesMap.insert("gbr", "British");
    eventNamesMap.insert("ger", "German");
    eventNamesMap.insert("hun", "Hungarian");
    eventNamesMap.insert("bel", "Belgian");
    eventNamesMap.insert("ita", "Italian");
    eventNamesMap.insert("sgp", "Singapore");
    eventNamesMap.insert("jpn", "Japanese");
    eventNamesMap.insert("kor", "Korean");
    eventNamesMap.insert("ind", "Indian");
    eventNamesMap.insert("abu", "Abu Dhabi");
    eventNamesMap.insert("usa", "US");
    eventNamesMap.insert("bra", "Brazilian");
}




