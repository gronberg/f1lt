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


#include "driverradarpositioner.h"

#include <QDebug>

#include "../core/colorsmanager.h"
#include "../core/eventdata.h"

DriverRadarPositioner::DriverRadarPositioner(DriverData *dd, int x, int y, double r, double r1, double rL) :
    driverData(dd), radarX(x), radarY(y), radarR(r), radarPitR(r1), radarLappedR(rL), currentDeg(0.0),
    avgTime(100), currSector(1), currentLapTime(0), startingNewLap(false), inPits(false), lapped(false), finished(false), qualiOut(false), speed(1), excluded(false)
{
    avgSectorTimes[0] = 0.0;
    avgSectorTimes[1] = 0.0;
    sectorPositions[0] = 0.0;
    sectorPositions[1] = 0.0;
}

void DriverRadarPositioner::setDriverData(DriverData *dd)
{
    driverData = dd;
}

void DriverRadarPositioner::setStartupPosition()
{
    int laps = EventData::getInstance().getEventInfo().laps;
    lapped = false;
    qualiOut = false;
    inPits = false;
    finished = false;
    wetTrack = false;

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

        qDebug() << "1111" << getCoordinates().x() << getCoordinates().y();
    }

    else if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT &&
        (!EventData::getInstance().isSessionStarted() || !driverData->getLastLap().getSectorTime(1).isValid()))
    {
        inPits = false;
        currentDeg = 360 - driverData->getPosition()*2;
        currentLapTime = -(avgTime-(avgTime * currentDeg) / 360);

        qDebug() << "2222" << currentDeg << currentLapTime << getCoordinates().x() << getCoordinates().y() << radarX << radarY << radarR;
    }
    else
    {
        currentLapTime = 0;
        for (int i = 0; i < 3; ++i)
            currentLapTime += driverData->getLastLap().getSectorTime(i+1).toDouble();

        calculatePosition();

        qDebug() << "3333" << currentDeg << currentLapTime;
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
    }
//    else
    {        
        if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT && driverData->getLastLap().getSectorTime(3).isValid() &&
                EventData::getInstance().getCompletedLaps() == EventData::getInstance().getEventInfo().laps && (finished || fabs(maxDeg() - currentDeg) < 5))
        {
            currentDeg = 0;
            finished = true;
            inPits = true;
            calculatePitPosition();
        }

        else if (driverData->isInPits() || driverData->isRetired() ||
            (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT && EventData::getInstance().getFlagStatus() == LTPackets::RED_FLAG))
        {
            inPits = true;
            finished = false;            
            calculatePitPosition();
        }

        else
        {
            if (inPits)            
                calculatePitOutPosition();


            inPits = false;
            finished = false;
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
    EventData &ev = EventData::getInstance();
    if (!ev.isSessionStarted())
        return;

    if (/*driverData->getLastLap().getTime().toString() != "OUT" &&*/ currentDeg > 0)
    {
        if (driverData->getLastLap().getSectorTime(1).isValid() &&
                ((ev.getEventType() == LTPackets::RACE_EVENT && driverData->getColorData().lapTimeColor() == LTPackets::YELLOW) ||
                 (!driverData->getLastLap().getSectorTime(2).isValid() || !driverData->getLastLap().getSectorTime(3).isValid())) &&
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
                 !driverData->getLastLap().getSectorTime(3).isValid() && currSector == 2 &&
                 driverData->getLastLap().getTime().toString() != "OUT")
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

            currentDeg += (360.0 / avgTime) / speed; //(360 * currentLapTime) / avgTime;
        }
    }
    else
    {
//        if (currentLapTime < 0)
//            currentDeg = (360 * (avgTime + currentLapTime)) / avgTime;

//        else
            currentDeg += (360.0 / avgTime) / speed;
//            currentDeg = (360 * currentLapTime) / avgTime;
    }
}

