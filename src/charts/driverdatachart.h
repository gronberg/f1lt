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


#ifndef DriverDataChart_H
#define DriverDataChart_H

#include <QList>
#include <QWidget>

#include "chartwidget.h"

#include "../core/eventdata.h"

struct PopupInfoBox
{
    PopupInfoBox() : width(0) { }
    PopupInfoBox(int w) : width(w) { }
    virtual ~PopupInfoBox() { }
    void paint(QPainter *p, int x, int y, const QRect &paintRect);

    virtual int getSize()
    {
        return values.size();
    }

    virtual QString getInfo(int i) = 0;

    class SortPred
    {
    public:
        virtual bool operator()(LapData &ld1, LapData &ld2)
        {
            return ld1.getPosition() < ld2.getPosition();
        }
    };
    virtual void sortValues()
    {
        qSort(values.begin(), values.end(), SortPred());
    }
    QList< LapData > values;
    int width;
    QString title;
};

struct PopupGapInfoBox : public PopupInfoBox
{
    PopupGapInfoBox() : PopupInfoBox()  { width = 110; }
    virtual QString getInfo(int i)
    {
        QString gap = values[i].getGap();
        if (values[i].getPosition() == 1)
            gap = "Leader";
        if (gap == "")
            gap = "1L";

        return QString("LAP %1: %2").arg(values[i].getLapNumber()).arg(gap);
    }
};

struct PopupLapTimeInfoBox : public PopupInfoBox
{
    PopupLapTimeInfoBox() : PopupInfoBox() { width = 150; }
    virtual QString getInfo(int i)
    {
        QString lap = values[i].getTime().toString();

        return QString("LAP %1: %2").arg(values[i].getLapNumber()).arg(lap);
    }
};

struct LapDataCoordinates
{
    LapDataCoordinates() : idx(0), x(0), y(0) { }
    LapDataCoordinates(int a, int b, int c) : idx(a), x(b), y(c) { }

    int idx;
    int x, y;    
};

class DriverDataChart : public ChartWidget
{
    Q_OBJECT
public:
//    explicit DriverDataChart(QWidget *parent = 0);
    DriverDataChart(double, double, QColor, QWidget *parent = 0);
    virtual ~DriverDataChart() { if (popupBox != 0) delete popupBox; }
    
    virtual void setData(DriverData *d) { driverData = d; }
    virtual void clearData() { driverData = 0; }
    void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p);
    virtual void drawChart(QPainter *p);
    virtual void drawLegend(QPainter *p);

    virtual void drawSCLap(QPainter *p, const LapData &ld, double xFactor);
    virtual void drawRetire(QPainter *p, int x, int y, int r, const LapData &ld);
    virtual void drawIntoImage(QImage &img);

    virtual void calculateTransformFactors();
    virtual void resetZoom();

    void pushScaleRect();

    virtual int checkLapDataCoordinates(int x, int y);

    virtual void clearLapDataCoordinates(int to)
    {
        for (int i = lapDataCoordinates.size()-1; i >= to; --i)
            lapDataCoordinates.removeAt(i);
    }

protected:
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void paintEvent(QPaintEvent *);


    virtual void resetPaintRect()
    {
        paintRect = QRect(27, 10, width()-32, height()-35);
    }

    DriverData *driverData;

    QList<LapDataCoordinates> lapDataCoordinates;

    int mousePosX, mousePosY;
    PopupInfoBox *popupBox;
    bool repaintPopup;    
};


class LapTimeChart : public DriverDataChart
{
    Q_OBJECT

public:
    LapTimeChart(QColor *col, QWidget *parent = 0) : DriverDataChart(0, 180, col[0], parent)
    {
        for (int i = 0; i < 5; ++i)
            colors[i] = col[i];

        popupBox = new PopupLapTimeInfoBox();
    }    

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);

    void setData(DriverData *dd);

    void resetZoom();

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(28, 10, width()-33, height()-35);
    }

private:
    QColor colors[5];
};

class GapChart : public DriverDataChart
{
    Q_OBJECT
public:
    GapChart(QColor col, QWidget *parent = 0) : DriverDataChart(0, 60, col, parent)
    {
        popupBox = new PopupGapInfoBox;
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);

    void resetZoom();


protected:
    void paintEvent(QPaintEvent *);
    virtual void resetPaintRect()
    {
        paintRect = QRect(28, 10, width()-33, height()-35);
    }
};


#endif // DriverDataChart_H
