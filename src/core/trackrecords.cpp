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


#include "trackrecords.h"

#include "../core/seasondata.h"

TrackRecords::TrackRecords()
{
}


bool TrackRecords::loadTrackRecords(QString fileName)
{
    QFile f(fileName);
    if (f.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&f);

        char *tab;
        stream >> tab;

        QString sbuf(tab);
        delete [] tab;

        if (QString(sbuf) != "F1LT2_TR")
        {
            f.close();
            return false;
        }

        trackRecords.clear();

        int size;
        stream >> size;
        for (int i = 0; i < size; ++i)
        {
            Track track;
            stream >> track.name;

            int verSize;
            stream >> verSize;

            for (int j = 0; j < verSize; ++j)
            {
                TrackVersion tv;
                stream >> tv.year;
                stream >> tv.map;

                QString time;
                for (int k = 0; k < 2; ++k)
                {
                    stream >> time;
                    tv.trackRecords[k].time = LapTime(time);
                    stream >> tv.trackRecords[k].driver;
                    stream >> tv.trackRecords[k].team;
                    stream >> tv.trackRecords[k].year;
                }

                int tsSize;
                stream >> tsSize;

                for (int k = 0; k < tsSize; ++k)
                {
                    TrackWeekendRecords ts;

                    stream >> ts.year;


                    for (int w = 0; w < 4; ++w)
                    {
                        stream >> time;
                        ts.sessionRecords[w].time = LapTime(time);
                        stream >> ts.sessionRecords[w].driver;
                        stream >> ts.sessionRecords[w].team;
                        stream >> ts.sessionRecords[w].session;
                    }

                    int dsSize;
                    stream >> dsSize;
                    for (int w = 0; w < dsSize; ++w)
                    {
                        DriverWeekendRecords ds;

                        stream >> ds.driver;
                        stream >> ds.team;

                        for (int x = 0; x < 5; ++x)
                        {
                            for (int y = 0; y < 4; ++y)
                            {
                                stream >> time;
                                ds.sessionRecords[x][y].time = LapTime(time);
                                stream >> ds.sessionRecords[x][y].session;
                            }
                        }

                        ts.driverRecords.append(ds);
                    }

                    tv.trackWeekendRecords.append(ts);
                }
                track.trackVersions.append(tv);
            }
            qSort(track.trackVersions);
            trackRecords.append(track);
        }
    }
    else
        return false;

    return true;
}

bool TrackRecords::saveTrackRecords(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);

        const char tab[9] = "F1LT2_TR";
        stream << tab;

        stream << trackRecords.size();

        for (int i = 0; i < trackRecords.size(); ++i)
        {
            Track &tr = trackRecords[i];
            stream << tr.name;
            stream << tr.trackVersions.size();

            for (int j = 0; j < tr.trackVersions.size(); ++j)
            {
                TrackVersion &tv = tr.trackVersions[j];

                stream << tv.year;
                stream << tv.map;

                for (int k = 0; k < 2; ++k)
                {
                    stream << tv.trackRecords[k].time;
                    stream << tv.trackRecords[k].driver;
                    stream << tv.trackRecords[k].team;
                    stream << tv.trackRecords[k].year;
                }

                stream << tv.trackWeekendRecords.size();

                for (int k = 0; k < tv.trackWeekendRecords.size(); ++k)
                {
                    TrackWeekendRecords ts = tv.trackWeekendRecords[k];

                    stream << ts.year;

                    for (int w = 0; w < 4; ++w)
                    {
                        stream << ts.sessionRecords[w].time;
                        stream << ts.sessionRecords[w].driver;
                        stream << ts.sessionRecords[w].team;
                        stream << ts.sessionRecords[w].session;
                    }

                    stream << ts.driverRecords.size();

                    for (int w = 0; w < ts.driverRecords.size(); ++w)
                    {
                        DriverWeekendRecords ds = ts.driverRecords[w];

                        stream << ds.driver;
                        stream << ds.team;

                        for (int x = 0; x < 5; ++x)
                        {
                            for (int y = 0; y < 4; ++y)
                            {
                                stream << ds.sessionRecords[x][y].time;
                                stream << ds.sessionRecords[x][y].session;
                            }
                        }
                    }
                }
            }
        }
    }
    else
        return false;

    return true;
}