void DriverRadarPositioner::calculatePitPosition()
{
//    int halfDrv = EventData::getInstance().getDriversData().size() / 2;

//    if (driverData->getCarID()-1 < halfDrv)
//        currentDeg = (halfDrv - driverData->getCarID()-1) * 5;
//    else
//        currentDeg = 360 - (driverData->getCarID()-1-halfDrv) * 5;

    int no = driverData->getNumber() - 1;
    if (no > 12)
        no -= 1;
    currentDeg = no * 7.75;

    EventData &ev = EventData::getInstance();
    if (ev.getEventType() == LTPackets::QUALI_EVENT &&
        ((ev.getQualiPeriod() == 2 && driverData->getPosition() > 17) ||
         (ev.getQualiPeriod() == 3 && driverData->getPosition() > 10)))
        qualiOut = true;
}

bool DriverRadarPositioner::isSelected(QPoint p)
{
    QPoint coord = getCoordinates();

    if (inPits)
    {
        if ((abs(p.x() - coord.x())) <= 13 && (abs(coord.y() - p.y()) <= 10))
            return true;

        if ((p.y() - coord.y() >= 0) && (p.y() - coord.y() <= 16) &&
            (coord.x() - p.x() >= 12) && (coord.x() - p.x() <= 48))
            return true;
    }
    else if ((abs(p.x() - coord.x())) <= 13 && ((coord.y() - p.y() <= 13) && (p.y() - coord.y() <= 26)))
        return true;

    return false;
}

void DriverRadarPositioner::calculateAvgs()
{
    if (driverData)
    {
        double sumT = 0, sumS1 = 0, sumS2 = 0;
        int k = 0, ks1=0, ks2=0;

        LapData last = driverData->getLastLap();

        if (!driverData->getLapData().isEmpty())
            last = driverData->getLapData().last();

        int i = driverData->getLapData().size()-1;

        //if the current lap is a second lap out of pits, synchronization has to be done to the best lap, not the last which was slow due to the pit stop
        if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT && i > 0 &&
                (driverData->getLapData()[i-1].getRaceLapExtraData().isPitLap() || driverData->getLapData()[i-1].getRaceLapExtraData().isSCLap()) &&
                !last.getRaceLapExtraData().isPitLap() &&
                !last.getRaceLapExtraData().isSCLap())
            last = driverData->getSessionRecords().getBestLap();

        //if the session wasn't dry, durig quali and fp avg time will be calculated using last lap, not the best one
        if (EventData::getInstance().getEventType() != LTPackets::RACE_EVENT && !wetTrack && EventData::getInstance().getWeather().getWetDry().getValue() < 1)
            wetTrack = true;

        for (i = driverData->getLapData().size()-1; i >= driverData->getLapData().size()-4 && i >= 0; --i)
        {
            LapData ld = driverData->getLapData()[i];            

            if ((EventData::getInstance().getEventType() != LTPackets::RACE_EVENT) && !wetTrack)
                last = driverData->getSessionRecords().getBestLap();

            if (ld.getTime().isValid() &&
               (ld.getRaceLapExtraData().isSCLap() == last.getRaceLapExtraData().isSCLap()) &&
                    (fabs(ld.getTime().toDouble() - last.getTime().toDouble()) < 5))
            {
                sumT += ld.getTime().toDouble();                
                ++k;
            }

            if (ld.getSectorTime(1).isValid() &&
                fabs(ld.getSectorTime(1).toDouble()-last.getSectorTime(1).toDouble()) < 5)
            {
                sumS1 += ld.getSectorTime(1).toDouble();
                ++ks1;
            }

            if (ld.getSectorTime(2).isValid() &&
                fabs(ld.getSectorTime(2).toDouble()-last.getSectorTime(2).toDouble()) < 5)
            {
                sumS2 += ld.getSectorTime(2).toDouble();
                ++ks2;
            }
        }
//        if (EventData::getInstance().getSessionRecords().getFastestLap().getTime().isValid())
//        {
//            avgTime = EventData::getInstance().getSessionRecords().getFastestLap().getTime().toDouble();

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
//        }
        if (sumT != 0 && k > 0)
            avgTime = sumT / k;



        if (sumS1 != 0 && ks1 > 0 && sumS2 != 0 && ks2 > 0)
        {

            avgSectorTimes[0] = sumS1 / ks1;
            sectorPositions[0] = (360.0 * avgSectorTimes[0]) / avgTime;

            avgSectorTimes[1] = sumS2 / ks2;
            sectorPositions[1] = (360.0 * (avgSectorTimes[1] + avgSectorTimes[0])) / avgTime;
        }

        if (driverData->getPosition() != 1 && driverData->getLastLap().getGap().contains("L"))
            lapped = true;
        else
            lapped = false;


        if (driverData->getLastLap().getRaceLapExtraData().isPitLap() || driverData->isRetired())
            inPits = true;
        else        
            inPits = false;


        if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT && driverData->getLastLap().getSectorTime(3).isValid() &&
                        EventData::getInstance().getCompletedLaps() == EventData::getInstance().getEventInfo().laps)
            avgTime += 60;

    }
}

