#ifndef FPLAPTIMESCHART_H
#define FPLAPTIMESCHART_H

#include "sessionlaptimeschart.h"

class FPLapTimesChart : public SessionLapTimesChart
{
    Q_OBJECT

public:
    FPLapTimesChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
        sessionLength = 90;
    }
    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
//    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);
    void setSessionLength(int sl) { sessionLength = sl; last = sl; }

//signals:
//    void zoomChanged(int, int, double, double);

protected:
    virtual void paintEvent(QPaintEvent *);
    int sessionLength;
};

class QualiLapTimesChart : public FPLapTimesChart
{
public:
    QualiLapTimesChart(QWidget *parent) : FPLapTimesChart(parent)
    {
        sessionLength = 20;
        first = 0;
        last = sessionLength;
        tMin = min;
        tMax = max;
        qualiPeriod = 1;
    }

    int getMin() { return tMin == min ? -1 : tMin; }
    int getMax() { return tMax == max ? -1 : tMax; }

//    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
    void setQualiPeriod(int q) { qualiPeriod = q; }
     virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:
    int qualiPeriod;
};


class AllQualiLapTimesChart : public QualiLapTimesChart
{
public:
    AllQualiLapTimesChart(QWidget *parent) : QualiLapTimesChart(parent)
    {
        sessionLength = 0;
        for (int i = 0; i < 3; ++i)
            sessionLength += LTData::getQualiLength(i+1);

        first = 0;
        last = sessionLength;
        tMin = min;
        tMax = max;
        qualiPeriod = 1;
    }

    int getMin() { return tMin == min ? -1 : tMin; }
    int getMax() { return tMax == max ? -1 : tMax; }


    virtual void drawAxes(QPainter *p, int firstMin, int lastMin);
    virtual void drawChart(QPainter *p);
    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:

};
#endif // FPLAPTIMESCHART_H
