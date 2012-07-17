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

    virtual QString getLapInfoXY(const LapData &ld)
    {
        int minute = LTData::getFPLength() - LTData::timeToMins(ld.sessionTime);
        return QString::number(minute) + ". minute";
    }

//signals:
//    void zoomChanged(int, int, double, double);

protected:
    virtual void paintEvent(QPaintEvent *);
    int sessionLength;

    void resetPaintRect()
    {
        paintRect = QRect(42, 10, width()-47, height()-35);
    }
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

    virtual QString getLapInfoXY(const LapData &ld)
    {
        int minute = sessionLength - LTData::timeToMins(ld.sessionTime);
        return QString::number(minute) + ". minute";
    }

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

    virtual QString getLapInfoXY(const LapData &ld)
    {
        int sessTime = LTData::getQualiLength(ld.qualiPeriod) - LTData::timeToMins(ld.sessionTime);
//        int sessTimeSecs = LTData::getQualiLength(lapDataArray[i].qualiPeriod) * 60 - LTData::timeToSecs(lapDataArray[i].sessionTime);

//        for (int k = 0; k < lapDataArray[i].qualiPeriod-1; ++k)
//        {
//            sessTime += LTData::getQualiLength(k+1);
//            sessTimeSecs += LTData::getQualiLength(k+1)*60;
//        }

        return "Q" + QString::number(ld.qualiPeriod) + " " + QString::number(sessTime) + ". minute";
    }


    virtual void drawAxes(QPainter *p, int firstMin, int lastMin);
    virtual void drawChart(QPainter *p);
    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:

};
#endif // FPLAPTIMESCHART_H
