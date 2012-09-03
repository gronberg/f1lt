#include "chartwidget.h"
#include "ui_chartwidget.h"
#include <QClipboard>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <cmath>
#include <QDebug>

#include "../core/eventdata.h"
#include "../core/seasondata.h"

ChartWidget::ChartWidget(double n, double x, QColor col, QWidget *parent) :
    QWidget(parent), min(n), max(x), color(col)
{
    menu = new QMenu(this);
    copyAction = menu->addAction("Copy chart");
    saveAction = menu->addAction("Save as image");
    zoomOutAction = menu->addAction("Zoom out");

    connect(copyAction, SIGNAL(triggered()), this, SLOT(onCopy()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(onZoomOut()));

    scaling = false;

    first = driverData.getPositionHistory().size() > driverData.getLapData().size() ? 2 : 1;
    last = driverData.getPositionHistory().size();
	tMin = min;
	tMax = max;

    setMouseTracking(true);
}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton)
    {
        menu->exec(QWidget::mapToGlobal(ev->pos()));
    }
    if (ev->button() == Qt::LeftButton)
    {
    	if (ev->pos().x() >= paintRect.left() && ev->pos().y() <= paintRect.bottom() &&
        		ev->pos().x() <= paintRect.right()&& ev->pos().y() >= paintRect.top())
    	{
			scaling = true;

			scaleRect = QRect();

			scaleRect.setX(ev->pos().x());
			scaleRect.setY(ev->pos().y());
			scaleRect.setWidth(1);
			scaleRect.setHeight(1);
    	}
    }

}

void ChartWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (/*ev->button() == Qt::LeftButton &&*/ scaling)
    {
    	if (ev->pos().x() >= paintRect.left() && ev->pos().y() <= paintRect.bottom() &&
    		ev->pos().x() <= paintRect.right()&& ev->pos().y() >= paintRect.top())
    	{
			scaleRect.setWidth(ev->pos().x() - scaleRect.x());
			scaleRect.setHeight(ev->pos().y() - scaleRect.y());
    	}

        update();
    }
    else
    {
        mousePosX = ev->pos().x();
        mousePosY = ev->pos().y();

        int items = itemsInXY.size();
        if (findLapDataXY(mousePosX, mousePosY))
        {
            repaintPopup = true;
            update();
            repaintPopup = false;
        }

        else if (items != 0)  //if the cursor has moved and a popup was displayed previously, it has to be cleared
            update();
    }
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *)
{
	if (scaling)
	{
		scaling = false;
		if (abs(scaleRect.width()) > 20 && abs(scaleRect.height()) > 20)
		{
			pushScaleRect();
	//		scaleRect = QRect();
            transform();
		}
		repaint();
	}
}

void ChartWidget::mouseDoubleClickEvent(QMouseEvent *)
{
	if (!scaling)
    {
		scaleRect = QRect();
		resetZoom();
		transform();
		scaleRectsStack.clear();
		repaint();
	}
}

void ChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

//    first = driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1;
//	last = driverData.posHistory.size();

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    double yFactor = (((double)paintRect.height())/4.0);
    double yFactor2 = ((double)(tMax-tMin)/4.0);
    double j = tMin;
    for (int i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
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

    if (driverData.getPositionHistory().size()>1)
    {
        int sz = last-first+1;//(driverData.posHistory.size() > driverData.lapData.size() ? driverData.posHistory.size()-1 : driverData.posHistory.size());
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(sz) /*: 5)*/;
        double j = 1.0;
        double i = paintRect.left();
        int prevJ = 1;

        double jFactor = last-first+1/*driverData.posHistory.size()*/ < 5 ? 1.0 : (double)((last-first+1/*driverData.posHistory.size()-1*/)/6.0);

        j = first-1;
        prevJ = first - 1;


        for (; i < width()-15.0 && round(j) < last && round(j) < driverData.getLapData().size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

            if (driverData.getPositionHistory().size() > driverData.getLapData().size())
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));
            else
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.getLapData()[round(j)].getLapNumber()));

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

void ChartWidget::checkX1(double &x1, double &y1, double x2, double y2)
{
	if (y1 > paintRect.bottom())
	{
		double h = y1 - y2;
		double w = x2 - x1;
		x1 = x1 + ((y1 - paintRect.bottom()) * w)/h;
		y1 = paintRect.bottom();
	}
}

