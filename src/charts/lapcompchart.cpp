#include "lapcompchart.h"

#include <QPainter>


bool PredXYTime::operator ()(int item1, int item2)
{
    int didx1 = chart.lapDataXYCompArray[item1].driverIdx;
    int lidx1 = chart.lapDataXYCompArray[item1].idx;

    int didx2 = chart.lapDataXYCompArray[item2].driverIdx;
    int lidx2 = chart.lapDataXYCompArray[item2].idx;

    LapData ld1 = chart.driverData[didx1].getLapData(lidx1);
    LapData ld2 = chart.driverData[didx2].getLapData(lidx2);

    if (ld1.getTime().isValid() && ld2.getTime().isValid())
        return ld1.getTime() < ld2.getTime();

    if (ld1.getTime().isValid() && !ld2.getTime().isValid())
        return true;

    return false;
}

bool PredXYGap::operator ()(int item1, int item2)
{
    return  chart.lapDataXYGapCompArray[item1].gap < chart.lapDataXYGapCompArray[item2].gap;
//    LapData ld1 = chart.lapDataArray[item1];
//    LapData ld2 = chart.lapDataArray[item2];

//    QString sgap1 = ld1.gap;
//    QString sgap2 = ld2.gap;

//    if (sgap1 == "")
//        sgap1 = "1L";

//    if (sgap2 == "")
//        sgap2 = "1L";

//    bool ok1=true, ok2=true;
//    double gap1 = sgap1.toDouble(&ok1);
//    double gap2 = sgap2.toDouble(&ok2);

//    if (ok1 && ok2)
//        return gap1 < gap2;

//    if (sgap1 == "LAP")
//        return true;

//    if (sgap2 == "LAP")
//        return false;

//    if (sgap1.size() > 1 && sgap1[sgap1.size()-1] == 'L' &&
//        sgap2.size() > 1 && sgap2[sgap2.size()-1] == 'L')
//    {
//        gap1 = sgap1.left(sgap1.size()-1).toDouble();
//        gap2 = sgap2.left(sgap2.size()-1).toDouble();

//        return gap1 < gap2;
//    }

//    return false;
}


void LapCompChart::setData(DriverData *dd)
{
    double lMin = 99999999.0, lMax = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        driverData[i] = dd[i];

        for (int j = 0; j < driverData[i].getLapData().size(); ++j)
        {
            double secs = driverData[i].getLapData()[j].getTime().toDouble();//-QTime::fromString(driverData[i].getLapData()[j].getTime(), "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));

            if (secs == 0 && j > 0)
                secs = driverData[i].getLapData()[j-1].getTime().toDouble();//-QTime::fromString(driverData[i].getLapData()[j-1].getTime(), "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));

            if (secs == 0 && j < driverData[i].getLapData().size()-1)
            {
                secs = driverData[i].getLapData()[j+1].getTime().toDouble();//-QTime::fromString(driverData[i].getLapData()[j+1].getTime(), "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
                if (driverData[i].getLapData()[j].getTime().toString() == "IN PIT")
                {
                    LapTime pl(driverData[i].getPitTime(driverData[i].getLapData()[j].getLapNumber()));
                    secs = (driverData[i].getLapData()[j+1].getTime() - pl + LapTime(5000)).toDouble();
                }
            }

            if (secs < lMin)
                lMin = secs;

            if (secs > lMax)
                lMax = secs;
        }
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

void LapCompChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    double yFactor = (double)((paintRect.height())/6.0);
    double yFactor2 = (double)((tMax-tMin)/6.0);
    double j = tMin;
    for (int i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(j, 0, 'f', 1));

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

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
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

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}

void LapCompChart::findFirstAndLastLap(int &firstLap, int &lastLap)
{
    firstLap = 99, lastLap = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (!driverData[i].getLapData().isEmpty())
        {
            for (int j = 0; j < driverData[i].getLapData().size(); ++j)
            {
                if (driverData[i].getLapData()[j].getLapNumber() < firstLap && driverData[i].getLapData()[j].getLapNumber() >= first)
                    firstLap = driverData[i].getLapData()[j].getLapNumber();

                if (driverData[i].getLapData()[j].getLapNumber() > lastLap && driverData[i].getLapData()[j].getLapNumber() <= last)
                    lastLap = driverData[i].getLapData()[j].getLapNumber();

            }
//			msecs = -QTime::fromString(driverData[i].getLapData()[0].getTime(), "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//			secs = (double)(msecs / 1000.0);
//
//			if (secs > max) secs = max;
//			if (secs < min) secs = min;
        }
    }
}

void LapCompChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[4] = {paintRect.left(), paintRect.left(), paintRect.left(), paintRect.left()};
    double y1[4];
    double y2[4];
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));
    int msecs;
    double secs;

    for (int i = 0; i < 4; ++i)
    {
        if (!driverData[i].getLapData().isEmpty())
        {
//            if (driverData[i].getLapData()[0].getLapNumber() < firstLap)
//                firstLap = driverData[i].getLapData()[0].getLapNumber();
//
//            if (driverData[i].lapData.last().getLapNumber() > lastLap)
//                lastLap = driverData[i].lapData.last().getLapNumber();
//
            msecs = -QTime::fromString(driverData[i].getLapData()[0].getTime(), "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
            secs = (double)(msecs / 1000.0);

            if (secs > max) secs = max;
            if (secs < min) secs = min;
            y1[i] = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);
        }
    }
    findFirstAndLastLap(firstLap, lastLap);
//    firstLap = first; lastLap = last;
    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);
    int index[4] = {0,0,0,0};
    double j[4] = {(x[0]), (x[0]), (x[0]), (x[0])};
    int lapsInWindow = 0;
    int lastPaintedSC = 0;
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 4; ++k)
        {
            LapData ld = driverData[k].getLapData(i);
//            if (!driverData[k].lapData.empty() && index[k] < driverData[k].lapData.size() && driverData[k].getLapData()[index[k]].getLapNumber() == i)
            if (ld.getCarID() == driverData[k].getCarID() && ld.getLapNumber() == i)
            {
                LapTime lapTime = ld.getTime();//driverData[k].getLapData()[index[k]].getTime();

                if (ld.getRaceLapExtraData().isSCLap() && ld.getLapNumber() > lastPaintedSC)
                {
                    drawSCLap(p, ld, xFactor);
                    lastPaintedSC = ld.getLapNumber();
                }

                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                if (lapTime.toString() == "IN PIT")
                {
                    LapData ldtmp = driverData[k].getLapData(i-1);
                    if (ldtmp.getCarID() == driverData[k].getCarID())
                        lapTime = ldtmp.getTime();

                    else
                    {
                        ldtmp = driverData[k].getLapData(i+1);
                        if (ldtmp.getCarID() == driverData[k].getCarID())
                        {
                            LapTime pl(driverData[k].getPitTime(ld.getLapNumber()));
                            lapTime = lapTime - pl + LapTime(5000);
                        }

                        if (lapTime.toString() == "IN PIT")
                        {
                            ldtmp = driverData[k].getLapData(i+2);
                            if (ldtmp.getCarID() == driverData[k].getCarID())
                            {
                                LapTime pl(driverData[k].getPitTime(ld.getLapNumber()+1));
                                lapTime = lapTime - pl + LapTime(5000);
                            }
                        }
                    }

                }
                msecs = -QTime::fromString(lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
                secs = (double)(msecs / 1000.0);

                if (secs > max) secs = max;
                if (secs < min) secs = min;
                y2[k] = (double)(paintRect.bottom() - (double)(secs-tMin) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                double dx1 = x[k], dx2 = j[k], dy1 = y1[k], dy2 = y2[k];

                if (dy1 <= paintRect.bottom() || dy2 <= paintRect.bottom())
                {
                    checkX1(dx1, dy1, dx2, dy2);
                    checkX2(dx1, dy1, dx2, dy2);

                    if (!scaling)
                    {
                        if (lapsInWindow >= lapDataXYCompArray.size())
                            lapDataXYCompArray.append(LapDataXYComp(i, (int)dx2, (int)dy2, k));
                        else
                            lapDataXYCompArray[lapsInWindow] = LapDataXYComp(i, (int)dx2, (int)dy2, k);
                    }
                    ++lapsInWindow;

                    int pointSize = 3;
                    if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
                    {
                        p->drawLine(dx1, dy1, dx2, dy2);
                        pointSize = 2;
                    }
                    p->setBrush(QBrush(colors[k]));

                    QPainterPath path;
                    if (y2[k] <= paintRect.bottom())
                    {
                        if (ld.getTime().toString() == "IN PIT")
                            path.addEllipse(QPoint(j[k], y2[k]), 6, 6);

                        else
                            path.addEllipse(QPoint(j[k], y2[k]), pointSize, pointSize);

                        p->drawPath(path);
                    }
                }

                y1[k] = y2[k];
                x[k] = j[k];

                j[k] += xFactor;

                ++index[k];
            }
            else
            {
                x[k] += xFactor;
                j[k] = x[k];
            }
        }
    }
    clearXYList(lapsInWindow);

}

void LapCompChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        resetZoom();
    }

    QPainter p;
    p.begin(this);

    drawChart(&p);
    if (scaling)
        drawScaleRect(&p);
    else
    {
        if (!repaintPopup)
            findLapDataXY(mousePosX, mousePosY);

        drawLapDataXY(&p);
    }

    p.end();
}

void LapCompChart::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    int yy = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (driverData[i].getCarID() > 0)
        {
            p->setPen(QColor(20, 20, 20));
            p->drawRect(40, yy, 115, 20);

            p->setPen(colors[i]);
            p->drawText(45, yy+20, driverData[i].getDriverName());
            yy += 20;
        }
    }
    if (yy > 5)
    {
        p->setBrush(QBrush());
        p->setPen(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
        p->drawRect(40, 5, 115, yy);
    }
}

int LapCompChart::findLapDataXY(int x, int y)
{
    itemsInXY.clear();
    for (int i = 0; i < lapDataXYCompArray.size(); ++i)
    {
        if (abs(lapDataXYCompArray[i].x - x) <= 3 && abs(lapDataXYCompArray[i].y - y) <= 3)
            itemsInXY.append(i);
    }
    qSort(itemsInXY.begin(), itemsInXY.end(), PredXYTime(*this));
    return itemsInXY.size();
}

void LapCompChart::drawLapDataXY(QPainter *p)
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

    int didx = lapDataXYCompArray[itemsInXY[0]].driverIdx;
    int lidx = lapDataXYCompArray[itemsInXY[0]].idx;

    p->drawText(x+25, y+15, getLapInfoXY(driverData[didx].getLapData(lidx)));

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    for (int i = 0; i < itemsInXY.size(); ++i)
    {
        int didx = lapDataXYCompArray[itemsInXY[i]].driverIdx;
        int lidx = lapDataXYCompArray[itemsInXY[i]].idx;
        p->drawText(x+25, y+(i+1)*20+15, getDriverInfoXY(driverData[didx].getLapData(lidx)));
    }
}

void LapCompChart::drawIntoImage(QImage &img)
{
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();
}

void LapCompChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1; last = 99;
}

void LapCompChart::transform()
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

    int firstLap, lastLap;
    findFirstAndLastLap(firstLap, lastLap);
    int sz = lastLap-firstLap+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);

    if (first >= last)
        first = last - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

//========================================================================

double GapCompChart::calculateInterval(int lap)
{
    LapData ld1 = (driverIdx[0] >= 0) ? eventData.getDriversData()[driverIdx[0]].getLapData(lap) : LapData();
    LapData ld2 = (driverIdx[1] >= 0) ? eventData.getDriversData()[driverIdx[1]].getLapData(lap) : LapData();
    QString gap1 = ld1.getGap();
    QString gap2 = ld2.getGap();

    if ((ld1.getTime().toString() == "" && ld1.getGap() == "") || (ld2.getTime().toString() == "" && ld2.getGap() == ""))
        return 0.0;

    if ((gap1 != "" && gap2 != "" && gap1[gap1.size()-1] != 'L' && gap2[gap2.size()-1] != 'L') ||
        ((ld1.getPosition() == 1 && gap1.isNull()) || (ld2.getPosition() == 1 && gap2.isNull())))
    {
        double interval = gap1.toDouble() - gap2.toDouble();
        return interval;
    }
    else if ((gap1 != "" && gap1[gap1.size()-1] == 'L') || (gap2 != "" && gap2[gap2.size()-1] == 'L'))
    {
        int pos1 = ld1.getPosition();
        int pos2 = ld2.getPosition();

        bool neg = true;
        if (pos2 < pos1)
        {
            int tmp = pos1;
            pos1 = pos2;
            pos2 = tmp;
            neg = false;
        }

        QList<QString> intervals;
//        intervals.reserve(pos2 - pos1);
        for (int i = 0; i < eventData.getDriversData().size(); ++i)
        {
            LapData ld = eventData.getDriversData()[i].getLapData(lap);
            int pos = ld.getPosition();
            if (pos > pos1 && pos <= pos2)
            {
                if (ld.getInterval() != "" && ld.getInterval()[ld.getInterval().size()-1] == 'L')
                    return neg ? -1000.0 : 1000.0;

                intervals.append(ld.getInterval());
            }
        }
        double interval = 0.0;
        for (int i = 0; i < intervals.size(); ++i)
            interval += intervals[i].toDouble();

        if (neg && ld1.getTime().isValid() && interval > ld1.getTime().toDouble())
            return -1000.0;
        if (!neg && ld2.getTime().isValid() && interval > ld2.getTime().toDouble())
            return 1000.0;


        return neg ? -interval : interval;
    }

    return 0.0;
}


void GapCompChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    if (tMax == max && max >= 60)
    {
        max = 60;
        tMax = 60;
    }

    double yFactor = (double)(paintRect.height()/6.0);
    double yFactor2 = (double)((tMax-tMin)/6.0);
    double j = tMin;
    for (int i = paintRect.bottom(); i > 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(j, 0, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (max == 60)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, 15, ">60");
        QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(paintRect.left(), 10, paintRect.right(), 10);
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = paintRect.left();
        int prevJ = firstLap;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
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

void GapCompChart::findFirstAndLastLap(int &firstLap, int &lastLap)
{
    firstLap = 99, lastLap = 0;
    for (int i = 0; i < 4; ++i)
    {
        DriverData dd = driverIdx[i] >= 0 ? eventData.getDriversData()[driverIdx[i]] : DriverData();
        if (!dd.getLapData().isEmpty())
        {
            for (int j = 0; j < dd.getLapData().size(); ++j)
            {
                if (dd.getLapData()[j].getLapNumber() < firstLap && dd.getLapData()[j].getLapNumber() >= first)
                    firstLap = dd.getLapData()[j].getLapNumber();

                if (dd.getLapData()[j].getLapNumber() > lastLap && dd.getLapData()[j].getLapNumber() <= last)
                    lastLap = dd.getLapData()[j].getLapNumber();

            }
        }
    }
}

void GapCompChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[2] = {(int)paintRect.left(), (int)paintRect.left()};
    double y1[2];
    double y2[2];

    findFirstAndLastLap(firstLap, lastLap);
    int index[2] = {0,0};
    min = 0.0; max = 0.0;
    QList<double> intervals;

    for (int i = firstLap; i <= lastLap; ++i)
    {
        intervals.append(calculateInterval(i));
        double interval = fabs(intervals.last());

        if (interval > max)
        {
            max = interval;
            tMax = max;
        }
    }

    if (max != 1000)
    {
        max += max * 0.05;
        tMax = max;
    }

    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);
    double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));

    p->setRenderHint(QPainter::Antialiasing);
    double j[2] = {(x[0]), (x[0])};
    int lapsInWindow = 0;
    int lastPaintedSC= 0;
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
            DriverData dd = (driverIdx[k] >= 0) ? eventData.getDriversData()[driverIdx[k]] : DriverData();
            LapData ld = dd.getLapData(i);
//            if (!dd.lapData.empty() && index[k] < dd.lapData.size() && dd.getLapData()[index[k]].getLapNumber() == i)
            if (ld.getCarID() == dd.getCarID() && ld.getLapNumber() == i)
            {

                if (ld.getRaceLapExtraData().isSCLap() && ld.getLapNumber() > lastPaintedSC)
                {
                    drawSCLap(p, ld, xFactor);
                    lastPaintedSC = ld.getLapNumber();
                }

                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                double driverY = fabs(intervals[i-firstLap]);
                double gap = driverY;

                int idx = i - firstLap;
                if (driverY == 0 && (i+1-firstLap) < intervals.size())
                {
                    idx = i + 1 - firstLap;
                    driverY = fabs(intervals[idx]);
                }


                if ((k == 0 && intervals[idx] < 0) || (k == 1 && intervals[idx] > 0))
                {
                    gap = 0;
                    driverY = 0;
                }

                if (driverY > max) driverY = max;

                y2[k] = (double)(paintRect.bottom() - (driverY - tMin) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                if (!(y1[k] > paintRect.bottom() && y2[k] > paintRect.bottom()))
                {
                    double dx1 = x[k], dx2 = j[k], dy1 = y1[k], dy2 = y2[k];
                    checkX1(dx1, dy1, dx2, dy2);
                    checkX2(dx1, dy1, dx2, dy2);

                    int pointSize = 3;
                    if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
                    {
                        pointSize = 2;
                        p->drawLine(dx1, dy1, dx2, dy2);
                    }

                    QPainterPath path;
                    p->setBrush(QBrush(colors[k]));

                    if (y2[k] <= paintRect.bottom())
                    {
                        if (ld.getTime().toString() == "IN PIT")
                            path.addEllipse(QPoint(j[k], y2[k]), 6, 6);

                        else                            
                            path.addEllipse(QPoint(j[k], y2[k]), pointSize, pointSize);

                        p->drawPath(path);

                        if (!scaling)
                        {
                            if (lapsInWindow >= lapDataXYGapCompArray.size())
                                lapDataXYGapCompArray.append(LapDataXYGapComp(i, (int)dx2, (int)dy2, k, gap));
                            else
                                lapDataXYGapCompArray[lapsInWindow] = LapDataXYGapComp(i, (int)dx2, (int)dy2, k, gap);
                        }
                        ++lapsInWindow;
                    }
                }


                y1[k] = y2[k];
                x[k] = j[k];

                j[k] += xFactor;

                ++index[k];
            }
            else
            {
                x[k] += xFactor;
                j[k] = x[k];
            }
        }
    }
    clearXYList(lapsInWindow);

}

void GapCompChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        resetZoom();
    }

    QPainter p;
    p.begin(this);

    drawChart(&p);

    if (scaling)
        drawScaleRect(&p);
    else
    {
        if (!repaintPopup)
            findLapDataXY(mousePosX, mousePosY);

        drawLapDataXY(&p);
    }

    p.end();
}

void GapCompChart::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    p->drawRect(40, 5, 115, 45);

    double yy = 0.0;
    for (int i = 0; i < 2; ++i)
    {
        DriverData dd = driverIdx[i] >= 0 ? eventData.getDriversData()[driverIdx[i]] : DriverData();
        if (dd.getCarID() > 0)
        {
            p->setPen(colors[i]);
            p->drawText(45, yy+20, dd.getDriverName());
            yy += 20;
        }
    }
}

int GapCompChart::findLapDataXY(int x, int y)
{
    itemsInXY.clear();
    for (int i = 0; i < lapDataXYGapCompArray.size(); ++i)
    {
        if (abs(lapDataXYGapCompArray[i].x - x) <= 3 && abs(lapDataXYGapCompArray[i].y - y) <= 3)
            itemsInXY.append(i);
    }
    qSort(itemsInXY.begin(), itemsInXY.end(), PredXYGap(*this));
    return itemsInXY.size();
}

void GapCompChart::drawLapDataXY(QPainter *p)
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

    int didx = lapDataXYGapCompArray[itemsInXY[0]].driverIdx;
    int lidx = lapDataXYGapCompArray[itemsInXY[0]].idx;

    DriverData dd = (driverIdx[didx] >= 0) ? eventData.getDriversData()[driverIdx[didx]] : DriverData();
    p->drawText(x+25, y+15, getLapInfoXY(dd.getLapData(lidx)));

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    for (int i = 0; i < itemsInXY.size(); ++i)
    {
        int didx = lapDataXYGapCompArray[itemsInXY[i]].driverIdx;
        int lidx = lapDataXYGapCompArray[itemsInXY[i]].idx;

        DriverData dd = (driverIdx[didx] >= 0) ? eventData.getDriversData()[driverIdx[didx]] : DriverData();
        p->drawText(x+25, y+(i+1)*20+15, getDriverInfoXY(dd.getLapData(lidx), lapDataXYGapCompArray[itemsInXY[i]].gap));
    }
}

void GapCompChart::drawIntoImage(QImage &img)
{
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();
}

void GapCompChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1; last = 99;
}

void GapCompChart::transform()
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

    int firstLap, lastLap;
    findFirstAndLastLap(firstLap, lastLap);
    int sz = lastLap-firstLap+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

//	if (first >= driverData.lapData.size())
//		first = driverData.lapData.size() - 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);

    if (first >= last)
        first = last - 1;
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

//=========================================================

void PosCompChart::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));


    double yFactor = (double)(paintRect.height()/6.0);
    double yFactor2 = (double)((tMax-tMin)/6.0);
    double j = tMin;

    for (int i = paintRect.bottom(); i >= paintRect.top(); i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(round(j)));

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

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
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

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}


