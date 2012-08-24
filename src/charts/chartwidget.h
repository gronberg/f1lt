#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QAction>
#include <QDebug>
#include <QList>
#include <QMenu>
#include <QMouseEvent>
#include <QStack>
#include <QWidget>


#include "../core/eventdata.h"

struct LapDataXY
{
    int idx;
    int x, y;

    LapDataXY() : idx(0), x(0), y(0) { }
    LapDataXY(int a, int b, int c) : idx(a), x(b), y(c) { }
};

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
    virtual void drawLegend(QPainter *p);
    virtual void drawScaleRect(QPainter *p);

    virtual void drawSCLap(QPainter *p, const LapData &ld, double xFactor);
    virtual void drawIntoImage(QImage &img);

    virtual void transform();
    virtual void resetZoom();       

    void pushScaleRect();


    void checkX1(double &x1, double &y1, double x2, double y2);
    void checkX2(double x1, double y1, double &x2, double &y2);

    virtual int findLapDataXY(int x, int y);
    virtual void drawLapDataXY(QPainter *p);
    virtual int getPopupWidth()
    {
        return 150;
    }

    virtual QString getLapInfoXY(const LapData &)
    {
        return "";
    }
    virtual QString getDriverInfoXY(const LapData &)
    {
        return "";
    }
    virtual void clearXYList(int to)
    {
        for (int i = lapDataXYArray.size()-1; i >= to; --i)
            lapDataXYArray.removeAt(i);
    }

    int getMin() { return min; }
    int getMax() { return max; }
    int getFirst() { return first; }
    int getLast() { return last; }

public slots:
    virtual void onCopy();
    virtual void onSave();
    virtual void onZoomOut();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent (QMouseEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(27, 10, width()-32, height()-35);
    }

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


    QList<LapDataXY> lapDataXYArray;
    QList<int> itemsInXY;

    int mousePosX, mousePosY;
    bool repaintPopup;

};

class LapTimeChart : public ChartWidget
{
    Q_OBJECT

public:
    LapTimeChart(QColor *col, QWidget *parent = 0) : ChartWidget(0, 180, col[0], parent)
    {
        for (int i = 0; i < 5; ++i)
            colors[i] = col[i];
    }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);
    void drawLegend(QPainter *p);

    void setData(const DriverData &dd);

    virtual QString getDriverInfoXY(const LapData &ld)
    {
        return "LAP "+QString::number(ld.getLapNumber())+": "+ld.getTime().toString();
    }

    void transform();
	void resetZoom();

protected:
    void paintEvent(QPaintEvent *);

    virtual void resetPaintRect()
    {
        paintRect = QRect(28, 10, width()-33, height()-35);
    }

private:
    QColor colors[5];
};

class GapChart : public ChartWidget
{
    Q_OBJECT
public:
    GapChart(QColor col, QWidget *parent = 0) : ChartWidget(0, 60, col, parent) { }

    void drawAxes(QPainter *p);
    void drawChart(QPainter *p);    

    void transform();
    void resetZoom();

    virtual QString getDriverInfoXY(const LapData &ld)
    {
        QString gap = ld.getGap();
        if (gap == "")
            gap = "1L";

        if (ld.getPosition() == 1)
            return "LAP "+QString::number(ld.getLapNumber()) + ": Leader";

        return "LAP "+QString::number(ld.getLapNumber())+": +"+gap;
    }
    virtual int getPopupWidth()
    {
        return 110;
    }

protected:
    void paintEvent(QPaintEvent *);
    virtual void resetPaintRect()
    {
        paintRect = QRect(28, 10, width()-33, height()-35);
    }
};



#endif // CHARTWIDGET_H