void ChartWidget::checkX2(double x1, double y1, double &x2, double &y2)
{
	if (y2 > paintRect.bottom())
	{
		double h = y2 - y1;
		double w = x2 - x1;
		x2 = x2 - ((y2 - paintRect.bottom()) * w)/h;
		y2 = paintRect.bottom();
	}
}

void ChartWidget::drawChart(QPainter *p)
{
    if (driverData.getPositionHistory().size()>1 && first < driverData.getPositionHistory().size())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last-first+1;//(driverData.posHistory.size() > driverData.lapData.size() ? driverData.posHistory.size()-1 : driverData.posHistory.size());
        double xFactor = ((double)paintRect.width()) / ((double)sz);
        double yFactor = (((double)paintRect.height()) / (double)(tMax-tMin));

        double x = paintRect.left(), j = x + xFactor;
        double y = (double)paintRect.bottom() - (double)(driverData.getPositionHistory()[first-1]-tMin) * yFactor;

        int i = first; //(driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1);
        int lastPaintedSC = 0;
        for (; i < last + 1 && i < driverData.getPositionHistory().size(); ++i, j += xFactor)
        {
            double y2 = (double)paintRect.bottom() - (double)(driverData.getPositionHistory()[i]-tMin) * yFactor;
            double x2 = j;
            if (driverData.getPositionHistory()[i] <= 0)
            {
                y2 = y;
            }

            LapData ld = driverData.getLapData(i+1);
//            int idx = driverData.posHistory.size() > driverData.lapData.size() ? i-5 : i-5;
//            if (idx >= 0 && idx < driverData.lapData.size())
            {
                if (ld.getRaceLapExtraData().isSCLap() && ld.getLapNumber() > lastPaintedSC)
                {
                    int tmp = first;
                    first -= 1;
                    drawSCLap(p, ld, xFactor);
                    lastPaintedSC = ld.getLapNumber();
                    first = tmp;
                }
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

            x = j;
            y = y2;

            if (i >= 0 && i < last && i < driverData.getLapData().size() && driverData.getLapData()[i-1].getTime().toString() == "IN PIT" && tmpY2 <= paintRect.bottom())
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

void ChartWidget::drawScaleRect(QPainter *p)
{
	p->setPen(QColor(255, 255, 255, 150));
	p->setBrush(QColor(240, 240, 240, 50));
	p->drawRect(scaleRect);
}

void ChartWidget::drawLegend(QPainter *p)
{
    p->setPen(SeasonData::getInstance().getColor(LTPackets::WHITE));
    p->drawText(40, 20, driverData.getDriverName());
}

int ChartWidget::findLapDataXY(int x, int y)
{
    itemsInXY.clear();
    for (int i = 0; i < lapDataXYArray.size(); ++i)
    {
        if (std::abs((float)(lapDataXYArray[i].x - x)) <= 3 && std::abs((float)(lapDataXYArray[i].y - y)) <= 3)
            itemsInXY.append(lapDataXYArray[i].idx);
    }
    qSort(itemsInXY.begin(), itemsInXY.end());
    return itemsInXY.size();
}

void ChartWidget::drawLapDataXY(QPainter *p)
{
    if (itemsInXY.isEmpty())
        return;

//    p.setPen(QColor(232, 227, 185, 200));
//    p.setBrush(QColor(232, 227, 185, 200));
    p->setPen(QColor(50, 50, 50));
    p->setBrush(QColor(50, 50, 50));

    int x = mousePosX;
    int y = mousePosY;

    int height = 20 * (itemsInXY.size());
    int bottom = y + height;
    int width = getPopupWidth()+20;
    int right = x + width;

    if (bottom > paintRect.bottom())
        y = paintRect.bottom() - height;

    if (right > paintRect.right())
        x = paintRect.right() - width;

    p->drawRect(x+20, y, getPopupWidth(), 20 * (itemsInXY.size()));


    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
    for (int i = 0; i < itemsInXY.size(); ++i)
    {
        if (itemsInXY[i] < driverData.getLapData().size())
            p->drawText(x+25, y+i*20+15, getDriverInfoXY(driverData.getLapData()[itemsInXY[i]]));
    }
}

void ChartWidget::drawSCLap(QPainter *p, const LapData &lapData, double xFactor)
{
    double sc_x1 = (double)(lapData.getLapNumber()-1 - first) * xFactor + (double)paintRect.left();
    double sc_x2 = (double)(lapData.getLapNumber() - first) * xFactor + (double)paintRect.left();

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

void ChartWidget::paintEvent(QPaintEvent *)
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

void ChartWidget::resetZoom()
{
    first = driverData.getPositionHistory().size() > driverData.getLapData().size() ? 2 : 1;
    last = driverData.getPositionHistory().size();
	tMin = min;
	tMax = max;
}

void ChartWidget::transform()
{
	if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
		return;

	if (scaleRect == QRect())
	{
        first = driverData.getPositionHistory().size() > driverData.getLapData().size() ? 2 : 1;
        last = driverData.getPositionHistory().size();
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
	double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

	first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
	if (first < 1)
		first = 1;

    if (first >= driverData.getPositionHistory().size())
        first = driverData.getPositionHistory().size() - 1;

	last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
    if (last >= driverData.getPositionHistory().size())
        last = driverData.getPositionHistory().size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);
}

void ChartWidget::pushScaleRect()
{
	ChartWidget::ScaleAtom sa;
	sa.first = first; sa.last = last;
	sa.min = tMin; sa.max = tMax;
//	sa.rect = scaleRect;

	scaleRectsStack.push(sa);
}

void ChartWidget::drawIntoImage(QImage &img)
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

void ChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);

    drawIntoImage(img);

    qApp->clipboard()->setImage(img);
}

void ChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);

        drawIntoImage(img);

        img.save(fName, "PNG");
    }
}

