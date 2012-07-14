#include "chartwidget.h"
#include "ui_chartwidget.h"
#include <QClipboard>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <cmath>
#include <QDebug>

#include "eventdata.h"
#include "ltdata.h"

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

    first = driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1;
	last = driverData.posHistory.size();
	tMin = min;
	tMax = max;

	lastOp = 0;
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

    	repaint();
    }
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	if (scaling)
	{
		scaling = false;
		if (abs(scaleRect.width()) > 20 && abs(scaleRect.height()) > 20)
		{
			pushScaleRect();
	//		scaleRect = QRect();
			transform();
			lastOp = 1;
		}
		repaint();
	}
}

void ChartWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if (!scaling)
	{
		lastOp = 0;
		scaleRect = QRect();
		resetZoom();
		transform();
		scaleRectsStack.clear();
		repaint();
	}
}

void ChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

//    first = driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1;
//	last = driverData.posHistory.size();

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (((double)paintRect.height())/4.0);
    double yFactor2 = ((double)(tMax-tMin)/4.0);
    double j = tMin;
    for (int i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
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

    if (driverData.posHistory.size()>1)
    {
        int sz = last-first+1;//(driverData.posHistory.size() > driverData.lapData.size() ? driverData.posHistory.size()-1 : driverData.posHistory.size());
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)(sz) /*: 5)*/;
        double j = 1.0;
        double i = paintRect.left();
        int prevJ = 1;

        double jFactor = last-first+1/*driverData.posHistory.size()*/ < 5 ? 1.0 : (double)((last-first+1/*driverData.posHistory.size()-1*/)/6.0);

        j = first-1;
        prevJ = first - 1;
//        if (driverData.posHistory.size() > driverData.lapData.size())
//        {
////            jFactor = driverData.posHistory.size() < 5 ? 1.0 : (double)((driverData.posHistory.size()-1)/5.0);
//            j = 1.0;
//        }
//        else
//        {
////            jFactor = driverData.posHistory.size() < 5 ? 1.0 : (double)((driverData.posHistory.size())/5.0);
//            j = 0.0;
//            prevJ = 0;
//        }

        for (; i < width()-15.0 && round(j) < last && round(j) < driverData.lapData.size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

            if (driverData.posHistory.size() > driverData.lapData.size())
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));
            else
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.lapData[round(j)].numLap));

            if (i > paintRect.left())
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
        }
    }

//    if (!data.isEmpty())
//    {
//        double xFactor = (width()-32) / 5;
//        double j = 1.0;
//        for (int i = 27; i < width()-15; i += xFactor, j += (double)(data.size()/5.0))
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 27)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }

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
    if (driverData.posHistory.size()>1 && first < driverData.posHistory.size())
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
        double y = (double)paintRect.bottom() - (double)(driverData.posHistory[first-1]-tMin) * yFactor;

        int i = first; //(driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1);
        for (; i < last + 1 && i < driverData.posHistory.size(); ++i, j += xFactor)
        {
            double y2 = (double)paintRect.bottom() - (double)(driverData.posHistory[i]-tMin) * yFactor;
            double x2 = j;
            if (driverData.posHistory[i] <= 0)
            {
                y2 = y;
            }
            if (y2 > paintRect.bottom() && y > paintRect.bottom())
            {
            	x = j;
				y = y2;
				continue;
            }

            if (i >= 0 && i < last && i < driverData.lapData.size() && driverData.lapData[i-1].lapTime.toString() == "IN PIT" && y2 <= paintRect.bottom())
			{
				QPainterPath path;
				path.addEllipse(QPoint(j, y2), 6, 6);
				p->setBrush(QBrush(color));
				p->setPen(color);

				p->drawPath(path);
			}

            checkX1(x, y, x2, y2);
            checkX2(x, y, x2, y2);


            pen.setWidth(2);
			p->setPen(pen);

            p->drawLine(x, y, x2, y2);
            x = j;
            y = (double)paintRect.bottom() - (double)(driverData.posHistory[i]-tMin) * yFactor;
        }
    }
}

void ChartWidget::drawScaleRect(QPainter *p)
{
	p->setPen(QColor(255, 255, 255, 150));
	p->setBrush(QColor(240, 240, 240, 50));
	p->drawRect(scaleRect);
}

void ChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(27, 10, width()-32, height()-35);

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
	first = driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1;
	last = driverData.posHistory.size();
	tMin = min;
	tMax = max;
}

void ChartWidget::transform()
{
	if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
		return;

	if (scaleRect == QRect())
	{
		first = driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1;
		last = driverData.posHistory.size();
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

	if (first >= driverData.posHistory.size())
		first = driverData.posHistory.size() - 1;

	last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
	if (last >= driverData.posHistory.size())
		last = driverData.posHistory.size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

//	scaleRect = paintRect;

	return;
}

void ChartWidget::pushScaleRect()
{
	ChartWidget::ScaleAtom sa;
	sa.first = first; sa.last = last;
	sa.min = tMin; sa.max = tMax;
//	sa.rect = scaleRect;

	scaleRectsStack.push(sa);
}

void ChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);
    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    //draw driver name:
    p.setPen(LTData::colors[LTData::WHITE]);
    p.drawText(40, 20, driverData.driver);

    p.end();

    qApp->clipboard()->setImage(img);
}

void ChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);
        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawAxes(&p);
        drawChart(&p);

        //draw driver name:
        p.setPen(LTData::colors[LTData::WHITE]);
        p.drawText(40, 20, driverData.driver);

        p.end();

        img.save(fName, "PNG");
    }
}

void ChartWidget::onZoomOut()
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
	lastOp = 0;
}

//========================================================================

