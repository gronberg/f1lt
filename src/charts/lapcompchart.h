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



#ifndef LAPCOMPCHART_H
#define LAPCOMPCHART_H

#include "driverdatachart.h"

#include <QPair>


struct LapDataCompCoordinates : public LapDataCoordinates
{
    int driverIdx;

    LapDataCompCoordinates() : LapDataCoordinates(), driverIdx(0) { }
    LapDataCompCoordinates(int a, int b, int c, int d) : LapDataCoordinates(a, b, c), driverIdx(d) { }
};

struct LapDataGapCompCoordinates : public LapDataCompCoordinates
{
    double gap;

    LapDataGapCompCoordinates() : LapDataCompCoordinates(), gap(0.0) { }
    LapDataGapCompCoordinates(int a, int b, int c, int d, double g) : LapDataCompCoordinates(a, b, c, d), gap(g) { }
};

//------------------------------------------------

struct PopupLapTimeCompInfoBox : public PopupInfoBox
{
    PopupLapTimeCompInfoBox() : PopupInfoBox() { width = 180; }

    virtual QString getInfo(int i)
    {
        DriverData dd = EventData::getInstance().getDriverDataById(values[i].getCarID());
        if (dd.getCarID() > 0)
            return QString("%1: %2").arg(dd.getDriverName()).arg(values[i].getTime().toString());

        return QString();
    }

    virtual bool sortPred(LapData &ld1, LapData &ld2)
    {
        return ld1.getTime() < ld2.getTime();
    }
};

struct PopupGapCompInfoBox : public PopupInfoBox
{
    PopupGapCompInfoBox() : PopupInfoBox() { width = 180; }

    virtual int getSize()
    {
        return gapValues.size();
    }

    virtual QString getInfo(int i)
    {
        DriverData dd = EventData::getInstance().getDriverDataById(values[i].getCarID());
        if (dd.getCarID() > 0)
        {
            if (gapValues[i] == 0.0)
                return dd.getDriverName();

            if (gapValues[i] == 1000.0)
                return dd.getDriverName() + ": +1L <";

            return dd.getDriverName() + ": +" + QString::number(gapValues[i], 'f', 1);
        }
        return QString();
    }
    QList< double > gapValues;
};

//----------------------------------------------

class LapCompChart : public DriverDataChart
{
    Q_OBJECT

public:
    explicit LapCompChart(QWidget *parent = 0) : DriverDataChart(0, 180, QColor(), parent)
    {
        for (int i = 0; i < 4; ++i)
        {
            driverData[i] = 0;
        }

        popupBox = new PopupLapTimeCompInfoBox();
    }

    void setData(DriverData **ld);
    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void drawIntoImage(QImage &img);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    void calculateTransformFactors();
    void resetZoom();


    virtual int checkLapDataCoordinates(int x, int y);

    virtual void clearLapDataCoordinates(int to)
    {
        for (int i = lapDataCompCoordinates.size()-1; i >= to; --i)
            lapDataCompCoordinates.removeAt(i);
    }

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(37, 10, width()-42, height()-35);
    }

private:
    DriverData *driverData[4];

    QList<LapDataCompCoordinates> lapDataCompCoordinates;
};



class GapCompChart : public DriverDataChart
{
    Q_OBJECT

public:
    explicit GapCompChart(QWidget *parent = 0) : DriverDataChart(0, 0, QColor(), parent), eventData(EventData::getInstance())
    {
        popupBox = new PopupGapCompInfoBox();
    }

    void setData(int *idx) { driverIdx[0] = idx[0]; driverIdx[1] = idx[1]; }
    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void drawIntoImage(QImage &img);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    virtual int checkLapDataCoordinates(int x, int y);

    virtual void clearLapDataCoordinates(int to)
    {
        for (int i = lapDataGapCompCoordinates.size()-1; i >= to; --i)
            lapDataGapCompCoordinates.removeAt(i);
    }


    void calculateTransformFactors();
    void resetZoom();

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(37, 10, width()-42, height()-35);
    }

private:
    EventData &eventData;
    int driverIdx[2];

    QList<LapDataGapCompCoordinates> lapDataGapCompCoordinates;
};

class PosCompChart : public DriverDataChart
{
    Q_OBJECT

public:
    explicit PosCompChart(QWidget *parent = 0) : DriverDataChart(0, 180, QColor(), parent)
    {
        for (int i = 0; i < 2; ++i)
        {
            driverData[i] = 0;
        }
    }

    void setData(DriverData **ld)
    {
        for (int i = 0; i < 2; ++i)
            driverData[i] = ld[i];
    }

    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void drawIntoImage(QImage &img);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    void calculateTransformFactors();
    void resetZoom();

protected:

    virtual void resetPaintRect()
    {
        paintRect = QRect(37, 10, width()-42, height()-35);
    }
    void paintEvent(QPaintEvent *);

private:
    DriverData *driverData[2];
};



#endif // LAPCOMPCHART_H
