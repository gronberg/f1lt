#include "sessionlaptimeschart.h"

#include <QDebug>
#include <QPainter>

bool lessThan(LapData ld1, LapData ld2)
{
    if (ld1.getCarID() > 0 && ld2.getCarID() > 0)
    {
        DriverData dd1 = EventData::getInstance().getDriversData()[ld1.getCarID()-1];
        DriverData dd2 = EventData::getInstance().getDriversData()[ld2.getCarID()-1];

        if (dd1.getPosition() > dd2.getPosition())
            return true;

        if (dd1.getPosition() < dd2.getPosition())
            return false;

        if (dd1.getPosition() == dd2.getPosition())
        {
            if (dd1.getCarID() != dd2.getCarID())
                return dd1.getCarID() < dd2.getCarID();

            if (ld1.getLapNumber() < ld2.getLapNumber())
                return true;
        }
    }
    return false;
}

bool PredXYPos::operator ()(int item1, int item2)
{
    LapData ld1 = chart.lapDataArray[item1];
    LapData ld2 = chart.lapDataArray[item2];

//    if (ld1.getTime().isValid() && ld2.getTime().isValid())
        return ld1.getPosition() < ld2.getPosition();

//    if (ld1.getTime().isValid() && !ld2.getTime().isValid())
//        return true;

//    return false;
}

//bool PredXYGap::operator ()(int item1, int item2)
//{
//    return PredXY::operator ()(item1, item2);
////    LapData ld1 = chart.lapDataArray[item1];
////    LapData ld2 = chart.lapDataArray[item2];

////    QString sgap1 = ld1.gap;
////    QString sgap2 = ld2.gap;

////    if (sgap1 == "")
////        sgap1 = "1L";

////    if (sgap2 == "")
////        sgap2 = "1L";

////    bool ok1=true, ok2=true;
////    double gap1 = sgap1.toDouble(&ok1);
////    double gap2 = sgap2.toDouble(&ok2);

////    if (ok1 && ok2)
////        return gap1 < gap2;

////    if (sgap1 == "LAP")
////        return true;

////    if (sgap2 == "LAP")
////        return false;

////    if (sgap1.size() > 1 && sgap1[sgap1.size()-1] == 'L' &&
////        sgap2.size() > 1 && sgap2[sgap2.size()-1] == 'L')
////    {
////        gap1 = sgap1.left(sgap1.size()-1).toDouble();
////        gap2 = sgap2.left(sgap2.size()-1).toDouble();

////        return gap1 < gap2;
////    }

////    return false;
//}

void SessionLapTimesChart::findFirstAndLastLap(int &firstLap, int &lastLap, int &size)
{
    firstLap = 99, lastLap = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        if (lapDataArray[j].getLapNumber() < firstLap && lapDataArray[j].getLapNumber() >= first)
            firstLap = lapDataArray[j].getLapNumber();

        if (lapDataArray[j].getLapNumber() > lastLap && lapDataArray[j].getLapNumber() <= last)
            lastLap = lapDataArray[j].getLapNumber();

        if (lapDataArray[j].getLapNumber() >= first && lapDataArray[j].getLapNumber() <= last && lapDataArray[j].getTime().toDouble() < lMin && lapDataArray[j].getTime().isValid())
            lMin = lapDataArray[j].getTime().toDouble();

        if (lapDataArray[j].getLapNumber() >= first && lapDataArray[j].getLapNumber() <= last && lapDataArray[j].getTime().toDouble() > lMax && lapDataArray[j].getTime().isValid())
            lMax = lapDataArray[j].getTime().toDouble();

        if (lapDataArray[j].getLapNumber() >= first && lapDataArray[j].getLapNumber() <= last)
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

void SessionLapTimesChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));


    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    double yFactor = (double)((paintRect.height())/10.0);
    double yFactor2 = (double)((tMax-tMin)/10.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        int msecs = j * 1000;
        LapTime lt(msecs);
        QString str = lt.toString();
        int idx = str.indexOf(".");
        if (idx != -1)
            str = str.left(idx+2);

        p->drawText(5, i+5, str);//QString("%1").arg(j, 0, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = paintRect.left();
        int prevJ = firstLap;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap) / 10.0);
        for (; i < width()-15.0 && round(j) < lastLap; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

            if (i > paintRect.left())
            {
                QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
        }
    }
}

