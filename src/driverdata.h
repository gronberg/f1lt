#ifndef DRIVERDATA_H
#define DRIVERDATA_H

#include <QPair>
#include <QTime>

#include "ltdata.h"

struct EventData;

class LapTime
{
public:
    LapTime() { }
    LapTime(QString t) { time = t; }
    LapTime(int msecs);
    LapTime(int m, int s, int ms)
    {
        QString strM = QString::number(m);
        QString strS = QString::number(s);
        QString strMS = QString::number(ms);

        time = strM + (s < 10 ? ":0": ":") + strS + (ms < 100 ? (ms < 10 ? ".00" : ".0") : ".")  + strMS;
    }

    QString toString() const { return time; }
    int toMsecs() const;
    QString toSecs() const;
    double toDouble() const
    {
        if (isValid())
            return (double)(toMsecs() / 1000.0);

        return 0.0;
    }

    LapTime calc107p()
    {
        double msecs = toMsecs();
        msecs = msecs * 1.07;

        return LapTime((int)(round(msecs)));
    }

    bool isValid() const
    {
        if (time == "")
            return false;

        bool ok;
        int idx = time.indexOf(":");

        if (idx > -1)
        {
            time.left(idx).toInt(&ok);
            if (!ok)
                return false;
        }

        int idx2 = time.indexOf(".", (idx < 0 ? 0 : idx));
        if (idx2 > -1)
        {
            time.mid(idx+1, idx2-idx-1).toInt(&ok);

            if (!ok)
                return false;

            time.right(time.size() - idx2 - 1).toInt(&ok);

            if (!ok)
                return false;
        }
        else
            return false;

        return true;
    }

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

//--------------------------------------------------------------------------------

struct LapData
{
    LapData() : carID(-1), pos(-1), numLap(0), qualiPeriod(0), scLap(false), approxLap(false) {}

    bool operator!=(const LapData ld)
    {
        if (gap.compare(ld.gap) != 0 ||
            interval.compare(ld.interval) != 0 ||
            lapTime.toString().compare(ld.lapTime.toString()) != 0 ||
            sector1.toString().compare(ld.sector1.toString()) != 0 ||
            sector2.toString().compare(ld.sector2.toString()) != 0 ||
            sector3.toString().compare(ld.sector3.toString()) != 0
           )
            return true;

        return false;
    }

    bool operator<(const LapData ld) const
    {
        return lapTime < ld.lapTime;
    }

//    void clearLapData()
//    {
//        sector1 = LapTime();
//        sector2 = LapTime();
//        sector3 = LapTime();
//    }

    static QString sumSectors(QString s1, QString s2, QString s3)
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

    int carID;
    int pos;
    QString gap;
    QString interval;

    LapTime lapTime;
    LapTime sector1;
    LapTime sector2;
    LapTime sector3;
    int numLap;

    QTime sessionTime;      //the time when driver set this lap (as remaining time) - used in practice and quali
    int qualiPeriod;        //only for quali - during which period this time was set
    bool scLap;             //only for race - indicates if current lap was behind the safety car
    bool approxLap;         //only for practice and quali - if the current lap was worse than the best lap we can only calculate approximate lap time from the sectors time
};

//--------------------------------------------------------------------------------

struct PitData
{
    PitData() : pitLap (-1) { }
    PitData(QString t, int p) : pitTime(t), pitLap(p) {}

    bool operator < (const PitData pd) const
    {
        return pitLap < pd.pitLap ? true : false;
    }       

    QString pitTime;
    int pitLap;
};

//--------------------------------------------------------------------------------

struct DriverData
{
    DriverData() : carID(-1), number(-1), pos(-1), numPits(0), retired(false), releasedFromPits(false)
    {
        for (int i = 0; i <14; ++i)
            colorData[i] = LTData::DEFAULT;

        lastLap.carID = carID;

        for (int i = 0; i < 3; ++i)
            bestSectors[i].second = 0;
    }

    DriverData &operator=(const DriverData &dd);

    bool operator<(const DriverData &dd) const
    {
        if (pos < 0)
            return false;

        if (dd.pos < 0)
            return true;

        return (pos < dd.pos) ? true : false;
    }
    static QString calculateGap(LapTime lap1, LapTime lap2)
    {
        if (lap1.isValid() && lap2.isValid())
        {
            double d = lap1.toDouble()-lap2.toDouble();

//            if (d != 0)
            return QString::number(d, 'f', 3);
        }
        return "";
    }

    //used in the head2head dialog - finds the best time and sets the differences between the best time and the others (max 4 lap times)
    static int lapDiff(LapTime *lap)
    {
        int msec;

        if (!lap[0].isValid())
            msec = LapTime("59:59.999").toMsecs();
        else
            msec = lap[0].toMsecs();

        int idx = 0;

        for (int i = 1; i < 4; ++i)
        {
            if (lap[i].isValid() && lap[i].toMsecs() < msec)
            {
                idx = i;
                msec = lap[i].toMsecs();
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            if (i != idx && lap[i].isValid())
                lap[i] = lap[i] - lap[idx];
        }
        return idx;
    }

    void addLap(const EventData &ed);
    void correctNumLap(int raceNumLap);
    void updateLastLap();
    void updateInPit();
    void updateGaps(const EventData &ed);

    void addPitStop(const PitData &pd)
    {
        if (pd.pitLap == 0)
            return;
        for (int i = 0; i < pitData.size(); ++i)
        {
            if (pitData[i].pitLap == pd.pitLap)
            {
                if (pitData[i].pitTime == "")
                    pitData[i].pitTime = pd.pitTime;

                return;
            }
        }
        pitData.append(pd);
        qSort(pitData);
    }

    LapData getLapData(int lap)
    {
        for (int i = 0; i < lapData.size(); ++i)
        {
            if (lap == lapData[i].numLap)
                return lapData[i];
        }
        return LapData();
    }

    void setFastestLap(LapTime lapTime, int lapNo)
    {
        if (lapNo == bestLap.numLap && lapTime == bestLap.lapTime)
            return;

        bestLap.carID = carID;
        bestLap.lapTime = lapTime;
        bestLap.numLap = lapNo;
        bestLap.sector1 = LapTime();
        bestLap.sector2 = LapTime();
        bestLap.sector3 = LapTime();
    }

    QString getPitTime(int lap)
    {
        for (int i = 0; i < pitData.size(); ++i)
        {
            if (lap == pitData[i].pitLap)
                return pitData[i].pitTime;
        }
        return "";
    }

    int carID;
    QString driver;
    int number;
    int pos;

    int numPits;

    bool retired;
    bool releasedFromPits;

    //quali additional data
    LapTime q1;
    LapTime q2;
    LapTime q3;

    QList<LapData> lapData;
    QList<int> posHistory;
    LTData::Colors colorData[14];
    QList<PitData> pitData;
    QPair<LapTime, int> bestSectors[3];

    LapData lastLap;
    LapData bestLap;
};


#endif // DRIVERDATA_H
