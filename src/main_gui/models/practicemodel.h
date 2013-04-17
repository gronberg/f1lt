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



#ifndef PRACTICEMODEL_H
#define PRACTICEMODEL_H

#include "ltmodel.h"
#include "../../core/eventdata.h"

class PracticeModel : public LTModel
{
public:
    PracticeModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    virtual QVariant driverRowData(const DriverData &dd, const QModelIndex &index, int role) const;
    virtual QVariant headerRowData(const QModelIndex &index, int role) const;
    virtual QVariant extraRowData(const QModelIndex &index, int role) const;

    QVariant gapToSelected(const DriverData &dd, int column) const;

    virtual bool selectDriver(int id, int column)
    {
        LTModel::selectDriver(id, column);
        if (column == 0 || (column >= 4 && column < 9))
        {
            updateLT();
            return true;
        }

        return false;
    }

private:
    EventData &eventData;
};

#endif // PRACTICEMODEL_H
