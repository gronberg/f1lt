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
            if (ld1.numLap < ld2.numLap)
                return true;
        }
    }
    else
    {
        if (ld1.carID < ld2.carID)
            return true;

        if (ld1.carID > ld2.carID)
            return false;

        if (ld1.carID == ld2.carID)
        {
            if (ld1.numLap < ld2.numLap)
                return true;
        }
    }
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
    for (int i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
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
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(lastLap - firstLap + 1) /*: 5)*/;
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

    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap + 1);

    p->setRenderHint(QPainter::Antialiasing);


    int firstSCLap = -1, lastSCLap = -1;
    for (int i = 0; i < lapDataArray.size(); ++i)
    {
        qDebug() << "nlap=" << lapDataArray[i].numLap << " " << firstLap << " " << lastLap;
        if (lapDataArray[i].numLap >= firstLap && lapDataArray[i].numLap <= lastLap + 1)// && lapDataArray[i].lapTime.isValid())
        {
            secs = lapDataArray[i].lapTime.toDouble();

            if (!lapDataArray[i].lapTime.isValid() && lapDataArray[i].numLap+1 <= lastLap && i < lapDataArray.size()-1)
            {
                secs = lapDataArray[i+1].lapTime.toDouble();
            }

            if (lapDataArray[i].scLap)
            {
                if (firstSCLap == -1)
                    firstSCLap = lapDataArray[i].numLap;

                lastSCLap = lapDataArray[i].numLap;
            }


            if (secs > tMax)
                secs = tMax;

//            if (secs < tMin)
//                secs = tMin;

            y = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
            x = (double)(lapDataArray[i].numLap - firstLap) * xFactor + (double)paintRect.left();

            //int no = EventData::getInstance()lapDataArray[i].carID
            QColor color = getCarColor(lapDataArray[i]);

            QPen pen;
            pen.setWidth(2);
            pen.setColor(color);
            p->setPen(pen);

            QPainterPath path;
            p->setBrush(QBrush(color));
            if (lapDataArray[i].lapTime.toString() == "IN PIT" && y < paintRect.bottom())
            {
//                p->setBrush(QBrush(QColor()));
                path.addEllipse(QPoint(x, y), 6, 6);
            }
//            else
//            {

//                path.addEllipse(QPoint(x, y), 3, 3);
//            }
            p->drawPath(path);

            if (lapDataArray[i].numLap-1 >= firstLap && i > 0)
            {
                double x1 = (double)(lapDataArray[i].numLap - 1 - firstLap) * xFactor + (double)paintRect.left();
                secs = lapDataArray[i-1].lapTime.toDouble();

                if (!lapDataArray[i-1].lapTime.isValid())
                {
                    secs = lapDataArray[i].lapTime.toDouble();

                    if (!lapDataArray[i].lapTime.isValid() && i < lapDataArray.size()-1)
                    {
                        secs = lapDataArray[i+1].lapTime.toDouble();
                    }
                }

                if (secs > tMax)
                    secs = tMax;

//                if (secs < tMin)
//                    secs = tMin;

                double y1 = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);


                if (y > paintRect.bottom() && y1 > paintRect.bottom())
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

    if (firstSCLap != lastSCLap)
    {
        double x1 = (double)(firstSCLap - firstLap) * xFactor + (double)paintRect.left();
        double x2 = (double)(lastSCLap - firstLap) * xFactor + (double)paintRect.left();
        p->setPen(QColor(255, 255, 0, 35));
        p->setBrush(QBrush(QColor(255, 255, 0, 35)));

        p->drawRect(x1, paintRect.top(), x2-x1, paintRect.height());
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
