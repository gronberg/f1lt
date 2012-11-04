#ifndef DRIVERRADAR_H
#define DRIVERRADAR_H

#include <QDebug>
#include <QVector>
#include <QWidget>

#include "driverradarpositioner.h"

#include "../core/eventdata.h"

class DriverRadar : public QWidget
{
    Q_OBJECT
public:
    explicit DriverRadar(QWidget *parent = 0);
           
    void update();
    void loadDriversList();
    void setupDrivers();

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
    
    QPixmap trackMap;
};

#endif // DRIVERRADAR_H