void SessionLapTimesChart::drawChart(QPainter *p)
{
    int firstLap = first, lastLap = last;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstLap, lastLap, size);
    firstLap = first; lastLap = last;

    if (/*lastLap - firstLap == 0 ||*/ lapDataArray.isEmpty())
        return;

    drawAxes(p, firstLap, lastLap);

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);


    int lastPaintedSCPixel = -1;
    int lastPaintedSC = -1;

//    lapDataXYArray.clear();
    int lapsInWindow = 0;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        if (lapDataArray[i].getLapNumber() >= firstLap && lapDataArray[i].getLapNumber() <= lastLap)// && lapDataArray[i].getTime().isValid())
        {
            secs = lapDataArray[i].getTime().toDouble();
            if (!lapDataArray[i].getTime().isValid() && lapDataArray[i].getLapNumber()-1 >= firstLap && i > 0)// && i < lapDataArray.size()-1)
            {
                secs = lapDataArray[i-1].getTime().toDouble();

                if (!lapDataArray[i-1].getTime().isValid() && lapDataArray[i].getLapNumber()+1 <= lastLap && i < lapDataArray.size()-1)
                {
                    QString pl =  EventData::getInstance().getDriversData()[lapDataArray[i].getCarID()-1].getPitTime(lapDataArray[i].getLapNumber());
                    secs = LapTime(lapDataArray[i+1].getTime() + LapTime(pl) + LapTime(5000)).toDouble();
                }
            }

            if (lapDataArray[i].getRaceLapExtraData().isSCLap() && lapDataArray[i].getLapNumber() > lastPaintedSC)
            {
                double sc_x1 = (double)(lapDataArray[i].getLapNumber() - firstLap) * xFactor + (double)paintRect.left();
                double sc_x2 = (double)(lapDataArray[i].getLapNumber()+1 - firstLap) * xFactor + (double)paintRect.left();

                if (sc_x1 < paintRect.left())
                    sc_x1 = paintRect.left();

                if (lastPaintedSCPixel == -1)
                    lastPaintedSCPixel = round(sc_x2);

                else if (abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
                {
                    sc_x1 = (double)lastPaintedSCPixel;
                    lastPaintedSCPixel = round(sc_x2);
                }
                p->setPen(QColor(255, 255, 0, 0));
                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

                p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height());
                lastPaintedSC = lapDataArray[i].getLapNumber();

            }


            if (secs > tMax && tMax == max)
                secs = tMax;

//            if (secs < tMin)
//                secs = tMin;

            y = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
            x = (double)(lapDataArray[i].getLapNumber() - firstLap) * xFactor + (double)paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].getCarID()
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            QPainterPath path;

            if (!scaling)
            {
                if (lapsInWindow >= lapDataXYArray.size())
                    lapDataXYArray.append(LapDataXY(i, (int)x, (int)y));
                else
                    lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)x, (int)y);
            }
