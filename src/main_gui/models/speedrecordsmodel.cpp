#include "speedrecordsmodel.h"

#include "../../core/eventdata.h"
#include <QModelIndex>

SpeedRecordsModel::SpeedRecordsModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int SpeedRecordsModel::rowCount(const QModelIndex &) const
{
    return 15;
}

int SpeedRecordsModel::columnCount(const QModelIndex &) const
{
    return 5;
}

void SpeedRecordsModel::update()
{
    QModelIndex topLeft = QAbstractTableModel::index(0, 0);
    QModelIndex bottomRight = QAbstractTableModel::index(rowCount()-1, columnCount()-1);

    emit dataChanged(topLeft, bottomRight);
}

QVariant SpeedRecordsModel::data(const QModelIndex & index, int role) const
{
    if (index.row() == 0 || index.row() == 8)
        return headerData(index, role);

    if (index.row() == 7)
        return QVariant();

    int row = index.row() - 1;
    int sector = index.column() < 2 ? 1 : 2;
    if (index.row() > 8)
    {
        row = index.row() - 9;
        sector = index.column() < 2 ? 3 : 4;
    }

    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)
                return EventData::getInstance().getSessionRecords().getSectorSpeed(sector, row).getDriverName();

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::WHITE];

            return QVariant();

        case 1:
            if (role == Qt::DisplayRole)
                return EventData::getInstance().getSessionRecords().getSectorSpeed(sector, row).getSpeed();

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::YELLOW];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);

            return QVariant();

        case 3:
            if (role == Qt::DisplayRole)
            {
                if (sector == 2)
                    return EventData::getInstance().getSessionRecords().getSectorSpeed(sector, row).getDriverName();

                return EventData::getInstance().getSessionRecords().getSpeedTrap(row).getDriverName();
            }

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::WHITE];

            return QVariant();

        case 4:
            if (role == Qt::DisplayRole)
            {
                if (sector == 2)
                    return EventData::getInstance().getSessionRecords().getSectorSpeed(sector, row).getSpeed();

                return EventData::getInstance().getSessionRecords().getSpeedTrap(row).getSpeed();
            }

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::YELLOW];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);
    }
    return QVariant();
}

QVariant SpeedRecordsModel::headerData(const QModelIndex & index, int role) const
{
    QString s1 = "Sector 1";
    QString s2 = "Sector 2";
    if (index.row() == 8)
    {
        s1 = "Sector 3";
        s2 = "Speed trap";
    }

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0: return s1;
            case 1: return "KPH";
            case 3: return s2;
            case 4: return "KPH";
        }
    }
    if (role == Qt::ForegroundRole)
        return LTData::colors[LTData::DEFAULT];

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() == 1 || index.column() == 4)
            return (int)(Qt::AlignVCenter | Qt::AlignRight);
    }

    return QVariant();
}