void PosCompChart::findFirstAndLastLap(int &firstLap, int &lastLap)
{
    firstLap = 99, lastLap = 0;
    for (int i = 0; i < 2; ++i)
    {
        if (!driverData[i].getLapData().isEmpty())
        {
            for (int j = 0; j < driverData[i].getLapData().size(); ++j)
            {
                if (driverData[i].getLapData()[j].getLapNumber() < firstLap && driverData[i].getLapData()[j].getLapNumber() >= first)
                    firstLap = driverData[i].getLapData()[j].getLapNumber();

                if (driverData[i].getLapData()[j].getLapNumber() > lastLap && driverData[i].getLapData()[j].getLapNumber() <= last)
                    lastLap = driverData[i].getLapData()[j].getLapNumber();

            }
//			msecs = -QTime::fromString(driverData[i].getLapData()[0].getTime(), "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//			secs = (double)(msecs / 1000.0);
//
//			if (secs > max) secs = max;
//			if (secs < min) secs = min;
        }
    }
}

void PosCompChart::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[2] = {paintRect.left(), paintRect.left()};
    double y1[2];
    double y2[2];
    double yFactor = ((double)paintRect.height() / (double)(tMax-tMin));

    findFirstAndLastLap(firstLap, lastLap);
    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);
    int index[2] = {0,0};
    double j[2] = {(x[0]), (x[0])};
    int lastPaintedSC = 0;
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
            LapData ld = driverData[k].getLapData(i);
//            if (!driverData[k].lapData.empty() && index[k] < driverData[k].lapData.size() && driverData[k].getLapData()[index[k]].getLapNumber() == i)
            if (ld.getCarID() == driverData[k].getCarID() && ld.getLapNumber() == i)
            {

                if (ld.getRaceLapExtraData().isSCLap() && ld.getLapNumber() > lastPaintedSC)
                {
                    drawSCLap(p, ld, xFactor);
                    lastPaintedSC = ld.getLapNumber();
                }

                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                int pos = ld.getPosition();//driverData[k].getLapData()[index[k]].pos;

                y2[k] = (double)(paintRect.bottom() - (double)(pos-tMin) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                double dx1 = x[k], dx2 = j[k], dy1 = y1[k], dy2 = y2[k];
                checkX1(dx1, dy1, dx2, dy2);
                checkX2(dx1, dy1, dx2, dy2);

                if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
                    p->drawLine(dx1, dy1, dx2, dy2);

                if (y2[k] <= paintRect.bottom())
                {
                    QPainterPath path;
                    p->setBrush(QBrush(colors[k]));

                    if (ld.getTime().toString() == "IN PIT")
                        path.addEllipse(QPoint(j[k], y2[k]), 6, 6);

                    else if (EventData::getInstance().getEventType() != LTPackets::RACE_EVENT)
                        path.addEllipse(QPoint(j[k], y2[k]), 3, 3);

                    p->drawPath(path);
                }

                y1[k] = y2[k];
                x[k] = j[k];

                j[k] += xFactor;

                ++index[k];
            }
            else
            {
                x[k] += xFactor;
                j[k] = x[k];
            }
        }
    }
}

void PosCompChart::paintEvent(QPaintEvent *)
{
    resetPaintRect();

    if (scaleRect.width() == 0 && scaleRect.height() == 0)
    {
        resetZoom();
    }

    QPainter p;
    p.begin(this);

    drawChart(&p);

    if (scaling)
        drawScaleRect(&p);

    p.end();
}

void PosCompChart::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    p->drawRect(40, 5, 115, 45);

    double yy = 0.0;
    for (int i = 0; i < 2; ++i)
    {
        if (driverData[i].getCarID() > 0)
        {
            p->setPen(colors[i]);
            p->drawText(45, yy+20, driverData[i].getDriverName());
            yy += 20;
        }
    }
}

void PosCompChart::drawIntoImage(QImage &img)
{
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();
}

void PosCompChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1; last = 99;
}

void PosCompChart::transform()
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

    int firstLap, lastLap;
    findFirstAndLastLap(firstLap, lastLap);
    int sz = lastLap-firstLap+1;
    double xFactor = ((double)paintRect.width()) / ((double)sz);
    double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

    first = firstLap + ceil((scaleRect.left() - paintRect.left()) / xFactor);
    if (first < 1)
        first = 1;

    last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);

    if (first >= last)
        first = last - 1;

    tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
    if (tMin < min)
        tMin = min;
    tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}