QPoint DriverRadarPositioner::getCoordinates()
{
    double r = radarR;

    if (inPits/* && !qualiOut*/)
        r = radarPitR;

    else if (lapped/* || qualiOut*/)
        r = radarLappedR;

    double x = radarX, y = radarY - r;

    if (currentDeg > 0)
    {
        double alpha = currentDeg / 360 * 2 * M_PI;// - quarter * 90;

        x = radarX + /*cos(currentDeg)/fabs(cos(currentDeg)) **/ sin(alpha) * r;
        y = radarY - /*sin(currentDeg)/fabs(sin(currentDeg)) **/ cos(alpha) * r;
    }

    return QPoint(x, y);
}

void DriverRadarPositioner::paint(QPainter *p, bool selected)
{
    if (!driverData || driverData->getCarID() < 1)
        return;

    LTPackets::EventType eType = EventData::getInstance().getEventType();
    if (!excluded && (eType != LTPackets::RACE_EVENT || !driverData->isRetired()))
    {
        QPoint point = getCoordinates();

        QColor drvColor = ColorsManager::getInstance().getCarColor(driverData->getNumber());
        p->setBrush(QBrush(drvColor));        

        QPen pen(drvColor);

        if (driverData->getPosition() == 1)
        {
            pen.setColor(ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN));
            pen.setWidth(3);
        }
        if (driverData->isRetired() || qualiOut)
        {
            pen.setColor(QColor(255, 0, 0));
            pen.setWidth(3);
        }
        if (selected)
        {
            pen.setColor(ColorsManager::getInstance().getDefaultColor(LTPackets::YELLOW));
            pen.setWidth(3);
        }

        p->setPen(pen);

        QPixmap helmet = ImagesFactory::getInstance().getHelmetsFactory().getHelmet(driverData->getNumber(), 24);
        p->drawPixmap(point.x()-15, point.y()-15, helmet);

        p->setFont(QFont("Arial", 8, 75));

        QString number = SeasonData::getInstance().getDriverShortName(driverData->getDriverName());//QString::number(driverData->getNumber());

        p->setBrush(drvColor);

        if (inPits)
//            p->drawRoundedRect(point.x()-15, point.y()-8, helmet.width()-4, 14, 4, 4);
            p->drawRoundedRect(point.x()-16-helmet.width(), point.y(), helmet.width(), 14, 4, 4);

        else
            p->drawRoundedRect(point.x()-12, point.y()+15, helmet.width(), 14, 4, 4);

        p->setPen(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND));

        int numX = point.x() - 18 + p->fontMetrics().width(number)/2;
        int numY = point.y() + 20 + p->fontMetrics().height()/2;

//        int numX = point.x() - 15 + p->fontMetrics().width(number)/2;
//        int numY = point.y() - 2 + p->fontMetrics().height()/2;

        if (inPits)
        {
            numX = point.x() - helmet.width() - 22 + p->fontMetrics().width(number)/2;
            numY = point.y() + 5 + p->fontMetrics().height()/2;
        }

        p->drawText(numX, numY, number);


//        p->setPen(QColor(20, 20, 20));


//        QString name = SeasonData::getInstance().getDriverShortName(driverData->getDriverName());

//        int numX = point.x() - p->fontMetrics().width(name)/2;
//        int numY = point.y() + 12 - p->fontMetrics().height()/2;

//        p->setFont(QFont("Arial", 10, 75));

//        p->drawText(numX, numY, name);
    }
}
