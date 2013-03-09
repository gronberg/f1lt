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


#include "driverdatachart.h"

#include "../core/colorsmanager.h"
#include <QPainter>

void PopupInfoBox::paint(QPainter *p, int x, int y, const QRect &paintRect)
{
    if (values.isEmpty())
        return;

//    p.setPen(QColor(232, 227, 185, 200));
//    p.setBrush(QColor(232, 227, 185, 200));

    p->setPen(QColor(50, 50, 50));
    p->setBrush(QColor(50, 50, 50));

    int titleRows = title.isNull() ? 0 : 1;

    int height = 20 * (getSize() + titleRows);
    int bottom = y + height;
    int right = x + width;

    if (bottom > paintRect.bottom())
        y = paintRect.bottom() - height;

    if (right > paintRect.right())
        x = paintRect.right() - width;

    p->drawRect(x+20, y, width, 20 * (getSize() + titleRows));

    p->setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE));
    if (!title.isNull())
    {
        p->setFont(QFont("Arial", 10, QFont::Normal, true));
        p->drawText(x+25, y+15, title);
    }

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    for (int i = 0; i < getSize(); ++i)
    {
        p->drawText(x+25, y+(i + titleRows)*20+15, getInfo(i));
    }
}

DriverDataChart::DriverDataChart(double n, double x, QColor col, QWidget *parent) :
    ChartWidget(n, x, col, parent), driverData(0), popupBox(0)
{    
    if (driverData != 0)
    {
        first = driverData->getPositionHistory().size() > driverData->getLapData().size() ? 2 : 1;
        last = driverData->getPositionHistory().size();
        tMin = min;
        tMax = max;
    }
}


void DriverDataChart::mouseMoveEvent(QMouseEvent *ev)
{
    ChartWidget::mouseMoveEvent(ev);
    if (!scaling)
    {
        mousePosX = ev->pos().x();
        mousePosY = ev->pos().y();

        if (popupBox != 0)
        {
            int items = popupBox->values.size();
            if (checkLapDataCoordinates(mousePosX, mousePosY))
            {
                repaintPopup = true;
                update();
                repaintPopup = false;
            }

            else if (items != 0)  //if the cursor has moved and a popup was displayed previously, it has to be cleared
                update();
        }
    }
}

void DriverDataChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

//    first = driverData->posHistory.size() > driverData->lapData.size() ? 2 : 1;
//	last = driverData->posHistory.size();

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    double yFactor = (((double)paintRect.height())/4.0);
    double yFactor2 = ((double)(tMax-tMin)/4.0);
    double j = tMin;
    for (int i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(round(j)));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (driverData != 0 && driverData->getPositionHistory().size()>1)
    {
        int sz = last-first+1;//(driverData->posHistory.size() > driverData->lapData.size() ? driverData->posHistory.size()-1 : driverData->posHistory.size());
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(sz) /*: 5)*/;
        double j = 1.0;
        double i = paintRect.left();
        int prevJ = 1;

        double jFactor = last-first+1/*driverData->posHistory.size()*/ < 5 ? 1.0 : (double)((last-first+1/*driverData->posHistory.size()-1*/)/6.0);

        j = first-1;
        prevJ = first - 1;


        for (; i < width()-15.0 && round(j) < last && round(j) < driverData->getLapData().size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

            if (driverData->getPositionHistory().size() > driverData->getLapData().size())
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));
            else
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData->getLapData()[round(j)].getLapNumber()));

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

