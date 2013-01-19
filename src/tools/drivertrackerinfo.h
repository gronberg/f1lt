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


#ifndef DRIVERTRACKERINFO_H
#define DRIVERTRACKERINFO_H

#include <QWidget>

#include "../core/driverdata.h"

class DriverTrackerInfo : public QWidget
{
    Q_OBJECT
public:
    explicit DriverTrackerInfo(QWidget *parent = 0);

    void setDriverData(DriverData *dd)
    {
        driverData = dd;
        ImagesFactory::getInstance().getCarThumbnailsFactory().loadCarThumbnails(240, false);

        repaint();
    }

    void setup();

    void paintDriverInfo(QPainter *p);
    void paintLapsInfo(QPainter *p);
    void paintGapsInfo(QPainter *p);

protected:
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:

private:
    QPixmap labelBig;
    QPixmap labelInfoLong;

    DriverData *driverData;
    
};

#endif // DRIVERTRACKERINFO_H
