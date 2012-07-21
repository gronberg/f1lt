#ifndef WEATHERCHART_H
#define WEATHERCHART_H

#include "chartwidget.h"

class WeatherChart : public ChartWidget
{
    Q_OBJECT

public:
    WeatherChart(QColor col, int id, int id2, QWidget *parent = 0) : ChartWidget(0, 180, col, parent)
    {
        weatherId = id;
        wetDryId = id2;
        menu->removeAction(zoomOutAction);
        allowedMin = 0;
    }

    virtual void drawAxes(QPainter *p);
    virtual void drawChart(QPainter *p);
    virtual void setMinMax();
    virtual void setAllowedMin(int am) { allowedMin = am; }

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
    int allowedMin;     //min could not be below this
};

class TempChart : public WeatherChart
{
    Q_OBJECT

public:
    TempChart(QColor col, QColor col2, int id, int tempId, int id2, QWidget *parent = 0) : WeatherChart(col, id, id2, parent)
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


class WetDryChart : public WeatherChart
{
    Q_OBJECT

public:
    WetDryChart(QColor col, int id, int id2, QWidget *parent = 0) : WeatherChart(col, id, id2, parent)
    {
        min = 0.0;
        max = 1.0;
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void setMinMax() {}

};

#endif // WEATHERCHART_H
