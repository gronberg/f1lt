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



#ifndef DRIVERINFOLABEL_H
#define DRIVERINFOLABEL_H

#include <QWidget>
#include "../core/eventdata.h"

class DriverInfoLabel : public QWidget
{
    Q_OBJECT
public:
    explicit DriverInfoLabel(QWidget *parent = 0);
    QSize sizeHint();
    QSize minimumSize();
    
signals:
    
public slots:
    void setDriver(const DriverData *dd)
    {
        driverData = dd;
    }

    void update();
    void clear()
    {
        driverData = 0;
    }

protected:
    void paintEvent(QPaintEvent *);

private:
    const DriverData *driverData;
    QPixmap backgroundPixmap;
    
};

#endif // DRIVERINFOLABEL_H
