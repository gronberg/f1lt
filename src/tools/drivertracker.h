#ifndef DRIVERTRACKER_H
#define DRIVERTRACKER_H

#include <QPixmap>
#include "driverradar.h"

class DriverTracker : public DriverRadar
{
    Q_OBJECT

public:
    DriverTracker(QWidget *parent = 0);

    virtual void loadDriversList();
    virtual void setupDrivers(int speed);

    void paintLegend(QPainter &p);

signals:

    void driverSelected(int);

protected:
    virtual void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

    bool isExcluded(int id);

    QPixmap label;
    QPixmap selectedLabel;

    QPixmap trackMap;

    QList<int> excludedDrivers;
};

#endif // DRIVERTRACKER_H
