#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QAction>
#include <QDebug>
#include <QList>
#include <QMenu>
#include <QMouseEvent>
#include <QStack>
#include <QWidget>


#include "eventdata.h"


class ChartWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChartWidget(double, double, QColor, QWidget *parent = 0);
    ~ChartWidget();

    virtual void setData(const DriverData &d) { driverData = d; }
    void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p);
    virtual void drawChart(QPainter *p);
    virtual void drawScaleRect(QPainter *p);

    virtual void transform();
    virtual void resetZoom();

    void pushScaleRect();


    void checkX1(double &x1, double &y1, double x2, double y2);
    void checkX2(double x1, double y1, double &x2, double &y2);

public slots:
    virtual void onCopy();
    virtual void onSave();
    virtual void onZoomOut();

protected:
    void paintEvent(QPaintEvent *);   
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent (QMouseEvent *);

    DriverData driverData;
    double min, max;
    double tMin, tMax;
    int first, last;
    QColor color;

    QMenu *menu;
    QAction *copyAction;
    QAction *saveAction;
    QAction *zoomOutAction;

    struct ScaleAtom
    {
    	double min, max;
    	int first, last;
    };

    QRect scaleRect;
    QStack<ScaleAtom> scaleRectsStack;
    QRect paintRect;
    bool scaling;
    int lastOp;		//0 - zoom out, 1 - zoom in

};

class LapTimeChartWidget : public ChartWidget
{
    Q_OBJECT

public:
    explicit LapTimeChartWidget(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent)
    {
        for (int i = 0; i < 5; ++i)
            colors[i] = col[i];
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p, int, int);
    void setData(const DriverData &dd);

    void transform();
	void resetZoom();

public slots:
    void onCopy();
    void onSave();

protected:
    void paintEvent(QPaintEvent *);

private:
//    DriverData driverData;
    QColor colors[5];
};

class GapChartWidget : public ChartWidget
{
    Q_OBJECT
public:
    explicit GapChartWidget(QColor col, QWidget *parent = 0) : ChartWidget(0, 60, col, parent) { }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);

    void transform();
    void resetZoom();


public slots:
    void onCopy();
    void onSave();

protected:
    void paintEvent(QPaintEvent *);
};

class LapCompChartWidget : public ChartWidget
{
    Q_OBJECT

public:
    explicit LapCompChartWidget(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent)
    {
        for (int i = 0; i < 4; ++i)
            colors[i] = col[i];
    }

    void setData(DriverData *ld);
    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

    void transform();
	void resetZoom();

public slots:
    void onCopy();
    void onSave();

protected:
    void paintEvent(QPaintEvent *);

private:
    DriverData driverData[4];
    QColor colors[4];
};

class GapCompChartWidget : public ChartWidget
{
    Q_OBJECT

public:
    explicit GapCompChartWidget(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent), eventData(EventData::getInstance())
    {
        for (int i = 0; i < 2; ++i)
            colors[i] = col[i];
    }

    void setData(int *idx) { driverIdx[0] = idx[0]; driverIdx[1] = idx[1]; }
    void drawAxes(QPainter *p, int, int);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);

    double calculateInterval(int lap);

    void findFirstAndLastLap(int &firstLap, int &lastLap);

	void transform();
	void resetZoom();

public slots:
    void onCopy();
    void onSave();

protected:
    void paintEvent(QPaintEvent *);

private:
    EventData &eventData;
    int driverIdx[2];
    QColor colors[2];
};

class PosCompChartWidget : public ChartWidget
{
    Q_OBJECT

public:
    explicit PosCompChartWidget(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent)
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

    void findFirstAndLastLap(int &firstLap, int &lastLap);

	void transform();
	void resetZoom();

public slots:
    void onCopy();
    void onSave();

protected:
    void paintEvent(QPaintEvent *);

private:
    DriverData driverData[2];
    QColor colors[2];
};

class WeatherChartWidget : public ChartWidget
{
    Q_OBJECT

public:
    WeatherChartWidget(QColor col, int id, int id2, QWidget *parent = 0) : ChartWidget(0, 180, col, parent)
    {
    	weatherId = id;
        wetDryId = id2;
    }

    virtual void drawAxes(QPainter *p);
    virtual void drawChart(QPainter *p);
    virtual void setMinMax();
//    void drawLegend(QPainter *p, int, int);

//public slots:
//    void onCopy();
//    void onSave();

protected:
    void paintEvent(QPaintEvent *);

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

//public slots:
//    void onCopy();
//    void onSave();

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
//    void drawLegend(QPainter *p, int, int);

//public slots:
//    void onCopy();
//    void onSave();

//protected:
//    void paintEvent(QPaintEvent *);

};

#endif // CHARTWIDGET_H
