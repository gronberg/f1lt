#include "fplaptimeschart.h"
#include "../core/ltdata.h"

#include <QPainter>

void FPLapTimesChart::findFirstAndLastLap(int &firstMin, int &lastMin, int &size)
{
    firstMin = sessionLength, lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int minute = LTData::getFPLength() - LTData::timeToMins(lapDataArray[j].sessionTime);

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


    int lapsInWindow = 0;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int minute = LTData::getFPLength() - LTData::timeToMins(lapDataArray[i].sessionTime);
        int second = LTData::getFPLength()*60 - LTData::timeToSecs(lapDataArray[i].sessionTime);

        if (lapDataArray[i].sessionTime.toString("h:mm:ss") == "")
        {
            minute = 0;
            second = 0;
        }

        if (minute >= first && minute <= last)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();

            if (secs > tMax && tMax == max)
                secs = tMax;

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

            if (lapsInWindow >= lapDataXYArray.size())
                lapDataXYArray.append(LapDataXY(i, (int)x, (int)y));
            else
                lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)x, (int)y);

            QPainterPath path;
            p->setBrush(QBrush(color));
            if (y < paintRect.bottom())
            {
                {
                    path.addEllipse(QPoint(x, y), 3, 3);
                }
                p->drawPath(path);
            }
            ++lapsInWindow;
        }
    }
    clearXYList(lapsInWindow);
}

void FPLapTimesChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        resetZoom();
    }

    if (!scaling && !repaintPopup)
    {
        chartImg = QImage(width(), height(), QImage::Format_ARGB32);
        drawIntoImage(chartImg);
    }

    QPainter p;
    p.begin(this);

//    p.setBrush(QColor(20,20,20));
//    p.setPen(QColor(20,20,20));
//    p.drawRect(0, 0, width(), height());
//    drawChart(&p);

    if (!scaling && !repaintPopup)
    {
        drawImage(&p);
        findLapDataXY(mousePosX, mousePosY);
        drawLapDataXY(&p);
    }
    if (scaling)
    {
        drawImage(&p);
        drawScaleRect(&p);
    }
    if (repaintPopup)
    {
        drawImage(&p);
        drawLapDataXY(&p);
    }

//    QPainter p;
//    p.begin(this);

//    p.setBrush(QColor(20,20,20));
//    p.setPen(QColor(20,20,20));
//    p.drawRect(0, 0, width(), height());
//    drawChart(&p);

//    if (scaling)
//        drawScaleRect(&p);

//    else
//    {
//        if (!repaintPopup)
//            findLapDataXY(mousePosX, mousePosY);

//        drawLapDataXY(&p);
//    }
//    drawLegend(&p, 35, 5);

    p.end();
}

void FPLapTimesChart::resetZoom()
{
    sessionLength = LTData::getFPLength();
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

    if (first >= sessionLength)
        first = sessionLength-1;

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

    int lapsInWindow = 0;

    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int minute = sessionLength - LTData::timeToMins(lapDataArray[i].sessionTime);
        int second = sessionLength*60 - LTData::timeToSecs(lapDataArray[i].sessionTime);

        if (lapDataArray[i].sessionTime.toString("h:mm:ss") == "")
        {
            minute = 0;
            second = 0;
        }
        if (minute >= first && minute <= last && lapDataArray[i].qualiPeriod == qualiPeriod)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();

            if (secs > tMax && tMax == max)
                secs = tMax;

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


            if (lapsInWindow >= lapDataXYArray.size())
                lapDataXYArray.append(LapDataXY(i, (int)x, (int)y));
            else
                lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)x, (int)y);

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
            ++lapsInWindow;
        }
    }
    clearXYList(lapsInWindow);
}

//=======================================

