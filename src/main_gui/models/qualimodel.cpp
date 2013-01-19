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


#include "qualimodel.h"

#include "../../core/colorsmanager.h"
#include <QDebug>

QualiModel::QualiModel(QObject *parent) : LTModel(parent), eventData(EventData::getInstance()), qualiPeriodSelected(0)
{
}

int QualiModel::rowCount(const QModelIndex &) const
{
    return eventData.getDriversData().size() + 3;
}

int QualiModel::columnCount(const QModelIndex &) const
{
    return 11;
}

void QualiModel::headerClicked(int column)
{
    int col = column - 3;

    if (col == eventData.getQualiPeriod() || col == 3)//col >= 1 && col <= 2)
    {
        if (qualiPeriodSelected == 0)
            qualiPeriodSelected = col * 10;

        else
            qualiPeriodSelected = 0;

        updateLT();
    }
    else if (col == 1 || col == 2)
    {
        if (qualiPeriodSelected == col)
            qualiPeriodSelected = col * 10;

        else if (qualiPeriodSelected == col * 10)
            qualiPeriodSelected = 0;

        else
            qualiPeriodSelected = col;

        updateLT();
    }
}

void QualiModel::updateLT()
{
    //driversData = QList<DriverData>(EventData::getInstance().getDriversData());
    gatherDriversData();
    int qPeriod = qualiPeriodSelected >= 10 ? qualiPeriodSelected / 10 : qualiPeriodSelected;

    qSort(driversData.begin(), driversData.end(), QualiLessThan(qPeriod));

    QModelIndex topLeft = QAbstractTableModel::index(firstRow(), 0);
    QModelIndex bottomRight = QAbstractTableModel::index(rowCount()-1, columnCount()-1);

    emit dataChanged(topLeft, bottomRight);
}

QVariant QualiModel::data(const QModelIndex &index, int role) const
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

QVariant QualiModel::driverRowData(const DriverData &dd, const QModelIndex &index, int role) const
{
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)                            
                return QString("%1.").arg(index.row());//dd.getPosition());


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
                if (selectedDriver.first != 0 && selectedDriver.second == 4)
                    return gapToSelected(dd);

                if (qualiPeriodSelected == 10 && &dd != driversData.first())
                    return DriverData::calculateGap(dd.getQualiTime(1), driversData.first()->getQualiTime(1));

                return dd.getQualiTime(1).toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().qualiTimeColor(1));
            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

        case 5:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second == 5)
                    return gapToSelected(dd);

                if (qualiPeriodSelected == 20 && &dd != driversData.first())
                    return DriverData::calculateGap(dd.getQualiTime(2), driversData.first()->getQualiTime(2));

                return dd.getQualiTime(2).toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().qualiTimeColor(2));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 6:
            if (role == Qt::DisplayRole)
            {
                if (selectedDriver.first != 0 && selectedDriver.second == 6)
                    return gapToSelected(dd);

                if (qualiPeriodSelected == 30 && &dd != driversData.first())
                    return DriverData::calculateGap(dd.getQualiTime(3), driversData.first()->getQualiTime(3));

                return dd.getQualiTime(3).toString();
            }

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().qualiTimeColor(3));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

            break;

        case 7:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getSectorTime(1).toString();

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().sectorColor(1));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 8:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getSectorTime(2).toString();

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().sectorColor(2));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 9:
            if (role == Qt::DisplayRole)
                return dd.getLastLap().getSectorTime(3).toString();

            if (role == Qt::ForegroundRole)
                return ColorsManager::getInstance().getColor(dd.getColorData().sectorColor(3));

            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;

            break;

        case 10:
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

QVariant QualiModel::headerRowData(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0: return "P";
            case 1: return "";
            case 2: return "";
            case 3: return "Name";
            case 4: return "Q1";
            case 5: return "Q2";
            case 6: return "Q3";
            case 7: return "S1";
            case 8: return "S2";
            case 9: return "S3";
            case 10: return "Laps";
            default: break;
        }
    }
    if (role == Qt::ForegroundRole)
    {
        if (index.column() >= 4 && index.column() <= 6)
        {
            int col = index.column() - 3;
            if (col == qualiPeriodSelected)
                return ColorsManager::getInstance().getColor(LTPackets::WHITE);

            if (col * 10 == qualiPeriodSelected)
                return ColorsManager::getInstance().getColor(LTPackets::YELLOW);
        }
        return ColorsManager::getInstance().getColor(LTPackets::DEFAULT);
    }

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() >= 0 && index.column() <= 1)
            return (int)(Qt::AlignVCenter | Qt::AlignRight);

        if (index.column() >= 4)
            return Qt::AlignCenter;

    }

    return QVariant();
}

QVariant QualiModel::extraRowData(const QModelIndex &index, int role) const
{
    if (index.row() == rowCount()-2)
        return QVariant();

    if (role == Qt::DisplayRole && !driversData.isEmpty() && driversData.first()->getQualiTime(1).isValid())
    {
        switch (index.column())
        {
            case 3: return "Q1 107% time";
            case 4:

                LapTime q1Best = findBestQ1Time();
                if (qualiPeriodSelected == 10)
                    return DriverData::calculateGap(q1Best.calc107p(), q1Best);

                return q1Best.calc107p().toString();
        }
    }
    else if (role == Qt::ForegroundRole)
        return ColorsManager::getInstance().getColor(LTPackets::RED);

    if (role == Qt::TextAlignmentRole && index.column() == 4)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant QualiModel::gapToSelected(const DriverData &dd) const
{
    int qPeriod = selectedDriver.second-3;
    if (selectedDriver.first == dd.getCarID())
        return dd.getQualiTime(qPeriod).toString();

    QString gap = DriverData::calculateGap(dd.getQualiTime(qPeriod), eventData.getDriverDataById(selectedDriver.first).getQualiTime(qPeriod));

    return gap;
}

LapTime QualiModel::findBestQ1Time() const
{
    LapTime bestTime = driversData.first()->getQualiTime(1);
    for (int i = 1; i < driversData.size(); ++i)
    {
        if (driversData[i]->getQualiTime(1) < bestTime)
            bestTime = driversData[i]->getQualiTime(1);
    }
    return bestTime;
}
