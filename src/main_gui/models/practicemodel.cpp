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


#include "practicemodel.h"

#include "../../core/colorsmanager.h"

PracticeModel::PracticeModel(QObject *parent) : LTModel(parent), eventData(EventData::getInstance())
{
}


int PracticeModel::rowCount(const QModelIndex &) const
{
    return eventData.getDriversData().size() + 3;
}

int PracticeModel::columnCount(const QModelIndex &) const
{
    return 10;
}

QVariant PracticeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row()-1;

    if (row >= 0 && row < driversData.size())
    {
        DriverData &dd = *driversData[row];//eventData.getDriverDataByPos(row);
        return driverRowData(dd, index, role);

    }
    else if (index.row() == 0)
        return headerRowData(index, role);

    else
        return extraRowData(index, role);
}

QVariant PracticeModel::driverRowData(const DriverData &dd, const QModelIndex &index, int role) const
{
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)
                return QString("%1.").arg(dd.getPosition());

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().positionColor());

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);

            break;

        case 1:
            if (role == Qt::DisplayRole)
                return dd.getNumber();

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().numberColor());

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);

            break;

        case 2:
//            if (role == Qt::DecorationRole)
//                return getCarImage(dd);

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 3:
            if (role == Qt::DisplayRole)
                return dd.getDriverName();

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().driverColor());

            break;

        case 4:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second >= 4)
                    return gapToSelected(dd, 4);

                return dd.getLastLap().getTime().toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().lapTimeColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

        case 5:
            if (role == Qt::DisplayRole)
            {                
                if (dd.getPosition() == 1)
                    return "";

                return dd.getLastLap().getGap();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().gapColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 6:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second >= 4 && dd.getLastLap().getSectorTime(1).isValid())
                    return gapToSelected(dd, 6);

                return dd.getLastLap().getSectorTime(1).toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().sectorColor(1));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 7:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second >= 4 && dd.getLastLap().getSectorTime(2).isValid())
                    return gapToSelected(dd, 7);

                return dd.getLastLap().getSectorTime(2).toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().sectorColor(2));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 8:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second >= 4 && dd.getLastLap().getSectorTime(3).isValid())
                    return gapToSelected(dd, 8);

                return dd.getLastLap().getSectorTime(3).toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().sectorColor(3));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 9:
            if (role == Qt::DisplayRole && dd.getLastLap().getLapNumber() > 0)
                return dd.getLastLap().getLapNumber();

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().numLapsColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;
    }
    return QVariant();
}

QVariant PracticeModel::headerRowData(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0: return "P";
            case 1: return "";
            case 2: return "";
            case 3: return "Name";
            case 4: return "Best";
            case 5: return "Gap";
            case 6: return "S1";
            case 7: return "S2";
            case 8: return "S3";
            case 9: return "Laps";
            default: break;
        }
    }
    if (role == Qt::ForegroundRole)
        return ColorsManager::getInstance().getColor(LTPackets::DEFAULT);

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() >= 0 && index.column() <= 1)
            return (int)(Qt::AlignVCenter | Qt::AlignRight);

        if (index.column() >= 4)
            return Qt::AlignCenter;

    }

    return QVariant();
}

QVariant PracticeModel::extraRowData(const QModelIndex &index, int role) const
{
    if (index.row() == rowCount()-2)
        return QVariant();

    if (role == Qt::DisplayRole && !eventData.getDriverDataByPos(1).getLapData().isEmpty() && !driversData.isEmpty())
    {
        switch (index.column())
        {
            case 3: return "107% time";
            case 4: return driversData.first()->getSessionRecords().getBestLap().getTime().calc107p().toString();//eventData.getDriverDataByPos(1).getSessionRecords().getBestLap().getTime().calc107p().toString();
            case 5: return DriverData::calculateGap(driversData.first()->getSessionRecords().getBestLap().getTime().calc107p(), driversData.first()->getSessionRecords().getBestLap().getTime());
        }
    }
    else if (role == Qt::ForegroundRole)
        return ColorsManager::getInstance().getColor(LTPackets::RED);

    if (role == Qt::TextAlignmentRole && ((index.column() == 4) || (index.column() == 5)))
        return Qt::AlignCenter;

    return QVariant();
}

QVariant PracticeModel::gapToSelected(const DriverData &dd, int column) const
{        
    DriverData currD = eventData.getDriverDataById(selectedDriver.first);

    if (column == 4)
    {
        if ((selectedDriver.first == dd.getCarID()) || !currD.getLastLap().getTime().isValid())
            return dd.getLastLap().getTime().toString();

        QString gap = DriverData::calculateGap(dd.getLastLap().getTime(), currD.getLastLap().getTime());

        if (gap.size() > 0 && gap[0] != '-')
            gap = "+" + gap;

        return gap;
    }

    if (column > 5)
    {
        int sector = column - 5;

        if ((selectedDriver.first == dd.getCarID()) || !currD.getLastLap().getSectorTime(sector).isValid())
            return dd.getLastLap().getSectorTime(sector).toString();

        else
        {
            QString gap = DriverData::calculateGap(dd.getLastLap().getSectorTime(sector), currD.getLastLap().getSectorTime(sector));

            if ((gap.size() > 0) && (gap[0] != '-') && (gap != "0.000"))
                gap = "+" + gap;

            return gap.left(gap.size()-2);
        }
    }

    return "";
}
