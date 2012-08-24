#ifndef LAPDATA_H
#define LAPDATA_H

#include <QPair>
#include <QTime>

#include "ltdata.h"

struct EventData;

class DriverData;
class PacketParser;

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

//-----------------------------------------------------------

class PracticeLapExtraData
{
public:
    PracticeLapExtraData() : approxLap(false) { }

    QTime getSessionTime() const { return sessionTime; }
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

class RaceLapExtraData
{
public:
    RaceLapExtraData() : scLap(false) { }

    bool isSCLap() const { return scLap; }
    void setSCLap(bool sc) { scLap = sc; }

    friend class LapData;
    friend class DriverData;

protected:
    bool scLap;
};

//-----------------------------------------------------------

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

    LapTime getTime() const { return lapTime; }
    void setTime(const LapTime &lt) { lapTime = lt; }

    QString getGap() const { return gap; }
    void setGap(QString g) { gap = g; }

    QString getInterval() const { return interval; }
    void setInterval(QString i) { interval = i; }

    LapTime getSectorTime(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return sectorTimes[idx-1];

        return LapTime();
    }


    PracticeLapExtraData getPracticeLapExtraData() const { return practiceLapExtraData; }
    void setPracticeLapExtraData(PracticeLapExtraData ed) { practiceLapExtraData = ed; }

    QualiLapExtraData getQualiLapExtraData() const { return qualiLapExtraData; }
    void setQualiLapExtraData(QualiLapExtraData ed) { qualiLapExtraData = ed; }

    RaceLapExtraData getRaceLapExtraData() const { return raceLapExtraData; }
    void setRaceLapExtraData(RaceLapExtraData ed) { raceLapExtraData = ed; }

    LapTime &operator[](int idx)
    {
        if (idx >= 0 && idx <= 3)
            return sectorTimes[idx];
    }

    bool operator!=(const LapData ld)
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

    bool operator<(const LapData ld) const
    {
        return lapTime < ld.lapTime;
    }

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
