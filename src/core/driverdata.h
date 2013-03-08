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



#ifndef DRIVERDATA_H
#define DRIVERDATA_H

#include "lapdata.h"


class DriverData;
class PacketParser;

/*!
 * \brief The PitData class stores basic informations about a pitstop - pit time and a lap when pitstop was made.
 */
class PitData
{
public:
    PitData() : pitLap (-1) { }
    PitData(const QString &t, int p) : pitTime(t), pitLap(p) {}

    const QString &getPitTime() const { return pitTime; }
    void setPitTime (const QString &pt) { pitTime = pt; }

    int getPitLap() const { return pitLap; }
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

/*!
 * \brief The ColorData class - a helper class used to color driver data output in the live timing view.
 */
class ColorData
{
public:

    ColorData()
    {
        for (int i = 0; i < 15; ++i)
            colorData[i] = LTPackets::DEFAULT;
    }

    LTPackets::Colors &positionColor() { return colorData[0]; }
    LTPackets::Colors &numberColor() { return colorData[1]; }
    LTPackets::Colors &driverColor() { return colorData[2]; }
    LTPackets::Colors &gapColor() { return colorData[3]; }
    LTPackets::Colors &intervalColor() { return colorData[4]; }
    LTPackets::Colors &lapTimeColor() { return colorData[5]; }

    LTPackets::Colors &qualiTimeColor(int idx)
    {
        switch (idx)
        {
            case 1: return colorData[5]; break;
            case 2: return colorData[13]; break;
            case 3: return colorData[14]; break;
            default: return colorData[5]; break;
        }
    }

    LTPackets::Colors &sectorColor(int idx)
    {
        if (idx >= 1 && idx <= 3)
            return colorData[5+idx];

        return colorData[6];
    }

    LTPackets::Colors &pitColor(int idx)
    {
        if (idx >= 1 && idx <= 3)
            return colorData[8+idx];

        return colorData[9];
    }
    LTPackets::Colors &numPitsColor() { return colorData[12]; }

    LTPackets::Colors &numLapsColor() { return colorData[12]; }

private:
    LTPackets::Colors colorData[15];
};

//------------------------------------------------------

/*!
 * \brief The DriverRecords class stores driver records - best lap time, best sector times and best quali times from all periods.
 */
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
        return *this;
    }

    QPair<LapTime, int> getBestSector(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return bestSectors[idx-1];

        return QPair<LapTime, int>();
    }

    int getBestSectorLapNumber(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return bestSectors[idx-1].second;

        return 0;
    }
    LapTime getBestSectorTime(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return bestSectors[idx-1].first;

        return LapTime();
    }

    const LapData &getBestLap() const { return bestLap; }

    LapData getBestQualiLap(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return bestQLaps[idx-1];

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

/*!
 * \brief The DriverData class contains all informations about a driver - his personal (name, number) and session (lap times, pit stops, position history, etc.) data
 */
class DriverData
{
public:

    DriverData() : carID(-1), number(-1), pos(-1), numPits(0), retired(false), releasedFromPits(false), inPits(true)
    {
        ++cnt;
        lastLap.setCarID(carID);
        lapData.reserve(80);
    }

    int getCarID()                  const { return carID; }
    const QString &getDriverName()  const { return driver; }
    int getNumber()                 const { return number; }
    int getPosition()               const { return pos; }

    const QVector<LapData> &getLapData()      const { return lapData; }
    const QVector<int> &getPositionHistory()  const { return posHistory; }
    const QVector<PitData> &getPitStops()     const { return pitData; }

    ColorData getColorData()    const { return colorData; }
    const LapData &getLastLap()        const { return lastLap; }

    bool isInPits() const
    {
        if (!lastLap.getTime().toString().isEmpty() &&
             (lastLap.getTime().toString() == "IN PIT" || getColorData().numberColor() == LTPackets::PIT))
            return true;

        return false;
    }
    void updatePitStatus(LTPackets::Colors pitColor, EventData &ed);

    LapTime getQualiTime(int idx) const
    {
        if (idx >= 1 && idx <= 3)
            return qualiTimes[idx-1];

        return LapTime();
    }
    const DriverRecords &getSessionRecords() const { return sessionRecords; }
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
    static QString calculateGap(const LapTime &lap1, const LapTime &lap2)
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
    void addRaceLap(const EventData &ed);
    void addFPQLap(const EventData &ed);
    void addInLap(const EventData &ed);

    void correctNumLap(int raceNumLap);
    void correctPosition(const EventData &ed);
    void updateLastLap();
    void updateInPit();
    void updateGaps(const EventData &ed);

    void updateSectorRecords();

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

    LapData getFPLapData(int min) const
    {
        for (int i = 0; i < lapData.size(); ++i)
        {
            SeasonData &sd = SeasonData::getInstance();
            int lapMin = sd.getSessionDefaults().timeToMins(sd.getSessionDefaults().correctFPTime(lapData[i].getPracticeLapExtraData().getSessionTime()));

            if (min == lapMin)
                return lapData[i];
        }
        return LapData();
    }

    LapData getQLapData(int min, int qPeriod) const
    {
        for (int i = 0; i < lapData.size(); ++i)
        {
            SeasonData &sd = SeasonData::getInstance();
            if (qPeriod == lapData[i].getQualiLapExtraData().getQualiPeriod())
            {
                int lapMin = sd.getSessionDefaults().timeToMins(sd.getSessionDefaults().correctQualiTime(lapData[i].getQualiLapExtraData().getSessionTime(), qPeriod));

                if (min == lapMin)
                    return lapData[i];
            }
        }
        return LapData();
    }

    void setFastestLap(const LapTime &lapTime, int lapNo)
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

    QString getPitTime(int lap) const
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
    static int cnt;

    int carID;
    QString driver;
    int number;
    int pos;

    QVector<LapData> lapData;
    QVector<int> posHistory;

    QVector<PitData> pitData;
    ColorData colorData;

    LapData lastLap;
    LapTime qualiTimes[3];

    DriverRecords sessionRecords;

    int numPits;

    bool retired;
    bool releasedFromPits;

    bool inPits;
    bool outLap;
};




#endif // DRIVERDATA_H
