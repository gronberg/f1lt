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


#include "fplaptimeschart.h"
#include "../core/colorsmanager.h"
#include "../core/seasondata.h"

#include <QPainter>

void FPLapTimesChart::findFirstAndLastLap(int &firstMin, int &lastMin, int &size)
{
    firstMin = getSessionLength(), lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int minute = SeasonData::getInstance().getFPLength() - SeasonData::getInstance().timeToMins(lapDataArray[j].getPracticeLapExtraData().getSessionTime());

        if (minute < firstMin && minute >= first)
            firstMin = minute;

        if (minute > lastMin && minute <= last)
            lastMin = minute;

        if (minute >= first && minute <= last && lapDataArray[j].getTime().toDouble() < lMin && lapDataArray[j].getTime().isValid())
            lMin = lapDataArray[j].getTime().toDouble();

        if (minute >= first && minute <= last && lapDataArray[j].getTime().toDouble() > lMax && lapDataArray[j].getTime().isValid())
            lMax = lapDataArray[j].getTime().toDouble();

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

int FPLapTimesChart::checkLapDataCoordinates(int x, int y)
{
    if (popupBox != 0)
    {
        popupBox->values.clear();
        for (int i = 0; i < lapDataCoordinates.size(); ++i)
        {
            if (std::abs((float)(lapDataCoordinates[i].x - x)) <= 3 && std::abs((float)(lapDataCoordinates[i].y - y)) <= 3 &&
                lapDataCoordinates[i].idx < lapDataArray.size())
            {
                LapData ld = lapDataArray[lapDataCoordinates[i].idx];
                popupBox->values.append(ld);
                int min = SeasonData::getInstance().timeToMins(SeasonData::getInstance().correctFPTime(ld.getPracticeLapExtraData().getSessionTime()));
                popupBox->title = QString("%1. minute").arg(min);
            }
        }
        popupBox->sortValues();
        return popupBox->values.size();
    }
    return 0;
}

void FPLapTimesChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    double yFactor = (double)((paintRect.height())/10.0);
    double yFactor2 = (double)((tMax-tMin)/10.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        int msecs = j * 1000;
        LapTime lt(msecs);
        QString str = lt.toString();
        int idx = str.indexOf(".");
        if (idx != -1)
            str = str.left(idx+2);

        p->drawText(5, i+5, str);//QString("%1").arg(j, 0, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
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
            p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("%1'").arg(round(j)));

            if (i > paintRect.left())
            {
                QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
        }
    }
}

void FPLapTimesChart::drawChart(QPainter *p)
{
    int firstMin = getSessionLength(), lastMin = 0;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstMin, lastMin, size);

    if (/*lastMin - firstMin == 0 ||*/ lapDataArray.isEmpty())
        return;

    drawAxes(p, firstMin, lastMin);

    double xFactor = ((double)paintRect.width()) / (double)((last - first)*60);

    p->setRenderHint(QPainter::Antialiasing);


    int lapsInWindow = 0;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int minute = SeasonData::getInstance().getFPLength() - SeasonData::getInstance().timeToMins(lapDataArray[i].getPracticeLapExtraData().getSessionTime());
        int second = SeasonData::getInstance().getFPLength()*60 - SeasonData::getInstance().timeToSecs(lapDataArray[i].getPracticeLapExtraData().getSessionTime());

        if (lapDataArray[i].getPracticeLapExtraData().getSessionTime().toString("h:mm:ss") == "")
        {
            minute = 0;
            second = 0;
        }

        if (minute >= first && minute <= last)// && lapDataArray[i].getTime().isValid())
        {
            secs = lapDataArray[i].getTime().toDouble();

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

            if (lapsInWindow >= lapDataCoordinates.size())
                lapDataCoordinates.append(LapDataCoordinates(i, (int)x, (int)y));
            else
                lapDataCoordinates[lapsInWindow] = LapDataCoordinates(i, (int)x, (int)y);

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
    clearLapDataCoordinates(lapsInWindow);
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
        checkLapDataCoordinates(mousePosX, mousePosY);
        popupBox->paint(&p, mousePosX, mousePosY, paintRect);
    }
    if (scaling)
    {
        drawImage(&p);
        drawScaleRect(&p);
    }
    if (repaintPopup)
    {
        drawImage(&p);
        popupBox->paint(&p, mousePosX, mousePosY, paintRect);
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

    if (lapDataArray.isEmpty())
        clearLapDataCoordinates(0);
}

