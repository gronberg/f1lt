#include "driverradarpositioner.h"

#include <QDebug>

#include "../core/eventdata.h"

DriverRadarPositioner::DriverRadarPositioner(DriverData *dd, int x, int y, double r, double r1, double rL) :
    driverData(dd), radarX(x), radarY(y), radarR(r), radarPitR(r1), radarLappedR(rL), currentDeg(0.0),
    avgTime(100), currSector(1), currentLapTime(0), startingNewLap(false), inPits(false), lapped(false)
{
    avgSectorTimes[0] = 0;
    avgSectorTimes[1] = 0;
    sectorPositions[0] = 0;
    sectorPositions[1] = 0;
}

void DriverRadarPositioner::setDriverData(DriverData *dd)
{
    driverData = dd;
}

void DriverRadarPositioner::setStartupPosition()
{
    int laps = EventData::getInstance().getEventInfo().laps;
    lapped = false;

    sectorPositions[0] = 0;
    sectorPositions[1] = 0;

    //a very rough estimate ;)
    avgTime = 200 - 30 * log10(laps*laps);

    if (EventData::getInstance().getSessionRecords().getFastestLap().getTime().isValid())
        avgTime = EventData::getInstance().getSessionRecords().getFastestLap().getTime().toDouble();

    //if it's raining, we add 10 seconds
    if (EventData::getInstance().getWeather().getWetDry().getValue() == 1)
        avgTime += 10;

    //if SC is on track, we add 1 minute
    if (EventData::getInstance().getFlagStatus() == LTPackets::SAFETY_CAR_DEPLOYED)
        avgTime += 60;

    if (driverData && (driverData->isInPits() || driverData->isRetired()))
    {
        inPits = true;
        calculatePitPosition();
    }

    else if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT &&
        (!EventData::getInstance().isSessionStarted() || !driverData->getLastLap().getSectorTime(1).isValid()))
    {
        inPits = false;
        currentDeg = 360 - driverData->getPosition()*2;
        currentLapTime = -(avgTime-(avgTime * currentDeg) / 360);
    }
    else
    {
        currentLapTime = 0;
        for (int i = 0; i < 3; ++i)
            currentLapTime += driverData->getLastLap().getSectorTime(i+1).toDouble();

        calculatePosition();
    }
}

void DriverRadarPositioner::update()
{
    if (startingNewLap && driverData && driverData->getLastLap().getSectorTime(3).isValid())
    {
        currSector = 1;
        currentLapTime = 0;
        currentDeg = 0;
        calculateAvgs();
        startingNewLap = false;

        if (inPits)
            currentDeg = (360.0 / (double)avgTime) * 5;
    }
    else
    {        
        if (driverData->isInPits() || driverData->isRetired() ||
            (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT && EventData::getInstance().getFlagStatus() == LTPackets::RED_FLAG))
        {
            inPits = true;
            calculatePitPosition();
        }

        else
        {
            if (inPits)
                currentDeg = (360.0 / (double)avgTime) * 5;

            inPits = false;
            calculatePosition();
        }
    }
    if (!driverData->getLastLap().getSectorTime(3).isValid())
    {
        startingNewLap = true;
    }
}

void DriverRadarPositioner::calculatePosition()
{
    if (!EventData::getInstance().isSessionStarted())
        return;

    if (driverData->getLastLap().getTime().toString() != "OUT" && currentDeg > 0)
    {
        if (driverData->getLastLap().getSectorTime(1).isValid() && !driverData->getLastLap().getSectorTime(2).isValid()
                && !driverData->getLastLap().getSectorTime(3).isValid() &&
                currSector == 1)
        {
            currSector = 2;
            if (sectorPositions[0] != 0)
            {
                currentDeg = sectorPositions[0];
//                int time = 360*currentLapTime / currentDeg;
//                avgTime = (avgTime + time) / 2;
//                currentLapTime = currentDeg * avgTime / 360;
            }
//            currentDeg += 360.0 / (double)avgTime;
        }
        else if (driverData->getLastLap().getSectorTime(2).isValid() &&
                 !driverData->getLastLap().getSectorTime(3).isValid() && currSector == 2)
        {
            currSector = 3;
            if (sectorPositions[1] != 0)
            {
                currentDeg = sectorPositions[1];
//                int time = 360*currentLapTime / currentDeg;
//                avgTime = (avgTime + time) / 2;
//                currentLapTime = currentDeg * avgTime / 360;
            }
        }
        else
        {
//            if (driverData->getLastLap().getSectorTime(1).isValid())
//            {
//                sumSectors += driverData->getLastLap().getSectorTime(1).toDouble();
//                if (driverData->getLastLap().getSectorTime(2).isValid())
//                    sumSectors += driverData->getLastLap().getSectorTime(2).toDouble();

//                if (currentLapTime < sumSectors)
//                    currentLapTime = sumSectors;
//            }

            currentDeg += 360.0 / (double)avgTime; //(360 * currentLapTime) / avgTime;
        }
    }
    else
    {
//        if (currentLapTime < 0)
//            currentDeg = (360 * (avgTime + currentLapTime)) / avgTime;

//        else
            currentDeg += 360.0 / (double)avgTime;
//            currentDeg = (360 * currentLapTime) / avgTime;
    }
}