void LapTimeChartWidget::setData(const DriverData &dd)
{
    driverData = dd;

    max = 0.0;
    for (int i = 0; i < dd.lapData.size(); ++i)
    {
        double secs = (double)(dd.lapData[i].lapTime.toMsecs() / 1000.0);

        if (secs > max)
            max = secs;
    }
    if (max == 0.0 || max > 180.0)
        max = 180.0;

    else
        max = max + 0.1*max;
}

void LapTimeChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

//    std::cout<<"max="<<max<<std::endl;
    for (int i = paintRect.bottom(), j = tMin; i >= 50; i-= (height()-55)/6, j += (tMax-tMin)/6)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (!driverData.lapData.isEmpty() && first < driverData.lapData.size())
    {
    	int sz = last - first + 1;
        double xFactor = ((double)width()-33.0) / /*((lapData.size() < 5) ?*/ (double)sz /*: 5)*/;
        double j = first-1;
        double i = paintRect.left();
        int prevJ = first-1;

        double jFactor = sz < 5 ? 1.0 : (double)(sz/6.0);
        for (; i < width()-15.0 && round(j) < last && round(j) < driverData.lapData.size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.lapData[round(j)].numLap));

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

void LapTimeChartWidget::drawChart(QPainter *p)
{
    if (!driverData.lapData.empty() && first < driverData.lapData.size())
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

        LapTime sector1 = driverData.lapData[first-1].sector1;
        LapTime sector2 = driverData.lapData[first-1].sector2;
        LapTime sector3 = driverData.lapData[first-1].sector3;
        LapTime lapTime = driverData.lapData[first-1].lapTime;

        if (sz > 1)
        {
            if (lapTime.toString() == "IN PIT")
            {
                lapTime = driverData.lapData[first].lapTime;
                LapTime pl = LapTime(driverData.getPitTime(driverData.lapData[first-1].numLap));

                lapTime = lapTime - pl + LapTime(5000);
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData.lapData[first].sector1;
                LapTime pl = LapTime(driverData.getPitTime(driverData.lapData[first-1].numLap));

                sector1 = sector1 - pl + LapTime(5000);
            }

            if (sector2.toString() == "")
                sector2 = driverData.lapData[first].sector2;

            if (sector3.toString() == "")
                sector3 = driverData.lapData[first].sector3;
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

        if (driverData.lapData[first-1].lapTime.toString() == "IN PIT")
        {
            QPainterPath path;
            path.addEllipse(QPoint(round(x), lapy), 6, 6);
            p->setBrush(QBrush(colors[3]));
            p->setPen(colors[3]);

            if (driverData.lapData[0].scLap)
            {
                pen.setColor(colors[4]);
                p->setBrush(QBrush(colors[4]));
            }

            p->drawPath(path);
        }


        for (int i = first; i < last+1 && i < driverData.lapData.size(); ++i, j += xFactor)
        {
            x2 = j;

            sector1 = driverData.lapData[i].sector1;
            sector2 = driverData.lapData[i].sector2;
            sector3 = driverData.lapData[i].sector3;
            lapTime = driverData.lapData[i].lapTime;


            if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
            {
                lapTime = driverData.lapData[i-1].lapTime;
                if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
                {
//                    if (i - 1 > 0)
//                        lapTime = lapData[i-2].lapTime;
                    if (i < last-1)
                    {
                        lapTime = driverData.lapData[i+1].lapTime;

                        LapTime pl = driverData.getPitTime(driverData.lapData[i].numLap);
                        lapTime = lapTime - pl + LapTime(5000);
                    }
                }
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData.lapData[i-1].sector1;

                if (sector1.toString() == "" && i < last-1)
                    sector1 = driverData.lapData[i+1].sector1;
            }

            if (sector2.toString() == "")
            {
                sector2 = driverData.lapData[i-1].sector2;

                if (sector2.toString() == "" && i < last-1)
                    sector2 = driverData.lapData[i+1].sector2;
            }

            if (sector3.toString() == "")
            {
                sector3 = driverData.lapData[i-1].sector3;
                if (sector3.toString() == "")
                {
//                    if (i - 1 > 0)
//                        sector3 = lapData[i-2].sector3;
                    if (i < driverData.lapData.size()-1)
                        sector3 = driverData.lapData[i+1].sector3;
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
				if (driverData.lapData[i].scLap)
					pen.setColor(colors[4]);

				p->setPen(pen);
				checkX1(dx1, dy1, dx2, dy2);
				checkX2(dx1, dy1, dx2, dy2);
				p->drawLine(dx1, dy1, dx2, dy2);

                p->setBrush(QBrush(colors[3]));
				if (driverData.lapData[i].lapTime.toString() == "IN PIT")
				{
					QPainterPath path;
					path.addEllipse(QPoint(round(x2), lapy2), 6, 6);					
//					p->setPen(colors[3]);

					if (driverData.lapData[i].scLap)
					{
						pen.setColor(colors[4]);
						p->setBrush(QBrush(colors[4]));
					}
					p->drawPath(path);
	//                pen.setWidth(4);
	//                p.setPen(pen);
	//                p.drawPoint(x2, lapy2);
	//                pen.setWidth(2);
				}
//                else
//                {
//                    QPainterPath path;
//                    path.addEllipse(QPoint(round(x2), lapy2), 2, 2);
////                    p->setPen(colors[3]);
//                    if (driverData.lapData[i].scLap)
//                        p->setBrush(colors[4]);

//                    p->drawPath(path);
//                }
            }

            x = x2;
            sec1y = sec1y2;
            sec2y = sec2y2;
            sec3y = sec3y2;
            lapy = lapy2;
        }
    }
    //------------------------------------draw chart legend--------------


    //--------------------------------------------------------
}

void LapTimeChartWidget::drawLegend(QPainter *p, int x, int y)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));
    p->setPen(LTData::colors[LTData::DEFAULT]);
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

void LapTimeChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(28, 10, width()-33, height()-35);

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

    drawLegend(&p, 35, 5);

    p.end();
}

void LapTimeChartWidget::resetZoom()
{
	first = 1;
	last = driverData.lapData.size();
	tMin = min;
	tMax = max;
}

void LapTimeChartWidget::transform()
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
	double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

	first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
	if (first < 1)
		first = 1;

	if (first >= driverData.lapData.size())
		first = driverData.lapData.size() - 1;

	last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
	if (last >= driverData.lapData.size())
		last = driverData.lapData.size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

void LapTimeChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);
    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);
    drawLegend(&p, 35, 25);

    //draw driver name:
    p.setPen(LTData::colors[LTData::WHITE]);
    p.drawText(40, 20, driverData.driver);

    p.end();

    qApp->clipboard()->setImage(img);
}

void LapTimeChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);
        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawAxes(&p);
        drawChart(&p);
        drawLegend(&p, 35, 25);

        //draw driver name:
        p.setPen(LTData::colors[LTData::WHITE]);
        p.drawText(40, 20, driverData.driver);

        p.end();

        img.save(fName, "PNG");
    }
}

//========================================================================

void GapChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));


    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

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
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != paintRect.height() && i < paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }
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

    if (!driverData.lapData.isEmpty())
    {
    	int sz = last - first + 1;
        double xFactor = ((double)paintRect.width()) / /*((lapData.size() < 5) ?*/ (double)sz /*: 5)*/;
        double j = first-1;
        double i = paintRect.left();
        int prevJ = first-1;

        double jFactor = sz < 5 ? 1.0 : (double)(sz/6.0);
        for (; i < width()-15.0 && round(j) < last && round(j) < driverData.lapData.size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.lapData[round(j)].numLap));

            if (i > paintRect.left())
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), paintRect.bottom(), round(i), paintRect.top());
            }
        }
    }

//    if (!lapData.isEmpty())
//    {
//        double xFactor = (width()-32) / ((lapData.size() < 5) ? lapData.size() : 5);
//        double j = 0.0;

//        double jFactor = lapData.size() < 5 ? 1.0 : (double)(lapData.size()/5.0);
//        for (int i = 28; i < width()-15 && round(j) < lapData.size(); i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(lapData[round(j)].numLap));

//            if (i > 28)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }

}

void GapChartWidget::drawChart(QPainter *p)
{
    if (!driverData.lapData.empty() && first < driverData.lapData.size() && last <= driverData.lapData.size())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = last - first + 1;
        double xFactor = ((double)width()-30.0) / (double)sz;
        double yFactor = (((double)height()-75.0) / (double)(tMax-tMin));

        double gap = driverData.lapData[first-1].gap.toDouble();

        if (driverData.lapData[first-1].gap.size() > 0 && driverData.lapData[first-1].gap[driverData.lapData[first-1].gap.size()-1] == 'L')
            gap = -1.0;

        double x = paintRect.left(), y = paintRect.bottom() - (gap - tMin) * yFactor, y2;

        if (gap == -1.0)
            y = 10;

        else if (gap > tMax)
            y = 30;

        double j = x + xFactor;
        for (int i = first; i < last+1 && i < driverData.lapData.size(); ++i, j += xFactor)
        {
            bool ok;
            gap = driverData.lapData[i].gap.toDouble(&ok);
            y2 = paintRect.bottom() - (gap - tMin) * yFactor;
            double x2 = j;

            if (gap > tMax)
                y2 = 30;

            if (EventData::getInstance().eventType == LTData::RACE_EVENT)
            {
                if (driverData.lapData[i].pos == 1 && driverData.lapData[i].gap == "LAP")
                {
                    gap = 0;
                    y2 = paintRect.bottom() - gap * yFactor;
                }
                else if (driverData.lapData[i].gap.size() > 0 && driverData.lapData[i].gap[driverData.lapData[i].gap.size()-1] == 'L')
                {
                    y2 = 10;
                    gap = -1.0;
                }
                else if (gap <= 0 || !ok || gap > tMax)
                    y2 = 30;

//                else
//                    y2 = height()-25 - gap * yFactor;
            }
            if (y2 > paintRect.bottom() && y > paintRect.bottom())
			{
				x = j;
				y = y2;
				continue;
			}

            if (driverData.lapData[i].lapTime.toString() == "IN PIT" && y2 < paintRect.bottom())
			{
				QPainterPath path;
				path.addEllipse(QPoint(j, y2), 6, 6);
				p->setBrush(QBrush(color));
				p->setPen(color);

				p->drawPath(path);
			}
			pen.setWidth(2);
			p->setPen(pen);

			checkX1(x, y, x2, y2);
			checkX2(x, y, x2, y2);

            p->drawLine(x, y, x2, y2);
            x = j;
            y = y2;
        }
    }
}

void GapChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(28, 10, width()-33, height()-35);

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


void GapChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    //draw driver name:
    p.setPen(LTData::colors[LTData::WHITE]);
    p.drawText(40, 25, driverData.driver);

    p.end();

    qApp->clipboard()->setImage(img);
}

void GapChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        drawAxes(&p);
        drawChart(&p);

        //draw driver name:
        p.setPen(LTData::colors[LTData::WHITE]);
        p.drawText(40, 25, driverData.driver);

        p.end();

        img.save(fName, "PNG");
    }
}


void GapChartWidget::resetZoom()
{
	first = 1;
	last = driverData.lapData.size();
	tMin = min;
	tMax = max;
}

void GapChartWidget::transform()
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
	double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

	first = first + ceil((scaleRect.left() - paintRect.left()) / xFactor);
	if (first < 1)
		first = 1;

	if (first >= driverData.lapData.size())
		first = driverData.lapData.size() - 1;

	last = first + ceil((scaleRect.right() - scaleRect.left()) / xFactor);
	if (last >= driverData.lapData.size())
		last = driverData.lapData.size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

