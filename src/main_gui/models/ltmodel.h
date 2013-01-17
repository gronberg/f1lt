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



#ifndef LTMODEL_H
#define LTMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QList>
#include <QPixmap>
#include "../../core/eventdata.h"

class LessThan
{
public:
    bool operator()(DriverData *d1, DriverData *d2)
    {
        if (d1->getPosition() <= 0)
            return false;

        if (d2->getPosition() <= 0)
            return true;

        return (d1->getPosition() < d2->getPosition());
    }
};



class LTModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LTModel(QObject *parent = 0);

    virtual void updateLT();


    virtual const DriverData *getDriverData(const QModelIndex &index) const;
    virtual bool indexInDriverRowsData(const QModelIndex &index) const;
    virtual int firstRow() const
    {
        return 1;
    }
    virtual int lastRow() const
    {
        return EventData::getInstance().getDriversData().size();
    }
    virtual int driverRows() const
    {
        return EventData::getInstance().getDriversData().size();
    }
    virtual QVariant driverRowData(const DriverData &, const QModelIndex &index, int role) const = 0;
    virtual QVariant headerRowData(const QModelIndex &index, int role) const = 0;
    virtual QVariant extraRowData(const QModelIndex &index, int role) const = 0;
    virtual QModelIndex indexOf(int driverId);

    virtual bool selectDriver(int id, int column)
    {
        if (selectedDriver.first == id)
        {
            if (selectedDriver.second == column)
                selectedDriver.second = 0;
            else
                selectedDriver.second = column;
        }
        else
        {
            selectedDriver.first = id;
            selectedDriver.second = column;
        }
        return true;
    }
    virtual void headerClicked(int)
    {

    }
    void clearData()
    {
        driversData.clear();
    }
    
signals:
    
public slots:

protected:
    virtual void gatherDriversData();
    QList<DriverData *> driversData;

    QPair<int, int> selectedDriver;     //first - driver id, second - column code
    
};

#endif // LTMODEL_H
