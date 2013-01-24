/*******************************************************************************
 *                                                                             *
 *   F1LT - unofficial Formula 1 live timing application                       *
 *   Copyright (C) 2012-2013  Mariusz Pilarek (pieczaro@gmail.com)             *
 *                                                                             *
 *   This program is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation, either version 3 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                             *
 *******************************************************************************/


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
    void driverSelected(int);
    
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
    void mousePressEvent(QMouseEvent *);

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
