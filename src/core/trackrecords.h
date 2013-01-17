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



#ifndef TRACKRECORDS_H
#define TRACKRECORDS_H

#include "../core/lapdata.h"
#include "../core/eventdata.h"

const int NUM_SESSIONS=5;       //FP1, FP2, FP3, Quali, Race
const int NUM_SECTORS=4;        //3 sectors + lap time

enum eTrackRecord
{
    QUALI_RECORD = 0,
    RACE_RECORD
};

enum eSectorRecord
{
    S1_RECORD = 0,
    S2_RECORD,
    S3_RECORD,
    TIME_RECORD
};

enum eSession
{
    FP1 = 0,
    FP2,
    FP3,
    QUALI,
    RACE
};

struct Record
{
    LapTime time;
    QString driver;
    QString team;
    QString year;
    QString session;
};

struct DriverWeekendRecords
{
    QString driver;
    QString team;
    Record sessionRecords[NUM_SESSIONS][NUM_SECTORS];

    DriverWeekendRecords &operator=(const DriverWeekendRecords &dr)
    {
        if (this == &dr)
            return *this;

        driver = dr.driver;
        team = dr.team;

        for (int i = 0; i < NUM_SESSIONS; ++i)
        {
            for (int j = 0; j < NUM_SECTORS; ++j)
                sessionRecords[i][j] = dr.sessionRecords[i][j];
        }

        return *this;
    }

    Record getWeekendRecord(eSectorRecord sector) const
    {
        Record rec = sessionRecords[0][sector];

        for (int i = 1; i < NUM_SESSIONS; ++i)
        {
           if (sessionRecords[i][sector].time < rec.time)
               rec = sessionRecords[i][sector];
        }
        return rec;
    }
};

struct TrackWeekendRecords
{
    int year;
    Record sessionRecords[NUM_SECTORS];

    QList<DriverWeekendRecords> driverRecords;

    static TrackWeekendRecords &null()
    {
        static TrackWeekendRecords nullTR;
        return nullTR;
    }

    bool operator==(const TrackWeekendRecords &tr) const
    {
        return year == tr.year;
    }

    bool operator!=(const TrackWeekendRecords &tr) const
    {
        return year != tr.year;
    }

    bool operator < (const TrackWeekendRecords &twr) const
    {
        return year < twr.year;
    }


    TrackWeekendRecords &operator=(const TrackWeekendRecords &tr)
    {
        if (this == &tr)
            return *this;

        year = tr.year;

        for (int i = 0; i < NUM_SECTORS; ++i)
        {
            sessionRecords[i] = tr.sessionRecords[i];
        }
        driverRecords = tr.driverRecords;

        return *this;
    }

    DriverWeekendRecords getDriverRecords(QString driver)
    {
        for (int i = 0; i < driverRecords.size(); ++i)
        {
            if (driver == driverRecords[i].driver)
                return driverRecords[i];
        }

        return DriverWeekendRecords();
    }
};

struct TrackVersion
{
    QPixmap map;
    int year;
    Record trackRecords[2];
    QList<TrackWeekendRecords> trackWeekendRecords;

    bool operator < (const TrackVersion &tr) const
    {
        return year < tr.year;
    }

    bool operator == (const TrackVersion &tr) const
    {
        return (year == tr.year);
    }

    bool operator != (const TrackVersion &tr) const
    {
        return (year != tr.year);
    }

    TrackWeekendRecords &operator[](int idx)
    {
        if (idx >= 0 && idx < trackWeekendRecords.size())
            return trackWeekendRecords[idx];

        return TrackWeekendRecords::null();
    }

    TrackVersion &operator=(const TrackVersion &tr)
    {
        if (this == &tr)
            return *this;

        year = tr.year;
        map = tr.map;

        for (int i = 0; i < 2; ++i)
            trackRecords[i] = tr.trackRecords[i];

        trackWeekendRecords = tr.trackWeekendRecords;

        return *this;
    }

    TrackWeekendRecords &getTrackWeekendRecords(int year)
    {
        for (int i = 0; i < trackWeekendRecords.size(); ++i)
        {
            if (trackWeekendRecords[i].year == year)
                return trackWeekendRecords[i];
        }
        return TrackWeekendRecords::null();
    }

    TrackWeekendRecords &removeTrackWeekendRecords(TrackWeekendRecords &tr)
    {
        int idx = trackWeekendRecords.indexOf(tr);

        if (idx != -1)
        {
            trackWeekendRecords.takeAt(idx);

            if (trackWeekendRecords.isEmpty())
                return TrackWeekendRecords::null();

            return trackWeekendRecords.first();
        }
        return tr;
    }

