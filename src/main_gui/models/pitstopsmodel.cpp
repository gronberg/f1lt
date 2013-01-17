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


#include "pitstopsmodel.h"

PitStopsModel::PitStopsModel(QObject *parent) :
    QAbstractTableModel(parent), rows(1)
{
}

int PitStopsModel::rowCount(const QModelIndex &) const
{
    return rows;
}

int PitStopsModel::columnCount(const QModelIndex &) const
{
    return 6;
}

void PitStopsModel::update()
{
    getPitstops(EventData::getInstance().getDriversData());
    int ps = pitStops.size();

    if (ps >= rows)
    {
        insertRows(rows, ps - rows);
        rows = ps + 1;
    }
    else if (ps < rows)
    {
        removeRows(ps + 1, rows - ps - 1);
        rows = ps + 1;
    }

    QModelIndex topLeft = QAbstractTableModel::index(0, 0);
    QModelIndex bottomRight = QAbstractTableModel::index(rowCount()-1, columnCount()-1);

    emit dataChanged(topLeft, bottomRight);
}

QVariant PitStopsModel::data(const QModelIndex & index, int role) const
{
    if (index.row() == 0)
        return headerData(index, role);

    if (index.row() > pitStops.size())
        return QVariant();

    PitStopAtom ps = pitStops[index.row() - 1];
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)
                return QString("%1.").arg(index.row());

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::CYAN);

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 1:
            if (role == Qt::DisplayRole)
                return ps.lap;

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::YELLOW);

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 2:
            if (role == Qt::DisplayRole)
                return SeasonData::getInstance().getDriverNo(ps.driver);

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::WHITE);

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 3:
            if (role == Qt::DisplayRole)
                return ps.driver;

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::WHITE);

            return QVariant();

        case 4:
            if (role == Qt::DisplayRole)
                return QString::number(ps.time, 'f', 1);

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::YELLOW);

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);

        case 5:
            if (role == Qt::DisplayRole)
            {
                QString str = QString("%1").arg(ps.pos);
                if (ps.pos > 0)
                    str = "+" + str;

                return str;
            }

            if (role == Qt::ForegroundRole)
            {
                if (ps.pos > 0)
                    return SeasonData::getInstance().getColor(LTPackets::GREEN);

                if (ps.pos < 0)
                    return SeasonData::getInstance().getColor(LTPackets::RED);

                return SeasonData::getInstance().getColor(LTPackets::WHITE);
            }

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);
    }
    return QVariant();
}


QVariant PitStopsModel::headerData(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 1: return "Lap";
            case 3: return "Name";
            case 4: return "Time";
            case 5: return "+/- P";
        }
    }
    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
            case 1: return (int)(Qt::AlignVCenter + Qt::AlignRight);
            case 2: return (int)(Qt::AlignVCenter + Qt::AlignRight);
            case 4: return (int)(Qt::AlignCenter);
            case 5: return (int)(Qt::AlignCenter);
        }
    }
    if (role == Qt::ForegroundRole)
        return SeasonData::getInstance().getColor(LTPackets::DEFAULT);

    return QVariant();
}

void PitStopsModel::getPitstops(const QVector<DriverData> &driversData)
{
    pitStops.clear();
    for (int i = 0; i < driversData.size(); ++i)
    {
        for (int j = 0; j < driversData[i].getPitStops().size(); ++j)
        {
            PitStopAtom pitAtom;
            pitAtom.driver = driversData[i].getDriverName();
            pitAtom.lap = driversData[i].getPitStops()[j].getPitLap();
            pitAtom.time = driversData[i].getPitStops()[j].getPitTime().toDouble();

            LapData ld1 = driversData[i].getLapData(driversData[i].getPitStops()[j].getPitLap());
            LapData ld2 = driversData[i].getLapData(driversData[i].getPitStops()[j].getPitLap()+1);

            pitAtom.pos = 0;
            if (ld1.getCarID() != -1 && ld2.getCarID() != -1)
                pitAtom.pos = ld1.getPosition() - ld2.getPosition();

//            QPair< QPair<double, int>, QString > pitAtom(QPair<double, int>(driversData[i].pitData[j].pitTime.toDouble(), driversData[i].pitData[j].pitLap), driversData[i].driver);
            pitStops.append(pitAtom);
        }
    }
    qSort(pitStops);
}

