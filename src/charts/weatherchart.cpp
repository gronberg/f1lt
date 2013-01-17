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


#include "weatherchart.h"
#include <QPainter>


void WeatherChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

    double yFactor = (double)(((double)paintRect.height()-40.0)/6.0);
    double yFactor2 = (double)((tMax-tMin)/6.0);
    double j = tMin;

    for (int i = paintRect.bottom(); i >= 50; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString::number(j, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    EventData &ed = EventData::getInstance();
    if (ed.getWeather().getSize(weatherId)>1)
    {
        int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / (double)sz;
        double j = first, jWD = first-1;
        double i = paintRect.left();
        int prevJ = first;

        double jFactor = sz < 5 ? 1.0 : (double)((sz-1)/6.0);
        double jWDFactor = /*ed.getWeather().getSize(wetDryId)-1*/sz < 5 ? 1.0 : (double)((/*ed.getWeather().getSize(wetDryId)-1*/sz)/6.0);

        QPixmap dryPix;
        if ((jFactor * xFactor) < 40)
            dryPix = QPixmap(":/ui_icons/weather_dry.png").scaledToWidth((jFactor * xFactor)*0.95, Qt::SmoothTransformation);
        else
            dryPix = QPixmap(":/ui_icons/weather_dry.png").scaledToWidth(40, Qt::SmoothTransformation);


        QPixmap wetPix;
        if ((jFactor * xFactor) < 40)
            wetPix = QPixmap(":/ui_icons/weather_wet.png").scaledToWidth((jFactor * xFactor)*0.95, Qt::SmoothTransformation);
        else
            wetPix = QPixmap(":/ui_icons/weather_wet.png").scaledToWidth(40, Qt::SmoothTransformation);

//        j = 0.0;
//        prevJ = 0;


        for (; i < width()-15.0 && round(j) < last + 1 && round(j) < ed.getWeather().getSize(weatherId); /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            if (round(j) == prevJ && prevJ != first)
                continue;

            i += (double)(round(j) - prevJ) * xFactor;           

            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

            if (i > paintRect.left() && i < paintRect.right()-xFactor)
            {                
                QString text = getSessionTime(ed.getWeather().getWeatherData(weatherId)[round(j)]);
                p->drawText(round(i)-10, paintRect.bottom()+15, text);
                QPen pen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);                
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
            if (/*round(jWD) < last && */round(jWD) < ed.getWeather().getSize(wetDryId) && /*round(j) < last-1 &&*/ round(j) < ed.getWeather().getSize(weatherId)-1)
            {
                int nextI = i + xFactor*jFactor;
                int cnt = 0;
                for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.getWeather().getSize(wetDryId); ++k)
                {
                    if (ed.getWeather().getWeatherData(wetDryId)[k].getValue() == 1)
                        ++cnt;
                    else
                        --cnt;
                }
                p->drawPixmap(round(i)+(nextI-round(i)-dryPix.width())/2, 5, cnt > 0 ? wetPix : dryPix);
            }
        }
    }
}

void WeatherChart::drawChart(QPainter *p)
{
    EventData &ed = EventData::getInstance();
    int sz = last - first + 1;
    if (sz > 1 && first < ed.getWeather().getSize(weatherId))
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        double xFactor = ((double)paintRect.width()) / ((double)sz);
        double yFactor = (((double)paintRect.height()-40.0) / (tMax - tMin));

        double x = paintRect.left(), j = x + xFactor;
        double y = (double)paintRect.bottom() - (double)(ed.getWeather().getWeatherData(weatherId)[first-1].getValue()-tMin) * yFactor;

        int i = first;
        for (; i < last + 1 && i < ed.getWeather().getSize(weatherId); ++i, j += xFactor)
        {
            double y2 = (double)paintRect.bottom() - (double)(ed.getWeather().getWeatherData(weatherId)[i].getValue()-tMin) * yFactor;
            if (ed.getWeather().getWeatherData(weatherId)[i].getValue() <= 0)
            {
                y2 = y;
            }

            double ty = y, ty2 = y2;
            drawLine(p, x, ty, j, ty2);

            x = j;
            y = y2;
        }
    }
}


