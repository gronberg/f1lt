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


#ifndef DRIVERRADARPOSITIONER_H
#define DRIVERRADARPOSITIONER_H


#include <QPainter>
#include "../core/driverdata.h"

class DriverRadarPositioner
{
public:
    DriverRadarPositioner(DriverData *dd = 0, int x = 0, int y = 0, double r = 0.0, double rP = 0.0, double rL = 0.0);
    virtual ~DriverRadarPositioner() { }

    virtual void paint(QPainter *p, bool selected = false);
    virtual QPoint getCoordinates();

    void update();


    void calculateAvgs();

    void setDriverData(DriverData *dd);

    virtual void setStartupPosition();

    void setRadarCoords(int x, int y, double r, double rP, double rL)
    {
        radarX = x;
        radarY = y;
        radarR = r;
        radarPitR = rP;
        radarLappedR = rL;
    }

    virtual void calculatePosition();
    virtual void calculatePitPosition();
    virtual void calculatePitOutPosition()
    {
        currentDeg = 0;
    }

    virtual int maxDeg()
    {
        return 360;
    }

    void setSpeed(int s)
    {
        speed = s;
    }

    int getDriverId()
    {
        if (driverData != 0)
            return driverData->getCarID();

        return -1;
    }

    virtual bool isSelected(QPoint p);

    bool isExcluded()
    {
        return excluded;
    }

    void setExcluded(bool ex)
    {
        excluded = ex;
    }

protected:
    DriverData *driverData;
    double avgTime;
    double avgSectorTimes[2];
    double sectorPositions[2];
    int currSector;
    double currentDeg;

    int currentLapTime;
    bool startingNewLap;

    bool inPits;
    bool lapped;
    bool finished;
    bool qualiOut;

    bool excluded;
    bool wetTrack;

    int speed;

private:
    int radarX, radarY;
    double radarR, radarPitR, radarLappedR;    
};

#endif // DRIVERRADARPOSITIONER_H
