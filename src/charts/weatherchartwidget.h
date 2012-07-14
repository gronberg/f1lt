#ifndef WEATHERCHARTWIDGET_H
#define WEATHERCHARTWIDGET_H

#include "chartwidget.h"

class WeatherChartWidget : public ChartWidget
{
    Q_OBJECT

public:
    WeatherChartWidget(QColor col, int id, int id2, QWidget *parent = 0) : ChartWidget(0, 180, col, parent)
    {
        weatherId = id;
        wetDryId = id2;
        menu->removeAction(zoomOutAction);
    }

    virtual void drawAxes(QPainter *p);
    virtual void drawChart(QPainter *p);
    virtual void setMinMax();

    virtual void mouseMoveEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseDoubleClickEvent (QMouseEvent *) {}

    void transform();
    void resetZoom();

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(40, 10, width()-45, height()-35);
    }

    int weatherId;
    int wetDryId;
};

class TempChartWidget : public WeatherChartWidget
{
    Q_OBJECT

public:
    TempChartWidget(QColor col, QColor col2, int id, int tempId, int id2, QWidget *parent = 0) : WeatherChartWidget(col, id, id2, parent)
    {
        trackTempId = tempId;
        trackTempCol = col2;
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void setMinMax();


protected:
    void paintEvent(QPaintEvent *);

private:
    int trackTempId;
    QColor trackTempCol;
};


class WetDryChartWidget : public WeatherChartWidget
{
    Q_OBJECT

public:
    WetDryChartWidget(QColor col, int id, int id2, QWidget *parent = 0) : WeatherChartWidget(col, id, id2, parent)
    {
        min = 0.0;
        max = 1.0;
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void setMinMax() {}

};

#endif // WEATHERCHARTWIDGET_H