void WeatherChart::drawLine(QPainter *p, int x1, int y1, int x2, int y2)
{
    int top = paintRect.top()+35;
    int bottom = paintRect.bottom();
    bool noTopMid = false;
    bool noBottomMid = false;

    if ((y2 < top && y1 < top) || (y2 > bottom && y1 > bottom))
        return;

    if (y2 < top)
    {
        noTopMid = true;
        y2 = top;
    }

    if (y2 > bottom)
    {
        noBottomMid = true;
        y2 = bottom;
    }

    if (y1 < top)
    {
        noTopMid = true;
        y1 = top;
    }

    if (y1 > bottom)
    {
        noBottomMid = true;
        y1 = bottom;
    }

    double midx = (x1 + x2)/2;

    if ((y1 >= y2 && !noBottomMid) || (y2 >= y1 && !noTopMid))
        p->drawLine(x1, y1, midx, y1);

    p->drawLine(midx, y1, midx, y2);

    if ((y1 >= y2 && !noTopMid) || (y2 >= y1 && !noBottomMid))
        p->drawLine(midx, y2, x2, y2);
}

void WeatherChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();

    QPainter p;
    p.begin(this);

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        setMinMax();
        resetZoom();
    }

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());

    drawAxes(&p);
    drawChart(&p);

    if (scaling)
        drawScaleRect(&p);

    p.end();
}

void WeatherChart::setMinMax()
{
    EventData &ed = EventData::getInstance();
    for (int i = 0; i < ed.getWeather().getSize(weatherId); ++i)
    {
        if (i == 0)
        {
            min = ed.getWeather().getWeatherData(weatherId)[i].getValue();
            max = ed.getWeather().getWeatherData(weatherId)[i].getValue();
        }
        else
        {
            if (ed.getWeather().getWeatherData(weatherId)[i].getValue() > 0)
            {
                if (ed.getWeather().getWeatherData(weatherId)[i].getValue() < min)
                    min = ed.getWeather().getWeatherData(weatherId)[i].getValue();

                if (ed.getWeather().getWeatherData(weatherId)[i].getValue() > max)
                    max = ed.getWeather().getWeatherData(weatherId)[i].getValue();
            }
        }
    }
//	min -= min * 0.01;
//	max += max * 0.01;    

    if (min < allowedMin)
        min = allowedMin;

    if ((max - min) < 0.6)
    {
        max += 0.3;
        min -= 0.3;
    }
}

void WeatherChart::resetZoom()
{
    setMinMax();
    first = 1;
    last = EventData::getInstance().getWeather().getSize(weatherId);
    tMin = min;
    tMax = max;
}

void WeatherChart::calculateTransformFactors()
{
    int sz = last-first+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()-40) / (double)(tMax - tMin));

    first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

    if (first >= EventData::getInstance().getWeather().getSize(weatherId))
        first = EventData::getInstance().getWeather().getSize(weatherId) - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
    if (last >= EventData::getInstance().getWeather().getSize(weatherId))
        last = EventData::getInstance().getWeather().getSize(weatherId) - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);
}


void TempChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

    double yFactor = (double)(((double)paintRect.height()-40.0)/8.0);
    double yFactor2 = (double)((tMax-tMin)/8.0);
    double j = tMin;
    for (int i = height()-25; i >= 50; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString::number(j, 'f', 1));

        if (i != height()-25)
        {
            QPen pen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    EventData &ed = EventData::getInstance();
    if (ed.getWeather().getSize(weatherId)>1 || ed.getWeather().getSize(trackTempId)>1)
    {
//        int end = std::max(ed.getWeather().getSize(weatherId), ed.weatherData[trackTempId].size());
        int id = ed.getWeather().getSize(weatherId) > ed.getWeather().getSize(trackTempId) ? weatherId : trackTempId;
        int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / (double)sz;
        double j = first, jWD = first-1;
        double i = paintRect.left();
        int prevJ = first;

        double jFactor = sz < 5 ? 1.0 : (double)((sz-1)/6.0);

        double jWDFactor = jFactor;//ed.getWeather().getSize(wetDryId)-1 < 5 ? 1.0 : (double)((ed.getWeather().getSize(wetDryId)-1)/6.0);

        QPixmap dryPix;
        if ((jFactor * xFactor) < 40)
            dryPix = QPixmap(":/ui_icons/weather_dry.png").scaledToWidth((jFactor * xFactor)*0.95, Qt::SmoothTransformation);
        else
            dryPix = QPixmap(":/ui_icons/weather_dry.png").scaledToWidth(40, Qt::SmoothTransformation);


        QPixmap wetPix;
        if ((jFactor * xFactor) < 40)
            wetPix = QPixmap(":/ui_icons/weather_wet.png").scaledToWidth((jFactor * xFactor)*0.95, Qt::SmoothTransformation);
        else
            wetPix = QPixmap(":/ui_icons/weather_wet.png").scaledToWidth(40, Qt::SmoothTransformation);

        for (; i < width()-15.0 && round(j) < last+1; /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            if (round(j) == prevJ && prevJ != first)
                continue;

            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

            if (i > paintRect.left() && i < paintRect.right()-xFactor)
            {
                QString text = getSessionTime(ed.getWeather().getWeatherData(id)[round(j)]);
                p->drawText(round(i)-10, paintRect.bottom()+15, text);
                QPen pen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
            if (round(jWD) < ed.getWeather().getSize(wetDryId) && round(j) < last-1)
            {
                int nextI = i + xFactor*jFactor;
                int cnt = 0;
                for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.getWeather().getSize(wetDryId); ++k)
                {
                    if (ed.getWeather().getWeatherData(wetDryId)[k].getValue() == 1)
                        ++cnt;
                    else
                        --cnt;
                }
                p->drawPixmap(round(i)+(nextI-round(i)-dryPix.width())/2, 5, cnt > 0 ? wetPix : dryPix);
            }
        }
    }

}

void TempChart::drawChart(QPainter *p)
{
    EventData &ed = EventData::getInstance();
//    if (ed.getWeather().getSize(weatherId)>1)
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last - first + 1;

        if (sz <= 0)
            return;

        double xFactor1 = ((double)paintRect.width()) / (double)sz;//(ed.getWeather().getSize(weatherId));
        double yFactor = ((double)paintRect.height()-40.0) / (double)(tMax-tMin);

        double x = paintRect.left(), j1 = x + xFactor1;

        double y1 = (double)paintRect.bottom();// - (double)(ed.getWeather().getWeatherData(weatherId)[first].getValue()-tMin) * yFactor;
        double y2 = (double)paintRect.bottom();// - (double)(ed.getWeather().getWeatherData(trackTempId)[first].getValue()-tMin) * yFactor;

        if (first < ed.getWeather().getWeatherData(weatherId).size())
            y1 -= (double)(ed.getWeather().getWeatherData(weatherId)[first].getValue()-tMin) * yFactor;

        if (first < ed.getWeather().getWeatherData(trackTempId).size())
            y2 -= (double)(ed.getWeather().getWeatherData(trackTempId)[first].getValue()-tMin) * yFactor;

        int i = first;
        pen.setColor(color);
        p->setPen(pen);

        int end = ed.getWeather().getWeatherData(weatherId).size() > ed.getWeather().getWeatherData(trackTempId).size() ?
                    ed.getWeather().getWeatherData(weatherId).size() : ed.getWeather().getWeatherData(trackTempId).size();

        for (; i < last + 1 && i < end; ++i, j1 += xFactor1)
        {
            double y3 = (double)paintRect.bottom();// - (double)(ed.getWeather().getWeatherData(weatherId)[i].getValue()-tMin) * yFactor;
            double y4 = (double)paintRect.bottom();// - (double)(ed.getWeather().getWeatherData(trackTempId)[i].getValue()-tMin) * yFactor;

            if (i < ed.getWeather().getWeatherData(weatherId).size())
                y3 -= (double)(ed.getWeather().getWeatherData(weatherId)[i].getValue()-tMin) * yFactor;

            if (i < ed.getWeather().getWeatherData(trackTempId).size())
                y4 -= (double)(ed.getWeather().getWeatherData(trackTempId)[i].getValue()-tMin) * yFactor;

            double ty1 = y1, ty3 = y3, ty2 = y2, ty4 = y4;

            pen.setColor(color);
            p->setPen(pen);
            drawLine(p, x, ty1, j1, ty3);

            pen.setColor(trackTempCol);
            p->setPen(pen);
            drawLine(p, x, ty2, j1, ty4);

            x = j1;
            y1 = y3;
            y2 = y4;
        }
//        i = 1;
//        x = paintRect.left();

//        pen.setColor(trackTempCol);
//        p->setPen(pen);
//        for (; i < ed.weatherData[trackTempId].size(); ++i, j2 += xFactor2)
//        {
//            double y4 = (double)paintRect.width() - (double)(ed.weatherData[trackTempId][i].getValue()-tMin) * yFactor;
//            double midx = (j2 + x)/2;

//            p->drawLine(x, y2, midx, y2);
//            p->drawLine(midx, y2, midx, y4);
//            p->drawLine(midx, y4, j2, y4);

//            x = j2;
//            y2 = y4;
//        }
    }
}


