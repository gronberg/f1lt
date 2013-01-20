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



#ifndef FPLAPTIMESCHART_H
#define FPLAPTIMESCHART_H

#include "sessionlaptimeschart.h"

class FPLapTimesChart : public SessionLapTimesChart
{
    Q_OBJECT

public:
    FPLapTimesChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
        popupBox = new PopupDriversLapTimesInfoBox();
    }
    virtual int getSessionLength()
    {
        return SeasonData::getInstance().getSessionDefaults().getFPLength();
    }
    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
//    virtual void drawScaleRect(QPainter *p);

    virtual void calculateTransformFactors();
    virtual void resetZoom();

    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

    virtual int checkLapDataCoordinates(int x, int y);

//signals:
//    void zoomChanged(int, int, double, double);

protected:
    virtual void paintEvent(QPaintEvent *);

    void resetPaintRect()
    {
        paintRect = QRect(42, 10, width()-47, height()-35);
    }
};

class QualiLapTimesChart : public FPLapTimesChart
{
public:
    QualiLapTimesChart(QWidget *parent) : FPLapTimesChart(parent)
    {
        qualiPeriod = 1;
        first = 0;
        last = getSessionLength();
        tMin = min;
        tMax = max;        
    }

    int getMin() { return tMin == min ? -1 : tMin; }
    int getMax() { return tMax == max ? -1 : tMax; }

    virtual int getSessionLength()
    {
        return SeasonData::getInstance().getSessionDefaults().getQualiLength(qualiPeriod);
    }

    virtual int checkLapDataCoordinates(int x, int y);

//    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
    void setQualiPeriod(int q) { qualiPeriod = q; }
     virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:
    int qualiPeriod;
};


class AllQualiLapTimesChart : public QualiLapTimesChart
{
public:
    AllQualiLapTimesChart(QWidget *parent) : QualiLapTimesChart(parent)
    {
        first = 0;
        last = getSessionLength();
        tMin = min;
        tMax = max;
        qualiPeriod = 1;
    }

    virtual int getSessionLength()
    {
        int sessionLength = 0;
        for (int i = 0; i < 3; ++i)
            sessionLength += SeasonData::getInstance().getSessionDefaults().getQualiLength(i+1);

        return sessionLength;
    }

    int getMin() { return tMin == min ? -1 : tMin; }
    int getMax() { return tMax == max ? -1 : tMax; }

    virtual int checkLapDataCoordinates(int x, int y);


    virtual void drawAxes(QPainter *p, int firstMin, int lastMin);
    virtual void drawChart(QPainter *p);
    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:

};
#endif // FPLAPTIMESCHART_H
