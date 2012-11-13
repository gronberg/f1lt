#ifndef DRIVERTRACKER_H
#define DRIVERTRACKER_H

#include <QPixmap>
#include "driverradar.h"

class DriverTracker : public DriverRadar
{
public:
    DriverTracker(QWidget *parent = 0);

    virtual void loadDriversList();
    virtual void setupDrivers();

protected:
    virtual void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    QPixmap label;
};

#endif // DRIVERTRACKER_H
