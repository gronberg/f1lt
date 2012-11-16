#ifndef DRIVERRADARPOSITIONER_H
#define DRIVERRADARPOSITIONER_H


#include <QPainter>
#include "../core/driverdata.h"

class DriverRadarPositioner
{
public:
    DriverRadarPositioner(DriverData *dd = 0, int x = 0, int y = 0, double r = 0.0, double rP = 0.0, double rL = 0.0);

    virtual void paint(QPainter *p);
    virtual QPoint getCoordinates();

    void update();


    void calculateAvgs();

    void setDriverData(DriverData *dd);

    virtual void setStartupPosition();

    void setRadarCoords(int x, int y, double r, double rP, double rL)
    {
        radarX = x;
        radarY = y;
        radarR = r;
        radarPitR = rP;
        radarLappedR = rL;
    }
    void setupHelmet(int size);

    virtual void calculatePosition();
    virtual void calculatePitPosition();
    virtual void calculatePitOutPosition()
    {
        currentDeg = 0;
    }

    virtual int maxDeg()
    {
        return 360;
    }

    void setSpeed(int s)
    {
        speed = s;
    }

    int getDriverId()
    {
        if (driverData != 0)
            return driverData->getCarID();

        return -1;
    }

    bool isExcluded()
    {
        return excluded;
    }

    void setExcluded(bool ex)
    {
        excluded = ex;
    }

    bool hasEmptyHelmet()
    {
        return helmet.isNull();
    }

protected:
    DriverData *driverData;
    double avgTime;
    double avgSectorTimes[2];
    double sectorPositions[2];
    int currSector;
    double currentDeg;

    int currentLapTime;
    bool startingNewLap;

    bool inPits;
    bool lapped;
    bool finished;
    bool qualiOut;

    bool excluded;

    QImage helmet;

    int speed;

private:
    int radarX, radarY;
    double radarR, radarPitR, radarLappedR;    
};

#endif // DRIVERRADARPOSITIONER_H