void ChartWidget::onZoomOut()
{
	if (scaleRectsStack.size() > 1)
    {
		ChartWidget::ScaleAtom sa = scaleRectsStack.pop();
		resetZoom();

		first = sa.first; last = sa.last;
		tMin = sa.min; tMax = sa.max;
//		scaleRect = sa.rect;

//		transform();
		repaint();
	}
	else
	{
		scaleRectsStack.clear();
		scaleRect = QRect();
		resetZoom();
//		transform();
		repaint();
    }
}

//========================================================================

void LapTimeChart::setData(const DriverData &dd)
{
    driverData = dd;

    max = 0.0;
    for (int i = 0; i < dd.getLapData().size(); ++i)
    {
        double secs = (double)(dd.getLapData()[i].getTime().toMsecs() / 1000.0);

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
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));

    for (int i = paintRect.bottom(), j = tMin; i >= 50; i-= (height()-55)/6, j += (tMax-tMin)/6)
    {
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT)));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (!driverData.getLapData().isEmpty() && first < driverData.getLapData().size())
    {
    	int sz = last - first + 1;
        double xFactor = ((double)width()-33.0) / /*((lapData.size() < 5) ?*/ (double)sz /*: 5)*/;
        double j = first-1;
        double i = paintRect.left();
        int prevJ = first-1;

        double jFactor = sz < 5 ? 1.0 : (double)(sz/6.0);
        for (; i < width()-15.0 && round(j) < last && round(j) < driverData.getLapData().size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.getLapData()[round(j)].getLapNumber()));

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