void DriverDataChart::drawChart(QPainter *p)
{
    if (driverData != 0 && driverData->getPositionHistory().size()>1 && first < driverData->getPositionHistory().size())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last-first+1;//(driverData->posHistory.size() > driverData->lapData.size() ? driverData->posHistory.size()-1 : driverData->posHistory.size());
        double xFactor = ((double)paintRect.width()) / ((double)sz);
        double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));

        double x = paintRect.left(), j = x + xFactor;
        double y = (double)paintRect.bottom() - (double)(driverData->getPositionHistory()[first-1]-tMin) * yFactor;

        int i = first; //(driverData->posHistory.size() > driverData->lapData.size() ? 2 : 1);
        int lastPaintedSC = 0;
        for (; i < last + 1 && i < driverData->getPositionHistory().size(); ++i, j += xFactor)
        {
            double y2 = (double)paintRect.bottom() - (double)(driverData->getPositionHistory()[i]-tMin) * yFactor;
            double x2 = j;
            if (driverData->getPositionHistory()[i] <= 0)
            {
                y2 = y;
            }

            LapData ld = driverData->getLapData(i-1);

            if (ld.getCarID() == driverData->getCarID() && ld.getRaceLapExtraData().isSCLap() && ld.getLapNumber() > lastPaintedSC)
            {
                int tmp = first;
                first -= 1;
                drawSCLap(p, ld, xFactor);
                lastPaintedSC = ld.getLapNumber();
                first = tmp;
            }


            if (y2 > paintRect.bottom() && y > paintRect.bottom())
            {
                x = j;
                y = y2;
                continue;
            }

            double tmpY2 = y2;
            checkX1(x, y, x2, tmpY2);
            checkX2(x, y, x2, tmpY2);

            pen.setWidth(2);
            p->setPen(pen);

            p->drawLine(x, y, x2, tmpY2);
            drawRetire(p, x2, tmpY2, 6, driverData->getLapData(i));

            x = j;
            y = y2;

            if (i >= 0 && i < last && i < driverData->getLapData().size() && driverData->getLapData()[i-1].getTime().toString() == "IN PIT" && tmpY2 <= paintRect.bottom())
            {
                QPainterPath path;
                path.addEllipse(QPoint(j, tmpY2), 6, 6);
                p->setBrush(QBrush(color));
                p->setPen(color);

                p->drawPath(path);
            }
        }
    }
}


void DriverDataChart::drawLegend(QPainter *p)
{
    p->setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE));
    p->drawText(40, 20, driverData->getDriverName());
}

int DriverDataChart::checkLapDataCoordinates(int x, int y)
{
    if (popupBox != 0 && driverData != 0)
    {
        popupBox->values.clear();
        for (int i = 0; i < lapDataCoordinates.size(); ++i)
        {
            if (std::abs((float)(lapDataCoordinates[i].x - x)) <= 3 && std::abs((float)(lapDataCoordinates[i].y - y)) <= 3 &&
                    lapDataCoordinates[i].idx < driverData->getLapData().size())
            {
                LapData ld = driverData->getLapData()[lapDataCoordinates[i].idx];
                popupBox->values.append(ld);
            }
        }
        popupBox->sortValues();
        return popupBox->values.size();
    }
    return 0;
}


//void DriverDataChart::drawLapDataXY(QPainter *p)
//{
//    if (itemsInXY.isEmpty())
//        return;

////    p.setPen(QColor(232, 227, 185, 200));
////    p.setBrush(QColor(232, 227, 185, 200));
//    p->setPen(QColor(50, 50, 50));
//    p->setBrush(QColor(50, 50, 50));

//    int x = mousePosX;
//    int y = mousePosY;

//    int height = 20 * (itemsInXY.size());
//    int bottom = y + height;
//    int width = getPopupWidth()+20;
//    int right = x + width;

//    if (bottom > paintRect.bottom())
//        y = paintRect.bottom() - height;

//    if (right > paintRect.right())
//        x = paintRect.right() - width;

//    p->drawRect(x+20, y, getPopupWidth(), 20 * (itemsInXY.size()));


//    p->setFont(QFont("Arial", 10, QFont::Bold, false));
//    p->setPen(QColor(ColorsManager::getInstance().getColor(LTPackets::WHITE)));
//    for (int i = 0; i < itemsInXY.size(); ++i)
//    {
//        if (itemsInXY[i] < driverData->getLapData().size())
//            p->drawText(x+25, y+i*20+15, getDriverInfoXY(driverData->getLapData()[itemsInXY[i]]));
//    }
//}

