#include "driverdata.h"
#include "eventdata.h"
#include "ltdata.h"

#include <QDebug>

LapTime::LapTime(int ms)
{
    int msec = ms;
    int sec = ms / 1000;
    int min = 0;

    if (sec > 0)
        msec = ms % (sec * 1000);


    if (sec > 60)
    {
        min = sec / 60;

        if (min > 0)
            sec = sec % (min * 60);
    }
    time = LapTime(min, sec, msec).toString();
}

//----------------------------------

int LapTime::toMsecs() const
{
    if (!isValid())
        return 0;

    int idx = time.indexOf(":");
    int min=0, sec=0, msec=0;

    if (idx > -1)
        min = time.left(idx).toInt();

    int idx2 = time.indexOf(".", idx < 0 ? 0 : idx);
    if(idx2 > -1)
    {
        sec = time.mid(idx+1, idx2 - idx - 1).toInt();
        QString strMS = time.right(time.size() - idx2 - 1);

        msec = strMS.toInt() * (strMS.size() < 3 ? (strMS.size() < 2 ? 100 : 10) : 1);
    }

    msec += sec * 1000 + min * 60000;
    return msec;
}

//----------------------------------

QString LapTime::toSecs() const
{
    double sec = (double)(toMsecs() / 1000.0);
    return QString::number(sec, 'f', 3);
}

//----------------------------------

bool LapTime::operator < (const LapTime &lt) const
{
    if (!isValid())
        return false;

    if (!lt.isValid())
        return true;

    return toMsecs() < lt.toMsecs();
}

//----------------------------------

bool LapTime::operator <= (const LapTime &lt) const
{
    if (!isValid())
        return false;

    if (!lt.isValid())
        return true;

    return toMsecs() <= lt.toMsecs();
}

//----------------------------------

bool LapTime::operator == (const LapTime &lt) const
{
    if (isValid() && lt.isValid())
        return toMsecs() == lt.toMsecs();

    return false;
}

//----------------------------------

LapTime LapTime::operator - (const LapTime &lt) const
{
    return LapTime(toMsecs() - lt.toMsecs());
}

//----------------------------------

LapTime LapTime::operator + (const LapTime &lt) const
{
    return LapTime(toMsecs() + lt.toMsecs());
}

//==================================================================

DriverData &DriverData::operator=(const DriverData &dd)
{
    if (this == &dd)
        return *this;

    carID = dd.carID;

    number = dd.number;
    pos = dd.pos;

    driver = dd.driver;

    numPits = dd.numPits;

    retired = dd.retired;

    pitData = dd.pitData;

    releasedFromPits = dd.releasedFromPits;

    for (int i = 0; i <14; ++i)
        colorData[i] = dd.colorData[i];

    lapData = dd.lapData;
    lastLap = dd.lastLap; //&lapData[lapData.size()-1];
    bestLap = dd.bestLap;

    posHistory = dd.posHistory;
    q1 = dd.q1;
    q2 = dd.q2;
    q3 = dd.q3;

    return *this;
}

//----------------------------------

//used in race - checks if the drivers numLap is correct according to the events total laps and gap to the leader
void DriverData::correctNumLap(int raceNumLap)
{
    //first of all check if we are lapped
    int lapped = 0;
    if (lastLap.gap!= "" && lastLap.gap[lastLap.gap.size()-1] == 'L')
        lapped = lastLap.gap.left(lastLap.gap.size()-1).toInt();

    //now correct the lap number, raceNumLap is obtained from the leaders interval info - that's the only way here to get it
    if (lastLap.numLap + lapped != raceNumLap)
        lastLap.numLap = raceNumLap - lapped;

    //sometimes, when the driver is close to being lapped server doesn't send his gap data,
    //so if the lastLap.numLap is greater by 2 or more laps than lapData.last().numLap - we have to correct it...
    if (lastLap.gap == "" && !lapData.isEmpty() && lastLap.numLap > lapData.last().numLap+1)
        lastLap.numLap = lapData.last().numLap+1;
}