//            p->setBrush(QBrush(SeasonData::getInstance().getColor(LTPackets::BACKGROUND]));
            p->setBrush(QBrush(color));
            if (y < paintRect.bottom())
            {
                if (lapDataArray[i].getTime().toString() == "IN PIT")
                {
    //                p->setBrush(QBrush(QColor()));
                    path.addEllipse(QPoint(x, y), 7, 7);
                }
                else
                {
                    path.addEllipse(QPoint(x, y), 2, 2);
                }
                p->drawPath(path);
            }

            if (lapDataArray[i].getLapNumber()-1 >= firstLap && i > 0)
            {
                double x1 = (double)(lapDataArray[i].getLapNumber() - 1 - firstLap) * xFactor + (double)paintRect.left();
                secs = lapDataArray[i-1].getTime().toDouble();

                if (!lapDataArray[i-1].getTime().isValid() && i-2 >= firstLap)
                {
                    secs = lapDataArray[i-2].getTime().toDouble();

                    if (!lapDataArray[i-2].getTime().isValid())// && i < lapDataArray.size()-1)
                    {
                        QString pl = EventData::getInstance().getDriversData()[lapDataArray[i-1].getCarID()-1].getPitTime(lapDataArray[i-1].getLapNumber());
                        secs = LapTime(lapDataArray[i].getTime() + LapTime(pl) + LapTime(5000)).toDouble();
                    }                    
                }

                if (secs > tMax && tMax == max)
                    secs = tMax;

//                if (secs < tMin)
//                    secs = tMin;

                double y1 = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);


                if ((y > paintRect.bottom() && y1 > paintRect.bottom()) || (y < paintRect.top() && y1 < paintRect.top()))
                    continue;

                ChartWidget::checkX1(x1, y1, x, y);
                ChartWidget::checkX2(x1, y1, x, y);

                p->drawLine(x1, y1, x, y);                               
            }
            ++lapsInWindow;
        }
    }   
    if (!scaling)
        clearXYList(lapsInWindow);
}

void SessionLapTimesChart::drawRetire(QPainter *p, int x, int y, const LapData &ld, QColor color)
{
    if (ld.getCarID() > 0)
    {
        LapData ld2 = EventData::getInstance().getDriverDataById(ld.getCarID()).getLapData(ld.getLapNumber()+1);
        LapData ld3 = EventData::getInstance().getDriverDataById(ld.getCarID()).getLapData(ld.getLapNumber()+2);

        if (ld2.getCarID() == -1 && ld3.getCarID() == -1)
        {
            QPen pen;
            pen.setWidth(3);
            pen.setColor(QColor(255, 0, 0));
            p->setPen(pen);

            p->drawLine(x-5, y-5, x+5, y+5);
            p->drawLine(x-5, y+5, x+5, y-5);

            pen.setColor(QColor(255, 255, 255));
            p->setPen(pen);
            p->setBrush(QColor(255, 0, 0));
            QPainterPath path;
            path.addEllipse(QPoint(x, y), 7, 7);
            p->drawPath(path);
        }
    }
}

int SessionLapTimesChart::findLapDataXY(int x, int y)
{
    itemsInXY.clear();
    for (int i = 0; i < lapDataXYArray.size(); ++i)
    {
        if (abs(lapDataXYArray[i].x - x) <= 3 && abs(lapDataXYArray[i].y - y) <= 3)
            itemsInXY.append(lapDataXYArray[i].idx);
    }
    qSort(itemsInXY.begin(), itemsInXY.end(), PredXYPos(*this));
    return itemsInXY.size();
}

void SessionLapTimesChart::drawLapDataXY(QPainter *p)
{
    if (itemsInXY.isEmpty())
        return;

//    p.setPen(QColor(232, 227, 185, 200));
//    p.setBrush(QColor(232, 227, 185, 200));
    p->setPen(QColor(50, 50, 50));
    p->setBrush(QColor(50, 50, 50));

    int x = mousePosX;
    int y = mousePosY;

    int height = 20 * (itemsInXY.size()+1);
    int bottom = y + height;
    int width = getPopupWidth()+20;
    int right = x + width;

    if (bottom > paintRect.bottom())
        y = paintRect.bottom() - height;

    if (right > paintRect.right())
        x = paintRect.right() - width;

    p->drawRect(x+20, y, getPopupWidth(), 20 * (itemsInXY.size()+1));

    p->setFont(QFont("Arial", 10, QFont::Normal, true));
    p->setPen(SeasonData::getInstance().getColor(LTPackets::WHITE));
    p->drawText(x+25, y+15, getLapInfoXY(lapDataArray[itemsInXY[0]]));

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    for (int i = 0; i < itemsInXY.size(); ++i)
    {
        p->drawText(x+25, y+(i+1)*20+15, getDriverInfoXY(lapDataArray[itemsInXY[i]]));
    }
}

