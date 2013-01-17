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



#ifndef DRIVERLAPHISTORYMODEL_H
#define DRIVERLAPHISTORYMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "../../core/eventdata.h"

class DriverLapHistoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DriverLapHistoryModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex())
    {
        beginInsertRows(parent, row, row + count);
        endInsertRows();
        return true;
    }

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex())
    {
        beginRemoveRows(parent, row, row + count - 1);
        endRemoveRows();
        return true;
    }

    QVariant getLapTime(const LapData &, int role) const;
    QVariant getSectorTime(const LapData &, int role, int sector) const;

    QVariant headerData(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void update(DriverData *dd);
    void clear()
    {
        driverData = 0;
        rows = 0;        
        reset();
    }
    
signals:
    
public slots:

private:
    DriverData *driverData;
    int rows;
    
};

#endif // DRIVERLAPHISTORYMODEL_H