//========================================================================

void LapCompChartWidget::setData(DriverData *dd)
{
    double lMin = 99999999.0, lMax = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        driverData[i] = dd[i];

        for (int j = 0; j < driverData[i].lapData.size(); ++j)
        {
            double secs = driverData[i].lapData[j].lapTime.toDouble();//-QTime::fromString(driverData[i].lapData[j].lapTime, "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));

            if (secs == 0 && j > 0)
                secs = driverData[i].lapData[j-1].lapTime.toDouble();//-QTime::fromString(driverData[i].lapData[j-1].lapTime, "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));

            if (secs == 0 && j < driverData[i].lapData.size()-1)
            {
                secs = driverData[i].lapData[j+1].lapTime.toDouble();//-QTime::fromString(driverData[i].lapData[j+1].lapTime, "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
                if (driverData[i].lapData[j].lapTime.toString() == "IN PIT")
                {
                    LapTime pl(driverData[i].getPitTime(driverData[i].lapData[j].numLap));
                    secs = (driverData[i].lapData[j+1].lapTime - pl + LapTime(5000)).toDouble();
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

void LapCompChartWidget::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((paintRect.height())/6.0);
    double yFactor2 = (double)((tMax-tMin)/6.0);
    double j = tMin;
    for (int i = paintRect.bottom(); i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j, 0, 'f', 1));

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

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
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

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}

void LapCompChartWidget::findFirstAndLastLap(int &firstLap, int &lastLap)
{
	firstLap = 99, lastLap = 0;
	int msecs;
	double secs;
	for (int i = 0; i < 4; ++i)
	{
		if (!driverData[i].lapData.isEmpty())
		{
			for (int j = 0; j < driverData[i].lapData.size(); ++j)
			{
				if (driverData[i].lapData[j].numLap < firstLap && driverData[i].lapData[j].numLap >= first)
					firstLap = driverData[i].lapData[j].numLap;

				if (driverData[i].lapData[j].numLap > lastLap && driverData[i].lapData[j].numLap <= last)
					lastLap = driverData[i].lapData[j].numLap;

			}
//			msecs = -QTime::fromString(driverData[i].lapData[0].lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//			secs = (double)(msecs / 1000.0);
//
//			if (secs > max) secs = max;
//			if (secs < min) secs = min;
		}
	}
}

void LapCompChartWidget::drawChart(QPainter *p)
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
        if (!driverData[i].lapData.isEmpty())
        {
//            if (driverData[i].lapData[0].numLap < firstLap)
//                firstLap = driverData[i].lapData[0].numLap;
//
//            if (driverData[i].lapData.last().numLap > lastLap)
//                lastLap = driverData[i].lapData.last().numLap;
//
            msecs = -QTime::fromString(driverData[i].lapData[0].lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
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
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 4; ++k)
        {
        	LapData ld = driverData[k].getLapData(i);
//            if (!driverData[k].lapData.empty() && index[k] < driverData[k].lapData.size() && driverData[k].lapData[index[k]].numLap == i)
        	if (ld.carID == driverData[k].carID && ld.numLap == i)
            {
                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                LapTime lapTime = ld.lapTime;//driverData[k].lapData[index[k]].lapTime;

                if (lapTime.toString() == "IN PIT")
                {
                	LapData ldtmp = driverData[k].getLapData(i-1);
                	if (ldtmp.carID == driverData[k].carID)
                		lapTime = ldtmp.lapTime;

                	else
                	{
                		ldtmp = driverData[k].getLapData(i+1);
                		if (ldtmp.carID == driverData[k].carID)
                		{
                			LapTime pl(driverData[k].getPitTime(ld.numLap));
                			lapTime = lapTime - pl + LapTime(5000);
                		}

                		if (lapTime.toString() == "IN PIT")
                		{
                			ldtmp = driverData[k].getLapData(i+2);
							if (ldtmp.carID == driverData[k].carID)
							{
								LapTime pl(driverData[k].getPitTime(ld.numLap+1));
								lapTime = lapTime - pl + LapTime(5000);
							}
                		}
                	}

//                    else if (index[k] < driverData[k].lapData.size()-1)
//                    {
//                    	lapTime = driverData[k].getLapData(i+1).lapTime;
////                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
//                        LapTime pl(driverData[k].getPitTime(ld.numLap));
//
//                        lapTime = lapTime - pl + LapTime(5000);
//                    }
//
//                    if  (lapTime.toString() == "IN PIT" && index[k] < driverData[k].lapData.size()-1)
//                    {
//                    	lapTime = driverData[k].getLapData(i+1).lapTime;
////                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
//                        LapTime pl(driverData[k].getPitTime(ld.numLap));
//
//                        lapTime = lapTime - pl + LapTime(5000);
//                    }
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

					p->drawLine(dx1, dy1, dx2, dy2);

					if (ld.lapTime.toString() == "IN PIT" && y2[k] <= paintRect.bottom())
					{
						QPainterPath path;
						path.addEllipse(QPoint(j[k], y2[k]), 6, 6);
						p->setBrush(QBrush(colors[k]));
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

}

void LapCompChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(37, 10, width()-42, height()-35);

	if (scaleRect.width() == 0 && scaleRect.height() == 0)
	{
		resetZoom();
	}

    QPainter p;
    p.begin(this);   

    drawChart(&p);
    if (scaling)
		drawScaleRect(&p);
    //------------------------------------draw chart legend--------------
//    p.setRenderHint(QPainter::Antialiasing, false);
//    p.setBrush(QColor(20, 20, 20));

//    int yy = 0;
//    for (int i = 0; i < 4; ++i)
//    {
//        if (driverData[i].carID > 0)
//        {
//            p.setPen(QColor(20, 20, 20));
//            p.drawRect(40, yy, 115, 20);

//            p.setPen(colors[i]);
//            p.drawText(45, yy+20, driverData[i].driver);
//            yy += 20;
//        }
//    }
//    if (yy > 5)
//    {
//        p.setBrush(QBrush());
//        p.setPen(LTData::colors[LTData::DEFAULT]);
//        p.drawRect(40, 5, 115, yy);
//    }
    //--------------------------------------------------------

    p.end();
}

void LapCompChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    int yy = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (driverData[i].carID > 0)
        {
            p->setPen(QColor(20, 20, 20));
            p->drawRect(40, yy, 115, 20);

            p->setPen(colors[i]);
            p->drawText(45, yy+20, driverData[i].driver);
            yy += 20;
        }
    }
    if (yy > 5)
    {
        p->setBrush(QBrush());
        p->setPen(LTData::colors[LTData::DEFAULT]);
        p->drawRect(40, 5, 115, yy);
    }
}

void LapCompChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();

    qApp->clipboard()->setImage(img);
}

void LapCompChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawChart(&p);

        drawLegend(&p);

        p.end();

        img.save(fName, "PNG");
    }
}

