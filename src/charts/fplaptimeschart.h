#ifndef FPLAPTIMESCHART_H
#define FPLAPTIMESCHART_H

#include "sessionlaptimeschart.h"

class FPLapTimesChart : public SessionLapTimesChart
{
    Q_OBJECT

public:
    FPLapTimesChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
    }
    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
//    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

    virtual int getSessionLength()
    {
        return SeasonData::getInstance().getFPLength();
    }

    virtual QString getLapInfoXY(const LapData &ld)
    {
        int minute = SeasonData::getInstance().getFPLength() - SeasonData::getInstance().timeToMins(ld.getPracticeLapExtraData().getSessionTime());
        if (ld.getPracticeLapExtraData().getSessionTime().toString("h:mm:ss") == "")
            minute = 0;

        return QString::number(minute) + ". minute";
    }

//signals:
//    void zoomChanged(int, int, double, double);

protected:
    virtual void paintEvent(QPaintEvent *);

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
        qualiPeriod = 1;
        first = 0;
        last = getSessionLength();
        tMin = min;
        tMax = max;        
    }

    int getMin() { return tMin == min ? -1 : tMin; }
    int getMax() { return tMax == max ? -1 : tMax; }

    virtual int getSessionLength()
    {
        return SeasonData::getInstance().getQualiLength(qualiPeriod);
    }

    virtual QString getLapInfoXY(const LapData &ld)
    {
        int minute = getSessionLength() - SeasonData::getInstance().timeToMins(ld.getPracticeLapExtraData().getSessionTime());

        if (ld.getPracticeLapExtraData().getSessionTime().toString("h:mm:ss") == "")
            minute = 0;
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
        first = 0;
        last = getSessionLength();
        tMin = min;
        tMax = max;
        qualiPeriod = 1;
    }

    virtual int getSessionLength()
    {
        int sessionLength = 0;
        for (int i = 0; i < 3; ++i)
            sessionLength += SeasonData::getInstance().getQualiLength(i+1);

        return sessionLength;
    }

    int getMin() { return tMin == min ? -1 : tMin; }
    int getMax() { return tMax == max ? -1 : tMax; }

    virtual QString getLapInfoXY(const LapData &ld)
    {
        int sessTime = SeasonData::getInstance().getQualiLength(ld.getQualiLapExtraData().getQualiPeriod()) - SeasonData::getInstance().timeToMins(ld.getQualiLapExtraData().getSessionTime());

        if (ld.getQualiLapExtraData().getSessionTime().toString("h:mm:ss") == "")
            sessTime = 0;
//        int sessTimeSecs = LTData::getQualiLength(lapDataArray[i].qualiPeriod) * 60 - LTData::timeToSecs(lapDataArray[i].sessionTime);

//        for (int k = 0; k < lapDataArray[i].qualiPeriod-1; ++k)
//        {
//            sessTime += LTData::getQualiLength(k+1);
//            sessTimeSecs += LTData::getQualiLength(k+1)*60;
//        }

        return "Q" + QString::number(ld.getQualiLapExtraData().getQualiPeriod()) + " " + QString::number(sessTime) + ". minute";
    }


    virtual void drawAxes(QPainter *p, int firstMin, int lastMin);
    virtual void drawChart(QPainter *p);
    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:

};
#endif // FPLAPTIMESCHART_H