void DriverDataChart::drawSCLap(QPainter *p, const LapData &lapData, double xFactor)
{    
    double sc_x1 = (double)(lapData.getLapNumber() - first) * xFactor + (double)paintRect.left();
    double sc_x2 = (double)(lapData.getLapNumber()+1 - first) * xFactor + (double)paintRect.left();

    if (sc_x1 < paintRect.left())
        sc_x1 = paintRect.left();

//    if (lastPaintedSCPixel == -1)
//        lastPaintedSCPixel = round(sc_x2);

//    else if (std::abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
//    {
//        sc_x1 = (double)lastPaintedSCPixel;
//        lastPaintedSCPixel = round(sc_x2);
//    }
    p->setPen(QColor(255, 255, 0, 0));
    p->setBrush(QBrush(QColor(255, 255, 0, 35)));

    p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height());
}

void DriverDataChart::drawRetire(QPainter *p, int x, int y, int r, const LapData &ld)
{
    if (ld.getCarID() > 0)
    {
        DriverData *dd = EventData::getInstance().getDriverDataByIdPtr(ld.getCarID());

        if (dd == 0)
            return;

        LapData ld2 = dd->getLapData(ld.getLapNumber()+1);
        LapData ld3 = dd->getLapData(ld.getLapNumber()+2);

        if (ld2.getCarID() == -1 && ld3.getCarID() == -1 && dd->isRetired())
        {
            QPen pen;
            pen.setWidth(3);

            pen.setColor(QColor(255, 255, 255));
            p->setPen(pen);
            p->setBrush(QColor(255, 255, 255));
            QPainterPath path;
            path.addEllipse(QPoint(x, y), r, r);
            p->drawPath(path);

            pen.setColor(QColor(255, 0, 0));
            p->setPen(pen);
            p->drawLine(x-r/2, y-r/2, x+r/2, y+r/2);
            p->drawLine(x-r/2, y+r/2, x+r/2, y-r/2);
        }
    }
}

void DriverDataChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        resetZoom();
    }

    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    if (scaling)
        drawScaleRect(&p);

    p.end();
}

void DriverDataChart::resetZoom()
{
    if (driverData != 0)
    {
        first = driverData->getPositionHistory().size() > driverData->getLapData().size() ? 2 : 1;
        last = driverData->getPositionHistory().size();
    }
    tMin = min;
    tMax = max;
}

void DriverDataChart::calculateTransformFactors()
{    
    int sz = last-first+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

    if (driverData != 0)
    {
        if (first >= driverData->getPositionHistory().size())
            first = driverData->getPositionHistory().size() - 1;

        last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
        if (last >= driverData->getPositionHistory().size())
            last = driverData->getPositionHistory().size() - 1;
    }

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);
}

void DriverDataChart::drawIntoImage(QImage &img)
{
    QPainter p;
    p.begin(&img);
    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());

    drawAxes(&p);
    drawChart(&p);
    drawLegend(&p);

    p.end();
}

//====================================================

void LapTimeChart::setData(DriverData *dd)
{        
    driverData = dd;

    if (dd == 0)
        return;

    max = 0.0;
    for (int i = 0; i < dd->getLapData().size(); ++i)
    {
        double secs = (double)(dd->getLapData()[i].getTime().toMsecs() / 1000.0);

        if (secs > max)
            max = secs;
    }
    if (max == 0.0 || max > 180.0)
        max = 180.0;

    else
        max = max + 0.1*max;
}

void LapTimeChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));

    for (int i = paintRect.bottom(), j = tMin; i >= 50; i-= (height()-55)/6, j += (tMax-tMin)/6)
    {
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (driverData != 0 && !driverData->getLapData().isEmpty() && first < driverData->getLapData().size())
    {
        int sz = last - first + 1;
        double xFactor = ((double)width()-33.0) / /*((lapData.size() < 5) ?*/ (double)sz /*: 5)*/;
        double j = first-1;
        double i = paintRect.left();
        int prevJ = first-1;

        double jFactor = sz < 5 ? 1.0 : (double)(sz/6.0);
        for (; i < width()-15.0 && round(j) < last && round(j) < driverData->getLapData().size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData->getLapData()[round(j)].getLapNumber()));

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

void LapTimeChart::drawChart(QPainter *p)
{
    if (driverData != 0 && !driverData->getLapData().empty() && first < driverData->getLapData().size())
    {
        QPen pen(colors[0]);
        pen.setWidth(2);

        p->setRenderHint(QPainter::Antialiasing);

        int sz = last - first + 1;
        double xFactor = ((double)width()-33.0) / (double)sz;
        double yFactor = (((double)height()-60.0) / (double)(tMax-tMin));

        double x, x2, j;
        double sec1y, sec2y, sec3y, lapy, sec1y2, sec2y2, sec3y2, lapy2;

        x = paintRect.left();
        j = x + xFactor;

        LapTime sector1 = driverData->getLapData()[first-1].getSectorTime(1);
        LapTime sector2 = driverData->getLapData()[first-1].getSectorTime(2);
        LapTime sector3 = driverData->getLapData()[first-1].getSectorTime(3);
        LapTime lapTime = driverData->getLapData()[first-1].getTime();

        if (sz > 1)
        {
            if (lapTime.toString() == "IN PIT")
            {
                lapTime = driverData->getLapData()[first].getTime();
                LapTime pl = LapTime(driverData->getPitTime(driverData->getLapData()[first-1].getLapNumber()));

                lapTime = lapTime - pl + LapTime(5000);
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData->getLapData()[first].getSectorTime(1);
                LapTime pl = LapTime(driverData->getPitTime(driverData->getLapData()[first-1].getLapNumber()));

                sector1 = sector1 - pl + LapTime(5000);
            }

            if (sector2.toString() == "")
                sector2 = driverData->getLapData()[first].getSectorTime(2);

            if (sector3.toString() == "")
                sector3 = driverData->getLapData()[first].getSectorTime(3);
        }

        double secs = sector1.toDouble();
        sec1y = (double)(paintRect.bottom() - (secs - tMin) * yFactor);


        secs = sector2.toDouble();
        sec2y = (double)(paintRect.bottom() - (secs - tMin) * yFactor);

        secs = sector3.toDouble();
        sec3y = (double)(paintRect.bottom() - (secs - tMin) * yFactor);

        secs = lapTime.toDouble();//-QTime::fromString(lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
        //secs = (double)(msecs/1000.0);
        lapy = (double)(paintRect.bottom() - (secs - tMin) * yFactor);

        if (driverData->getLapData()[first-1].getTime().toString() == "IN PIT")
        {
            QPainterPath path;
            path.addEllipse(QPoint(round(x), lapy), 6, 6);
            p->setBrush(QBrush(colors[3]));
            p->setPen(colors[3]);

            if (driverData->getLapData()[0].getRaceLapExtraData().isSCLap())
            {
                pen.setColor(colors[4]);
                p->setBrush(QBrush(colors[4]));
            }

            p->drawPath(path);
        }

        int lapsInWindow = 0;
        int lastPaintedSC = -1;
        for (int i = first; i < last+1 && i < driverData->getLapData().size(); ++i, j += xFactor)
        {
            x2 = j;

            sector1 = driverData->getLapData()[i].getSectorTime(1);
            sector2 = driverData->getLapData()[i].getSectorTime(2);
            sector3 = driverData->getLapData()[i].getSectorTime(3);
            lapTime = driverData->getLapData()[i].getTime();

            if (driverData->getLapData()[i-1].getRaceLapExtraData().isSCLap() && driverData->getLapData()[i-1].getLapNumber() > lastPaintedSC)
            {
                drawSCLap(p, driverData->getLapData()[i-1], xFactor);
                lastPaintedSC = driverData->getLapData()[i-1].getLapNumber();
            }


            if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
            {
                lapTime = driverData->getLapData()[i-1].getTime();
                if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
                {
//                    if (i - 1 > 0)
//                        lapTime = lapData[i-2].lapTime;
                    if (i < last-1)
                    {
                        lapTime = driverData->getLapData()[i+1].getTime();

                        LapTime pl = driverData->getPitTime(driverData->getLapData()[i].getLapNumber());
                        lapTime = lapTime - pl + LapTime(5000);
                    }
                }
            }

            if (lapTime.toString().contains("LAP"))
                continue;

            if (sector1.toString() == "")
            {
                sector1 = driverData->getLapData()[i-1].getSectorTime(1);

                if (sector1.toString() == "" && i < last-1)
                    sector1 = driverData->getLapData()[i+1].getSectorTime(1);
            }

            if (sector2.toString() == "")
            {
                sector2 = driverData->getLapData()[i-1].getSectorTime(2);

                if (sector2.toString() == "" && i < last-1)
                    sector2 = driverData->getLapData()[i+1].getSectorTime(2);
            }

            if (sector3.toString() == "")
            {
                sector3 = driverData->getLapData()[i-1].getSectorTime(3);
                if (sector3.toString() == "")
                {
//                    if (i - 1 > 0)
//                        sector3 = lapData[i-2].sector3;
                    if (i < driverData->getLapData().size()-1)
                        sector3 = driverData->getLapData()[i+1].getSectorTime(3);
                }
            }

            secs = sector1.toDouble();
            if (secs > max) secs = max;
            sec1y2 = (double)(paintRect.bottom() - (secs - tMin)* yFactor);

            secs = sector2.toDouble();
            if (secs > max) secs = max;
            sec2y2 = (double)(paintRect.bottom() - (secs - tMin) * yFactor);

            secs = sector3.toDouble();
            if (secs > max) secs = max;
            sec3y2 = (double)(paintRect.bottom() - (secs - tMin) * yFactor);

            secs = lapTime.toDouble();
            if (secs > max) secs = max;
            lapy2 = (double)(paintRect.bottom() - (secs - tMin) * yFactor);

            double dx1 = x, dx2 = x2, dy1 = sec1y, dy2 = sec1y2;
            if (sec1y <= paintRect.bottom() || sec1y2 <= paintRect.bottom())
            {
                pen.setColor(colors[0]);
                p->setPen(pen);
                checkX1(dx1, dy1, dx2, dy2);
                checkX2(dx1, dy1, dx2, dy2);
                p->drawLine(dx1, dy1, dx2, dy2);
            }

            if (sec2y <= paintRect.bottom() || sec2y2 <= paintRect.bottom())
            {
                dx1 = x, dx2 = x2; dy1 = sec2y; dy2 = sec2y2;
                pen.setColor(colors[1]);
                p->setPen(pen);
                checkX1(dx1, dy1, dx2, dy2);
                checkX2(dx1, dy1, dx2, dy2);
                p->drawLine(dx1, dy1, dx2, dy2);
            }

            if (sec3y <= paintRect.bottom() || sec3y2 <= paintRect.bottom())
            {
                dx1 = x, dx2 = x2; dy1 = sec3y; dy2 = sec3y2;
                pen.setColor(colors[2]);
                p->setPen(pen);
                checkX1(dx1, dy1, dx2, dy2);
                checkX2(dx1, dy1, dx2, dy2);
                p->drawLine(dx1, dy1, dx2, dy2);
            }

            if (lapy <= paintRect.bottom() || lapy2 <= paintRect.bottom())
            {
                dx1 = x, dx2 = x2; dy1 = lapy; dy2 = lapy2;
                pen.setColor(colors[3]);

                p->setPen(pen);
                checkX1(dx1, dy1, dx2, dy2);
                checkX2(dx1, dy1, dx2, dy2);
                p->drawLine(dx1, dy1, dx2, dy2);                

                if (!scaling)
                {
                    if (lapsInWindow >= lapDataCoordinates.size())
                        lapDataCoordinates.append(LapDataCoordinates(i, (int)dx2, (int)dy2));
                    else
                        lapDataCoordinates[lapsInWindow] = LapDataCoordinates(i, (int)dx2, (int)dy2);
                }
                ++lapsInWindow;

                p->setBrush(QBrush(colors[3]));
                if (driverData->getLapData()[i].getTime().toString() == "IN PIT")
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(round(x2), lapy2), 6, 6);
//					p->setPen(colors[3]);

                    p->drawPath(path);
    //                pen.setWidth(4);
    //                p.setPen(pen);
    //                p.drawPoint(x2, lapy2);
    //                pen.setWidth(2);
                }
                else
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(round(x2), lapy2), 2, 2);
//                    p->setPen(colors[3]);

                    p->drawPath(path);
                }
                drawRetire(p, dx2, dy2, 6, driverData->getLapData()[i]);
            }

            x = x2;
            sec1y = sec1y2;
            sec2y = sec2y2;
            sec3y = sec3y2;
            lapy = lapy2;
        }
        clearLapDataCoordinates(lapsInWindow);
    }
}

