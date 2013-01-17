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



#ifndef PITSTOPSMODEL_H
#define PITSTOPSMODEL_H

#include <QAbstractTableModel>

#include "../../core/eventdata.h"

class PitStopsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PitStopsModel(QObject *parent = 0);

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

    QVariant headerData(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void update();

signals:
    
public slots:

private:
    int rows;

    struct PitStopAtom
    {
        double time;
        int lap;
        QString driver;
        int pos;

        bool operator <(const PitStopAtom &psa) const
        {
            return time < psa.time;
        }
    };

    void getPitstops(const QVector<DriverData> &driversData);
    QList< PitStopAtom > pitStops;
    
};

#endif // PITSTOPSMODEL_H
