#include "sessionlaptimeschart.h"

#include <QDebug>
#include <QPainter>

bool lessThan(LapData ld1, LapData ld2)
{
    if (ld1.carID > 0 && ld2.carID > 0)
    {
        DriverData dd1 = EventData::getInstance().driversData[ld1.carID-1];
        DriverData dd2 = EventData::getInstance().driversData[ld2.carID-1];

        if (dd1.pos > dd2.pos)
            return true;

        if (dd1.pos < dd2.pos)
            return false;

        if (dd1.pos == dd2.pos)
        {
            if (dd1.carID != dd2.carID)
                return dd1.carID < dd2.carID;

            if (ld1.numLap < ld2.numLap)
                return true;
        }
    }
//    else
//    {
//        if (ld1.carID < ld2.carID)
//            return true;

//        if (ld1.carID > ld2.carID)
//            return false;

//        if (ld1.carID == ld2.carID)
//        {
//            if (ld1.numLap < ld2.numLap)
//                return true;
//        }
//    }
    return false;
}

void SessionLapTimesChart::findFirstAndLastLap(int &firstLap, int &lastLap, int &size)
{
    firstLap = 99, lastLap = 0, size = 0;
    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        if (lapDataArray[j].numLap < firstLap && lapDataArray[j].numLap >= first)
            firstLap = lapDataArray[j].numLap;

        if (lapDataArray[j].numLap > lastLap && lapDataArray[j].numLap <= last)
            lastLap = lapDataArray[j].numLap;

        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last && lapDataArray[j].lapTime.toDouble() < lMin && lapDataArray[j].lapTime.isValid())
            lMin = lapDataArray[j].lapTime.toDouble();

        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last && lapDataArray[j].lapTime.toDouble() > lMax && lapDataArray[j].lapTime.isValid())
            lMax = lapDataArray[j].lapTime.toDouble();

        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last)
            ++size;
    }
    qDebug() << "min=" << lMin;

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
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

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

void SessionLapTimesChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    double secs;

    int size;
    findFirstAndLastLap(firstLap, lastLap, size);

    if (lastLap - firstLap == 0 || lapDataArray.isEmpty())
        return;

    drawAxes(p, firstLap, lastLap);

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);


    int lastPaintedSCPixel = -1;
    int lastPaintedSC = -1;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        if (lapDataArray[i].numLap >= firstLap && lapDataArray[i].numLap <= lastLap)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();
            if (!lapDataArray[i].lapTime.isValid() && lapDataArray[i].numLap-1 >= firstLap && i > 0)// && i < lapDataArray.size()-1)
            {
                secs = lapDataArray[i-1].lapTime.toDouble();

                if (!lapDataArray[i-1].lapTime.isValid() && lapDataArray[i].numLap+1 <= lastLap && i < lapDataArray.size()-1)
                {
                    QString pl =  EventData::getInstance().driversData[lapDataArray[i].carID-1].getPitTime(lapDataArray[i].numLap);
                    secs = LapTime(lapDataArray[i+1].lapTime + LapTime(pl) + LapTime(5000)).toDouble();
                }
            }

            if (lapDataArray[i].scLap && lapDataArray[i].numLap > lastPaintedSC)
            {
                double sc_x1 = (double)(lapDataArray[i].numLap-1 - firstLap) * xFactor + (double)paintRect.left();
                double sc_x2 = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

                if (sc_x1 < paintRect.left())
                    sc_x1 = paintRect.left();

                if (lastPaintedSCPixel == -1)
                    lastPaintedSCPixel = round(sc_x2);

                else if (std::abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
                {
                    sc_x1 = (double)lastPaintedSCPixel;
                    lastPaintedSCPixel = round(sc_x2);
                }
                p->setPen(QColor(255, 255, 0, 0));
                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

                p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height());
                lastPaintedSC = lapDataArray[i].numLap;

//                lastSCLap = lapDataArray[i].numLap;
            }
//            if (firstSCLap != lastSCLap && firstSCLap >= lastPaintedSC)
//            {
//                double x1 = (double)(firstSCLap - firstLap) * xFactor + (double)paintRect.left();
//                double x2 = (double)(lastSCLap - firstLap) * xFactor + (double)paintRect.left();
//                p->setPen(QColor(255, 255, 0, 35));
//                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