void LapCompChartWidget::resetZoom()
{
	first = 1; last = 99;
	tMin = min;
	tMax = max;
}

void LapCompChartWidget::transform()
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
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

//========================================================================

double GapCompChartWidget::calculateInterval(int lap)
{
    LapData ld1 = (driverIdx[0] >= 0) ? eventData.driversData[driverIdx[0]].getLapData(lap) : LapData();
    LapData ld2 = (driverIdx[1] >= 0) ? eventData.driversData[driverIdx[1]].getLapData(lap) : LapData();
    QString gap1 = ld1.gap;
    QString gap2 = ld2.gap;

    if ((ld1.lapTime.toString() == "" && ld1.gap == "") || (ld2.lapTime.toString() == "" && ld2.gap == ""))
        return 0.0;

    if ((gap1 != "" && gap2 != "" && gap1[gap1.size()-1] != 'L' && gap2[gap2.size()-1] != 'L') ||
        ((ld1.pos == 1 && gap1.isNull()) || (ld2.pos == 1 && gap2.isNull())))
    {
        double interval = gap1.toDouble() - gap2.toDouble();
        return interval;
    }
    else if ((gap1 != "" && gap1[gap1.size()-1] == 'L') || (gap2 != "" && gap2[gap2.size()-1] == 'L'))
    {
        int pos1 = ld1.pos;
        int pos2 = ld2.pos;

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
        for (int i = 0; i < eventData.driversData.size(); ++i)
        {
            LapData ld = eventData.driversData[i].getLapData(lap);
            int pos = ld.pos;
            if (pos > pos1 && pos <= pos2)
            {
                if (ld.interval != "" && ld.interval[ld.interval.size()-1] == 'L')
                    return neg ? -1000.0 : 1000.0;

                intervals.append(ld.interval);
            }
        }
        double interval = 0.0;
        for (int i = 0; i < intervals.size(); ++i)
            interval += intervals[i].toDouble();

        if (neg && ld1.lapTime.isValid() && interval > ld1.lapTime.toDouble())
            return -1000.0;
        if (!neg && ld2.lapTime.isValid() && interval > ld2.lapTime.toDouble())
            return 1000.0;


        return neg ? -interval : interval;
    }

    return 0.0;
}


void GapCompChartWidget::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

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
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j, 0, 'f', 1));

        if (i != paintRect.bottom())
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(paintRect.left(), i, paintRect.right(), i);
        }
    }

    if (max == 60)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, 15, ">60");
        QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
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

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}

void GapCompChartWidget::findFirstAndLastLap(int &firstLap, int &lastLap)
{
	firstLap = 99, lastLap = 0;
	for (int i = 0; i < 4; ++i)
	{
		DriverData dd = driverIdx[i] >= 0 ? eventData.driversData[driverIdx[i]] : DriverData();
		if (!dd.lapData.isEmpty())
		{
			for (int j = 0; j < dd.lapData.size(); ++j)
			{
				if (dd.lapData[j].numLap < firstLap && dd.lapData[j].numLap >= first)
					firstLap = dd.lapData[j].numLap;

				if (dd.lapData[j].numLap > lastLap && dd.lapData[j].numLap <= last)
					lastLap = dd.lapData[j].numLap;

			}
		}
	}
}

void GapCompChartWidget::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[2] = {paintRect.left(), paintRect.left()};
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
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
            DriverData dd = (driverIdx[k] >= 0) ? eventData.driversData[driverIdx[k]] : DriverData();
            LapData ld = dd.getLapData(i);
