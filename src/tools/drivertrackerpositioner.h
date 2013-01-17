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


#ifndef DRIVERTRACKERPOSITIONER_H
#define DRIVERTRACKERPOSITIONER_H

#include "driverradarpositioner.h"

class DriverTrackerPositioner : public DriverRadarPositioner
{
public:
    DriverTrackerPositioner(DriverData *dd = 0);

    virtual void paint(QPainter *p, bool selected = false);
    virtual QPoint getCoordinates();
    virtual QPoint getSCCoordinates();

    virtual void setStartupPosition();
    virtual void calculatePosition();
    virtual void calculatePitPosition();

    void setMapCoords(int x, int y, int px, int py, int pw, int ph)
    {
        mapX = x;
        mapY = y;
        pitX = px;
        pitY = py;
        pitW = pw;
        pitH = ph;
    }

    virtual int maxDeg()
    {
        return coordinatesCount;
    }

    virtual void calculatePitOutPosition();


private:
    int coordinatesCount;
    int mapX, mapY;
    int pitX, pitY, pitW, pitH;

};

#endif // DRIVERTRACKERPOSITIONER_H
