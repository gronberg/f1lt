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


#ifndef DRIVERTRACKER_H
#define DRIVERTRACKER_H

#include <QPixmap>
#include "driverradar.h"

class DriverTracker : public DriverRadar
{
    Q_OBJECT

public:
    DriverTracker(QWidget *parent = 0);

    virtual void loadDriversList();
    virtual void setupDrivers(int speed);

    void paintClassification(QPainter &p);

    void setDrawDriverClassification(bool val)
    {
        drawClassification = val;
        setMinimumSize();
        resizeEvent(0);
        repaint();
    }
    bool drawDriverClassification()
    {
        return drawClassification;
    }

    void setMinimumSize();

signals:

    void driverSelected(int);

protected:
    virtual void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

    bool isExcluded(int id);

    QPixmap label;
    QPixmap selectedLabel;

    QPixmap trackMap;

    QList<int> excludedDrivers;

    bool drawClassification;
};

#endif // DRIVERTRACKER_H
