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



#ifndef WEATHERCHART_H
#define WEATHERCHART_H

#include "chartwidget.h"
#include "../core/eventdata.h"

class WeatherChart : public ChartWidget
{
    Q_OBJECT

public:
    WeatherChart(QColor col, int id, int id2, QWidget *parent = 0) : ChartWidget(0, 180, col, parent)
    {
        weatherId = id;
        wetDryId = id2;
//        menu->removeAction(zoomOutAction);
        allowedMin = 0;
    }

    virtual void drawAxes(QPainter *p);
    virtual void drawChart(QPainter *p);
    virtual void drawLegend(QPainter *) { }
    void drawLine(QPainter *p, int x1, int y1, int x2, int y2);
    virtual void setMinMax();
    virtual void setAllowedMin(int am) { allowedMin = am; }

    void resetZoom();
    virtual void calculateTransformFactors();

    QString getSessionTime(const WeatherData &wd)
    {
        QString text = QString("L%1").arg(wd.getLap());
        if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
        {
            QTime t = SeasonData::getInstance().getSessionDefaults().correctFPTime(wd.getSessionTime());
            text = QString::number(SeasonData::getInstance().getSessionDefaults().timeToMins(t)) + "'";
        }

        if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
        {
            QTime t = SeasonData::getInstance().getSessionDefaults().correctQualiTime(wd.getSessionTime(), wd.getQualiPeriod());
            text = QString("Q%1 %2'").arg(wd.getQualiPeriod()).arg(SeasonData::getInstance().getSessionDefaults().timeToMins(t));
        }
        return text;
    }

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(40, 10, width()-45, height()-35);
    }

    int weatherId;
    int wetDryId;
    int allowedMin;     //min could not be below this
};

class TempChart : public WeatherChart
{
    Q_OBJECT

public:
    TempChart(QColor col, QColor col2, int id, int tempId, int id2, QWidget *parent = 0) : WeatherChart(col, id, id2, parent)
    {
        trackTempId = tempId;
        trackTempCol = col2;
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);    
    void drawLegend(QPainter *p);
    void setMinMax();


protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(40, 10, width()-45, height()-35);
    }

private:
    int trackTempId;
    QColor trackTempCol;
};


class WetDryChart : public WeatherChart
{
    Q_OBJECT

public:
    WetDryChart(QColor col, int id, int id2, QWidget *parent = 0) : WeatherChart(col, id, id2, parent)
    {
        min = 0.0;
        max = 1.0;

        menu->removeAction(zoomOutAction);
    }


protected:
    void paintEvent(QPaintEvent *);

    virtual void mouseMoveEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseDoubleClickEvent (QMouseEvent *) {}

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void setMinMax() {}

};

#endif // WEATHERCHART_H