void TrackRecords::getCurrentTrackRecords(Track **track, TrackWeekendRecords **twr, TrackVersion **tv)
{
    LTEvent event = EventData::getInstance().getEventInfo();

    for (int i = 0; i < trackRecords.size(); ++i)
    {
        if (trackRecords[i].name == event.eventPlace)
        {
            int j = 0;
            for (; j < trackRecords[i].trackVersions.size(); ++j)
            {
                if (trackRecords[i].trackVersions[j].year > event.fpDate.year())
                    break;

                TrackWeekendRecords *t = &trackRecords[i].trackVersions[j].getTrackWeekendRecords(event.fpDate.year());

                if (*t != TrackWeekendRecords::null())
                {
                    *track = &trackRecords[i];
                    *twr = t;
                    *tv = &trackRecords[i].trackVersions[j];
                    return;
                }
            }
            //if no records from this year were found, they have to be added
            --j;

            TrackWeekendRecords tr;
            tr.year = event.fpDate.year();
            trackRecords[i].trackVersions[j].trackWeekendRecords.append(tr);

            qSort(trackRecords[i].trackVersions[j].trackWeekendRecords);

            TrackWeekendRecords *t = &trackRecords[i].trackVersions[j].getTrackWeekendRecords(event.fpDate.year());
            if (*t != TrackWeekendRecords::null())
            {
                *track = &trackRecords[i];
                *twr = t;
                *tv = &trackRecords[i].trackVersions[j];
                return;
            }
        }
    }
}

QString TrackRecords::getCurrentSessionAsString()
{
    EventData &ed = EventData::getInstance();
    QString tmp;
    switch (ed.getEventType())
    {
        case LTPackets::PRACTICE_EVENT:
            tmp = "FP" + QString::number(ed.getFPNumber());
            break;

        case LTPackets::QUALI_EVENT:
            tmp = "Q" + QString::number(ed.getQualiPeriod());
            break;

        case LTPackets::RACE_EVENT:
            tmp = "R";
            break;
    }
    return tmp;
}

eSession TrackRecords::getSessionFromString(QString ses)
{
    if (ses == "FP1")
        return FP1;

    if (ses == "FP2")
        return FP2;

    if (ses == "FP3")
        return FP3;

    if (ses.contains("Q"))
        return QUALI;

    if (ses == "RACE")
        return RACE;

    return FP1;
}

eSession TrackRecords::getCurrentSession()
{
    EventData &ed = EventData::getInstance();
    switch (ed.getEventType())
    {
        case LTPackets::PRACTICE_EVENT:
            return (eSession)(ed.getFPNumber() - 1);

        case LTPackets::QUALI_EVENT:
            return QUALI;

        case LTPackets::RACE_EVENT:
            return RACE;
    }

    return FP1;
}

