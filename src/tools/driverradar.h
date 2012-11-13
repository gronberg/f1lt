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
    virtual ~DriverRadar();
           
    void update();
    virtual void loadDriversList();
    virtual void setupDrivers();

signals:
    
public slots:

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    QVector<DriverRadarPositioner*> drp;
    QPixmap trackMap;

private:    
    int radarX, radarY;
    double radarR;
    double radarPitR;
    double radarLappedR;        
};

#endif // DRIVERRADAR_H