    static TrackVersion &null()
    {
        static TrackVersion tvNull;
        return tvNull;
    }

};

struct Track
{
    QString name;
    QList<TrackVersion> trackVersions;

    TrackVersion &operator[](int idx)
    {
        if (idx >= 0 && idx < trackVersions.size())
            return trackVersions[idx];

        return TrackVersion::null();
    }

    bool operator < (const Track &tr) const
    {
        return name < tr.name;
    }

    static Track &null()
    {
        static Track nullTrack;
        return nullTrack;
    }

    bool operator!=(const Track &tr)
    {
        return (name != tr.name || trackVersions.size() != tr.trackVersions.size());
    }

    TrackVersion &getTrackVersion(int year)
    {
        for (int i = 0; i < trackVersions.size(); ++i)
        {
            if (trackVersions[i].year == year)
                return trackVersions[i];
        }
        return TrackVersion::null();
    }

    TrackVersion &removeTrackVersion(TrackVersion &tv)
    {
        int idx = trackVersions.indexOf(tv);

        if (idx != -1)
        {
            trackVersions.takeAt(idx);

            if (trackVersions.isEmpty())
                return TrackVersion::null();

            return trackVersions.first();
        }
        return tv;
    }
};

class TrackRecords
{
public:   

    static TrackRecords &getInstance()
    {
        static TrackRecords instance;
        return instance;
    }

    bool loadTrackRecords(QString fileName);
    bool saveTrackRecords(QString fileName);

    QList<Track> &getTrackRecords() { return trackRecords; }

    Track &operator[](int idx)
    {
        if (idx >= 0 && idx < trackRecords.size())
            return trackRecords[idx];

        return Track::null();
    }

    void getCurrentTrackRecords(TrackWeekendRecords **twr, TrackVersion **tv);
    void gatherSessionRecords(bool withDriverRecords = false);
    void gatherDriverRecords(TrackWeekendRecords *twr, TrackVersion *tv);

    QStringList getTrackList()
    {
        QStringList sList;

        for (int i = 0; i < trackRecords.size(); ++i)
        {
            QString trackName = trackRecords[i].name;
            sList.append(trackName);
        }

        sList.sort();

        return sList;
    }

    QString getCurrentSessionAsString();
    eSession getCurrentSession();
    eSession getSessionFromString(QString ses);

private:    

    TrackRecords();
    QList<Track> trackRecords;

};



//struct TrackRecordsAtom
//{
//    QString trackName;
//    QPixmap trackMap;
//    Record trackRecords[2];
//    Record sessionRecords[4];
//    QList<DriverWeekendRecords> driverRecords;

////    Record getDriverSessionRecord(QString driver, eSectorRecord sector, eSession session)
////    {
////        QList<Record> foundRecords;
////        for (int i = 0; i < driverRecords.size(); ++i)
////        {
////            if (driver == driverRecords[i].driver)
////            {
////                if (session == WEEKEND)
////                    foundRecords.append(driverRecords[i].sessionRecords[sector]);
////                else
////                {
////                    if (driverRecords[i].sessionRecords[sector].session == "FP1" && session == FP1)
////                        return driverRecords[i].sessionRecords[sector];

////                    if (driverRecords[i].sessionRecords[sector].session == "FP2" && session == FP2)
////                        return driverRecords[i].sessionRecords[sector];

////                    if (driverRecords[i].sessionRecords[sector].session == "FP3" && session == FP3)
////                        return driverRecords[i].sessionRecords[sector];

////                    if (driverRecords[i].sessionRecords[sector].session == "Race" && session == Race)
////                        return driverRecords[i].sessionRecords[sector];

////                    if (driverRecords[i].sessionRecords[sector].session.contains("Q") && session == Quali)
////                        return driverRecords[i].sessionRecords[sector];
////                }
////            }
////        }

////        //search for a weekend record
////        if (session == WEEKEND && foundRecords.size() > 0)
////        {
////            Record rec = foundRecords[0];

////            for (int i = 1; i < foundRecords.size(); ++i)
////            {
////                if (foundRecords[i].time < rec.time)
////                    rec = foundRecords[i];
////            }
////            return rec;
////        }
////        return Record();
////    }

//    static TrackRecordsAtom &null()
//    {
//        static TrackRecordsAtom nullTR;
//        return nullTR;
//    }

//    bool operator < (const TrackRecordsAtom &tr) const
//    {
//        return trackName < tr.trackName;
//    }

//    bool operator == (const TrackRecordsAtom &tr) const
//    {
//        return (trackName == tr.trackName);
//    }

//    bool operator != (const TrackRecordsAtom &tr) const
//    {
//        return (trackName != tr.trackName);
//    }
//};
#endif // TRACKRECORDS_H
