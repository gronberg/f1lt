#include "weatherchart.h"
#include <QPainter>


void WeatherChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)(((double)paintRect.height()-40.0)/4.0);
    double yFactor2 = (double)((tMax-tMin)/4.0);
    double j = tMin;

    for (int i = paintRect.bottom(); i >= 50; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString::number(j, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    EventData &ed = EventData::getInstance();
    if (ed.weatherData[weatherId].size()>1)
    {
        int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / (double)sz;
        double j = first, jWD = first-1;
        double i = paintRect.left();
        int prevJ = first;

        double jFactor = sz < 5 ? 1.0 : (double)((sz-1)/6.0);
        double jWDFactor = /*ed.weatherData[wetDryId].size()-1*/sz < 5 ? 1.0 : (double)((/*ed.weatherData[wetDryId].size()-1*/sz)/6.0);

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


        for (; i < width()-15.0 && round(j) < last + 1 && round(j) < ed.weatherData[weatherId].size(); /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            if (round(j) == prevJ && prevJ != first)
                continue;

            i += (double)(round(j) - prevJ) * xFactor;           

            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));            

            if (i > paintRect.left() && i < paintRect.right()-xFactor)
            {                
                QString text = getSessionTime(ed.weatherData[weatherId][round(j)]);
                p->drawText(round(i)-10, paintRect.bottom()+15, text);
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);                
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
            if (/*round(jWD) < last && */round(jWD) < ed.weatherData[wetDryId].size() && /*round(j) < last-1 &&*/ round(j) < ed.weatherData[weatherId].size()-1)
            {
                int nextI = i + xFactor*jFactor;
                int cnt = 0;
                for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.weatherData[wetDryId].size(); ++k)
                {
                    if (ed.weatherData[wetDryId][k].value == 1)
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
    if (sz > 1 && first < ed.weatherData[weatherId].size())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        double xFactor = ((double)paintRect.width()) / ((double)sz);
        double yFactor = (((double)paintRect.height()-40.0) / (tMax - tMin));

        double x = paintRect.left(), j = x + xFactor;
        double y = (double)paintRect.bottom() - (double)(ed.weatherData[weatherId][first-1].value-tMin) * yFactor;

        int i = first;
        for (; i < last + 1 && i < ed.weatherData[weatherId].size(); ++i, j += xFactor)
        {
            double y2 = (double)paintRect.bottom() - (double)(ed.weatherData[weatherId][i].value-tMin) * yFactor;
            if (ed.weatherData[weatherId][i].value <= 0)
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
    for (int i = 0; i < ed.weatherData[weatherId].size(); ++i)
    {
        if (i == 0)
        {
            min = ed.weatherData[weatherId][i].value;
            max = ed.weatherData[weatherId][i].value;
        }
        else
        {
            if (ed.weatherData[weatherId][i].value > 0)
            {
                if (ed.weatherData[weatherId][i].value < min)
                    min = ed.weatherData[weatherId][i].value;

                if (ed.weatherData[weatherId][i].value > max)
                    max = ed.weatherData[weatherId][i].value;
            }
        }
    }
//	min -= min * 0.01;
//	max += max * 0.01;

    if (min < allowedMin)
        min = allowedMin;
}

void WeatherChart::resetZoom()
{
    setMinMax();
    first = 1;
    last = EventData::getInstance().weatherData[weatherId].size();
    tMin = min;
    tMax = max;
}

void WeatherChart::transform()
{
    if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
        return;

    if (scaleRect == QRect())
    {
        first = 1;
        last = driverData.lapData.size();
        tMin = min;
        tMax = max;
        return;
    }

    if (scaleRect.left() > scaleRect.right())
        scaleRect = QRect(scaleRect.right(), scaleRect.top(), -scaleRect.width(), scaleRect.height());

    if (scaleRect.top() > scaleRect.bottom())
        scaleRect = QRect(scaleRect.left(), scaleRect.bottom(), scaleRect.width(), -scaleRect.height());

    int sz = last-first+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()-40) / (double)(tMax - tMin));

    first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

    if (first >= EventData::getInstance().weatherData[weatherId].size())
        first = EventData::getInstance().weatherData[weatherId].size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
    if (last >= EventData::getInstance().weatherData[weatherId].size())
        last = EventData::getInstance().weatherData[weatherId].size() - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}


void TempChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)(((double)paintRect.height()-40.0)/4.0);
    double yFactor2 = (double)((tMax-tMin)/4.0);
    double j = tMin;
    for (int i = height()-25; i >= 50; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString::number(j, 'f', 1));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    EventData &ed = EventData::getInstance();
    if (ed.weatherData[weatherId].size()>1 || ed.weatherData[trackTempId].size()>1)
    {
//        int end = std::max(ed.weatherData[weatherId].size(), ed.weatherData[trackTempId].size());
        int id = ed.weatherData[weatherId].size() > ed.weatherData[trackTempId].size() ? weatherId : trackTempId;
        int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / (double)sz;
        double j = first, jWD = first-1;
        double i = paintRect.left();
        int prevJ = first;

        double jFactor = sz < 5 ? 1.0 : (double)((sz-1)/6.0);

        double jWDFactor = jFactor;//ed.weatherData[wetDryId].size()-1 < 5 ? 1.0 : (double)((ed.weatherData[wetDryId].size()-1)/6.0);

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
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

            if (i > paintRect.left() && i < paintRect.right()-xFactor)
            {
                QString text = getSessionTime(ed.weatherData[id][round(j)]);
                p->drawText(round(i)-10, paintRect.bottom()+15, text);
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
            if (round(jWD) < ed.weatherData[wetDryId].size() && round(j) < last-1)
            {
                int nextI = i + xFactor*jFactor;
                int cnt = 0;
                for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.weatherData[wetDryId].size(); ++k)
                {
                    if (ed.weatherData[wetDryId][k].value == 1)
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
    if (ed.weatherData[weatherId].size()>1)
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last - first + 1;

        if (sz <= 0)
            return;

        double xFactor1 = ((double)paintRect.width()) / (double)sz;//(ed.weatherData[weatherId].size());
        double yFactor = ((double)paintRect.height()-40.0) / (double)(tMax-tMin);

        double x = paintRect.left(), j1 = x + xFactor1;
        double y1 = (double)paintRect.bottom() - (double)(ed.weatherData[weatherId][first].value-tMin) * yFactor;
        double y2 = (double)paintRect.bottom() - (double)(ed.weatherData[trackTempId][first].value-tMin) * yFactor;

        int i = first;
        pen.setColor(color);
        p->setPen(pen);
        for (; i < last + 1 && i < ed.weatherData[weatherId].size(); ++i, j1 += xFactor1)
        {
            double y3 = (double)paintRect.bottom() - (double)(ed.weatherData[weatherId][i].value-tMin) * yFactor;
            double y4 = (double)paintRect.bottom() - (double)(ed.weatherData[trackTempId][i].value-tMin) * yFactor;

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
//            double y4 = (double)paintRect.width() - (double)(ed.weatherData[trackTempId][i].value-tMin) * yFactor;
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

    p->setPen(LTData::colors[LTData::DEFAULT]);
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
    int end = std::max(ed.weatherData[weatherId].size(), ed.weatherData[trackTempId].size());
    for (int i = 0; i < end; ++i)
    {
        if (i == 0)
        {
            max = std::max(ed.weatherData[weatherId][i].value, ed.weatherData[trackTempId][i].value);
        }
        else
        {
            if (i < ed.weatherData[weatherId].size() && ed.weatherData[weatherId][i].value > max)
                max = ed.weatherData[weatherId][i].value;

            if (i < ed.weatherData[trackTempId].size() && ed.weatherData[trackTempId][i].value > max)
                max = ed.weatherData[trackTempId][i].value;
        }
    }
    min = 0.0;
}


void WetDryChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(40, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(40, height()-25, 40, 10);

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((height()-75.0)/2.0);

    p->setPen(QColor(LTData::colors[LTData::WHITE]));
    p->drawText(5, yFactor+yFactor*0.5, "Dry");
    p->drawText(5, yFactor-yFactor*0.5, "Wet");

    QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
    pen.setStyle(Qt::DashLine);
    p->setPen(pen);
    p->drawLine(40, yFactor+yFactor*0.5, width()-5, yFactor+yFactor*0.5);
    p->drawLine(40, yFactor-yFactor*0.5, width()-5, yFactor-yFactor*0.5);


    EventData &ed = EventData::getInstance();
    if (ed.weatherData[weatherId].size()>1)
    {
        double xFactor = ((double)width()-45.0) / (double)ed.weatherData[weatherId].size();
        double j = 1.0, jWD = 0.0;
        double i = 40.0;
        int prevJ = 1;

        double jFactor = ed.weatherData[weatherId].size() < 5 ? 1.0 : (double)((ed.weatherData[weatherId].size()-1)/6.0);        
        double jWDFactor = ed.weatherData[wetDryId].size()-1 < 5 ? 1.0 : (double)((ed.weatherData[wetDryId].size()-1)/6.0);

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


        for (; i < width()-15.0 && round(j) < ed.weatherData[weatherId].size(); /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            if (round(j) == prevJ && prevJ != 0)
                continue;

            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

//			p->drawText(round(i)-5, height()-10, QString("%1").arg(ed.weatherData[weatherId][round(j)]));

            if (i > 40 && i < paintRect.right()-xFactor)
            {
                QString text = getSessionTime(ed.weatherData[weatherId][round(j)]);
                p->drawText(round(i)-10, paintRect.bottom()+15, text);
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
            if (round(jWD) < ed.weatherData[wetDryId].size() && round(j) < ed.weatherData[weatherId].size()-1)
            {
                int nextI = i + xFactor*jFactor;

                int cnt = 0;
                for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.weatherData[wetDryId].size(); ++k)
                {
                    if (ed.weatherData[wetDryId][k].value == 1)
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
    if (ed.weatherData[weatherId].size()>1)
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        double xFactor = ((double)width()-45.0) / ((double)ed.weatherData[weatherId].size());
        double yFactor = (((double)height()-75.0) / 2.0);

        double x = 40.0, j = x + xFactor;
        double y;

        if (ed.weatherData[weatherId][0].value == 0)
            y = yFactor + yFactor*0.5;
        else
            y = yFactor - yFactor*0.5;

        int i = 1;
        for (; i < ed.weatherData[weatherId].size(); ++i, j += xFactor)
        {
            double y2 = (double)(height())-25.0 - (double)(ed.weatherData[weatherId][i].value-min) * yFactor;
            if (ed.weatherData[weatherId][i].value == 0)
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
