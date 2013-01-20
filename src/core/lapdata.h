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



#ifndef LAPDATA_H
#define LAPDATA_H

#include <QDebug>
#include <QMetaType>
#include <QPair>
#include <QTime>

#include <cmath>

#include "seasondata.h"

struct EventData;

class DriverData;
class PacketParser;

/*!
 * \brief The LapTime class stores lap and sector times. Contains methods to easily manipulate time data, including conversion to and from seconds and miliseconds.
 * LapTime objects can be constructed using a QString in format "m:ss.zzz" (minutes, seconds, miliseconds) or from integer values.
 */
class LapTime
{
public:
    LapTime() { }
    ~LapTime() { }
    LapTime(const QString &t) { time = t; }
    LapTime(const LapTime &t) { time = t.time; }
    LapTime(int msecs);
    LapTime(int m, int s, int ms)
    {
        QString strM = QString::number(m);
        QString strS = QString::number(s);
        QString strMS = QString::number(ms);

        time = strM + (s < 10 ? ":0": ":") + strS + (ms < 100 ? (ms < 10 ? ".00" : ".0") : ".")  + strMS;
    }

    const QString &toString() const { return time; }
    int toMsecs() const;
    QString toSecs() const;
    double toDouble() const
    {
        if (isValid())
            return (double)(toMsecs() / 1000.0);

        return 0.0;
    }

    LapTime calc107p() const
    {
        double msecs = toMsecs();
        msecs = msecs * 1.07;

        return LapTime((int)(round(msecs)));
    }

    bool isValid() const;

    bool operator < (const LapTime &) const;
    bool operator <= (const LapTime &) const;
    operator QString() const
    {
        return toString();
    }
    bool operator == (const LapTime &) const;
    bool operator != (const LapTime &lt) const
    {
        return !(*this == lt);
    }
    LapTime operator - (const LapTime&) const;
    LapTime operator + (const LapTime&) const;

private:
    QString time;
};

Q_DECLARE_METATYPE(LapTime)

//-----------------------------------------------------------

/*!
 * \brief The PracticeLapExtraData class stores extra information about practice session used in LapData
 */
class PracticeLapExtraData
{
public:
    PracticeLapExtraData() : approxLap(false) { }

    const QTime &getSessionTime() const { return sessionTime; }
    void setSessionTime(QTime s) { sessionTime = s; }

    bool isApproxLap() const { return approxLap; }
    void setApproxLap(bool ap) { approxLap = ap; }

    friend class LapData;
    friend class DriverData;

protected:
    QTime sessionTime;      //the time when driver set this lap (as remaining time)
    bool approxLap;         //if the current lap was worse than the best lap we can only calculate approximate lap time from the sectors time
};

//-----------------------------------------------------------

/*!
 * \brief The QualiLapExtraData class stores extra information about quali session used in LapData
 */
class QualiLapExtraData : public PracticeLapExtraData
{
public:
    QualiLapExtraData() : qualiPeriod(1) { }

    int getQualiPeriod() const { return qualiPeriod; }
    void setQualiPeriod(int q) { qualiPeriod = q; }

    friend class LapData;
    friend class DriverData;

protected:
    int qualiPeriod;
};

//-----------------------------------------------------------

/*!
 * \brief The RaceLapExtraData class stores extra information about race session used in LapData
 */
class RaceLapExtraData
{
public:
    RaceLapExtraData() : scLap(false), pitLap(false) { }

    bool isSCLap() const { return scLap; }
    void setSCLap(bool sc) { scLap = sc; }

    bool isPitLap() const { return pitLap; }

    friend class LapData;
    friend class DriverData;

protected:
    bool scLap;
    bool pitLap;
};

//-----------------------------------------------------------

/*!
 * \brief The LapData class stores all necessary informations about a lap - lap time, sector times, lap number, gap to leader, gap to driver in front, etc.
 */
class LapData
{
public:
    LapData() : carID(-1), pos(-1), lapNum(0), gap(), interval() {}

    int getCarID() const { return carID; }
    void setCarID(int id) { carID = id; }

    int getPosition() const { return pos; }
    void setPosition(int p) { pos = p; }

    int getLapNumber() const { return lapNum; }
    void setLapNumber(int lap) { lapNum = lap; }

    const LapTime &getTime() const { return lapTime; }
    void setTime(const LapTime &lt) { lapTime = lt; }

    const QString &getGap() const { return gap; }
    void setGap(QString g) { gap = g; }

    const QString &getInterval() const { return interval; }
    void setInterval(QString i) { interval = i; }

    LapTime getSectorTime(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return sectorTimes[idx-1];

        return LapTime();
    }


    const PracticeLapExtraData &getPracticeLapExtraData() const { return practiceLapExtraData; }
    void setPracticeLapExtraData(const PracticeLapExtraData &ed) { practiceLapExtraData = ed; }

    const QualiLapExtraData &getQualiLapExtraData() const { return qualiLapExtraData; }
    void setQualiLapExtraData(const QualiLapExtraData &ed) { qualiLapExtraData = ed; }

    const RaceLapExtraData &getRaceLapExtraData() const { return raceLapExtraData; }
    void setRaceLapExtraData(const RaceLapExtraData &ed) { raceLapExtraData = ed; }

    LapTime &operator[](int idx)
    {
        if (idx >= 0 && idx <= 3)
            return sectorTimes[idx];
    }

    bool operator!=(const LapData &ld)
    {
        if (gap != ld.gap ||
            interval != ld.interval ||
            lapTime.toString().compare(ld.lapTime.toString()) != 0 ||
            sectorTimes[0].toString().compare(ld.sectorTimes[0].toString()) != 0 ||
            sectorTimes[1].toString().compare(ld.sectorTimes[1].toString()) != 0 ||
            sectorTimes[2].toString().compare(ld.sectorTimes[2].toString()) != 0
           )
            return true;

        return false;
    }

    bool operator<(const LapData &ld) const
    {
        return lapTime < ld.lapTime;
    }

    bool operator==(const LapData &ld) const
    {
        return lapTime == ld.lapTime;
    }

    static QString sumSectors(const QString &s1, const QString &s2, const QString &s3)
    {
        LapTime ls1(s1);
        LapTime ls2(s2);
        LapTime ls3(s3);

        return (ls1 + ls2 + ls3).toString();
    }
    QTime toTime() const
    {
        return QTime::fromString(lapTime.toString(), "m:ss.zzz");
    }

    friend class DriverData;
    friend class PacketParser;

private:
    int carID;
    int pos;
    int lapNum;
    LapTime lapTime;
    QString gap;
    QString interval;
    LapTime sectorTimes[3];


    PracticeLapExtraData practiceLapExtraData;
    QualiLapExtraData qualiLapExtraData;
    RaceLapExtraData raceLapExtraData;
};

#endif // LAPDATA_H