void TrackRecords::gatherSessionRecords(bool withDriverRecords)
{
    EventData &ed = EventData::getInstance();
    TrackWeekendRecords *twr = 0;
    TrackVersion *tv = 0;
    Track *track;
    getCurrentTrackRecords(&track, &twr, &tv);

    if (twr != 0 && tv != 0)
    {
        for (int i = 0; i < NUM_SECTORS; ++i)
        {
            LapTime sessionRecord = ed.getSessionRecords().getSectorRecord(i+1).getTime();
            if (i == 3)
                sessionRecord = ed.getSessionRecords().getFastestLap().getTime();

            if (sessionRecord.isValid() && sessionRecord <= twr->sessionRecords[i].time)
            {
                twr->sessionRecords[i].time = sessionRecord;

                int no;
                if (i < 3)
                {
                    twr->sessionRecords[i].driver = ed.getSessionRecords().getSectorRecord(i+1).getDriverName();
                    no = ed.getSessionRecords().getSectorRecord(i+1).getNumber();
                }
                else
                {
                    twr->sessionRecords[i].driver = ed.getSessionRecords().getFastestLap().getDriverName();
                    no = ed.getSessionRecords().getFastestLap().getNumber();
                }                
                twr->sessionRecords[i].team = SeasonData::getInstance().getTeamName(no);
                twr->sessionRecords[i].session = getCurrentSessionAsString();
            }
        }

        if (ed.getEventType() != LTPackets::PRACTICE_EVENT)
        {
            LapTime sessionRecord = ed.getSessionRecords().getFastestLap().getTime();

            eTrackRecord currEvent = QUALI_RECORD;
            if (ed.getEventType() == LTPackets::RACE_EVENT)
                currEvent = RACE_RECORD;

            if (sessionRecord.isValid() && sessionRecord < tv->trackRecords[currEvent].time)
            {
                tv->trackRecords[currEvent].time = sessionRecord;
                tv->trackRecords[currEvent].driver = ed.getSessionRecords().getFastestLap().getDriverName();
                int no = ed.getSessionRecords().getFastestLap().getNumber();
                tv->trackRecords[currEvent].team = SeasonData::getInstance().getTeamName(no);
                tv->trackRecords[currEvent].year = QString::number(ed.getEventInfo().fpDate.year());
            }
        }

        if (withDriverRecords)
            gatherDriverRecords(twr, tv);
    }
}

void TrackRecords::gatherDriverRecords(TrackWeekendRecords *twr, TrackVersion *tv)
{
    EventData &ed = EventData::getInstance();
    if (twr != 0 && tv != 0)
    {
        int k = 0;
        for (int i = 0; i < 30; ++i)
        {
            DriverData *dd = ed.getDriverDataPtr(i+1);

            if (dd == 0)
                continue;

            DriverWeekendRecords dsRecords;

            if (k < twr->driverRecords.size())
            {
                dsRecords = twr->driverRecords[k];

                //to keep the records of test drivers too, we have to insert them between usual drivers
                if (dsRecords.driver != dd->getDriverName())
                    ++k;

                if (k < twr->driverRecords.size())
                {
                    dsRecords = twr->driverRecords[k];
                    if (dsRecords.driver != dd->getDriverName())
                        twr->driverRecords.insert(k, dsRecords);
                }
            }

            dsRecords.driver = dd->getDriverName();
            dsRecords.team = SeasonData::getInstance().getTeamName(i+1);

            for (int j = 0; j < 4; ++j)
            {
                LapTime driverRecord = dd->getSessionRecords().getBestSector(j+1).first;
                if (j == 3)
                {
                    driverRecord = dd->getSessionRecords().getBestLap().getTime();
                }

                if (driverRecord.isValid() && driverRecord <= dsRecords.sessionRecords[getCurrentSession()][j].time)
                {                    
                    dsRecords.sessionRecords[getCurrentSession()][j].time = driverRecord;
                    dsRecords.sessionRecords[getCurrentSession()][j].session = getCurrentSessionAsString();

                    if (ed.getEventType() == LTPackets::QUALI_EVENT)
                    {
                        dsRecords.sessionRecords[getCurrentSession()][j].session = "Q" +
                                QString::number(dd->getSessionRecords().getBestLap().getQualiLapExtraData().getQualiPeriod());
                    }
                }                
            }

            if (twr->driverRecords.size() <= k)
                twr->driverRecords.append(dsRecords);
            else
                twr->driverRecords[k] = dsRecords;

            ++k;
        }
    }
}

//TrackRecordsAtom TrackRecords::nullRecord;


void Track::getTrackRecords(TrackVersion **tv, TrackWeekendRecords **trw, int year)
{
    int j = 0;
    for (; j < trackVersions.size(); ++j)
    {
        if (trackVersions[j].year > year)
            break;

        TrackWeekendRecords *t = &trackVersions[j].getTrackWeekendRecords(year);

        if (*t != TrackWeekendRecords::null())
        {
            *trw = t;
            *tv = &trackVersions[j];
            return;
        }
    }
}