void LapTimeChart::drawChart(QPainter *p)
{
    if (!driverData.getLapData().empty() && first < driverData.getLapData().size())
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

        LapTime sector1 = driverData.getLapData()[first-1].getSectorTime(1);
        LapTime sector2 = driverData.getLapData()[first-1].getSectorTime(2);
        LapTime sector3 = driverData.getLapData()[first-1].getSectorTime(3);
        LapTime lapTime = driverData.getLapData()[first-1].getTime();

        if (sz > 1)
        {
            if (lapTime.toString() == "IN PIT")
            {
                lapTime = driverData.getLapData()[first].getTime();
                LapTime pl = LapTime(driverData.getPitTime(driverData.getLapData()[first-1].getLapNumber()));

                lapTime = lapTime - pl + LapTime(5000);
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData.getLapData()[first].getSectorTime(1);
                LapTime pl = LapTime(driverData.getPitTime(driverData.getLapData()[first-1].getLapNumber()));

                sector1 = sector1 - pl + LapTime(5000);
            }

            if (sector2.toString() == "")
                sector2 = driverData.getLapData()[first].getSectorTime(2);

            if (sector3.toString() == "")
                sector3 = driverData.getLapData()[first].getSectorTime(3);
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

        if (driverData.getLapData()[first-1].getTime().toString() == "IN PIT")
        {
            QPainterPath path;
            path.addEllipse(QPoint(round(x), lapy), 6, 6);
            p->setBrush(QBrush(colors[3]));
            p->setPen(colors[3]);

            if (driverData.getLapData()[0].getRaceLapExtraData().isSCLap())
            {
                pen.setColor(colors[4]);
                p->setBrush(QBrush(colors[4]));
            }

            p->drawPath(path);
        }

        int lapsInWindow = 0;
        int lastPaintedSC = 0;
        for (int i = first; i < last+1 && i < driverData.getLapData().size(); ++i, j += xFactor)
        {
            x2 = j;

            sector1 = driverData.getLapData()[i].getSectorTime(1);
            sector2 = driverData.getLapData()[i].getSectorTime(2);
            sector3 = driverData.getLapData()[i].getSectorTime(3);
            lapTime = driverData.getLapData()[i].getTime();


            if (driverData.getLapData()[i].getRaceLapExtraData().isSCLap() && driverData.getLapData()[i].getLapNumber() > lastPaintedSC)
            {
                drawSCLap(p, driverData.getLapData()[i], xFactor);
                lastPaintedSC = driverData.getLapData()[i].getLapNumber();
            }


            if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
            {
                lapTime = driverData.getLapData()[i-1].getTime();
                if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
                {
//                    if (i - 1 > 0)
//                        lapTime = lapData[i-2].lapTime;
                    if (i < last-1)
                    {
                        lapTime = driverData.getLapData()[i+1].getTime();

                        LapTime pl = driverData.getPitTime(driverData.getLapData()[i].getLapNumber());
                        lapTime = lapTime - pl + LapTime(5000);
                    }
                }
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData.getLapData()[i-1].getSectorTime(1);

                if (sector1.toString() == "" && i < last-1)
                    sector1 = driverData.getLapData()[i+1].getSectorTime(1);
            }

            if (sector2.toString() == "")
            {
                sector2 = driverData.getLapData()[i-1].getSectorTime(2);

                if (sector2.toString() == "" && i < last-1)
                    sector2 = driverData.getLapData()[i+1].getSectorTime(2);
            }

            if (sector3.toString() == "")
            {
                sector3 = driverData.getLapData()[i-1].getSectorTime(3);
                if (sector3.toString() == "")
                {
//                    if (i - 1 > 0)
//                        sector3 = lapData[i-2].sector3;
                    if (i < driverData.getLapData().size()-1)
                        sector3 = driverData.getLapData()[i+1].getSectorTime(3);
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
                    if (lapsInWindow >= lapDataXYArray.size())
                        lapDataXYArray.append(LapDataXY(i, (int)dx2, (int)dy2));
                    else
                        lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)dx2, (int)dy2);
                }
                ++lapsInWindow;

                p->setBrush(QBrush(colors[3]));
                if (driverData.getLapData()[i].getTime().toString() == "IN PIT")
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
            }

            x = x2;
            sec1y = sec1y2;
            sec2y = sec2y2;
            sec3y = sec3y2;
            lapy = lapy2;
        }
        clearXYList(lapsInWindow);
    }
}

void LapTimeChart::drawLegend(QPainter *p)
{
    int x = 35, y = 25;
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));
    p->setPen(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
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
    ChartWidget::paintEvent(pe);

    QPainter p;
    p.begin(this);
    if (!scaling)
    {
        if (!repaintPopup)
            findLapDataXY(mousePosX, mousePosY);

        drawLapDataXY(&p);
    }
    drawLegend(&p);
    p.end();
}

void LapTimeChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1;
    last = driverData.getLapData().size();
}