QColor SessionLapTimesChart::getCarColor(const LapData &ld)
{
    if (ld.getCarID() > 0)
    {
        DriverData dd = EventData::getInstance().getDriverDataById(ld.getCarID());
        return getCarColor(dd);
    }    
    return SeasonData::getInstance().getColor(LTPackets::BACKGROUND);
}

QColor SessionLapTimesChart::getCarColor(const DriverData &dd)
{
    QColor color = SeasonData::getInstance().getColor(LTPackets::BACKGROUND);
    if (dd.getCarID() > 0)
    {
        int no = dd.getNumber();

        if (no > 0 && no < colors.size()+2)
            color = colors[no <= 12 ? no-1 : no -2];
    }
    return color;
}

void SessionLapTimesChart::drawIntoImage(QImage &img)
{
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());

    drawChart(&p);

    p.end();
}

void SessionLapTimesChart::drawImage(QPainter *p)
{
    p->drawImage(0, 0, chartImg);
}

void SessionLapTimesChart::paintEvent(QPaintEvent *)
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
    p.end();
}

void SessionLapTimesChart::resetZoom()
{
    first = 1; last = 99;
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
    first = 1; last = EventData::getInstance().getEventInfo().laps;
    tMin = min;
    tMax = max;
}

void SessionLapTimesChart::transform()
{
    if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
        return;

    if (scaleRect == QRect())
    {
        first = 1;
        last = 99;
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
    int sz = last-first+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
    if (last > EventData::getInstance().getEventInfo().laps)
        last = EventData::getInstance().getEventInfo().laps;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

    emit zoomChanged(first, last, tMin, tMax);
}

void SessionLapTimesChart::onZoomOut()
{
    ChartWidget::onZoomOut();
    if (scaleRectsStack.size() > 1)
        emit zoomChanged(first, last, tMin, tMax);
    else
        emit zoomChanged(first, last, -1, -1);
}

void SessionLapTimesChart::mouseDoubleClickEvent(QMouseEvent *ev)
{
    ChartWidget::mouseDoubleClickEvent(ev);
    if (!scaling)    
        emit zoomChanged(first, last, -1, -1);    
}

void SessionLapTimesChart::mouseMoveEvent(QMouseEvent *ev)
{
    if (scaling)
        ChartWidget::mouseMoveEvent(ev);

    else
    {        
        mousePosX = ev->pos().x();
        mousePosY = ev->pos().y();

        int items = itemsInXY.size();
        if (findLapDataXY(mousePosX, mousePosY))
        {
            repaintPopup = true;
            repaint();
            repaintPopup = false;
        }

        else if (items != 0)  //if the cursor has moved and a popup was displayed previously, it has to be cleared
            update();
    }
}

//=========================================================================

void SessionPositionsChart::findFirstAndLastLap(int &firstLap, int &lastLap, int &size)
{
    firstLap = 99, lastLap = 0, size = 0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        if (lapDataArray[j].getLapNumber() < firstLap && lapDataArray[j].getLapNumber() >= first)
            firstLap = lapDataArray[j].getLapNumber();

        if (lapDataArray[j].getLapNumber() > lastLap && lapDataArray[j].getLapNumber() <= last)
            lastLap = lapDataArray[j].getLapNumber();

        if (lapDataArray[j].getLapNumber() >= first && lapDataArray[j].getLapNumber() <= last)
            ++size;
    }
}

