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

    void paintClassification(QPainter &p);

    void setDrawDriverClassification(bool val)
    {
        drawClassification = val;
        setMinimumSize();
        resizeEvent(0);
        repaint();
    }

    void setMinimumSize();

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

    bool drawClassification;
};

#endif // DRIVERTRACKER_H
