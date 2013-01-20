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

#include "eventdata.h"
#include "trackrecords.h"


SeasonData::SeasonData() : season(2012), baseEventId (7066), baseEventInc (6)
{
    ltTeams.clear();
    fillEventNamesMap();
}

bool SeasonData::loadSeasonFile()
{
    ltTeams.clear();
    QString fName = F1LTCore::seasonDataFile();
    if (!fName.isNull())
    {
        QFile f(fName);
        if (f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);
            int size;
            QString sbuf;

            stream >> season;
//            ui->spinBox->setValue(ibuf);

            stream >> size;
            ltEvents.resize(size);
            for (int i = 0; i < size; ++i)
            {
                stream >> ltEvents[i].eventNo;
                stream >> ltEvents[i].eventName;
                stream >> ltEvents[i].eventShortName;
                stream >> ltEvents[i].eventPlace;
                stream >> ltEvents[i].laps;

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
                stream >> ltTeams[i].driver1Name;
                stream >> ltTeams[i].driver1ShortName;
                stream >> ltTeams[i].driver1No;
                stream >> ltTeams[i].driver2Name;
                stream >> ltTeams[i].driver2ShortName;
                stream >> ltTeams[i].driver2No;
                stream >> ltTeams[i].carImg;
            }
        }
        else
            return false;
    }
    else
        return false;
    qSort(ltTeams);
    qSort(ltEvents);

    bool ok;
    ok = trackMapsCoordinates.loadTrackDataFile();

    ok = TrackRecords::getInstance().loadTrackRecords(F1LTCore::trackRercordsFile());    
    return ok;
}



QPixmap SeasonData::getCarImg(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1No == no || ltTeams[i].driver2No == no)
            return ltTeams[i].carImg;
    }
    return QPixmap();
}

QString SeasonData::getDriverName(QString &name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        QString buf1 = ltTeams[i].driver1Name.toUpper();
        QString buf2 = ltTeams[i].driver2Name.toUpper();

        if (buf1 == name)
            return ltTeams[i].driver1Name;

        if (buf2 == name)
            return ltTeams[i].driver2Name;
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
        QString buf1 = ltTeams[i].driver1Name.toUpper();
        QString buf2 = ltTeams[i].driver2Name.toUpper();

        if (buf1 == name.toUpper())
            return ltTeams[i].driver1ShortName;

        if (buf2 == name.toUpper())
            return ltTeams[i].driver2ShortName;
    }
    return name.toUpper().mid(3, 3);
}

QString SeasonData::getDriverNameFromShort(const QString &name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        QString buf1 = ltTeams[i].driver1ShortName;
        QString buf2 = ltTeams[i].driver2ShortName;

        if (buf1 == name)
            return ltTeams[i].driver1Name;

        if (buf2 == name)
            return ltTeams[i].driver2Name;
    }
    return name.left(1) + name.right(name.size()-1).toLower();
}

QString SeasonData::getTeamName(const QString &driver)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1Name == driver || ltTeams[i].driver2Name == driver)
            return ltTeams[i].teamName;
    }
    return QString();
}

QString SeasonData::getTeamName(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1No == no || ltTeams[i].driver2No == no)
            return ltTeams[i].teamName;
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
        if (ltTeams[i].driver1Name == name)
            return ltTeams[i].driver1No;

        if (ltTeams[i].driver2Name == name)
            return ltTeams[i].driver2No;
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
            list.append(QString::number(ltTeams[i].driver1No) + " " + ltTeams[i].driver1Name);
            list.append(QString::number(ltTeams[i].driver2No) + " " + ltTeams[i].driver2Name);
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
            list.append(QString::number(ltTeams[i].driver1No) + " " + ltTeams[i].driver1ShortName);
            list.append(QString::number(ltTeams[i].driver2No) + " " + ltTeams[i].driver2ShortName);
        }
    }

    return list;
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