QList<SessionPositionsChart::DriverPosAtom> SessionPositionsChart::getDriverStartingPositions()
{
    QList<SessionPositionsChart::DriverPosAtom> positionList;
    for (int i = 0; i < EventData::getInstance().getDriversData().size(); ++i)
    {
        DriverData &dd = EventData::getInstance().getDriversData()[i];
        positionList.append(SessionPositionsChart::DriverPosAtom(dd.getStartingPos(), dd.getCarID()));
    }
    qSort(positionList);
    return positionList;
}

int SessionPositionsChart::getDriverStartingPosition(const LapData &ld)
{
    DriverData dd = EventData::getInstance().getDriverDataById(ld.getCarID());
    if (dd.getCarID() > 0)
        return dd.getStartingPos();

    return 0;
}

void SessionPositionsChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.top(), paintRect.right(), paintRect.top());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom()-10, paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    QList<SessionPositionsChart::DriverPosAtom> positionList = getDriverStartingPositions();

    int chartMin = (tMin == 1 ? 0 : tMin), k = 0;
    double yFactor = (double)((paintRect.height())/(tMax-chartMin));
    double yFactor2 = 1.0;//(double)((tMax-chartMin)/12.0);
    double j = tMin;
    for (double i = paintRect.top(); i <= paintRect.bottom(); i+= yFactor, j += yFactor2, ++k)
    {
        if (firstLap == 0)
        {
            DriverData dd;
            int k = round(j)-1;
            if (k >= 0 && k < positionList.size() && positionList[k].id-1 >= 0 && positionList[k].id-1 < EventData::getInstance().getDriversData().size())
                dd = EventData::getInstance().getDriversData()[positionList[round(j)-1].id-1];
            QColor color = getCarColor(dd.getLastLap());
            p->setBrush(color);
            p->setPen(color);
            p->drawRect(5, i-6, 4, 11);
            QString driver = SeasonData::getInstance().getDriverShortName(dd.getDriverName());
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
            p->drawText(13, i+5, QString("%1 %2").arg(round(j)).arg(driver));
        }

        if (i != paintRect.top())
        {
            QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = paintRect.left();
        int prevJ = firstLap;
        bool lap1Line = false;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap) / 10.0);
        for (; i < width()-15.0 && round(j) < lastLap; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

            QString lap = QString("L%1").arg(round(j));
            if (lap == "L0")
                lap = "S";
            p->drawText(round(i)-5, 15, lap);

            if (i > paintRect.left())
            {
                QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom()-10, round(i), paintRect.top());
            }

            if (firstLap == 0 && !lap1Line)
            {
                p->drawText(round(i)-5+xFactor, 15, "L1");

                QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i) + xFactor, paintRect.bottom()-10, round(i) + xFactor, paintRect.top());
                lap1Line = true;
            }
        }
    }
}

void SessionPositionsChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    int chartMin = tMin == 1 ? 0 : tMin;

    double left = paintRect.left();
    double x = left;
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-chartMin));    

    int size;
    findFirstAndLastLap(firstLap, lastLap, size);
    firstLap = first; lastLap = last;

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);
    if (firstLap == 1)
    {
        firstLap = 0;
        xFactor = ((double)paintRect.width()) / (double)(lastLap);
    }

    if (/*lastLap - firstLap == 0 ||*/ lapDataArray.isEmpty())
        return;

    drawAxes(p, firstLap, lastLap);


    p->setRenderHint(QPainter::Antialiasing);


    int lastPaintedSCPixel = -1;
    int lastPaintedSC = -1;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        if (lapDataArray[i].getLapNumber() >= firstLap && lapDataArray[i].getLapNumber() <= lastLap)// && lapDataArray[i].getTime().isValid())
        {
            if (lapDataArray[i].getRaceLapExtraData().isSCLap() && lapDataArray[i].getLapNumber() > lastPaintedSC)
            {
                double sc_x1 = (double)(lapDataArray[i].getLapNumber() - firstLap) * xFactor + left;
                double sc_x2 = (double)(lapDataArray[i].getLapNumber()+1 - firstLap) * xFactor + left;

                if (sc_x1 < paintRect.left())
                    sc_x1 = paintRect.left();

                if (lastPaintedSCPixel == -1)
                    lastPaintedSCPixel = round(sc_x2);

                else if (abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
                {
                    sc_x1 = (double)lastPaintedSCPixel;
                    lastPaintedSCPixel = round(sc_x2);
                }
                p->setPen(QColor(255, 255, 0, 0));
                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

                p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height()-10);
                lastPaintedSC = lapDataArray[i].getLapNumber();

//                lastSCLap = lapDataArray[i].getLapNumber();
            }

            y = (double)(paintRect.top() + (double)(lapDataArray[i].getPosition()-tMin) * yFactor);
            x = (double)(lapDataArray[i].getLapNumber() - firstLap) * xFactor + left;

            //int no = EventData::getInstance()lapDataArray[i].getCarID()
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            if (y <= paintRect.bottom())
            {
                if (lapDataArray[i].getTime().toString() == "IN PIT")
                {
                    QPainterPath path;
                    p->setBrush(QBrush(color));
                    path.addEllipse(QPoint(x, y), 7, 7);
                    p->drawPath(path);
                }

            }

            if (lapDataArray[i].getLapNumber()-1 >= firstLap)//&& i > 0)
            {                
                double x1 = x;
                double y1 = y;

                if (firstLap == 0 && lapDataArray[i].getLapNumber() == 1)
                {
                    x1 = (double)(lapDataArray[i].getLapNumber() - 1) * xFactor + left;
                    y1 = (double)(paintRect.top() + (double)(getDriverStartingPosition(lapDataArray[i])-tMin) * yFactor);
                }

                else if (i > 0)
                {
                    x1 = (double)(lapDataArray[i].getLapNumber() - 1 - firstLap) * xFactor + left;
                    y1 = (double)(paintRect.top() + (double)(lapDataArray[i-1].getPosition()-tMin) * yFactor);
                }

                if ((y > paintRect.bottom() && y1 > paintRect.bottom()) || (y < paintRect.top() && y1 < paintRect.top()))
                    continue;

                ChartWidget::checkX1(x1, y1, x, y);
                ChartWidget::checkX2(x1, y1, x, y);

                p->drawLine(x1, y1, x, y);

                if (i > 0 && x1 == left && firstLap != 0)
                    drawDriversLegend(p, lapDataArray[i-1], y1);
            }
//            drawRetire(p, x, y, lapDataArray[i], color);
        }        
    }
}

void SessionPositionsChart::drawDriversLegend(QPainter *p, const LapData &ld, double y)
{
    DriverData dd = EventData::getInstance().getDriverDataById(ld.getCarID());
    if (dd.getCarID() > 0)
    {
        QColor color = getCarColor(dd.getLastLap());
        p->setBrush(color);
        p->setPen(color);
        p->drawRect(5, y-6, 4, 11);
        QString driver = SeasonData::getInstance().getDriverShortName(dd.getDriverName());
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(13, y+5, QString("%1 %2").arg(ld.getPosition()).arg(driver));
    }
}

void SessionPositionsChart::paintEvent(QPaintEvent *)
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
    drawChart(&p);

    if (scaling)
        drawScaleRect(&p);

//    drawLegend(&p, 35, 5);

    p.end();
}

void SessionPositionsChart::transform()
{
    if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
        return;

    double tmpMin = tMin;
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tmpMin));
    SessionLapTimesChart::transform();

    tMin = tmpMin + ceil((paintRect.top() + scaleRect.top()) / yFactor)-2;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor)+1;

    if (tMax > max)
        tMax = max;

//    emit zoomChanged(first, last, tMin, tMax);
}

//=========================================================================

//void SessionGapsChart::resetZoom()
//{
//    first = 1; last = 99;
//    int firstLap, lastLap, size;
//    findFirstAndLastLap(firstLap, lastLap, size);
//    tMin = min;
//    tMax = max;
//}

//void SessionGapsChart::transform()
//{
//    if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
//        return;