void FPLapTimesChart::resetZoom()
{
    first = 0; last = getSessionLength();
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
    tMin = min;
    tMax = max;
}

void FPLapTimesChart::calculateTransformFactors()
{
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
//    int sz = lastLap-firstLap+1;
    int sz = last - first + 1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 0)
        first = 0;

    if (first >= getSessionLength())
        first = getSessionLength()-1;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
    if (last > getSessionLength())
        last = getSessionLength();

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
    firstMin = getSessionLength(), lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int minute = getSessionLength() - SeasonData::getInstance().timeToMins(lapDataArray[j].getQualiLapExtraData().getSessionTime());

        if (minute < firstMin && minute >= first)
            firstMin = minute;

        if (minute > lastMin && minute <= last)
            lastMin = minute;

        if (minute >= first && minute <= last && lapDataArray[j].getTime().toDouble() < lMin && lapDataArray[j].getTime().isValid() && lapDataArray[j].getQualiLapExtraData().getQualiPeriod() == qualiPeriod)
            lMin = lapDataArray[j].getTime().toDouble();

        if (minute >= first && minute <= last && lapDataArray[j].getTime().toDouble() > lMax && lapDataArray[j].getTime().isValid() && lapDataArray[j].getQualiLapExtraData().getQualiPeriod() == qualiPeriod)
            lMax = lapDataArray[j].getTime().toDouble();

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

int QualiLapTimesChart::checkLapDataCoordinates(int x, int y)
{
    if (popupBox != 0)
    {
        popupBox->values.clear();
        for (int i = 0; i < lapDataCoordinates.size(); ++i)
        {
            if (std::abs((float)(lapDataCoordinates[i].x - x)) <= 3 && std::abs((float)(lapDataCoordinates[i].y - y)) <= 3 &&
                    lapDataCoordinates[i].idx < lapDataArray.size())
            {
                LapData ld = lapDataArray[lapDataCoordinates[i].idx];
                popupBox->values.append(ld);
                int min = SeasonData::getInstance().timeToMins(SeasonData::getInstance().correctQualiTime(ld.getQualiLapExtraData().getSessionTime(), ld.getQualiLapExtraData().getQualiPeriod()));
                popupBox->title = QString("%1. minute").arg(min);
            }
        }
        popupBox->sortValues();
        return popupBox->values.size();
    }
    return 0;
}

void QualiLapTimesChart::drawChart(QPainter *p)
{
    int firstMin = getSessionLength(), lastMin = 0;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstMin, lastMin, size);

    if (/*lastMin - firstMin == 0 ||*/ lapDataArray.isEmpty())
        return;

    drawAxes(p, firstMin, lastMin);

    double xFactor = ((double)paintRect.width()) / (double)((last - first)*60);

    p->setRenderHint(QPainter::Antialiasing);

    int lapsInWindow = 0;

    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int minute = getSessionLength() - SeasonData::getInstance().timeToMins(lapDataArray[i].getQualiLapExtraData().getSessionTime());
        int second = getSessionLength()*60 - SeasonData::getInstance().timeToSecs(lapDataArray[i].getQualiLapExtraData().getSessionTime());

        if (lapDataArray[i].getQualiLapExtraData().getSessionTime().toString("h:mm:ss") == "")
        {
            minute = 0;
            second = 0;
        }
        if (minute >= first && minute <= last && lapDataArray[i].getQualiLapExtraData().getQualiPeriod() == qualiPeriod)// && lapDataArray[i].getTime().isValid())
        {
            secs = lapDataArray[i].getTime().toDouble();

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


            if (lapsInWindow >= lapDataCoordinates.size())
                lapDataCoordinates.append(LapDataCoordinates(i, (int)x, (int)y));
            else
                lapDataCoordinates[lapsInWindow] = LapDataCoordinates(i, (int)x, (int)y);

            QPainterPath path;
//            p->setBrush(QBrush(SeasonData::getInstance().getColor(LTPackets::BACKGROUND]));
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
    clearLapDataCoordinates(lapsInWindow);
}

