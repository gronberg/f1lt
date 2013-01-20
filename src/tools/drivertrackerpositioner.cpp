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


#include "drivertrackerpositioner.h"

#include "../core/colorsmanager.h"
#include "../core/eventdata.h"

DriverTrackerPositioner::DriverTrackerPositioner(DriverData *dd) : DriverRadarPositioner(dd), coordinatesCount(0), mapX(0), mapY(0)
{
}

void DriverTrackerPositioner::setStartupPosition()
{
    int laps = EventData::getInstance().getEventInfo().laps;
    lapped = false;
    qualiOut = false;
    inPits = false;
    finished = false;

    coordinatesCount = SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates().coordinates.size();//EventData::getInstance().getEventInfo().trackCoordinates.coordinates.size();

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
        currentDeg = coordinatesCount-(round((double)driverData->getPosition()/3))-1;
        currentLapTime = -(avgTime-(avgTime * currentDeg) / coordinatesCount);
    }
    else
    {
        currentLapTime = 0;
        for (int i = 0; i < 3; ++i)
            currentLapTime += driverData->getLastLap().getSectorTime(i+1).toDouble();

        calculatePosition();
    }
}


void DriverTrackerPositioner::calculatePosition()
{
    EventData &ev = EventData::getInstance();
    if (!ev.isSessionStarted())
        return;

    TrackCoordinates trackCoordinates = SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates();
    if (/*driverData->getLastLap().getTime().toString() != "OUT" &&*/ currentDeg > 0)
    {
        if (driverData->getLastLap().getSectorTime(1).isValid() &&
                ((ev.getEventType() == LTPackets::RACE_EVENT && driverData->getColorData().lapTimeColor() == LTPackets::YELLOW) ||
                 (!driverData->getLastLap().getSectorTime(2).isValid() || !driverData->getLastLap().getSectorTime(3).isValid())) &&
                currSector == 1)
        {
            currSector = 2;
            currentDeg = trackCoordinates.indexes[0];
        }
        else if (driverData->getLastLap().getSectorTime(2).isValid() &&
                 !driverData->getLastLap().getSectorTime(3).isValid() && currSector == 2 &&
                 driverData->getLastLap().getTime().toString() != "OUT")
        {
            currSector = 3;
            currentDeg = trackCoordinates.indexes[1];
        }
        else
        {
            currentDeg += ((double)coordinatesCount / avgTime) / speed; //(360 * currentLapTime) / avgTime;
        }
    }
    else
    {
        currentDeg += ((double)coordinatesCount / avgTime) / speed;
    }

    if ((int)(round(currentDeg)) >= coordinatesCount)
        currentDeg = 0;
}

void DriverTrackerPositioner::calculatePitOutPosition()
{
    currentDeg = SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates().indexes[2];
}

void DriverTrackerPositioner::calculatePitPosition()
{
    EventData &ev = EventData::getInstance();

    if (ev.getEventType() == LTPackets::QUALI_EVENT &&
        ((ev.getQualiPeriod() == 2 && driverData->getPosition() > 17) ||
         (ev.getQualiPeriod() == 3 && driverData->getPosition() > 10)))
         qualiOut = true;
}

QPoint DriverTrackerPositioner::getCoordinates()
{
    if (inPits)
    {
        int no = driverData->getNumber() - 1;
        if (no > 12)
            no -= 1;

        int x = pitX + no * pitW / EventData::getInstance().getDriversData().size() + 13;
        int y = pitY + pitH/3;

        return QPoint(x, y);
    }

    if (coordinatesCount == 0)
        return QPoint(0, 0);

//    if ((int)round(currentDeg) >= 0 && (int)round(currentDeg) < coordinatesCount)
    {
        int idx = (int)round(currentDeg);
        if (idx == coordinatesCount)
            idx = coordinatesCount - 1;

        double x = mapX + SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates().coordinates[idx].x();
        double y = mapY + SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates().coordinates[idx].y();

        return QPoint(x, y);
    }
}

QPoint DriverTrackerPositioner::getSCCoordinates()
{
//    if (inPits)
//    {
//        int no = driverData->getNumber() - 1;
//        if (no > 12)
//            no -= 1;

//        int x = pitX + no * pitW / EventData::getInstance().getDriversData().size() + 13;
//        int y = pitY + pitH/2;

//        return QPoint(x, y);
//    }

    if (coordinatesCount == 0)
        return QPoint(0, 0);

//    if ((int)round(currentDeg) >= 0 && (int)round(currentDeg) < coordinatesCount)
    {
        int idx = (int)round(currentDeg + 5 * ((double)coordinatesCount / avgTime) / speed) % coordinatesCount;

        if (!driverData->getLapData().isEmpty() && !driverData->getLapData().last().getRaceLapExtraData().isSCLap())
            idx = 0;

        double x = mapX + SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates().coordinates[idx].x();
        double y = mapY + SeasonData::getInstance().getTrackMapsCoordinates().getCurrentTrackCoordinates().coordinates[idx].y();

        return QPoint(x, y);
    }
}

void DriverTrackerPositioner::paint(QPainter *p, bool selected)
{
    if (driverData && driverData->getCarID() > 0)
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
        p->setFont(QFont("Arial", 8, 75));

        if (!excluded)
        {
            QPixmap helmet = ImagesFactory::getInstance().getHelmetsFactory().getHelmet(driverData->getNumber(), 24);
            p->drawPixmap(point.x()-12, point.y()-12, helmet);

            QString number = SeasonData::getInstance().getDriverShortName(driverData->getDriverName());//QString::number(driverData->getNumber());

            p->setBrush(drvColor);
            p->drawRoundedRect(point.x()-12, point.y()+15, helmet.width(), 14, 4, 4);

            p->setPen(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND));

            int numX = point.x() - 18 + p->fontMetrics().width(number)/2;
            int numY = point.y() + 20 + p->fontMetrics().height()/2;
            p->drawText(numX, numY, number);
        }

        if (driverData->getPosition() == 1 && EventData::getInstance().getFlagStatus() == LTPackets::SAFETY_CAR_DEPLOYED)
        {
            point = getSCCoordinates();
            p->setPen(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND));
            p->setBrush(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::YELLOW)));
            p->drawEllipse(point, 15, 15);

            p->setFont(QFont("Arial", 12, 100));
            QString number = "SC";
            int numX = point.x() - p->fontMetrics().width(number)/2;
            int numY = point.y() + 14 - p->fontMetrics().height()/2;

            p->drawText(numX, numY, number);
        }
    }
}