void DriverData::addLap(const EventData &ed)
{
    //ok, this looks a bit complicated, but since the LT server doesn't give us the actuall lap number for every driver during the race we have to find another way to gather laps:
    //- first of all - don't append laps if driver has retired (it's rather obvious)
    //- don't add empty lap time, except for the first lap of the race - it's always empty
    //- if this lap is in-lap - add it, if no - add it only if we have the sector 3 time
    //- if the lapData array is empty - check if lapNum is greater than 0
    //- don't add the out lap - "OUT",
    //- finally - check if we don't try to add the same lap again, we use the gap, interval and lap time info for this
    if (ed.eventType == LTData::RACE_EVENT)
    {
        if (!retired && ed.lapsCompleted > 0 && (lastLap.lapTime.toString() != "" || lastLap.numLap == 1) &&
    //        ((lastLap.lapTime.toString() != "IN PIT" && lastLap.sector3.toString() != "") || lastLap.lapTime.toString() == "IN PIT") &&
             ((lastLap.numLap > 0 && lapData.isEmpty()) ||
             (!lapData.isEmpty() &&
    //          (lastLap.numLap > lapData.last().numLap) &&
              (lastLap.lapTime.toString() != "OUT" /*&& !(lastLap.sector3.toString() == "STOP" && lapData.last().sector3.toString() == "STOP")*/) &&
              !(lapData.last().gap == lastLap.gap && lapData.last().interval == lastLap.interval && lapData.last().lapTime == lastLap.lapTime)
               )))
        {
            //this is tricky - if driver goes to the pits, we get this info before he crosses the finish line, but not always...
            //therefore, we don't correct the lap number, assuming that everything is ok, and the lap number is last lap + 1
            if ((lastLap.lapTime.toString() != "IN PIT" && !lapData.isEmpty()) || (lapData.isEmpty() && lastLap.lapTime.toString() == "OUT"))
                correctNumLap(ed.lapsCompleted);

            //1st lap is always empty (excluding situations when driver goes to pit or retires), so if we got a valid time on the first lap
            //it means that LT server has sent us some junk (probably time from quali)
            if (lastLap.numLap == 1 && lastLap.lapTime.isValid())
                lastLap.lapTime = LapTime();

            //if this is RETIRED lap, update only the lap time
            if (lastLap.lapTime.toString() == "RETIRED" && !lapData.isEmpty())
            {
                lapData.last().lapTime = lastLap.lapTime;
                return;
            }
            //if we get "IN PIT" before driver crossed the line, we get it again after he croses, in that case update only gap and interval
            if (!lapData.empty() && lastLap.lapTime.toString() == "IN PIT" && lapData.last().lapTime.toString() == "IN PIT" && !releasedFromPits)
            {
                lapData.last().gap = lastLap.gap;
                lapData.last().interval = lastLap.interval;
                return;
            }
            //when connected to LT during the race and driver was going out of the pits we save this lap as PIT lap
            if (lastLap.lapTime.toString() == "OUT")
                lastLap.lapTime = LapTime("IN PIT");

            lastLap.carID = carID;

            if (!lapData.empty() && lapData.last().numLap >= lastLap.numLap)
                lapData.last() = lastLap;

            else
            {
                lapData.append(lastLap);
                posHistory.append(lastLap.pos);
            }

            releasedFromPits = false;

            if (lastLap < bestLap)
                bestLap = lastLap;

            //best sectors
            if (colorData[LTData::RACE_SECTOR_1] == LTData::GREEN || colorData[LTData::RACE_SECTOR_1] == LTData::VIOLET)
            {
                bestSectors[0].first = lapData.last().sector1;
                bestSectors[0].second = lapData.last().numLap;
            }
            if (colorData[LTData::RACE_SECTOR_2] == LTData::GREEN || colorData[LTData::RACE_SECTOR_2] == LTData::VIOLET)
            {
                bestSectors[1].first = lapData.last().sector2;
                bestSectors[1].second = lapData.last().numLap;
            }

//            if (lastLap.lapTime == bestLap.lapTime && lastLap.numLap == bestLap.numLap && bestLap.sector1.toString() == "")
//            {
//                bestLap.sector1 = lastLap.sector1;
//                bestLap.sector2 = lastLap.sector2;
//                bestLap.sector3 = lastLap.sector3;
//            }

            if (ed.flagStatus == LTData::SAFETY_CAR_DEPLOYED || ed.flagStatus == LTData::RED_FLAG)
                lapData.last().scLap = true;

            else
                lapData.last().scLap = false;

        }

        //driver was set out from the pits, if he pits again on the next lap we will add it
        if (lastLap.lapTime.toString() == "OUT")
            releasedFromPits = true;
    }
    else
    {
        //during practice and quali we only save timed laps
        if ((lastLap.lapTime.toString() != "") && (lapData.empty() ||
            ((lastLap.numLap > lapData.last().numLap) && lastLap.sector1.toString() != "" && lastLap.sector2.toString() != "" && lastLap.sector3.toString() != "")))
        {            
//            std::cout<<"LASTLAP="<<lastLap.lapTime.toString().toStdString()<<std::endl;

            //if decryption fails, replace the garbage we obtained with the best lap time
            if (lastLap.lapTime.toString() != "" && !lastLap.lapTime.isValid())
                lastLap.lapTime = bestLap.lapTime;

            lastLap.sessionTime = ed.remainingTime;
            lapData.append(lastLap);

            if (ed.eventType == LTData::QUALI_EVENT)
            {
                if (q1.toString() != "")
                    lapData.last().qualiPeriod = 1;

                if (q2.toString() != "")
                    lapData.last().qualiPeriod = 2;

                if (q3.toString() != "")
                    lapData.last().qualiPeriod = 3;

                //best sectors
                if ((colorData[LTData::QUALI_SECTOR_1] == LTData::GREEN || colorData[LTData::QUALI_SECTOR_1] == LTData::VIOLET) &&
                    ((lapData.last().sector1 <= bestSectors[0].first && bestSectors[0].second != 0) || bestSectors[0].second == 0))
                {
                    bestSectors[0].first = lapData.last().sector1;
                    bestSectors[0].second = lapData.last().numLap;
                }
                if ((colorData[LTData::QUALI_SECTOR_2] == LTData::GREEN || colorData[LTData::QUALI_SECTOR_2] == LTData::VIOLET) &&
                    ((lapData.last().sector2 <= bestSectors[1].first && bestSectors[1].second != 0) || bestSectors[1].second == 0))
                {
                    bestSectors[1].first = lapData.last().sector2;
                    bestSectors[1].second = lapData.last().numLap;
                }
                if ((colorData[LTData::QUALI_SECTOR_3] == LTData::GREEN || colorData[LTData::QUALI_SECTOR_3] == LTData::VIOLET) &&
                    ((lapData.last().sector3 <= bestSectors[2].first && bestSectors[2].second != 0) || bestSectors[2].second == 0))
                {
                    bestSectors[2].first = lapData.last().sector3;
                    bestSectors[2].second = lapData.last().numLap;
                }
            }
            else
            {
                if (colorData[LTData::PRACTICE_SECTOR_1] == LTData::GREEN || colorData[LTData::PRACTICE_SECTOR_1] == LTData::VIOLET)
                {
                    bestSectors[0].first = lapData.last().sector1;
                    bestSectors[0].second = lapData.last().numLap;
                }
                if (colorData[LTData::PRACTICE_SECTOR_2] == LTData::GREEN || colorData[LTData::PRACTICE_SECTOR_2] == LTData::VIOLET)
                {
                    bestSectors[1].first = lapData.last().sector2;
                    bestSectors[1].second = lapData.last().numLap;
                }
                if (colorData[LTData::PRACTICE_SECTOR_3] == LTData::GREEN || colorData[LTData::PRACTICE_SECTOR_3] == LTData::VIOLET)
                {
                    bestSectors[2].first = lapData.last().sector3;
                    bestSectors[2].second = lapData.last().numLap;
                }
            }

            if (lastLap < bestLap)
                bestLap = lapData.last();            



            //if the current lap time is the same as the best lap, probably the driver hasn't improved so we have to calculate the real lap time from the sectors time
             else if (lastLap.lapTime == bestLap.lapTime)
            {
                lapData.last().lapTime = LapData::sumSectors(lapData.last().sector1.toString(), lapData.last().sector2.toString(), lapData.last().sector3.toString());
                lapData.last().approxLap = true;
            }
            else
                lapData.last().approxLap = false;

            lapData.last().gap = QString::number((lapData.last().lapTime - ed.FLTime).toDouble());

            posHistory.append(lastLap.pos);
        }
    }
}

