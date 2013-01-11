#ifndef SESSIONLAPTIMESCHART_H
#define SESSIONLAPTIMESCHART_H

#include <QColor>
#include <QList>

#include "driverdatachart.h"
#include "../core/driverdata.h"


extern bool lessThan(LapData ld1, LapData ld2);

struct PopupDriversGapsInfoBox : public PopupInfoBox
{
    PopupDriversGapsInfoBox() : PopupInfoBox() { width = 200; }
    virtual QString getInfo(int i)
    {
        QString gap = values[i].getGap();
        if (values[i].getPosition() == 1)
            gap = "Leader";
        if (gap == "")
            gap = "+1L";

        DriverData dd = EventData::getInstance().getDriverDataById(values[i].getCarID());

        if (dd.getCarID() > 0)
            return QString("%1. %2: %3").arg(values[i].getPosition()).arg(dd.getDriverName()).arg(gap);

        return QString();
    }
};

struct PopupDriversLapTimesInfoBox : public PopupInfoBox
{
    PopupDriversLapTimesInfoBox() : PopupInfoBox() { width = 200; }
    virtual QString getInfo(int i)
    {
        DriverData dd = EventData::getInstance().getDriverDataById(values[i].getCarID());

        if (dd.getCarID() > 0)
            return QString("%1. %2: %3").arg(values[i].getPosition()).arg(dd.getDriverName()).arg(values[i].getTime().toString());

        return QString();
    }
};

class SessionLapTimesChart : public DriverDataChart
{

    Q_OBJECT

public:
    SessionLapTimesChart(QWidget *parent) : DriverDataChart(0, 180, QColor(), parent)
    {
        setMouseTracking(true);
        mousePosX = 0;
        mousePosY = 0;
        repaintPopup = false;

        popupBox = new PopupDriversLapTimesInfoBox();
    }

    virtual void setData(const QList<LapData> ld) { lapDataArray = ld; qSort(lapDataArray.begin(), lapDataArray.end(), lessThan);}

    virtual void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);
    virtual void drawIntoImage(QImage &img);
    virtual void drawImage(QPainter *p);
    virtual void drawLegend(QPainter *) { }
//    virtual void drawRetire(QPainter *p, int x, int y, const LapData &ld, QColor color);
//    virtual void drawScaleRect(QPainter *p);

    virtual void calculateTransformFactors();
    virtual void resetZoom();

    virtual void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);
    QColor getCarColor(const LapData &ld);
    QColor getCarColor(const DriverData &dd);

    virtual int checkLapDataCoordinates(int x, int y);

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
    virtual void onZoomOut();

protected:
    void paintEvent(QPaintEvent *);

    QList<LapData> lapDataArray;

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
    void drawDriversLegend(QPainter *p, const LapData &ld, double y);

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);
    virtual void calculateTransformFactors();

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(60, 20, width()-65, height()-20);
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
    int getDriverStartingPosition(const LapData &ld);    
};


class SessionGapsChart : public SessionLapTimesChart
{
public:
    SessionGapsChart(QWidget *parent) : SessionLapTimesChart(parent)
    {
        min = 0;
        max = 90;

        popupBox = new PopupDriversGapsInfoBox();
    }

    virtual void drawAxes(QPainter *p, int firstLap, int lastLap);
    virtual void drawChart(QPainter *p);

    void findFirstAndLastLap(int &firstLap, int &lastLap, int &size);

protected:
    void paintEvent(QPaintEvent *);

    void resetPaintRect()
    {
        paintRect = QRect(30, 10, width()-35, height()-35);
    }
};


#endif // SESSIONLAPTIMESCHART_H