void LapTimeChart::drawLegend(QPainter *p)
{
    int x = 35, y = 25;
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));
    p->setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT));
    p->drawRect(x, y, 200, 20);

    p->setPen(colors[0]);
    p->drawText(x+5, y+15, "S1");

    p->setPen(colors[1]);
    p->drawText(x+35, y+15, "S2");

    p->setPen(colors[2]);
    p->drawText(x+65, y+15, "S3");

    p->setPen(colors[3]);
    p->drawText(x+95, y+15, "Lap");

    p->drawText(x+145, y+15, "Pit");

    p->setPen(colors[4]);
    p->drawText(x+175, y+15, "SC");


    p->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    p->setPen(colors[3]);
    path.addEllipse(QPoint(x+138, y+10), 4, 4);
    p->setBrush(colors[3]);
    p->drawPath(path);
    p->setBrush(QBrush());
}

void LapTimeChart::paintEvent(QPaintEvent *pe)
{
    DriverDataChart::paintEvent(pe);

    QPainter p;
    p.begin(this);
    if (!scaling)
    {
        if (!repaintPopup)
            checkLapDataCoordinates(mousePosX, mousePosY);

        popupBox->paint(&p, mousePosX, mousePosY, paintRect);
    }
    drawLegend(&p);
    p.end();
}

void LapTimeChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1;

    if (driverData != 0)
        last = driverData->getLapData().size();
}

//========================================================================

void GapChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));


    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    //double factor =

//    min = 0;
//    max = 0;

//    for (int i = 0; i < lapData.size(); ++i)
//    {
//        if (lapData[i].gap[lapData[i].gap.size()-1] != 'L' && lapData[i].gap != "")
//        {
//            double gap = lapData[i].gap.toDouble();
//            if (gap > max)
//                max = gap;
//        }
//    }
    for (int i = paintRect.bottom(), j = tMin; i >= 50; i-= (height()-75)/4, j += (tMax-tMin)/4)
    {
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != paintRect.height() && i < paintRect.bottom())
        {
            QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (tMax >= max)
    {
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, 15, ">1L");
        QPen pen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(paintRect.left(), paintRect.top(), paintRect.right(), paintRect.top());

        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
        p->drawText(5, 35, QString(">%1").arg(max));
        pen.setColor(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::DEFAULT)));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(paintRect.left(), 30, paintRect.right(), 30);
    }

    if (driverData != 0 && !driverData->getLapData().isEmpty())
    {
        int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)sz /*: 5)*/;
        double j = first-1;
        double i = paintRect.left();
        int prevJ = first-1;

        double jFactor = sz < 5 ? 1.0 : (double)(sz/6.0);
        for (; i < width()-15.0 && round(j) < last && round(j) < driverData->getLapData().size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData->getLapData()[round(j)].getLapNumber()));

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

