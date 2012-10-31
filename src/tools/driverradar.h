#ifndef DRIVERRADAR_H
#define DRIVERRADAR_H

#include <QVector>
#include <QWidget>

#include "driverradarpositioner.h"

class DriverRadar : public QWidget
{
    Q_OBJECT
public:
    explicit DriverRadar(QWidget *parent = 0);
           
    void update();
    void loadDriversList();
    void setupDrivers()
    {
        for (int i = 0; i < drp.size(); ++i)
            drp[i].setStartupPosition();
    }

signals:
    
public slots:

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    QVector<DriverRadarPositioner> drp;
    int radarX, radarY;
    double radarR;
    double radarPitR;
    double radarLappedR;
    
};

#endif // DRIVERRADAR_H
