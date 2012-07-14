#include "fplaptimeschart.h"
#include "ltdata.h"

#include <QPainter>

void FPLapTimesChart::findFirstAndLastLap(int &firstMin, int &lastMin, int &size)
{
    firstMin = sessionLength, lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int minute = LTData::currentEventFPLength() - LTData::timeToMins(lapDataArray[j].sessionTime);

        if (minute < firstMin && minute >= first)
            firstMin = minute;

        if (minute > lastMin && minute <= last)
            lastMin = minute;

        if (minute >= first && minute <= last && lapDataArray[j].lapTime.toDouble() < lMin && lapDataArray[j].lapTime.isValid())
            lMin = lapDataArray[j].lapTime.toDouble();

        if (minute >= first && minute <= last && lapDataArray[j].lapTime.toDouble() > lMax && lapDataArray[j].lapTime.isValid())
            lMax = lapDataArray[j].lapTime.toDouble();

        if (minute >= first && minute <= last)
            ++size;
    }

    if (lMax != 0)
    {
        max = (double)(lMax + lMax * 0.01);
        min = (double)(lMin - lMin * 0.01);

        if (max > 180)
            max = 180;

        if (min < 0)
           min = 0;
    }
}

void FPLapTimesChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((paintRect.height())/10.0);
    double yFactor2 = (double)((tMax-tMin)/10.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        int msecs = j * 1000;
        LapTime lt(msecs);
        QString str = lt.toString();
        int idx = str.indexOf(".");
        if (idx != -1)
            str = str.left(idx+2);

        p->drawText(5, i+5, str);//QString("%1").arg(j, 0, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (last - first > 0)
    {
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(last - first) /*: 5)*/;
        double j = first;
        double i = paintRect.left();
        int prevJ = first;

        double jFactor = (last - first) < 6 ? 1.0 : (double)((last - first) / 10.0);
        for (; i < width()-15.0 && round(j) < last; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("%1").arg(round(j)));

            if (i > paintRect.left())
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
        }
    }
}

void FPLapTimesChart::drawChart(QPainter *p)
{
    int firstMin = sessionLength, lastMin = 0;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstMin, lastMin, size);

    if (lastMin - firstMin == 0 || lapDataArray.isEmpty())
        return;

    drawAxes(p, firstMin, lastMin);

    double xFactor = ((double)paintRect.width()) / (double)((last - first)*60);

    p->setRenderHint(QPainter::Antialiasing);


    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int minute = LTData::currentEventFPLength() - LTData::timeToMins(lapDataArray[i].sessionTime);
        int second = LTData::currentEventFPLength()*60 - LTData::timeToSecs(lapDataArray[i].sessionTime);
        if (minute >= first && minute <= last)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();
//            if (!lapDataArray[i].lapTime.isValid() && lapDataArray[i].numLap-1 >= firstMin && i > 0)// && i < lapDataArray.size()-1)
//            {
//                secs = lapDataArray[i-1].lapTime.toDouble();

//                if (!lapDataArray[i-1].lapTime.isValid() && lapDataArray[i].numLap+1 <= lastLap && i < lapDataArray.size()-1)
//                {
//                    QString pl =  EventData::getInstance().driversData[lapDataArray[i].carID-1].getPitTime(lapDataArray[i].numLap);
//                    secs = LapTime(lapDataArray[i+1].lapTime + LapTime(pl) + LapTime(5000)).toDouble();
//                }
//            }


            if (secs > tMax && tMax == max)
                secs = tMax;

//            if (secs < tMin)
//                secs = tMin;

            y = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
            x = (double)(second - first*60) * xFactor + (double)paintRect.left();

            if (x < paintRect.left())
                x = paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].carID
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            QPainterPath path;
//            p->setBrush(QBrush(LTData::colors[LTData::BACKGROUND]));
            p->setBrush(QBrush(color));
            if (y < paintRect.bottom())
            {
                {
                    path.addEllipse(QPoint(x, y), 3, 3);
                }
                p->drawPath(path);
            }
        }
    }
}

void FPLapTimesChart::paintEvent(QPaintEvent *)
{
    paintRect = QRect(42, 10, width()-47, height()-35);

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        resetZoom();
    }

    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    if (scaling)
        drawScaleRect(&p);