void DriverRadarPositioner::calculatePitPosition()
{
    int halfDrv = EventData::getInstance().getDriversData().size() / 2;

    if (driverData->getCarID()-1 < halfDrv)
        currentDeg = (halfDrv - driverData->getCarID()-1) * 5;
    else
        currentDeg = 360 - (driverData->getCarID()-1-halfDrv) * 5;
}

void DriverRadarPositioner::calculateAvgs()
{
    if (driverData)
    {
        double sumT = 0, sumS1 = 0, sumS2 = 0;
        int k = 0;
        for (int i = driverData->getLapData().size()-1; i >= driverData->getLapData().size()-4 && i >= 0; --i)
        {
            LapData ld = driverData->getLapData()[i];
            LapData last = driverData->getLapData().last();
            if (ld.getTime().isValid() &&
               (ld.getRaceLapExtraData().isSCLap() == last.getRaceLapExtraData().isSCLap()) &&
                    (fabs(ld.getTime().toDouble() - last.getTime().toDouble()) < 5))
            {
                sumT += ld.getTime().toDouble();
                sumS1 += ld.getSectorTime(1).toDouble();
                sumS2 += ld.getSectorTime(2).toDouble();
                ++k;
            }
        }
        if (EventData::getInstance().getSessionRecords().getFastestLap().getTime().isValid())
        {
            avgTime = EventData::getInstance().getSessionRecords().getFastestLap().getTime().toDouble();

//            int drvNo = EventData::getInstance().getSessionRecords().getFastestLap().getNumber();
//            int lapNo = EventData::getInstance().getSessionRecords().getFastestLap().getLapNumber();
//            DriverData *dd = EventData::getInstance().getDriverDataPtr(drvNo);
//            if (dd)
//            {
//                LapData ld = dd->getLapData(lapNo);

//                double s1 = ld.getSectorTime(1).toDouble();
//                double s2 = ld.getSectorTime(2).toDouble();

//                if (s1 != 0 && s2 != 0)
//                {
//                    sectorPositions[0] = (360 * s1) / avgTime;
//                    sectorPositions[1] = (360 * (s1 + s2)) / avgTime;
//                }
//            }
        }
        if (sumT != 0 && k > 0)
            avgTime = sumT / k;

        if (sumS1 != 0 && k > 0 && sumS2 != 0 && k > 0)
        {

            avgSectorTimes[0] = sumS1 / k;
            sectorPositions[0] = (360 * avgSectorTimes[0]) / avgTime;

            avgSectorTimes[1] = sumS2 / k;
            sectorPositions[1] = (360 * (avgSectorTimes[1] + avgSectorTimes[0])) / avgTime;
        }

        if (driverData->getPosition() != 1 && driverData->getLastLap().getGap().contains("L"))
            lapped = true;
        else
            lapped = false;


        if (driverData->getLastLap().getTime().toString() == "IN PIT" || driverData->isRetired())
            inPits = true;
        else
        {
            inPits = false;
        }


    }
}

void DriverRadarPositioner::paint(QPainter *p)
{
    if (driverData && driverData->getCarID() > 0)
    {
        double r = radarR;

        if (inPits)
            r = radarPitR;

        else if (lapped)
            r = radarLappedR;

        double x = radarX, y = radarY - r;

        if (currentDeg > 0)
        {
            double alpha = currentDeg / 360 * 2 * M_PI;// - quarter * 90;

            x = radarX + /*cos(currentDeg)/fabs(cos(currentDeg)) **/ sin(alpha) * r;
            y = radarY - /*sin(currentDeg)/fabs(sin(currentDeg)) **/ cos(alpha) * r;

        }


        p->setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(QPoint(x, y), 12, 12);
        QColor drvColor = SeasonData::getInstance().getCarColor(*driverData);
        p->setBrush(QBrush(drvColor));

        QPen pen(drvColor);

        if (driverData->getPosition() == 1)
        {
            pen.setColor(QColor(255, 255, 0));
            pen.setWidth(2);
        }
        p->setPen(pen);

        p->drawPath(path);

        p->setPen(QColor(20, 20, 20));


        QString number = QString::number(driverData->getNumber());

        int numX = x - p->fontMetrics().width(number)/2;
        int numY = y + 12 - p->fontMetrics().height()/2;

        p->setFont(QFont("Arial", 12, 75));

        p->setRenderHint(QPainter::Antialiasing);
        p->drawText(numX, numY, number);
    }
}