//                p->drawRect(x1, paintRect.top(), x2-x1, paintRect.height());
//                lastPaintedSC = lastSCLap;
//                firstSCLap = -1, lastSCLap = -1;
//            }


            if (secs > tMax && tMax == max)
                secs = tMax;

//            if (secs < tMin)
//                secs = tMin;

            y = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
            x = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

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
                if (lapDataArray[i].lapTime.toString() == "IN PIT")
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

            if (lapDataArray[i].numLap-1 >= firstLap && i > 0)
            {
                double x1 = (double)(lapDataArray[i].numLap - 1 - firstLap) * xFactor + (double)paintRect.left();
                secs = lapDataArray[i-1].lapTime.toDouble();

                if (!lapDataArray[i-1].lapTime.isValid() && i-2 >= firstLap)
                {
                    LapTime lt;
                    secs = lapDataArray[i-2].lapTime.toDouble();

                    if (!lapDataArray[i-2].lapTime.isValid())// && i < lapDataArray.size()-1)
                    {
                        QString pl = EventData::getInstance().driversData[lapDataArray[i-1].carID-1].getPitTime(lapDataArray[i-1].numLap);
                        secs = LapTime(lapDataArray[i].lapTime + LapTime(pl) + LapTime(5000)).toDouble();
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

//                LapData ld = dd.getLapData(lapDataArray[i].numLap-1);

//                if (ld.carID == dd.carID)
//                {
//                    secs = ld.lapTime.toDouble();
//                    double x1 = (double)(lapDataArray[i].numLap - 1 - firstLap) * xFactor + (double)paintRect.left();
//                    double y1 = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);

//                    p->drawLine(x1, y1, x, y);
//                }
            }
        }
    }   
}

QColor SessionLapTimesChart::getCarColor(const LapData &ld)
{
    QColor color;
    if (ld.carID > 0)
    {
        int no = EventData::getInstance().driversData[ld.carID-1].number;

        if (no > 0 && no < colors.size()+2)
            color = colors[no <= 12 ? no-1 : no -2];
    }

    return color;
}

void SessionLapTimesChart::paintEvent(QPaintEvent *)
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

void SessionLapTimesChart::resetZoom()
{
    first = 1; last = 99;
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
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
    int sz = lastLap-firstLap+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

    emit zoomChanged(first, last, tMin, tMax);
}

void SessionLapTimesChart::onZoomOut()
{
    if (scaleRectsStack.size() > 1)
    {
//		if (lastOp == 1)
//			scaleRectsStack.pop();

        ChartWidget::ScaleAtom sa = scaleRectsStack.pop();
        resetZoom();

        first = sa.first; last = sa.last;
        tMin = sa.min; tMax = sa.max;
//		scaleRect = sa.rect;

//		transform();
        emit zoomChanged(first, last, tMin, tMax);
        repaint();
    }
    else
    {
        scaleRectsStack.clear();
        scaleRect = QRect();
        resetZoom();
//		transform();
        repaint();
        emit zoomChanged(first, last, -1, -1);
    }
    lastOp = 0;

}

void SessionLapTimesChart::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (!scaling)
    {
        lastOp = 0;
        scaleRect = QRect();
        resetZoom();
        transform();
        scaleRectsStack.clear();
        repaint();

        emit zoomChanged(first, last, -1, -1);
    }
}

//=========================================================================

void SessionPositionsChart::resetZoom()
{
    first = 1; last = 99;
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
    tMin = min;
    tMax = max;
}

void SessionPositionsChart::transform()
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
    int sz = lastLap-firstLap+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);
}

void SessionPositionsChart::findFirstAndLastLap(int &firstLap, int &lastLap, int &size)
{
    firstLap = 99, lastLap = 0, size = 0;
//    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        if (lapDataArray[j].numLap < firstLap && lapDataArray[j].numLap >= first)
            firstLap = lapDataArray[j].numLap;

        if (lapDataArray[j].numLap > lastLap && lapDataArray[j].numLap <= last)
            lastLap = lapDataArray[j].numLap;

//        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last && lapDataArray[j].pos < lMin)
//            lMin = (int)lapDataArray[j].pos;

//        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last && lapDataArray[j].pos)
//            lMax = (int)lapDataArray[j].pos;

        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last)
            ++size;
    }
}

void SessionPositionsChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    int chartMin = (tMin == 1 ? 0 : tMin);
    double yFactor = (double)((paintRect.height())/(tMax-chartMin));
    double yFactor2 = 1.0;//(double)((tMax-chartMin)/12.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(round(j)));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
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
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

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

void SessionPositionsChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    int chartMin = tMin == 1 ? 0 : tMin;

    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-chartMin));

    int size;
    findFirstAndLastLap(firstLap, lastLap, size);

    if (lastLap - firstLap == 0 || lapDataArray.isEmpty())
        return;

    drawAxes(p, firstLap, lastLap);

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);


    int lastPaintedSCPixel = -1;
    int lastPaintedSC = -1;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        if (lapDataArray[i].numLap >= firstLap && lapDataArray[i].numLap <= lastLap)// && lapDataArray[i].lapTime.isValid())
        {


            if (lapDataArray[i].scLap && lapDataArray[i].numLap > lastPaintedSC)
            {
                double sc_x1 = (double)(lapDataArray[i].numLap-1 - firstLap) * xFactor + (double)paintRect.left();
                double sc_x2 = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

                if (sc_x1 < paintRect.left())
                    sc_x1 = paintRect.left();

                if (lastPaintedSCPixel == -1)
                    lastPaintedSCPixel = round(sc_x2);

                else if (std::abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
                {
                    sc_x1 = (double)lastPaintedSCPixel;
                    lastPaintedSCPixel = round(sc_x2);
                }
                p->setPen(QColor(255, 255, 0, 0));
                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

                p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height());
                lastPaintedSC = lapDataArray[i].numLap;

//                lastSCLap = lapDataArray[i].numLap;
            }

            y = (double)(paintRect.bottom() - (double)(lapDataArray[i].pos-tMin) * yFactor);
            x = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].carID
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            if (y <= paintRect.bottom())
            {
                if (lapDataArray[i].lapTime.toString() == "IN PIT")
                {
                    QPainterPath path;
                    p->setBrush(QBrush(color));
                    path.addEllipse(QPoint(x, y), 7, 7);
                    p->drawPath(path);
                }

            }

            if (lapDataArray[i].numLap-1 >= firstLap && i > 0)
            {
                double x1 = (double)(lapDataArray[i].numLap - 1 - firstLap) * xFactor + (double)paintRect.left();
                double y1 = (double)(paintRect.bottom() - (double)(lapDataArray[i-1].pos-tMin) * yFactor);


                if ((y > paintRect.bottom() && y1 > paintRect.bottom()) || (y < paintRect.top() && y1 < paintRect.top()))
                    continue;

                ChartWidget::checkX1(x1, y1, x, y);
                ChartWidget::checkX2(x1, y1, x, y);

                p->drawLine(x1, y1, x, y);
            }
        }
    }
}

void SessionPositionsChart::paintEvent(QPaintEvent *)
{
    paintRect = QRect(30, 10, width()-35, height()-35);

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

//=========================================================================

void SessionGapsChart::resetZoom()
{
    first = 1; last = 99;
    int firstLap, lastLap, size;
    findFirstAndLastLap(firstLap, lastLap, size);
    tMin = min;
    tMax = max;
}

void SessionGapsChart::transform()
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
    int sz = lastLap-firstLap+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);
}

void SessionGapsChart::findFirstAndLastLap(int &firstLap, int &lastLap, int &size)
{
    firstLap = 99, lastLap = 0, size = 0;
//    double lMin = 99999999.0, lMax = 0.0;

    for (int j = 0; j < lapDataArray.size(); ++j)
    {
        if (lapDataArray[j].numLap < firstLap && lapDataArray[j].numLap >= first)
            firstLap = lapDataArray[j].numLap;

        if (lapDataArray[j].numLap > lastLap && lapDataArray[j].numLap <= last)
            lastLap = lapDataArray[j].numLap;

//        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last && lapDataArray[j].pos < lMin)
//            lMin = (int)lapDataArray[j].pos;

//        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last && lapDataArray[j].gap.toDouble() > lMax)
//            lMax = lapDataArray[j].gap.toDouble();

        if (lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last)
            ++size;
    }
}

void SessionGapsChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((paintRect.height()-40)/10.0);
    double yFactor2 = (double)((tMax-tMin)/10.0);
    double j = tMin;
    for (double i = paintRect.bottom(); i >= 50; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString::number(j, 'f', 0));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (tMax >= max)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, 15, ">1L");
        QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(paintRect.left(), paintRect.top(), paintRect.right(), paintRect.top());

        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, 35, QString(">%1").arg(max));
        pen.setColor(QColor(LTData::colors[LTData::DEFAULT]));
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
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

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

void SessionGapsChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;
    double x = paintRect.left();
    double y;
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));

    int size;
    findFirstAndLastLap(firstLap, lastLap, size);

    if (lastLap - firstLap == 0 || lapDataArray.isEmpty())
        return;

    drawAxes(p, firstLap, lastLap);

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);


    int lastPaintedSCPixel = -1;
    int lastPaintedSC = -1;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        if (lapDataArray[i].numLap >= firstLap && lapDataArray[i].numLap <= lastLap)// && lapDataArray[i].lapTime.isValid())
        {


            if (lapDataArray[i].scLap && lapDataArray[i].numLap > lastPaintedSC)
            {
                double sc_x1 = (double)(lapDataArray[i].numLap-1 - firstLap) * xFactor + (double)paintRect.left();
                double sc_x2 = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

                if (sc_x1 < paintRect.left())
                    sc_x1 = paintRect.left();

                if (lastPaintedSCPixel == -1)
                    lastPaintedSCPixel = round(sc_x2);

                else if (std::abs(round(sc_x1) - lastPaintedSCPixel) <= 5)
                {
                    sc_x1 = (double)lastPaintedSCPixel;
                    lastPaintedSCPixel = round(sc_x2);
                }
                p->setPen(QColor(255, 255, 0, 0));
                p->setBrush(QBrush(QColor(255, 255, 0, 35)));

                p->drawRect(round(sc_x1), paintRect.top(), round(sc_x2-sc_x1), paintRect.height());
                lastPaintedSC = lapDataArray[i].numLap;

//                lastSCLap = lapDataArray[i].numLap;
            }

            double gap = lapDataArray[i].gap.toDouble();

            if (tMax >= max && lapDataArray[i].gap.size() > 0 && lapDataArray[i].gap[lapDataArray[i].gap.size()-1] == 'L')
                gap = -1.0;

            if (lapDataArray[i].pos == 1)
                gap = 0.0;

            if (tMax >= max && lapDataArray[i].gap == "")
                gap = max;

            y = (double)(paintRect.bottom() - (double)(gap-tMin) * yFactor);

            if (gap == -1.0 && tMax >= max)
                y = 10;

            else if (gap > tMax && tMax >= max)
                y = 30;

            x = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].carID
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(3);
            pen.setColor(color);
            p->setPen(pen);

            if (y <= paintRect.bottom())
            {
                if (lapDataArray[i].lapTime.toString() == "IN PIT")
                {
                    QPainterPath path;
                    p->setBrush(QBrush(color));
                    path.addEllipse(QPoint(x, y), 7, 7);
                    p->drawPath(path);
                }

            }

            if (lapDataArray[i].numLap-1 >= firstLap && i > 0)
            {

                gap = lapDataArray[i-1].gap.toDouble();

                if (tMax >= max && lapDataArray[i-1].gap.size() > 0 && lapDataArray[i-1].gap[lapDataArray[i-1].gap.size()-1] == 'L')
                    gap = -1.0;

                if (lapDataArray[i-1].pos == 1)
                    gap = 0.0;

                if (tMax >= max && lapDataArray[i-1].gap == "")
                    gap = max;

                double y1 = (double)(paintRect.bottom() - (double)(gap-tMin) * yFactor);

                if (gap == -1.0 && tMax >= max)
                    y1 = 10;

                else if (gap > tMax && tMax >= max)
                    y1 = 30;


                double x1 = (double)(lapDataArray[i].numLap - 1 - firstLap) * xFactor + (double)paintRect.left();


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

void SessionGapsChart::paintEvent(QPaintEvent *)
{
    paintRect = QRect(30, 10, width()-35, height()-35);

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