//            if (!dd.lapData.empty() && index[k] < dd.lapData.size() && dd.lapData[index[k]].numLap == i)
            if (ld.carID == dd.carID && ld.numLap == i)
            {
                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                double driverY = fabs(intervals[i-firstLap]);

                int idx = i - firstLap;
                if (driverY == 0 && (i+1-firstLap) < intervals.size())
                {
                    idx = i + 1 - firstLap;
                    driverY = fabs(intervals[idx]);
                }


                if ((k == 0 && intervals[idx] < 0) || (k == 1 && intervals[idx] > 0))
                    driverY = 0;


//                if (dd.lapTime.toString() == "IN PIT")
//                {
//                    if (index[k] > 0)
//                        lapTime = driverData[k].lapData[index[k]-1].lapTime;

//                    else if (index[k] < driverData[k].lapData.size()-1)
//                    {
//                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
//                        LapTime pl(driverData[k].getPitTime(driverData[k].lapData[index[k]].numLap));

//                        lapTime = lapTime - pl + LapTime(5000);
//                    }

//                    if  (lapTime.toString() == "IN PIT" && index[k] < driverData[k].lapData.size()-1)
//                    {
//                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
//                        LapTime pl(driverData[k].getPitTime(driverData[k].lapData[index[k]].numLap));

//                        lapTime = lapTime - pl + LapTime(5000);
//                    }
//                }
//                msecs = -QTime::fromString(lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//                secs = (double)(msecs / 1000.0);

                if (driverY > max) driverY = max;

                y2[k] = (double)(paintRect.bottom() - (driverY - tMin) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                double dx1 = x[k], dx2 = j[k], dy1 = y1[k], dy2 = y2[k];
                checkX1(dx1, dy1, dx2, dy2);
                checkX2(dx1, dy1, dx2, dy2);

                p->drawLine(dx1, dy1, dx2, dy2);

                if (ld.lapTime.toString() == "IN PIT" && y2[k] <= paintRect.bottom())
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(j[k], y2[k]), 6, 6);
                    p->setBrush(QBrush(colors[k]));
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

void GapCompChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(37, 10, width()-42, height()-35);

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

void GapCompChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(LTData::colors[LTData::DEFAULT]);
    p->drawRect(40, 5, 115, 45);

    double yy = 0.0;
    for (int i = 0; i < 2; ++i)
    {
        DriverData dd = driverIdx[i] >= 0 ? eventData.driversData[driverIdx[i]] : DriverData();
        if (dd.carID > 0)
        {
            p->setPen(colors[i]);
            p->drawText(45, yy+20, dd.driver);
            yy += 20;
        }
    }
}

void GapCompChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();

    qApp->clipboard()->setImage(img);
}

void GapCompChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawChart(&p);

        drawLegend(&p);

        p.end();

        img.save(fName, "PNG");
    }
}

void GapCompChartWidget::resetZoom()
{
	first = 1; last = 99;
	tMin = min;
	tMax = max;
}

void GapCompChartWidget::transform()
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
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

//=========================================================

void PosCompChartWidget::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));


    double yFactor = (double)(paintRect.height()/6.0);
    double yFactor2 = (double)((tMax-tMin)/6.0);
    double j = tMin;

    for (int i = paintRect.bottom(); i >= paintRect.top(); i-= yFactor, j += yFactor2)
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

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
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

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}


void PosCompChartWidget::findFirstAndLastLap(int &firstLap, int &lastLap)
{
	firstLap = 99, lastLap = 0;
	int msecs;
	double secs;
	for (int i = 0; i < 2; ++i)
	{
		if (!driverData[i].lapData.isEmpty())
		{
			for (int j = 0; j < driverData[i].lapData.size(); ++j)
			{
				if (driverData[i].lapData[j].numLap < firstLap && driverData[i].lapData[j].numLap >= first)
					firstLap = driverData[i].lapData[j].numLap;

				if (driverData[i].lapData[j].numLap > lastLap && driverData[i].lapData[j].numLap <= last)
					lastLap = driverData[i].lapData[j].numLap;

			}
//			msecs = -QTime::fromString(driverData[i].lapData[0].lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//			secs = (double)(msecs / 1000.0);
//
//			if (secs > max) secs = max;
//			if (secs < min) secs = min;
		}
	}
}

void PosCompChartWidget::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[2] = {paintRect.left(), paintRect.left()};
    double y1[2];
    double y2[2];
    double yFactor = ((double)paintRect.height() / (double)(tMax-tMin));

//    for (int i = 0; i < 2; ++i)
//    {
//        if (!driverData[i].lapData.isEmpty())
//        {
//            if (driverData[i].lapData[0].numLap < firstLap)
//                firstLap = driverData[i].lapData[0].numLap;
//
//            if (driverData[i].lapData.last().numLap > lastLap)
//                lastLap = driverData[i].lapData.last().numLap;
//        }
//    }
    findFirstAndLastLap(firstLap, lastLap);
    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = ((double)paintRect.width()) / (double)(lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);
    int index[2] = {0,0};
    double j[2] = {(x[0]), (x[0])};
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
        	LapData ld = driverData[k].getLapData(i);
//            if (!driverData[k].lapData.empty() && index[k] < driverData[k].lapData.size() && driverData[k].lapData[index[k]].numLap == i)
        	if (ld.carID == driverData[k].carID && ld.numLap == i)
            {
                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                int pos = ld.pos;//driverData[k].lapData[index[k]].pos;

                y2[k] = (double)(paintRect.bottom() - (double)(pos-tMin) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                double dx1 = x[k], dx2 = j[k], dy1 = y1[k], dy2 = y2[k];
				checkX1(dx1, dy1, dx2, dy2);
				checkX2(dx1, dy1, dx2, dy2);

				p->drawLine(dx1, dy1, dx2, dy2);

				if (ld.lapTime.toString() == "IN PIT" && y2[k] <= paintRect.bottom())
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(j[k], y2[k]), 6, 6);
                    p->setBrush(QBrush(colors[k]));
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

void PosCompChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(37, 10, width()-42, height()-35);

	if (scaleRect.width() == 0 && scaleRect.height() == 0)
	{
		resetZoom();
	}

    QPainter p;
    p.begin(this);

    drawChart(&p);

    if (scaling)
		drawScaleRect(&p);
    //------------------------------------draw chart legend--------------
//    p.setRenderHint(QPainter::Antialiasing, false);
//    p.setBrush(QColor(20, 20, 20));

//    int yy = 0;
//    for (int i = 0; i < 4; ++i)
//    {
//        if (driverData[i].carID > 0)
//        {
//            p.setPen(QColor(20, 20, 20));
//            p.drawRect(40, yy, 115, 20);

//            p.setPen(colors[i]);
//            p.drawText(45, yy+20, driverData[i].driver);
//            yy += 20;
//        }
//    }
//    if (yy > 5)
//    {
//        p.setBrush(QBrush());
//        p.setPen(LTData::colors[LTData::DEFAULT]);
//        p.drawRect(40, 5, 115, yy);
//    }
    //--------------------------------------------------------

    p.end();
}

void PosCompChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(LTData::colors[LTData::DEFAULT]);
    p->drawRect(40, 5, 115, 45);

    double yy = 0.0;
    for (int i = 0; i < 2; ++i)
    {
        if (driverData[i].carID > 0)
        {
            p->setPen(colors[i]);
            p->drawText(45, yy+20, driverData[i].driver);
            yy += 20;
        }
    }
}

void PosCompChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();

    qApp->clipboard()->setImage(img);
}

void PosCompChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawChart(&p);

        drawLegend(&p);

        p.end();

        img.save(fName, "PNG");
    }
}

void PosCompChartWidget::resetZoom()
{
	first = 1; last = 99;
	tMin = min;
	tMax = max;
}

void PosCompChartWidget::transform()
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
//	if (last >= driverData.lapData.size())
//		last = driverData.lapData.size() - 1;

	tMin = tMin + ceil((paintRect.bottom() - scaleRect.bottom()) / yFactor)-1;
	if (tMin < min)
		tMin = min;
	tMax = tMin + ceil((scaleRect.bottom() - scaleRect.top()) / yFactor);

}

void WeatherChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.right(), paintRect.bottom());

    //y axe
    p->drawLine(paintRect.left(), paintRect.bottom(), paintRect.left(), paintRect.top());

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((height()-75.0)/4.0);
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
        double jWDFactor = (double)((ed.weatherData[wetDryId].size()-1)/6.0);

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


        for (; i < width()-15.0 && round(j) < last + 1 && round(j) < ed.weatherData[weatherId].size(); /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

//			p->drawText(round(i)-5, height()-10, QString("%1").arg(ed.weatherData[weatherId][round(j)]));

            if (i > 40)
            {
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
					if (ed.weatherData[wetDryId][k] == 1)
						++cnt;
					else
						--cnt;
				}
				p->drawPixmap(round(i)+(nextI-round(i)-dryPix.width())/2, 5, cnt >= 0 ? wetPix : dryPix);
			}
        }
    }

}

void WeatherChartWidget::drawChart(QPainter *p)
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
        double yFactor = (((double)height()-75.0) / (tMax - tMin));

        double x = paintRect.left(), j = x + xFactor;
        double y = (double)paintRect.height() - (double)(ed.weatherData[weatherId][first-1]-tMin) * yFactor;

        int i = first;
        for (; i < last + 1 && i < ed.weatherData[weatherId].size(); ++i, j += xFactor)
        {
            double y2 = (double)(height())-25.0 - (double)(ed.weatherData[weatherId][i]-tMin) * yFactor;
            if (ed.weatherData[weatherId][i] <= 0)
            {
                y2 = y;
            }

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

void WeatherChartWidget::paintEvent(QPaintEvent *)
{
	paintRect = QRect(40, 10, width()-45, height()-35);

//	if (scaleRect.width() == 0 && scaleRect.height() == 0)
//	{
//		resetZoom();
//	}
    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    setMinMax();
    resetZoom();
    drawAxes(&p);
    drawChart(&p);

//    if (scaling)
//		drawScaleRect(&p);

    p.end();
}

void WeatherChartWidget::setMinMax()
{
	EventData &ed = EventData::getInstance();
	for (int i = 0; i < ed.weatherData[weatherId].size(); ++i)
	{
		if (i == 0)
		{
			min = ed.weatherData[weatherId][i];
			max = ed.weatherData[weatherId][i];
		}
		else
		{
			if (ed.weatherData[weatherId][i] < min)
				min = ed.weatherData[weatherId][i];

			if (ed.weatherData[weatherId][i] > max)
				max = ed.weatherData[weatherId][i];
		}
	}
//	min -= min * 0.01;
//	max += max * 0.01;

	if (min < 0.0)
		min = 0.0;
}

void WeatherChartWidget::resetZoom()
{
	setMinMax();
	first = 1;
	last = EventData::getInstance().weatherData[weatherId].size();
	tMin = min;
	tMax = max;
}

void WeatherChartWidget::transform()
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
	double yFactor = (((double)paintRect.height()) / (double)(tMax - tMin));

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


void TempChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(35, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(35, height()-25, 35, 10);

    p->setFont(QFont("Arial", 10, QFont::Bold, false));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)(((double)height()-75.0)/4.0);
    double yFactor2 = (double)((max-min)/4.0);
    double j = min;
    for (int i = height()-25; i >= 50; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString::number(j, 'f', 1));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(35, i, width()-5, i);
        }
    }

    EventData &ed = EventData::getInstance();
	if (ed.weatherData[weatherId].size()>1 || ed.weatherData[trackTempId].size()>1)
    {
		int end = std::max(ed.weatherData[weatherId].size(), ed.weatherData[trackTempId].size());
        double xFactor = ((double)width()-40.0) / (double)end;
        double j = 1.0, jWD = 0.0;
        double i = 35.0;
        int prevJ = 1;

        double jFactor = end < 5 ? 1.0 : (double)((end-1)/6.0);

        double jWDFactor = (double)((ed.weatherData[wetDryId].size()-1)/6.0);

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


        for (; i < width()-15.0 && round(j) < end; /*i += xFactor,*/ j += jFactor, jWD += jWDFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

//			p->drawText(round(i)-5, height()-10, QString("%1").arg(ed.weatherData[weatherId][round(j)]));

            if (i > 35)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
            if (round(jWD) < ed.weatherData[wetDryId].size() && round(j) < end-1)
			{
            	int nextI = i + xFactor*jFactor;
            	int cnt = 0;
				for (int k = round(jWD); k < round(jWD+jWDFactor) && k < ed.weatherData[wetDryId].size(); ++k)
				{
					if (ed.weatherData[wetDryId][k] == 1)
						++cnt;
					else
						--cnt;
				}
				p->drawPixmap(round(i)+(nextI-round(i)-dryPix.width())/2, 5, cnt >= 0 ? wetPix : dryPix);
			}
        }
    }

}

