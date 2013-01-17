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



#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QAction>
#include <QDebug>
#include <QList>
#include <QMenu>
#include <QMouseEvent>
#include <QStack>
#include <QWidget>




class ChartWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChartWidget(double, double, QColor, QWidget *parent = 0);
    ~ChartWidget();

    void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p) = 0;
    virtual void drawChart(QPainter *p) = 0;
    virtual void drawLegend(QPainter *p) = 0;
    virtual void drawScaleRect(QPainter *p);
    virtual void drawIntoImage(QImage &img);

    virtual void transform();
    virtual void calculateTransformFactors() = 0;
    virtual void resetZoom();       

    void pushScaleRect();

    void checkX1(double &x1, double &y1, double x2, double y2);
    void checkX2(double x1, double y1, double &x2, double &y2);

    int getMin() { return min; }
    int getMax() { return max; }
    int getFirst() { return first; }
    int getLast() { return last; }

public slots:
    virtual void onCopy();
    virtual void onSave();
    virtual void onZoomOut();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent (QMouseEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(27, 10, width()-32, height()-35);
    }

    double min, max;
    double tMin, tMax;
    int first, last;
    QColor color;

    QMenu *menu;
    QAction *copyAction;
    QAction *saveAction;
    QAction *zoomOutAction;

    struct ScaleAtom
    {
    	double min, max;
    	int first, last;
    };

    QRect scaleRect;
    QStack<ScaleAtom> scaleRectsStack;
    QRect paintRect;
    bool scaling;
};




#endif // CHARTWIDGET_H