void AllQualiLapTimesChart::findFirstAndLastLap(int &firstMin, int &lastMin, int &size)
{
    firstMin = sessionLength, lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int sessTime = LTData::timeToMins(lapDataArray[j].sessionTime);
        for (int i = 0; i < lapDataArray[j].qualiPeriod-1; ++i)
            sessTime += LTData::getQualiLength(i+1);

        int minute = sessionLength - sessTime;

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

void AllQualiLapTimesChart::drawAxes(QPainter *p, int firstLap, int lastLap)
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

        double jFactor = (last - first) < 6 ? 1.0 : (double)((last - first) / 9.0);
        double q1Line = (LTData::getQualiLength(1) - first) * xFactor + paintRect.left();
        double q2Line = (LTData::getQualiLength(1)+LTData::getQualiLength(2) - first) * xFactor + paintRect.left();
        QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
        pen.setWidth(1);
        for (; i < width()-15.0 && round(j) < last; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            int qTime = round(j);
            if (qTime > (LTData::getQualiLength(1) + LTData::getQualiLength(2)))
                qTime -= (LTData::getQualiLength(1) + LTData::getQualiLength(2));

            else if (qTime > LTData::getQualiLength(1))
                qTime -= LTData::getQualiLength(1);

            prevJ = round(j);
            p->setFont(QFont("Arial", 10, QFont::Bold, false));
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("%1").arg(qTime));

            if (i > paintRect.left())
            {                
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->setRenderHint(QPainter::Antialiasing);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }            
        }
        int qpos[6] = {-1, -1, -1, -1, -1, -1};
        if (i >= q1Line && q1Line > paintRect.left())
        {
            pen.setColor(QColor(LTData::colors[LTData::WHITE]));
            pen.setWidth(4);
            pen.setStyle(Qt::SolidLine);
            p->setPen(pen);
            p->drawLine(round(q1Line), paintRect.bottom(), round(q1Line), paintRect.top());

            qpos[0] = paintRect.left();
            qpos[1] = q1Line;

            qpos[2] = q1Line;
            qpos[3] = paintRect.right();
        }
        else if (last <= LTData::getQualiLength(1))
        {
            qpos[0] = paintRect.left();
            qpos[1] = paintRect.right();
        }
        if (i >= q2Line && q2Line > paintRect.left())
        {
            pen.setWidth(4);
            pen.setColor(QColor(LTData::colors[LTData::WHITE]));
            pen.setStyle(Qt::SolidLine);
            p->setPen(pen);
            p->drawLine(round(q2Line), paintRect.bottom(), round(q2Line), paintRect.top());

            qpos[2] = first < LTData::getQualiLength(1) ? q1Line : paintRect.left();
            qpos[3] = q2Line;

            qpos[4] = q2Line;
            qpos[5] = paintRect.right();
        }
        else if (first >= LTData::getQualiLength(1) && last <= (LTData::getQualiLength(1) + LTData::getQualiLength(2)))
        {
            qpos[2] = paintRect.left();
            qpos[3] = paintRect.right();
        }
        else if (first >= (LTData::getQualiLength(1) + LTData::getQualiLength(2)))
        {
            qpos[4] = paintRect.left();
            qpos[5] = paintRect.right();
        }

        p->setPen(QColor(150, 150, 150, 150));
        p->setFont(QFont("Arial", 50, QFont::Bold, false));
        p->setRenderHint(QPainter::Antialiasing);
        if (qpos[0] != -1 && qpos[1] != -1)
        {
            QString text = "Q1";
            int textWidth = p->fontMetrics().width(text);

            p->drawText(qpos[1] - (qpos[1] - qpos[0] + textWidth)/2, paintRect.top() + 100, text);
        }
        if (qpos[2] != -1 && qpos[3] != -1)
        {
            QString text = "Q2";
            int textWidth = p->fontMetrics().width(text);

            p->drawText(qpos[3] - (qpos[3] - qpos[2] + textWidth)/2, paintRect.top() + 100, text);
        }
        if (qpos[4] != -1 && qpos[5] != -1)
        {
            QString text = "Q3";
            int textWidth = p->fontMetrics().width(text);

            p->drawText(qpos[5] - (qpos[5] - qpos[4] + textWidth)/2, paintRect.top() + 100, text);
        }
    }
}

void AllQualiLapTimesChart::drawChart(QPainter *p)
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

    int lapsInWindow = 0;

    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int sessTime = LTData::getQualiLength(lapDataArray[i].qualiPeriod) - LTData::timeToMins(lapDataArray[i].sessionTime);
        int sessTimeSecs = LTData::getQualiLength(lapDataArray[i].qualiPeriod) * 60 - LTData::timeToSecs(lapDataArray[i].sessionTime);

        if (lapDataArray[i].sessionTime.toString("h:mm:ss") == "")
        {
            sessTime = 0;
            sessTimeSecs = 0;
        }

        for (int k = 0; k < lapDataArray[i].qualiPeriod-1; ++k)
        {
            sessTime += LTData::getQualiLength(k+1);
            sessTimeSecs += LTData::getQualiLength(k+1)*60;
        }


//        int minute = LTData::currentEventFPLength() - LTData::timeToMins(lapDataArray[i].sessionTime);
//        int second = LTData::currentEventFPLength()*60 - LTData::timeToSecs(lapDataArray[i].sessionTime);
        if (sessTime >= first && sessTime <= last)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();

            if (secs > tMax && tMax == max)
                secs = tMax;

            y = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
            x = (double)(sessTimeSecs - first*60) * xFactor + (double)paintRect.left();

            if (x < paintRect.left())
                x = paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].carID
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            if (lapsInWindow >= lapDataXYArray.size())
                lapDataXYArray.append(LapDataXY(i, (int)x, (int)y));
            else
                lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)x, (int)y);

            QPainterPath path;
            p->setBrush(QBrush(color));
            if (y < paintRect.bottom())
            {
                {
                    path.addEllipse(QPoint(x, y), 3, 3);
                }
                p->drawPath(path);
            }
            ++lapsInWindow;
        }
    }
    clearXYList(lapsInWindow);
}
