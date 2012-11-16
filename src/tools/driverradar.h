#ifndef DRIVERRADAR_H
#define DRIVERRADAR_H

#include <QDebug>
#include <QVector>
#include <QWidget>

#include "driverradarpositioner.h"
#include "drivertrackerinfo.h"

#include "../core/eventdata.h"

class DriverRadar : public QWidget
{
    Q_OBJECT
public:
    explicit DriverRadar(QWidget *parent = 0);
    virtual ~DriverRadar();
           
    void update();
    virtual void loadDriversList();
    virtual void setupDrivers(int speed);

    virtual void checkSetupCorrect(int speed);

signals:
    void driverExcluded(int, bool);
    
public slots:
    void excludeDriver(int id, bool exclude);
    void selectDriver(int id)
    {
        selectedDriver = id;

        if (dti)
        {
            dti->setVisible(id == -1 ? false : true);

            if (id != -1)
            {
                DriverData *dd = EventData::getInstance().getDriverDataByIdPtr(id);
                if (dd)
                    dti->setDriverData(dd);
            }
        }
        repaint();
    }

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    QVector<DriverRadarPositioner*> drp;

    int selectedDriver;

    DriverTrackerInfo *dti;

private:    
    int radarX, radarY;
    double radarR;
    double radarPitR;
    double radarLappedR;
};

#endif // DRIVERRADAR_H