//    if (scaleRect == QRect())
//    {
//        first = 1;
//        last = 99;
//        tMin = min;
//        tMax = max;
//        return;
//    }

//    if (scaleRect.left() > scaleRect.right())
//        scaleRect = QRect(scaleRect.right(), scaleRect.top(), -scaleRect.width(), scaleRect.height());

//    if (scaleRect.top() > scaleRect.bottom())
//        scaleRect = QRect(scaleRect.left(), scaleRect.bottom(), scaleRect.width(), -scaleRect.height());

//    int firstLap, lastLap, size;
//    findFirstAndLastLap(firstLap, lastLap, size);
//    int sz = lastLap-firstLap+1;
//    double xFactor = ((double)paintRect.width()) / ((double)sz);
//    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

//    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
//    if (first < 1)
//        first = 1;

////	if (first >= driverData.lapData.size())
////		first = driverData.lapData.size() - 1;

//    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
////	if (last >= driverData.lapData.size())
////		last = driverData.lapData.size() - 1;

//    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
//    if (tMin < min)
//        tMin = min;
//    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);
//}

void SessionGapsChart::findFirstAndLastLap(int &firstLap, int &lastLap, int &size)
{
    firstLap = 99, lastLap = 0, size = 0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        if (lapDataArray[j].getLapNumber() < firstLap && lapDataArray[j].getLapNumber() >= first)
            firstLap = lapDataArray[j].getLapNumber();

        if (lapDataArray[j].getLapNumber() > lastLap && lapDataArray[j].getLapNumber() <= last)
            lastLap = lapDataArray[j].getLapNumber();

        if (lapDataArray[j].getLapNumber() >= first && lapDataArray[j].getLapNumber() <= last)
            ++size;
    }
}

void SessionGapsChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    double yFactor = (double)((paintRect.height()-40)/9.0);
    double yFactor2 = (double)((tMax-tMin)/9.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 40; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString::number(j, 'f', 0));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (tMax >= max)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, 15, ">1L");
        QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(paintRect.left(), paintRect.top(), paintRect.right(), paintRect.top());

        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, 35, QString(">%1").arg(max));
        pen.setColor(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(paintRect.left(), 30, paintRect.right(), 30);
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = paintRect.left();
        int prevJ = firstLap;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap) / 10.0);
        for (; i < width()-15.0 && round(j) < lastLap; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

            if (i > paintRect.left())
            {
                QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
        }
    }
}

