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


#include "ltmodel.h"
#include <QDebug>

#include "../../core/eventdata.h"

LTModel::LTModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    selectedDriver = QPair<int, int>(0, 0);
}

void LTModel::updateLT()
{
    //driversData = QList<DriverData>(EventData::getInstance().getDriversData());
    gatherDriversData();
    qSort(driversData.begin(), driversData.end(), LessThan());

    QModelIndex topLeft = QAbstractTableModel::index(firstRow(), 0);
    QModelIndex bottomRight = QAbstractTableModel::index(rowCount()-1, columnCount()-1);

    emit dataChanged(topLeft, bottomRight);
}

const DriverData *LTModel::getDriverData(const QModelIndex &index) const
{
    int row = index.row();
    if (row >= 1 && row <= EventData::getInstance().getDriversData().size() && (row - 1) < driversData.size())
    {
        return driversData[row-1];
    }
    return 0;
}

bool LTModel::indexInDriverRowsData(const QModelIndex &index) const
{
    int row = index.row();
    if (row >= 1 && row <= EventData::getInstance().getDriversData().size())
        return true;

    return false;
}

QModelIndex LTModel::indexOf(int driverId)
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i]->getCarID() == driverId)
        {
            QModelIndex idx = index(i+1, 0);
            return idx;
        }
    }
//    DriverData dd = EventData::getInstance().getDriverDataById(driverId);
//    if (dd.getCarID() > 0)
//    {

//    }
    return QModelIndex();
}

void LTModel::gatherDriversData()
{
    driversData.clear();
    for (int i = 0; i < EventData::getInstance().getDriversData().size(); ++i)
    {
        DriverData *dd = &EventData::getInstance().getDriversData()[i];
        driversData.append(dd);
    }
}
