#ifndef SESSIONLAPTIMESCHART_H
#define SESSIONLAPTIMESCHART_H

#include <QColor>
#include <QList>

#include "chartwidget.h"
#include "../core/driverdata.h"


extern bool lessThan(LapData ld1, LapData ld2);

class SessionLapTimesChart;

class PredXYPos
{
public:
    PredXYPos (SessionLapTimesChart &sltc) : chart(sltc) { }
    bool operator()(int item1, int item2);

protected:
    SessionLapTimesChart &chart;
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
    virtual void drawImage(QPainter *p);
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
        return "LAP " + QString::number(ld.getLapNumber());
    }
    virtual QString getDriverInfoXY(const LapData &ld)
    {
        if (ld.getCarID() > 0)
        {
            DriverData dd = EventData::getInstance().getDriversData()[ld.getCarID()-1];
            return QString::number(ld.getPosition()) + ". " +  dd.getDriverName() + ": " + ld.getTime().toString();
        }
        return "";
    }
    void clearXYList(int to)
    {
        for (int i = lapDataXYArray.size()-1; i >= to; --i)
            lapDataXYArray.removeAt(i);
    }
    virtual int getPopupWidth()
    {
        return 200;
    }

    friend class PredXYPos;

protected:
    virtual void mouseDoubleClickEvent (QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(42, 10, width()-47, height()-35);
    }

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

    QImage chartImg;
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

    virtual void resetPaintRect()
    {
        paintRect = QRect(60, 10, width()-35, height()-35);
    }

    struct DriverPosAtom
    {
        int pos;
        int id;

        DriverPosAtom(int p, int i) : pos(p), id(i) { }

        bool operator < (const DriverPosAtom &p)const
        {
            if (pos <= 0)
                return false;

            if (p.pos <= 0)
                return true;

            return pos < p.pos;
        }
    };
    QList<DriverPosAtom> getDriverStartingPositions();
};


class SessionGapsChart : public SessionLapTimesChart
{
public:
    SessionGapsChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
        min = 0;
        max = 90;
    }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

    virtual QString getDriverInfoXY(const LapData &ld)
    {
        if (ld.getCarID() > 0)
        {
            DriverData dd = EventData::getInstance().getDriversData()[ld.getCarID()-1];
            if (ld.getPosition() == 1)
                return QString::number(ld.getPosition()) + ". " + dd.getDriverName() + ": Leader";

            QString gap = ld.getGap();
            if (gap == "")
                gap = "1L";
            return QString::number(ld.getPosition()) + ". " +  dd.getDriverName() + ": +" + gap;
        }
        return "";
    }

    virtual int findLapDataXY(int x, int y);

protected:
    void paintEvent(QPaintEvent *);

    void resetPaintRect()
    {
        paintRect = QRect(30, 10, width()-35, height()-35);
    }
};


#endif // SESSIONLAPTIMESCHART_H
