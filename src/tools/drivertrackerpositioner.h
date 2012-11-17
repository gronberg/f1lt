#ifndef DRIVERTRACKERPOSITIONER_H
#define DRIVERTRACKERPOSITIONER_H

#include "driverradarpositioner.h"

class DriverTrackerPositioner : public DriverRadarPositioner
{
public:
    DriverTrackerPositioner(DriverData *dd = 0);

    virtual void paint(QPainter *p, bool selected = false);
    virtual QPoint getCoordinates();
    virtual QPoint getSCCoordinates();

    virtual void setStartupPosition();
    virtual void calculatePosition();
    virtual void calculatePitPosition();

    void setMapCoords(int x, int y, int px, int py, int pw, int ph)
    {
        mapX = x;
        mapY = y;
        pitX = px;
        pitY = py;
        pitW = pw;
        pitH = ph;
    }

    virtual int maxDeg()
    {
        return coordinatesCount;
    }

    virtual void calculatePitOutPosition();


private:
    int coordinatesCount;
    int mapX, mapY;
    int pitX, pitY, pitW, pitH;

};

#endif // DRIVERTRACKERPOSITIONER_H
