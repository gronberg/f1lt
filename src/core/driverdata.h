#ifndef DRIVERDATA_H
#define DRIVERDATA_H

#include "lapdata.h"


class DriverData;
class PacketParser;

class PitData
{
public:
    PitData() : pitLap (-1) { }
    PitData(QString t, int p) : pitTime(t), pitLap(p) {}

    QString getPitTime() { return pitTime; }
    void setPitTime (QString pt) { pitTime = pt; }

    int getPitLap() { return pitLap; }
    void setPitLap(int pl) { pitLap = pl; }

    bool operator < (const PitData pd) const
    {
        return pitLap < pd.pitLap ? true : false;
    }           

    friend class DriverData;

private:
    QString pitTime;
    int pitLap;
};

//------------------------------------------------------

class ColorData
{
public:

    ColorData()
    {
        for (int i = 0; i < 15; ++i)
            colorData[i] = LTData::DEFAULT;
    }

    LTData::Colors &positionColor() { return colorData[0]; }
    LTData::Colors &numberColor() { return colorData[1]; }
    LTData::Colors &driverColor() { return colorData[2]; }
    LTData::Colors &gapColor() { return colorData[3]; }
    LTData::Colors &intervalColor() { return colorData[4]; }
    LTData::Colors &lapTimeColor() { return colorData[5]; }

    LTData::Colors &qualiTimeColor(int idx)
    {
        switch (idx)
        {
            case 1: return colorData[5]; break;
            case 2: return colorData[13]; break;
            case 3: return colorData[14]; break;
            default: return colorData[5]; break;
        }
    }

    LTData::Colors &sectorColor(int idx)
    {
        if (idx >= 1 && idx <= 3)
            return colorData[5+idx];

        return colorData[6];
    }

    LTData::Colors &pitColor(int idx)
    {
        if (idx >= 1 && idx <= 3)
            return colorData[8+idx];

        return colorData[9];
    }
    LTData::Colors &numPitsColor() { return colorData[12]; }

    LTData::Colors &numLapsColor() { return colorData[12]; }

private:
    LTData::Colors colorData[15];
};

//------------------------------------------------------

class DriverRecords
{
public:
    DriverRecords()
    {
        for (int i = 0; i < 3; ++i)
            bestSectors[i].second = 0;
    }

    DriverRecords &operator=(const DriverRecords &sr)
    {
        if (this == &sr)
            return *this;

        bestLap = sr.bestLap;
        for (int i = 0; i < 3; ++i)
        {
            bestSectors[i] = sr.bestSectors[i];
            bestQLaps[i] = sr.bestQLaps[i];
        }
    }

    QPair<LapTime, int> getBestSector(int idx)
    {
        if (idx >= 0 && idx < 3)
            return bestSectors[idx];

        return QPair<LapTime, int>();
    }

    int getBestSectorLapNumber(int idx)
    {
        if (idx >= 0 && idx < 3)
            return bestSectors[idx].second;

        return 0;
    }
    LapTime getBestSectorTime(int idx)
    {
        if (idx >= 0 && idx < 3)
            return bestSectors[idx].first;

        return LapTime();
    }

    LapData getBestLap() { return bestLap; }

    LapData getBestQualiLap(int idx)
    {
        if (idx >= 0 && idx < 3)
            return bestQLaps[idx];

        return LapData();
    }

    friend class DriverData;
    friend class PacketParser;

private:
    QPair<LapTime, int> bestSectors[3];
    LapData bestLap;
    LapData bestQLaps[3];
};

//------------------------------------------------------

class DriverData
{
public:

    DriverData() : carID(-1), number(-1), pos(-1), numPits(0), retired(false), releasedFromPits(false)
    {
        lastLap.setCarID(carID);
    }

    int getCarID()              const { return carID; }
    QString getDriverName()     const { return driver; }
    int getNumber()             const { return number; }
    int getPosition()           const { return pos; }

    const QList<LapData> &getLapData() const { return lapData; }
    QList<int> &getPositionHistory()  { return posHistory; }
    QList<PitData> getPitStops() const { return pitData; }

    ColorData getColorData()    const { return colorData; }
    LapData getLastLap()        const { return lastLap; }

    LapTime getQualiTime(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return qualiTimes[idx-1];

        return LapTime();
    }
    DriverRecords getSessionRecords() const { return sessionRecords; }
    int getNumPits()            const { return numPits; }

    bool isRetired()            const { return retired; }


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
    static int lapDiff(LapTime *lap);

    int getStartingPos() const
    {
        if (!posHistory.isEmpty())
            return posHistory[0];

        return 0;
    }

    void addLap(const EventData &ed);       

    void correctNumLap(int raceNumLap);
    void updateLastLap();
    void updateInPit();
    void updateGaps(const EventData &ed);

    void addPitStop(const PitData &pd);


    LapData getLapData(int lap) const
    {
        for (int i = 0; i < lapData.size(); ++i)
        {
            if (lap == lapData[i].lapNum)
                return lapData[i];
        }
        return LapData();
    }

    void setFastestLap(LapTime lapTime, int lapNo)
    {
        if (lapNo == sessionRecords.bestLap.lapNum && lapTime == sessionRecords.bestLap.lapTime)
            return;

        sessionRecords.bestLap.carID = carID;
        sessionRecords.bestLap.lapTime = lapTime;
        sessionRecords.bestLap.lapNum = lapNo;
        sessionRecords.bestLap.sectorTimes[0] = LapTime();
        sessionRecords.bestLap.sectorTimes[1] = LapTime();
        sessionRecords.bestLap.sectorTimes[2] = LapTime();
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

    friend class PacketParser;

private:
    int carID;
    QString driver;
    int number;
    int pos;

    QList<LapData> lapData;
    QList<int> posHistory;

    QList<PitData> pitData;
    ColorData colorData;

    LapData lastLap;
    LapTime qualiTimes[3];

    DriverRecords sessionRecords;

    int numPits;

    bool retired;
    bool releasedFromPits;
};


#endif // DRIVERDATA_H