void DriverData::updateLastLap()
{
    if (!lapData.isEmpty() && lapData.last().numLap == lastLap.numLap)
    {
        if (lapData.last().lapTime.toString() != "IN PIT" && lapData.last().sector3.toString() == "" && lastLap.sector3.toString() != "")
        {
            if (/*lapData.last().lapTime.toString() != "IN PIT" && lapData.last().sector1.toString() == "" &&*/ lastLap.sector1.toString() != "")
                lapData.last().sector1 = lastLap.sector1;

            if (/*lapData.last().lapTime.toString() != "IN PIT" && lapData.last().sector2.toString() == "" && */lastLap.sector2.toString() != "")
                lapData.last().sector2 = lastLap.sector2;

            lapData.last().sector3 = lastLap.sector3;

            if (lapData.last().numLap == bestLap.numLap)
            {
                bestLap.sector1 = lapData.last().sector1;
                bestLap.sector2 = lapData.last().sector2;
                bestLap.sector3 = lapData.last().sector3;
            }

        }
    }
}

void DriverData::updateInPit()
{
    if (!lapData.isEmpty())
    {
        lapData.last().pos = lastLap.pos;
        lapData.last().gap = lastLap.gap;
        lapData.last().interval = lastLap.interval;
        posHistory.last() = lastLap.pos;
    }
}

//during quali and free practice - gap for all laps is calculated to the best session time
void DriverData::updateGaps(const EventData &ed)
{
    for (int i = 0; i < lapData.size(); ++i)
        lapData[i].gap = QString::number((lapData[i].lapTime - ed.FLTime).toDouble());


}
