#ifndef SESSIONLAPTIMESCHART_H
#define SESSIONLAPTIMESCHART_H

#include <QColor>
#include <QList>

#include "chartwidget.h"
#include "../core/driverdata.h"


extern bool lessThan(LapData ld1, LapData ld2);

struct LapDataXY
{
    int idx;
    int x, y;

    LapDataXY() : idx(0), x(0), y(0) { }
    LapDataXY(int a, int b, int c) : idx(a), x(b), y(c) { }
};

class SessionLapTimesChart;

class PredXY
{
public:
    PredXY (SessionLapTimesChart &sltc) : chart(sltc) { }
    bool operator()(int item1, int item2);

protected:
    SessionLapTimesChart &chart;
};

class PredXYGap : public PredXY
{
public:
    PredXYGap (SessionLapTimesChart &sltc) : PredXY(sltc) { }
    bool operator()(int item1, int item2);

};

class SessionLapTimesChart : public ChartWidget
{

    Q_OBJECT

public:
    SessionLapTimesChart(QWidget *parent) : ChartWidget(0, 180, QColor(), parent)
    {
        setMouseTracking(true);
        mousePosX = 0;
        mousePosY = 0;
        repaintPopup = false;
    }

    virtual void setData(const QList<LapData> ld) { lapDataArray = ld; qSort(lapDataArray.begin(), lapDataArray.end(), lessThan);}
    virtual void setColors(const QList<QColor> cl) { colors = cl; }

    virtual void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
    virtual void drawIntoImage(QImage &img);
    virtual void drawLegend(QPainter *) { }
//    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    void checkX1(double &x1, double &y1, double x2, double y2);
    void checkX2(double x1, double y1, double &x2, double &y2);

    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);
    QColor getCarColor(const LapData &ld);

    virtual int findLapDataXY(int x, int y);
    virtual void drawLapDataXY(QPainter *p);

    virtual QString getLapInfoXY(const LapData &ld)
    {
        return "LAP " + QString::number(ld.numLap);
    }
    virtual QString getDriverInfoXY(const LapData &ld)
    {
        if (ld.carID > 0)
        {
            DriverData dd = EventData::getInstance().driversData[ld.carID-1];
            return dd.driver + " - " + ld.lapTime.toString();
        }
        return "";
    }
    void clearXYList(int to)
    {
        for (int i = lapDataXYArray.size()-1; i >= to; --i)
            lapDataXYArray.removeAt(i);
    }

    friend class PredXY;
    friend class PredXYGap;

protected:
    virtual void mouseDoubleClickEvent (QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

signals:
    void zoomChanged(int, int, double, double);

public slots:
//    virtual void onCopy();
//    virtual void onSave();
    virtual void onZoomOut();

protected:
    void paintEvent(QPaintEvent *);

    QList<LapData> lapDataArray;
    QList<LapDataXY> lapDataXYArray;
    QList<int> itemsInXY;
    QList<QColor> colors;

    int mousePosX, mousePosY;
    bool repaintPopup;

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

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

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

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

    virtual QString getDriverInfoXY(const LapData &ld)
    {
        if (ld.carID > 0)
        {
            DriverData dd = EventData::getInstance().driversData[ld.carID-1];
            if (ld.pos == 1)
                return dd.driver;

            return dd.driver + " +" + ld.gap;
        }
        return "";
    }

    virtual int findLapDataXY(int x, int y);

protected:
    void paintEvent(QPaintEvent *);
};


#endif // SESSIONLAPTIMESCHART_H
