#ifndef SESSIONLAPTIMESCHART_H
#define SESSIONLAPTIMESCHART_H

#include <QColor>
#include <QList>

#include "chartwidget.h"
#include "driverdata.h"


extern bool lessThan(LapData ld1, LapData ld2);


class SessionLapTimesChart : public ChartWidget
{

    Q_OBJECT

public:
    SessionLapTimesChart(QWidget *parent) : ChartWidget(0, 180, QColor(), parent)
    {

    }

    virtual void setData(const QList<LapData> ld) { lapDataArray = ld; qSort(lapDataArray.begin(), lapDataArray.end(), lessThan);}
    virtual void setColors(const QList<QColor> cl) { colors = cl; }

    virtual void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
//    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    void checkX1(double &x1, double &y1, double x2, double y2);
    void checkX2(double x1, double y1, double &x2, double &y2);

    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);
    QColor getCarColor(const LapData &ld);

protected:
    virtual void mouseDoubleClickEvent (QMouseEvent *);

signals:
    void zoomChanged(int, int, double, double);

public slots:
//    virtual void onCopy();
//    virtual void onSave();
    virtual void onZoomOut();

protected:
    void paintEvent(QPaintEvent *);

    QList<LapData> lapDataArray;
    QList<QColor> colors;


};

class SessionPositionsChart : public SessionLapTimesChart
{
public:
    SessionPositionsChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
        min = 1;
        max = 24;
    }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
//    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

//protected:
//    virtual void mouseDoubleClickEvent (QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);
};


class SessionGapsChart : public SessionLapTimesChart
{
public:
    SessionGapsChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
        min = 0;
        max = 100;
    }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
//    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

//protected:
//    virtual void mouseDoubleClickEvent (QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SESSIONLAPTIMESCHART_H