void LapTimeChart::transform()
{
    ChartWidget::transform();
	if (scaleRect == QRect())
	{
		first = 1;
        last = driverData.getLapData().size();
		tMin = min;
		tMax = max;
		return;
	}	
}

//========================================================================

void GapChart::drawAxes(QPainter *p)
{
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));


    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));

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
        p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != paintRect.height() && i < paintRect.bottom())
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

    if (!driverData.getLapData().isEmpty())
    {
    	int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)sz /*: 5)*/;
        double j = first-1;
        double i = paintRect.left();
        int prevJ = first-1;

        double jFactor = sz < 5 ? 1.0 : (double)(sz/6.0);
        for (; i < width()-15.0 && round(j) < last && round(j) < driverData.getLapData().size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.getLapData()[round(j)].getLapNumber()));

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

void GapChart::drawChart(QPainter *p)
{
    if (!driverData.getLapData().empty() && first < driverData.getLapData().size() && last <= driverData.getLapData().size())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last - first + 1;
        double xFactor = ((double)width()-30.0) / (double)sz;
        double yFactor = (((double)height()-75.0) / (double)(tMax-tMin));

        double gap = driverData.getLapData()[first-1].getGap().toDouble();

        if (driverData.getLapData()[first-1].getGap().size() > 0 && driverData.getLapData()[first-1].getGap()[driverData.getLapData()[first-1].getGap().size()-1] == 'L')
            gap = -1.0;

        double x = paintRect.left(), y = paintRect.bottom() - (gap - tMin) * yFactor, y2;

        if (gap == -1.0 && tMax >= max)
            y = 10;

        else if (gap > tMax && tMax >= max)
            y = 30;

        double j = x + xFactor;
        int lapsInWindow = 0;
        int lastPaintedSC = 0;
        for (int i = first; i < last+1 && i < driverData.getLapData().size(); ++i, j += xFactor)
        {
            bool ok;
            gap = driverData.getLapData()[i].getGap().toDouble(&ok);
            y2 = paintRect.bottom() - (gap - tMin) * yFactor;
            double x2 = j;

            if (gap > tMax && tMax >= max)
                y2 = 30;

            if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
            {
                if (driverData.getLapData()[i].getPosition() == 1 && driverData.getLapData()[i].getGap() == "LAP")
                {
                    gap = 0;
                    y2 = paintRect.bottom() - gap * yFactor;
                }
                else if (driverData.getLapData()[i].getGap().size() > 0 && driverData.getLapData()[i].getGap()[driverData.getLapData()[i].getGap().size()-1] == 'L')
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

            if (driverData.getLapData()[i].getRaceLapExtraData().isSCLap() && driverData.getLapData()[i].getLapNumber() > lastPaintedSC)
            {
                drawSCLap(p, driverData.getLapData()[i], xFactor);
                lastPaintedSC = driverData.getLapData()[i].getLapNumber();
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
                if (driverData.getLapData()[i].getTime().toString() == "IN PIT")
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
                    if (lapsInWindow >= lapDataXYArray.size())
                        lapDataXYArray.append(LapDataXY(i, (int)x2, (int)y2));
                    else
                        lapDataXYArray[lapsInWindow] = LapDataXY(i, (int)x2, (int)y2);
                }
                ++lapsInWindow;
            }


            double tmpY2 = y2;
            checkX1(x, y, x2, tmpY2);
            checkX2(x, y, x2, tmpY2);

            p->drawLine(x, y, x2, tmpY2);
            x = j;
            y = y2;
        }
        clearXYList(lapsInWindow);
    }
}

void GapChart::paintEvent(QPaintEvent *pe)
{
    ChartWidget::paintEvent(pe);
    if (!scaling)
    {
        if (!repaintPopup)
            findLapDataXY(mousePosX, mousePosY);

        QPainter p;
        p.begin(this);
        drawLapDataXY(&p);
        p.end();
    }
}


void GapChart::resetZoom()
{
    ChartWidget::resetZoom();
    first = 1;
    last = driverData.getLapData().size();
}

void GapChart::transform()
{
    ChartWidget::transform();
	if (scaleRect == QRect())
	{
		first = 1;
        last = driverData.getLapData().size();
		tMin = min;
		tMax = max;
		return;
	}
}

//========================================================================