void TempChartWidget::drawChart(QPainter *p)
{
	EventData &ed = EventData::getInstance();
    if (ed.weatherData[weatherId].size()>1)
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        double xFactor1 = ((double)width()-40.0) / (ed.weatherData[weatherId].size());
        double xFactor2 = ((double)width()-40.0) / (ed.weatherData[trackTempId].size());
        double yFactor = ((double)height()-75.0) / (double)(max-min);

        double x = 35, j1 = x + xFactor1, j2 = x + xFactor2;
        double y1 = (double)(height())-25.0 - (double)(ed.weatherData[weatherId][0]-min) * yFactor;
        double y2 = (double)(height())-25.0 - (double)(ed.weatherData[trackTempId][0]-min) * yFactor;

        int i = 1;
        pen.setColor(color);
        p->setPen(pen);
        for (; i < ed.weatherData[weatherId].size(); ++i, j1 += xFactor1)
        {
            double y3 = (double)(height())-25.0 - (double)(ed.weatherData[weatherId][i]-min) * yFactor;

            double midx = (j1 + x)/2;

			p->drawLine(x, y1, midx, y1);
			p->drawLine(midx, y1, midx, y3);
			p->drawLine(midx, y3, j1, y3);

            x = j1;
            y1 = y3;
        }
        i = 1;
        x = 35;
        pen.setColor(trackTempCol);
        p->setPen(pen);
        for (; i < ed.weatherData[trackTempId].size(); ++i, j2 += xFactor2)
	    {
        	double y4 = (double)(height())-25.0 - (double)(ed.weatherData[trackTempId][i]-min) * yFactor;
        	double midx = (j2 + x)/2;

        	p->drawLine(x, y2, midx, y2);
			p->drawLine(midx, y2, midx, y4);
			p->drawLine(midx, y4, j2, y4);

			x = j2;
			y2 = y4;
	    }
    }
}

void TempChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(LTData::colors[LTData::DEFAULT]);
    p->drawRect(width()-85, height()-80, 80, 50);

    p->setPen(color);
    p->drawText(width()-80, height()-60, "Air temp");

    p->setPen(trackTempCol);
	p->drawText(width()-80, height()-40, "Track temp");
}

void TempChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    setMinMax();
    drawAxes(&p);
    drawChart(&p);
    drawLegend(&p);

    p.end();
}

void TempChartWidget::setMinMax()
{
	EventData &ed = EventData::getInstance();
	int end = std::max(ed.weatherData[weatherId].size(), ed.weatherData[trackTempId].size());
	for (int i = 0; i < end; ++i)
	{
		if (i == 0)
		{
			max = std::max(ed.weatherData[weatherId][i], ed.weatherData[trackTempId][i]);
		}
		else
		{
			if (i < ed.weatherData[weatherId].size() && ed.weatherData[weatherId][i] > max)
				max = ed.weatherData[weatherId][i];

			if (i < ed.weatherData[trackTempId].size() && ed.weatherData[trackTempId][i] > max)
				max = ed.weatherData[trackTempId][i];
		}
	}
	min = 0.0;
}


void WetDryChartWidget::drawAxes(QPainter *p)
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

//    for (int i = height()-25; i >= 50; i-= yFactor, j += yFactor2)
//    {
//        p->setPen(QColor(LTData::colors[LTData::WHITE]));
//
//        if (j == min)
//        	p->drawText(5, i+5, "Dry");
//        else
//        	p->drawText(5, i+5, "Wet");
//
//        if (i != height()-25)
//        {
//            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//            pen.setStyle(Qt::DashLine);
//            p->setPen(pen);
//            p->drawLine(40, i, width()-5, i);
//        }
//    }

    EventData &ed = EventData::getInstance();
	if (ed.weatherData[weatherId].size()>1)
    {
        double xFactor = ((double)width()-45.0) / (double)ed.weatherData[weatherId].size();
        double j = 1.0, jWD = 0.0;
        double i = 40.0;
        int prevJ = 1;

        double jFactor = ed.weatherData[weatherId].size() < 5 ? 1.0 : (double)((ed.weatherData[weatherId].size()-1)/6.0);
        double jWDFactor = (double)((ed.weatherData[wetDryId].size()-1)/6.0);

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
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

//			p->drawText(round(i)-5, height()-10, QString("%1").arg(ed.weatherData[weatherId][round(j)]));

            if (i > 40)
            {
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
					if (ed.weatherData[wetDryId][k] == 1)
						++cnt;
					else
						--cnt;
				}
				p->drawPixmap(round(i)+(nextI-round(i)-dryPix.width())/2, 5, cnt >= 0 ? wetPix : dryPix);
			}
        }
    }

}

void WetDryChartWidget::drawChart(QPainter *p)
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

        if (ed.weatherData[weatherId][0] == 0)
			y = yFactor + yFactor*0.5;
		else
			y = yFactor - yFactor*0.5;

        int i = 1;
        for (; i < ed.weatherData[weatherId].size(); ++i, j += xFactor)
        {
            double y2 = (double)(height())-25.0 - (double)(ed.weatherData[weatherId][i]-min) * yFactor;
            if (ed.weatherData[weatherId][i] == 0)
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

