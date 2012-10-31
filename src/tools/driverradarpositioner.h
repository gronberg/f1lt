#ifndef DRIVERRADARPOSITIONER_H
#define DRIVERRADARPOSITIONER_H


#include <QPainter>
#include "../core/driverdata.h"

class DriverRadarPositioner
{
public:
    DriverRadarPositioner(DriverData *dd = 0, int x = 0, int y = 0, double r = 0.0, double rP = 0.0, double rL = 0.0);
    void paint(QPainter *p);
    void update();

    void calculateAvgs();

    void setDriverData(DriverData *dd);

    void setStartupPosition();

    void setRadarCoords(int x, int y, double r, double rP, double rL)
    {
        radarX = x;
        radarY = y;
        radarR = r;
        radarPitR = rP;
        radarLappedR = rL;
    }

    void calculatePosition();
    void calculatePitPosition();

private:
    DriverData *driverData;
    int avgTime;
    int avgSectorTimes[2];
    int sectorPositions[2];
    int currSector;

    double currentDeg;
    int radarX, radarY;
    double radarR, radarPitR, radarLappedR;

    int currentLapTime;
    bool startingNewLap;

    bool inPits;
    bool lapped;
};

#endif // DRIVERRADARPOSITIONER_H