void GapChart::drawChart(QPainter *p)
{
    if (driverData != 0 && !driverData->getLapData().empty() && first < driverData->getLapData().size() && last <= driverData->getLapData().size())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last - first + 1;
        double xFactor = ((double)width()-30.0) / (double)sz;
        double yFactor = (((double)height()-75.0) / (double)(tMax-tMin));

        double gap = driverData->getLapData()[first-1].getGap().toDouble();

        if (driverData->getLapData()[first-1].getGap().size() > 0 && driverData->getLapData()[first-1].getGap()[driverData->getLapData()[first-1].getGap().size()-1] == 'L')
            gap = -1.0;

        double x = paintRect.left(), y = paintRect.bottom() - (gap - tMin) * yFactor, y2;

        if (gap == -1.0 && tMax >= max)
            y = 10;

        else if (gap > tMax && tMax >= max)
            y = 30;

        double j = x + xFactor;
        int lapsInWindow = 0;
        int lastPaintedSC = 0;
        for (int i = first; i < last+1 && i < driverData->getLapData().size(); ++i, j += xFactor)
        {
            bool ok;
            gap = driverData->getLapData()[i].getGap().toDouble(&ok);
            y2 = paintRect.bottom() - (gap - tMin) * yFactor;
            double x2 = j;

            if (gap > tMax && tMax >= max)
                y2 = 30;

            if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
            {
                if (driverData->getLapData()[i].getPosition() == 1 && driverData->getLapData()[i].getGap() == "LAP")
                {
                    gap = 0;
                    y2 = paintRect.bottom() - gap * yFactor;
                }
                else if (driverData->getLapData()[i].getGap().size() > 0 && driverData->getLapData()[i].getGap()[driverData->getLapData()[i].getGap().size()-1] == 'L')
                {
                    if (tMax >= max)
                    {
                        y2 = 10;
                        gap = -1.0;
                    }
                    else
                    {
                        gap = max;
                        y2 = paintRect.top()-10;
                    }
                }
                else if ((gap <= 0 || !ok || gap > tMax)  && tMax >= max)
                    y2 = 30;

//                else
//                    y2 = height()-25 - gap * yFactor;
            }


            if (driverData->getLapData()[i].getTime().toString().contains("LAP"))
                continue;

            if (driverData->getLapData()[i-1].getRaceLapExtraData().isSCLap() && driverData->getLapData()[i-1].getLapNumber() > lastPaintedSC)
            {
                drawSCLap(p, driverData->getLapData()[i-1], xFactor);
                lastPaintedSC = driverData->getLapData()[i-1].getLapNumber();
            }

            if ((y2 > paintRect.bottom() && y > paintRect.bottom()) ||
                 (y2 < paintRect.top() && y < paintRect.top()))
            {
                x = j;
                y = y2;
                continue;
            }

            pen.setWidth(2);
            p->setPen(pen);
            QPainterPath path;
            p->setBrush(QBrush(color));
            if (y2 <= paintRect.bottom())
            {
                if (driverData->getLapData()[i].getTime().toString() == "IN PIT")
                {
                    path.addEllipse(QPoint(x2, y2), 6, 6);
                }
                else
                {
                    path.addEllipse(QPoint(x2, y2), 2, 2);
                }
                p->drawPath(path);
                if (!scaling)
                {
                    if (lapsInWindow >= lapDataCoordinates.size())
                        lapDataCoordinates.append(LapDataCoordinates(i, (int)x2, (int)y2));
                    else
                        lapDataCoordinates[lapsInWindow] = LapDataCoordinates(i, (int)x2, (int)y2);
                }
                ++lapsInWindow;
            }


            double tmpY2 = y2;
            checkX1(x, y, x2, tmpY2);
            checkX2(x, y, x2, tmpY2);

            p->drawLine(x, y, x2, tmpY2);
            drawRetire(p, x2, tmpY2, 6, driverData->getLapData()[i]);
            x = j;
            y = y2;
        }
        clearLapDataCoordinates(lapsInWindow);
    }
}

void GapChart::paintEvent(QPaintEvent *pe)
{
    DriverDataChart::paintEvent(pe);
    if (!scaling)
    {
        if (!repaintPopup)
            checkLapDataCoordinates(mousePosX, mousePosY);

        QPainter p;
        p.begin(this);
        popupBox->paint(&p, mousePosX, mousePosY, paintRect);
        p.end();
    }
}


void GapChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1;

    if (driverData != 0)
        last = driverData->getLapData().size();
}