void TempChart::drawLegend(QPainter *p)
{
    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT));
    p->drawRect(width()-85, height()-80, 80, 50);

    p->setPen(color);
    p->drawText(width()-80, height()-60, "Air temp");

    p->setPen(trackTempCol);
    p->drawText(width()-80, height()-40, "Track temp");
}

void TempChart::paintEvent(QPaintEvent *pe)
{
    WeatherChart::paintEvent(pe);
//    resetPaintRect();

    QPainter p;
    p.begin(this);

//    p.setBrush(QColor(20,20,20));
//    p.setPen(QColor(20,20,20));
//    p.drawRect(0, 0, width(), height());
//    setMinMax();
//    drawAxes(&p);
//    drawChart(&p);
    drawLegend(&p);

    p.end();
}

void TempChart::setMinMax()
{
    EventData &ed = EventData::getInstance();
    int end = std::max(ed.getWeather().getSize(weatherId), ed.getWeather().getSize(trackTempId));
    max = 0.0;
    min = 100.0;
    for (int i = 0; i < end; ++i)
    {
//        if (i == 0)
//        {
//            max = std::max(ed.getWeather().getWeatherData(weatherId)[i].getValue(), ed.getWeather().getWeatherData(trackTempId)[i].getValue());
//        }
//        else
        {
            if (i < ed.getWeather().getSize(weatherId) && ed.getWeather().getWeatherData(weatherId)[i].getValue() > max)
                max = ed.getWeather().getWeatherData(weatherId)[i].getValue();

            if (i < ed.getWeather().getSize(trackTempId) && ed.getWeather().getWeatherData(trackTempId)[i].getValue() > max)
                max = ed.getWeather().getWeatherData(trackTempId)[i].getValue();

            if (i < ed.getWeather().getSize(weatherId) && ed.getWeather().getWeatherData(weatherId)[i].getValue() < min)
                min = ed.getWeather().getWeatherData(weatherId)[i].getValue();

            if (i < ed.getWeather().getSize(trackTempId) && ed.getWeather().getWeatherData(trackTempId)[i].getValue() < min)
                min = ed.getWeather().getWeatherData(trackTempId)[i].getValue();
        }
    }
    max += max * 0.05;
    min -= min * 0.15;
}


void WetDryChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(40, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(40, height()-25, 40, 10);

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

    double yFactor = (double)((height()-75.0)/2.0);

    p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));
    p->drawText(5, yFactor+yFactor*0.5, "Dry");
    p->drawText(5, yFactor-yFactor*0.5, "Wet");

    QPen pen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT)));
    pen.setStyle(Qt::DashLine);
    p->setPen(pen);
    p->drawLine(40, yFactor+yFactor*0.5, width()-5, yFactor+yFactor*0.5);
    p->drawLine(40, yFactor-yFactor*0.5, width()-5, yFactor-yFactor*0.5);


    EventData &ed = EventData::getInstance();
    if (ed.getWeather().getSize(weatherId)>1)
    {
        double xFactor = ((double)width()-45.0) / (double)ed.getWeather().getSize(weatherId);
        double j = 1.0, jWD = 0.0;
        double i = 40.0;
        int prevJ = 1;

        double jFactor = ed.getWeather().getSize(weatherId) < 5 ? 1.0 : (double)((ed.getWeather().getSize(weatherId)-1)/6.0);
        double jWDFactor = ed.getWeather().getSize(wetDryId)-1 < 5 ? 1.0 : (double)((ed.getWeather().getSize(wetDryId)-1)/6.0);

        QPixmap dryPix;
        if ((jFactor * xFactor) < 40)
            dryPix = QPixmap(":/ui_icons/weather_dry.png").scaledToWidth((jFactor * xFactor)*0.95, Qt::SmoothTransformation);
        else
            dryPix = QPixmap(":/ui_icons/weather_dry.png").scaledToWidth(40, Qt::SmoothTransformation);


        QPixmap wetPix;
        if ((jFactor * xFactor) < 40)
            wetPix = QPixmap(":/ui_icons/weather_wet.png").scaledToWidth((jFactor * xFactor)*0.95, Qt::SmoothTransformation);
        else
            wetPix = QPixmap(":/ui_icons/weather_wet.png").scaledToWidth(40, Qt::SmoothTransformation);

        j = 0.0;
        prevJ = 0;


        for (; i < width()-15.0 && round(j) < ed.getWeather().getSize(weatherId); /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            if (round(j) == prevJ && prevJ != 0)
                continue;

            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));

//			p->drawText(round(i)-5, height()-10, QString("%1").arg(ed.getWeather().getWeatherData(weatherId)[round(j)]));

            if (i > 40 && i < paintRect.right()-xFactor)
            {
                QString text = getSessionTime(ed.getWeather().getWeatherData(weatherId)[round(j)]);
                p->drawText(round(i)-10, paintRect.bottom()+15, text);
                QPen pen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
            if (round(jWD) < ed.getWeather().getSize(wetDryId) && round(j) < ed.getWeather().getSize(weatherId)-1)
            {
                int nextI = i + xFactor*jFactor;

                int cnt = 0;
                for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.getWeather().getSize(wetDryId); ++k)
                {
                    if (ed.getWeather().getWeatherData(wetDryId)[k].getValue() == 1)
                        ++cnt;
                    else
                        --cnt;
                }
                p->drawPixmap(round(i)+(nextI-round(i)-dryPix.width())/2, 5, cnt > 0 ? wetPix : dryPix);
            }
        }
    }

}

void WetDryChart::drawChart(QPainter *p)
{
    EventData &ed = EventData::getInstance();
    if (ed.getWeather().getSize(weatherId)>1)
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        double xFactor = ((double)width()-45.0) / ((double)ed.getWeather().getSize(weatherId));
        double yFactor = (((double)height()-75.0) / 2.0);

        double x = 40.0, j = x + xFactor;
        double y;

        if (ed.getWeather().getWeatherData(weatherId)[0].getValue() == 0)
            y = yFactor + yFactor*0.5;
        else
            y = yFactor - yFactor*0.5;

        int i = 1;
        for (; i < ed.getWeather().getSize(weatherId); ++i, j += xFactor)
        {
            double y2 = (double)(height())-25.0 - (double)(ed.getWeather().getWeatherData(weatherId)[i].getValue()-min) * yFactor;
            if (ed.getWeather().getWeatherData(weatherId)[i].getValue() == 0)
                y2 = yFactor + yFactor*0.5;
            else
                y2 = yFactor - yFactor*0.5;


            double midx = (j + x)/2;

            p->drawLine(x, y, midx, y);
            p->drawLine(midx, y, midx, y2);
            p->drawLine(midx, y2, j, y2);

//            p->drawLine(x, y, j, y2);
            x = j;
            y = y2;
        }
    }
}

void WetDryChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();
    setMinMax();

    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());

    drawAxes(&p);
    drawChart(&p);

    p.end();
}
