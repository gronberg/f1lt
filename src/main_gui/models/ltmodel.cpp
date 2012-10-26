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
