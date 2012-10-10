#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QAction>
#include <QDebug>
#include <QList>
#include <QMenu>
#include <QMouseEvent>
#include <QStack>
#include <QWidget>




class ChartWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChartWidget(double, double, QColor, QWidget *parent = 0);
    ~ChartWidget();

    void setMinMax(double min, double max) { this->min = min, this->max = max; }

    virtual void drawAxes(QPainter *p) = 0;
    virtual void drawChart(QPainter *p) = 0;
    virtual void drawLegend(QPainter *p) = 0;
    virtual void drawScaleRect(QPainter *p);
    virtual void drawIntoImage(QImage &img);

    virtual void transform();
    virtual void calculateTransformFactors() = 0;
    virtual void resetZoom();       

    void pushScaleRect();

    void checkX1(double &x1, double &y1, double x2, double y2);
    void checkX2(double x1, double y1, double &x2, double &y2);

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
};




#endif // CHARTWIDGET_H