//    drawLegend(&p, 35, 5);

    p.end();
}

void FPLapTimesChart::resetZoom()
{
    first = 0; last = sessionLength;
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
    tMin = min;
    tMax = max;
}

void FPLapTimesChart::transform()
{
    if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
        return;

    if (scaleRect == QRect())
    {
        first = 0;
        last = sessionLength;
        tMin = min;
        tMax = max;
        return;
    }

    if (scaleRect.left() > scaleRect.right())
        scaleRect = QRect(scaleRect.right(), scaleRect.top(), -scaleRect.width(), scaleRect.height());

    if (scaleRect.top() > scaleRect.bottom())
        scaleRect = QRect(scaleRect.left(), scaleRect.bottom(), scaleRect.width(), -scaleRect.height());

    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
//    int sz = lastLap-firstLap+1;
    int sz = last - first + 1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 0)
        first = 0;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
    if (last > sessionLength)
        last = sessionLength;

//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

    emit zoomChanged(first, last, tMin, tMax);
}

//=========================================

void QualiLapTimesChart::findFirstAndLastLap(int &firstMin, int &lastMin, int &size)
{
    firstMin = sessionLength, lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int minute = sessionLength - LTData::timeToMins(lapDataArray[j].sessionTime);

        if (minute < firstMin && minute >= first)
            firstMin = minute;

        if (minute > lastMin && minute <= last)
            lastMin = minute;

        if (minute >= first && minute <= last && lapDataArray[j].lapTime.toDouble() < lMin && lapDataArray[j].lapTime.isValid() && lapDataArray[j].qualiPeriod == qualiPeriod)
            lMin = lapDataArray[j].lapTime.toDouble();

        if (minute >= first && minute <= last && lapDataArray[j].lapTime.toDouble() > lMax && lapDataArray[j].lapTime.isValid() && lapDataArray[j].qualiPeriod == qualiPeriod)
            lMax = lapDataArray[j].lapTime.toDouble();

        if (minute >= first && minute <= last)
            ++size;
    }

    if (lMax != 0)
    {
        max = (double)(lMax + lMax * 0.01);
        min = (double)(lMin - lMin * 0.01);

        if (max > 180)
            max = 180;

        if (min < 0)
           min = 0;
    }
}

void QualiLapTimesChart::drawChart(QPainter *p)
{
    int firstMin = sessionLength, lastMin = 0;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstMin, lastMin, size);

    if (lastMin - firstMin == 0 || lapDataArray.isEmpty())
        return;

    drawAxes(p, firstMin, lastMin);

    double xFactor = ((double)paintRect.width()) / (double)((last - first)*60);

    p->setRenderHint(QPainter::Antialiasing);


    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int minute = sessionLength - LTData::timeToMins(lapDataArray[i].sessionTime);
        int second = sessionLength*60 - LTData::timeToSecs(lapDataArray[i].sessionTime);
        if (minute >= first && minute <= last && lapDataArray[i].qualiPeriod == qualiPeriod)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();
//            if (!lapDataArray[i].lapTime.isValid() && lapDataArray[i].numLap-1 >= firstMin && i > 0)// && i < lapDataArray.size()-1)
//            {
//                secs = lapDataArray[i-1].lapTime.toDouble();

//                if (!lapDataArray[i-1].lapTime.isValid() && lapDataArray[i].numLap+1 <= lastLap && i < lapDataArray.size()-1)
//                {
//                    QString pl =  EventData::getInstance().driversData[lapDataArray[i].carID-1].getPitTime(lapDataArray[i].numLap);
//                    secs = LapTime(lapDataArray[i+1].lapTime + LapTime(pl) + LapTime(5000)).toDouble();
//                }
//            }


            if (secs > tMax && tMax == max)
                secs = tMax;

//            if (secs < tMin)
//                secs = tMin;

            y = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
            x = (double)(second - first*60) * xFactor + (double)paintRect.left();

            if (x < paintRect.left())
                x = paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].carID
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            QPainterPath path;
//            p->setBrush(QBrush(LTData::colors[LTData::BACKGROUND]));
            p->setBrush(QBrush(color));
            if (y < paintRect.bottom())
            {
                {
                    path.addEllipse(QPoint(x, y), 3, 3);
                }
                p->drawPath(path);
            }
        }
    }
}