void SessionGapsChart::drawChart(QPainter *p)
{
    int firstLap = first, lastLap = last;
    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()-40) / (double)(tMax-tMin));

    int size;
    findFirstAndLastLap(firstLap, lastLap, size);
    firstLap = first; lastLap = last;

    if (/*lastLap - firstLap == 0 ||*/ lapDataArray.isEmpty())
        return;

    drawAxes(p, firstLap, lastLap);

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);


    int lastPaintedSCPixel = -1;
    int lastPaintedSC = -1;

    int lapsInWindow = 0;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        if (lapDataArray[i].getLapNumber() >= firstLap && lapDataArray[i].getLapNumber() <= lastLap)// && lapDataArray[i].getTime().isValid())
        {
            if (lapDataArray[i].getRaceLapExtraData().isSCLap() && lapDataArray[i].getLapNumber() > lastPaintedSC)
            {
                double sc_x1 = (double)(lapDataArray[i].getLapNumber()-1 - firstLap) * xFactor + (double)paintRect.left();
                double sc_x2 = (double)(lapDataArray[i].getLapNumber() - firstLap) * xFactor + (double)paintRect.left();

                if (sc_x1 < paintRect.left())
                    sc_x1 = paintRect.left();

                if (lastPaintedSCPixel == -1)
                    lastPaintedSCPixel = round(sc_x2);

                else if (abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
                {
                    sc_x1 = (double)lastPaintedSCPixel;
                    lastPaintedSCPixel = round(sc_x2);
                }
                p->setPen(QColor(255, 255, 0, 0));
                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

                p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height());
                lastPaintedSC = lapDataArray[i].getLapNumber();

//                lastSCLap = lapDataArray[i].getLapNumber();
            }

            double gap = lapDataArray[i].getGap().toDouble();

            if (lapDataArray[i].getGap().size() > 0 && lapDataArray[i].getGap()[lapDataArray[i].getGap().size()-1] == 'L')
            {
                if (tMax >= max)
                    gap = -1.0;
                else
                    gap = max;
            }

            if (lapDataArray[i].getPosition() == 1)
                gap = 0.0;

            if (tMax >= max && lapDataArray[i].getGap() == "")
                gap = -1.0;

            y = (double)(paintRect.bottom() - (double)(gap-tMin) * yFactor);

            if (gap == -1.0 && tMax >= max)
                y = 10;

            else if (gap > tMax && tMax >= max)
                y = 30;

            x = (double)(lapDataArray[i].getLapNumber() - firstLap) * xFactor + (double)paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].getCarID()
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            if (y <= paintRect.bottom())
            {
                QPainterPath path;
                p->setBrush(QBrush(color));
                if (lapDataArray[i].getTime().toString() == "IN PIT")
                {                    
                    path.addEllipse(QPoint(x, y), 7, 7);                    
                }
                else
                {
                    path.addEllipse(QPoint(x, y), 2, 2);
                }
                p->drawPath(path);

                if (!scaling)
                {
                    if (lapsInWindow >= lapDataXYArray.size())
                        lapDataXYArray.append(LapDataXY(i, (int)x, (int)y));
                    else
                        lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)x, (int)y);
                }
                ++lapsInWindow;
            }

            if (lapDataArray[i].getLapNumber()-1 >= firstLap && i > 0)
            {
                if (lapDataArray[i].getCarID() == lapDataArray[i-1].getCarID())
                {
                    gap = lapDataArray[i-1].getGap().toDouble();

                    if (lapDataArray[i-1].getGap().size() > 0 && lapDataArray[i-1].getGap()[lapDataArray[i-1].getGap().size()-1] == 'L')
                    {
                        if (tMax >= max)
                            gap = -1.0;
                        else
                            gap = max;
                    }

                    if (lapDataArray[i-1].getPosition() == 1)
                        gap = 0.0;

                    if (tMax >= max && lapDataArray[i-1].getGap() == "")
                        gap = -1.0;

                    double y1 = (double)(paintRect.bottom() - (double)(gap-tMin) * yFactor);

                    if (gap == -1.0 && tMax >= max)
                        y1 = 10;

                    else if (gap > tMax && tMax >= max)
                        y1 = 30;


                    double x1 = (double)(lapDataArray[i].getLapNumber() - 1 - firstLap) * xFactor + (double)paintRect.left();


                    if ((y > paintRect.bottom() && y1 > paintRect.bottom()) || (y < paintRect.top() && y1 < paintRect.top()))
                        continue;

                    ChartWidget::checkX1(x1, y1, x, y);
                    ChartWidget::checkX2(x1, y1, x, y);

                    if (y1 >= paintRect.top() || y >= paintRect.top())
                    p->drawLine(x1, y1, x, y);
                }
            }
        }       
    }
    if (!scaling)
        clearXYList(lapsInWindow);
}

void SessionGapsChart::paintEvent(QPaintEvent *)
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

int SessionGapsChart::findLapDataXY(int x, int y)
{
    itemsInXY.clear();
    for (int i = 0; i < lapDataXYArray.size(); ++i)
    {
        if (abs(lapDataXYArray[i].x - x) <= 3 && abs(lapDataXYArray[i].y - y) <= 3)
            itemsInXY.append(lapDataXYArray[i].idx);
    }
    qSort(itemsInXY.begin(), itemsInXY.end(), PredXYPos(*this));
    return itemsInXY.size();
}
