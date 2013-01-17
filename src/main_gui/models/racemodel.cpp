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


#include "racemodel.h"

#include <QPixmap>

RaceModel::RaceModel(QObject *parent) : LTModel(parent), eventData(EventData::getInstance())
{
    arrowIcons[0] = QIcon(QPixmap(":/track/up-arrow.png").scaledToHeight(10, Qt::SmoothTransformation));
    arrowIcons[1] = QIcon(QPixmap(":/track/down-arrow.png").scaledToHeight(10, Qt::SmoothTransformation));
}

int RaceModel::rowCount(const QModelIndex &) const
{
    return eventData.getDriversData().size() + 3;
}

int RaceModel::columnCount(const QModelIndex &) const
{
    return 11;
}

QVariant RaceModel::data(const QModelIndex &index, int role) const
{
    int row = index.row()-1;

    if (row >= 0 && row < driversData.size() && driversData[row] != 0)
    {
        DriverData &dd = *driversData[row];//eventData.getDriverDataByPos(row);
        return driverRowData(dd, index, role);

    }
    else if (index.row() == 0)
        return headerRowData(index, role);

    else
        return extraRowData(index, role);
}

QVariant RaceModel::driverRowData(const DriverData &dd, const QModelIndex &index, int role) const
{
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole && dd.getPosition() > 0 && !dd.isRetired())
            {
                return QString("%1.").arg(dd.getPosition());
            }

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().positionColor());

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);

            break;

        case 1:
            if (role == Qt::DisplayRole && dd.getPosition() > 0)
                return dd.getNumber();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().numberColor());

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);

            if (role == Qt::DecorationRole)
                return getIcon(dd);

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
                return SeasonData::getInstance().getColor(dd.getColorData().driverColor());

            break;

        case 4:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getGap();


            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().gapColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

        case 5:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second == 5)
                    return gapToSelected(dd);

                return dd.getLastLap().getInterval();
            }

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().intervalColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 6:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second == 6 && dd.getLastLap().getTime().isValid())
                    return gapToSelected(dd);

                return dd.getLastLap().getTime().toString();
            }

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().lapTimeColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            if (role == Qt::ToolTipRole)
                return "Best lap: " + dd.getSessionRecords().getBestLap().getTime().toString() + QString(" (L%1)").arg(dd.getSessionRecords().getBestLap().getLapNumber());

            break;

            break;

        case 7:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getSectorTime(1).toString();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().sectorColor(1));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 8:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getSectorTime(2).toString();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().sectorColor(2));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 9:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getSectorTime(3).toString();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().sectorColor(3));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 10:
            if (role == Qt::DisplayRole && dd.getNumPits() > 0)
                return dd.getNumPits();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(dd.getColorData().numLapsColor());

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;
    }
    return QVariant();
}

QVariant RaceModel::headerRowData(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0: return "P";
            case 1: return "";
            case 2: return "";
            case 3: return "Name";
            case 4: return "Gap";
            case 5: return "Interval";
            case 6: return "Time";
            case 7: return "S1";
            case 8: return "S2";
            case 9: return "S3";
            case 10: return "Pit";
            default: break;
        }
    }
    if (role == Qt::ForegroundRole)
        return SeasonData::getInstance().getColor(LTPackets::DEFAULT);

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() >= 0 && index.column() <= 1)
            return (int)(Qt::AlignVCenter | Qt::AlignRight);

        if (index.column() >= 4)
            return Qt::AlignCenter;

    }

    return QVariant();
}

QVariant RaceModel::extraRowData(const QModelIndex &index, int role) const
{
    if (index.row() == rowCount()-2)
        return QVariant();

    if (role == Qt::DisplayRole && eventData.getSessionRecords().getFastestLap().getDriverName() != "")
    {
        switch (index.column())
        {
            case 1: return "FL:";
            case 3: return eventData.getSessionRecords().getFastestLap().getDriverName();
            case 4: return "lap";
            case 5: return eventData.getSessionRecords().getFastestLap().getLapNumber();
            case 6: return eventData.getSessionRecords().getFastestLap().getTime().toString();
        }
    }
    else if (role == Qt::ForegroundRole && (index.column() == 3 || index.column() == 5 || index.column() == 6))
        return SeasonData::getInstance().getColor(LTPackets::VIOLET);

    else if (role == Qt::ForegroundRole && (index.column() == 1 || index.column() == 4))
        return SeasonData::getInstance().getColor(LTPackets::DEFAULT);

    if (role == Qt::TextAlignmentRole && index.column() == 1)
        return (int)(Qt::AlignVCenter | Qt::AlignRight);

    if (role == Qt::TextAlignmentRole && (index.column() >= 4 && index.column() <= 6))
        return Qt::AlignCenter;

    return QVariant();
}

QVariant RaceModel::gapToSelected(const DriverData &dd) const
{
    DriverData currD = eventData.getDriverDataById(selectedDriver.first);

    if (selectedDriver.first == dd.getCarID() && selectedDriver.second == 5)
        return "";

    else if (selectedDriver.second == 6 && (selectedDriver.first == dd.getCarID() || !currD.getLastLap().getTime().isValid() || !dd.getLastLap().getTime().isValid()))
    {        
        return dd.getLastLap().getTime().toString();
    }

    if (selectedDriver.second == 5)
    {
        return eventData.calculateInterval(dd, currD, -1);//EventData.getInstance().lapsCompleted);//.driversData.get(currentCar-1).lastLap.numLap);
    }
    if (selectedDriver.second == 6)
    {
        return DriverData::calculateGap(dd.getLastLap().getTime(), currD.getLastLap().getTime());
    }

    return "";
}

QVariant RaceModel::getIcon(const DriverData &dd) const
{
    if (!dd.isRetired() && dd.getNumber() > 0)
    {
        int lastPos = 0, prevPos = 0;
        if (dd.getLapData().size() > 1)
        {
            lastPos = dd.getLastLap().getPosition();
            prevPos = dd.getLapData()[dd.getLapData().size()-2].getPosition();
        }
        else if (!dd.getPositionHistory().isEmpty())
        {
            lastPos = dd.getLastLap().getPosition();
            prevPos = dd.getPositionHistory()[dd.getLastLap().getLapNumber() == 1 ? 0 : dd.getPositionHistory().size()-1];
        }
        if (lastPos != prevPos)
            return (lastPos < prevPos ? arrowIcons[0] : arrowIcons[1]);
    }
    return QVariant();
}
