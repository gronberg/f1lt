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

    first = 0;
    last = 0;
	tMin = min;
	tMax = max;

    setMouseTracking(true);
}

ChartWidget::~ChartWidget()
{
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
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *)
{
	if (scaling)
	{
		scaling = false;
		if (abs(scaleRect.width()) > 20 && abs(scaleRect.height()) > 20)
		{
            pushScaleRect();
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

void ChartWidget::drawScaleRect(QPainter *p)
{
	p->setPen(QColor(255, 255, 255, 150));
	p->setBrush(QColor(240, 240, 240, 50));
	p->drawRect(scaleRect);
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
	tMin = min;
	tMax = max;
}

void ChartWidget::transform()
{
	if (scaling || scaleRect == paintRect || (abs(scaleRect.width()) < 20 || abs(scaleRect.height()) < 20))
		return;

	if (scaleRect == QRect())
	{
        resetZoom();
		return;
	}

	if (scaleRect.left() > scaleRect.right())
		scaleRect = QRect(scaleRect.right(), scaleRect.top(), -scaleRect.width(), scaleRect.height());

	if (scaleRect.top() > scaleRect.bottom())
		scaleRect = QRect(scaleRect.left(), scaleRect.bottom(), scaleRect.width(), -scaleRect.height());

    calculateTransformFactors();
}

void ChartWidget::pushScaleRect()
{
	ChartWidget::ScaleAtom sa;
	sa.first = first; sa.last = last;
    sa.min = tMin; sa.max = tMax;

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

		repaint();
	}
	else
	{
		scaleRectsStack.clear();
		scaleRect = QRect();
        resetZoom();
		repaint();
    }
}