//=======================================

void AllQualiLapTimesChart::findFirstAndLastLap(int &firstMin, int &lastMin, int &size)
{
    firstMin = getSessionLength(), lastMin = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        int sessTime = SeasonData::getInstance().timeToMins(lapDataArray[j].getQualiLapExtraData().getSessionTime());
        for (int i = 0; i < lapDataArray[j].getQualiLapExtraData().getQualiPeriod()-1; ++i)
            sessTime += SeasonData::getInstance().getQualiLength(i+1);

        int minute = getSessionLength() - sessTime;

        if (minute < firstMin && minute >= first)
            firstMin = minute;

        if (minute > lastMin && minute <= last)
            lastMin = minute;

        if (minute >= first && minute <= last && lapDataArray[j].getTime().toDouble() < lMin && lapDataArray[j].getTime().isValid())
            lMin = lapDataArray[j].getTime().toDouble();

        if (minute >= first && minute <= last && lapDataArray[j].getTime().toDouble() > lMax && lapDataArray[j].getTime().isValid())
            lMax = lapDataArray[j].getTime().toDouble();

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

int AllQualiLapTimesChart::checkLapDataCoordinates(int x, int y)
{
    if (popupBox != 0)
    {
        popupBox->values.clear();
        for (int i = 0; i < lapDataCoordinates.size(); ++i)
        {
            if (std::abs((float)(lapDataCoordinates[i].x - x)) <= 3 && std::abs((float)(lapDataCoordinates[i].y - y)) <= 3 &&
                    lapDataCoordinates[i].idx < lapDataArray.size())
            {
                LapData ld = lapDataArray[lapDataCoordinates[i].idx];
                popupBox->values.append(ld);
                int q = ld.getQualiLapExtraData().getQualiPeriod();
                int min = SeasonData::getInstance().timeToMins(SeasonData::getInstance().correctQualiTime(ld.getQualiLapExtraData().getSessionTime(), q));
                popupBox->title = QString("Q%1 %2. minute").arg(q).arg(min);
            }
        }
        popupBox->sortValues();
        return popupBox->values.size();
    }
    return 0;
}

void AllQualiLapTimesChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    double yFactor = (double)((paintRect.height())/10.0);
    double yFactor2 = (double)((tMax-tMin)/10.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        int msecs = j * 1000;
        LapTime lt(msecs);
        QString str = lt.toString();
        int idx = str.indexOf(".");
        if (idx != -1)
            str = str.left(idx+2);

        p->drawText(5, i+5, str);//QString("%1").arg(j, 0, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
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
        double q1Line = (SeasonData::getInstance().getQualiLength(1) - first) * xFactor + paintRect.left();
        double q2Line = (SeasonData::getInstance().getQualiLength(1)+SeasonData::getInstance().getQualiLength(2) - first) * xFactor + paintRect.left();
        QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
        pen.setWidth(1);
        for (; i < width()-15.0 && round(j) < last; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            int qTime = round(j);
            if (qTime > (SeasonData::getInstance().getQualiLength(1) + SeasonData::getInstance().getQualiLength(2)))
                qTime -= (SeasonData::getInstance().getQualiLength(1) + SeasonData::getInstance().getQualiLength(2));

            else if (qTime > SeasonData::getInstance().getQualiLength(1))
                qTime -= SeasonData::getInstance().getQualiLength(1);

            prevJ = round(j);
            p->setFont(QFont("Arial", 10, QFont::Bold, false));
            p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("%1'").arg(qTime));

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
            pen.setColor(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
            pen.setWidth(4);
            pen.setStyle(Qt::SolidLine);
            p->setPen(pen);
            p->drawLine(round(q1Line), paintRect.bottom(), round(q1Line), paintRect.top());

            qpos[0] = paintRect.left();
            qpos[1] = q1Line;

            qpos[2] = q1Line;
            qpos[3] = paintRect.right();
        }
        else if (last <= SeasonData::getInstance().getQualiLength(1))
        {
            qpos[0] = paintRect.left();
            qpos[1] = paintRect.right();
        }
        if (i >= q2Line && q2Line > paintRect.left())
        {
            pen.setWidth(4);
            pen.setColor(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
            pen.setStyle(Qt::SolidLine);
            p->setPen(pen);
            p->drawLine(round(q2Line), paintRect.bottom(), round(q2Line), paintRect.top());

            qpos[2] = first < SeasonData::getInstance().getQualiLength(1) ? q1Line : paintRect.left();
            qpos[3] = q2Line;

            qpos[4] = q2Line;
            qpos[5] = paintRect.right();
        }
        else if (first >= SeasonData::getInstance().getQualiLength(1) && last <= (SeasonData::getInstance().getQualiLength(1) + SeasonData::getInstance().getQualiLength(2)))
        {
            qpos[2] = paintRect.left();
            qpos[3] = paintRect.right();
        }
        else if (first >= (SeasonData::getInstance().getQualiLength(1) + SeasonData::getInstance().getQualiLength(2)))
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
    int firstMin = getSessionLength(), lastMin = 0;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstMin, lastMin, size);

    if (/*lastMin - firstMin == 0 ||*/ lapDataArray.isEmpty())
        return;

    drawAxes(p, firstMin, lastMin);

    double xFactor = ((double)paintRect.width()) / (double)((last - first)*60);

    p->setRenderHint(QPainter::Antialiasing);

    int lapsInWindow = 0;

    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        int sessTime = SeasonData::getInstance().getQualiLength(lapDataArray[i].getQualiLapExtraData().getQualiPeriod()) -
                SeasonData::getInstance().timeToMins(lapDataArray[i].getQualiLapExtraData().getSessionTime());
        int sessTimeSecs = SeasonData::getInstance().getQualiLength(lapDataArray[i].getQualiLapExtraData().getQualiPeriod()) * 60 -
                SeasonData::getInstance().timeToSecs(lapDataArray[i].getQualiLapExtraData().getSessionTime());

        if (lapDataArray[i].getQualiLapExtraData().getSessionTime().toString("h:mm:ss") == "")
        {
            sessTime = 0;
            sessTimeSecs = 0;
        }

        for (int k = 0; k < lapDataArray[i].getQualiLapExtraData().getQualiPeriod()-1; ++k)
        {
            sessTime += SeasonData::getInstance().getQualiLength(k+1);
            sessTimeSecs += SeasonData::getInstance().getQualiLength(k+1)*60;
        }


//        int minute = LTPackets::currentEventFPLength() - LTPackets::timeToMins(lapDataArray[i].sessionTime);
//        int second = LTPackets::currentEventFPLength()*60 - LTPackets::timeToSecs(lapDataArray[i].sessionTime);
        if (sessTime >= first && sessTime <= last)// && lapDataArray[i].getTime().isValid())
        {
            secs = lapDataArray[i].getTime().toDouble();

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

            if (lapsInWindow >= lapDataCoordinates.size())
                lapDataCoordinates.append(LapDataCoordinates(i, (int)x, (int)y));
            else
                lapDataCoordinates[lapsInWindow] = LapDataCoordinates(i, (int)x, (int)y);

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
    clearLapDataCoordinates(lapsInWindow);
}
