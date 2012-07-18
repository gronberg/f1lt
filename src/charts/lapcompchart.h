#ifndef LAPCOMPCHART_H
#define LAPCOMPCHART_H

#include "chartwidget.h"


struct LapDataXYComp : public LapDataXY
{
    int driverIdx;

    LapDataXYComp() : LapDataXY(), driverIdx(0) { }
    LapDataXYComp(int a, int b, int c, int d) : LapDataXY(a, b, c), driverIdx(d) { }
};

struct LapDataXYGapComp : public LapDataXYComp
{
    double gap;

    LapDataXYGapComp() : LapDataXYComp(), gap(0.0) { }
    LapDataXYGapComp(int a, int b, int c, int d, double g) : LapDataXYComp(a, b, c, d), gap(g) { }
};

class LapCompChart;
class GapCompChart;

class PredXYTime
{
public:
    PredXYTime (LapCompChart &sltc) : chart(sltc) { }
    bool operator()(int item1, int item2);

protected:
    LapCompChart &chart;
};

class PredXYGap
{
public:
    PredXYGap (GapCompChart &sltc) : chart(sltc) { }
    bool operator()(int item1, int item2);

protected:
    GapCompChart &chart;
};

class LapCompChart : public ChartWidget
{
    Q_OBJECT

public:
    explicit LapCompChart(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent)
    {
        for (int i = 0; i < 4; ++i)
            colors[i] = col[i];
    }

    void setData(DriverData *ld);
    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void drawIntoImage(QImage &img);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    void transform();
    void resetZoom();

    virtual QString getLapInfoXY(const LapData &ld)
    {
        return "LAP " + QString::number(ld.numLap);
    }
    virtual QString getDriverInfoXY(const LapData &ld)
    {
        DriverData dd = EventData::getInstance().driversData[ld.carID-1];
        return dd.driver + ": " + ld.lapTime.toString();
    }
    virtual int getPopupWidth()
    {
        return 200;
    }

    virtual int findLapDataXY(int x, int y);
    virtual void drawLapDataXY(QPainter *p);

    virtual void clearXYList(int to)
    {
        for (int i = lapDataXYCompArray.size()-1; i >= to; --i)
            lapDataXYCompArray.removeAt(i);
    }

    friend class PredXYTime;

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(37, 10, width()-42, height()-35);
    }

private:
    DriverData driverData[4];
    QColor colors[4];

    QList<LapDataXYComp> lapDataXYCompArray;
};



class GapCompChart : public ChartWidget
{
    Q_OBJECT

public:
    explicit GapCompChart(QColor *col, QWidget *parent = 0) : ChartWidget(0, 0, col[0], parent), eventData(EventData::getInstance())
    {
        for (int i = 0; i < 2; ++i)
            colors[i] = col[i];
    }

    void setData(int *idx) { driverIdx[0] = idx[0]; driverIdx[1] = idx[1]; }
    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void drawIntoImage(QImage &img);

    double calculateInterval(int lap);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    virtual int findLapDataXY(int x, int y);
    virtual void drawLapDataXY(QPainter *p);

    virtual void clearXYList(int to)
    {
        for (int i = lapDataXYGapCompArray.size()-1; i >= to; --i)
            lapDataXYGapCompArray.removeAt(i);
    }

    virtual QString getLapInfoXY(const LapData &ld)
    {
        return "LAP " + QString::number(ld.numLap);
    }
    virtual QString getDriverInfoXY(const LapData &ld, double gap)
    {
        DriverData dd = EventData::getInstance().driversData[ld.carID-1];
        if (gap == 0.0)
            return dd.driver;

        if (gap == 1000.0)
            return dd.driver + ": +1L <";

        return dd.driver + ": +" + QString::number(gap, 'f', 1);
    }
    virtual int getPopupWidth()
    {
        return 150;
    }

    friend class PredXYGap;


    void transform();
    void resetZoom();

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(37, 10, width()-42, height()-35);
    }

private:
    EventData &eventData;
    int driverIdx[2];
    QColor colors[2];

    QList<LapDataXYGapComp> lapDataXYGapCompArray;
};

class PosCompChart : public ChartWidget
{
    Q_OBJECT

public:
    explicit PosCompChart(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent)
    {
        for (int i = 0; i < 2; ++i)
            colors[i] = col[i];
    }

    void setData(DriverData *ld)
    {
        for (int i = 0; i < 2; ++i)
            driverData[i] = ld[i];
    }

    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);
    void drawIntoImage(QImage &img);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    void transform();
    void resetZoom();

protected:

    virtual void resetPaintRect()
    {
        paintRect = QRect(37, 10, width()-42, height()-35);
    }
    void paintEvent(QPaintEvent *);

private:
    DriverData driverData[2];
    QColor colors[2];
};



#endif // LAPCOMPCHART_H
