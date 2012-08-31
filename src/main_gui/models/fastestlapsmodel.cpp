#include "fastestlapsmodel.h"

FastestLapsModel::FastestLapsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int FastestLapsModel::rowCount(const QModelIndex &) const
{
    return EventData::getInstance().getDriversData().size() + 1;
}

int FastestLapsModel::columnCount(const QModelIndex &) const
{
    return 8;
}

void FastestLapsModel::update()
{
    EventData &ed = EventData::getInstance();
    for (int i = 0; i < ed.getDriversData().size(); ++i)
    {
        DriverData &dd = ed.getDriversData()[i];
        if (fastestLaps.size() <= i)
            fastestLaps.append(dd.getSessionRecords().getBestLap());
        else
            fastestLaps[i] = dd.getSessionRecords().getBestLap();
    }
    qSort(fastestLaps);

    QModelIndex topLeft = QAbstractTableModel::index(0, 0);
    QModelIndex bottomRight = QAbstractTableModel::index(rowCount()-1, columnCount()-1);

    emit dataChanged(topLeft, bottomRight);
}

QVariant FastestLapsModel::data(const QModelIndex & index, int role) const
{
    if (index.row() == 0)
        return headerData(index, role);

    LapData ld = fastestLaps[index.row() - 1];
    DriverData dd = EventData::getInstance().getDriverDataById(ld.getCarID());
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)
            {
                if (ld.getTime().isValid())
                    return QString("%1.").arg(index.row());
            }

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::CYAN];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);

            return QVariant();

        case 1:
            if (role == Qt::DisplayRole)
                return dd.getDriverName();

            if (role == Qt::ForegroundRole)
            {
                if (index.row() == 1)
                    return LTData::colors[LTData::VIOLET];

                return LTData::colors[LTData::WHITE];
            }
            return QVariant();

        case 2:
            if (role == Qt::DisplayRole)
                return ld.getTime().toString();

            if (role == Qt::ForegroundRole)
            {
                if (index.row() == 1)
                    return LTData::colors[LTData::VIOLET];

                return LTData::colors[LTData::GREEN];
            }
            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);

            return QVariant();

        case 3:
            if (role == Qt::DisplayRole && index.row() != 1)
                return DriverData::calculateGap(ld.getTime(), fastestLaps.first().getTime());


            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::YELLOW];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);

            return QVariant();

        case 4:
            if (role == Qt::DisplayRole)
                return ld.getSectorTime(1).toString();

            if (role == Qt::ForegroundRole)
            {
                if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getSectorRecord(1).getLapNumber() &&
                    dd.getDriverName() == EventData::getInstance().getSessionRecords().getSectorRecord(1).getDriverName())
                    return LTData::colors[LTData::VIOLET];


                if (ld.getLapNumber() == dd.getSessionRecords().getBestSectorLapNumber(1))
                    return LTData::colors[LTData::GREEN];

                return LTData::colors[LTData::WHITE];
            }

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);

        case 5:
            if (role == Qt::DisplayRole)
                return ld.getSectorTime(2).toString();

            if (role == Qt::ForegroundRole)
            {
                if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getSectorRecord(2).getLapNumber() &&
                    dd.getDriverName() == EventData::getInstance().getSessionRecords().getSectorRecord(2).getDriverName())
                    return LTData::colors[LTData::VIOLET];

                if (ld.getLapNumber() == dd.getSessionRecords().getBestSectorLapNumber(2))
                    return LTData::colors[LTData::GREEN];                

                return LTData::colors[LTData::WHITE];
            }

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);

        case 6:
            if (role == Qt::DisplayRole)
                return ld.getSectorTime(3).toString();

            if (role == Qt::ForegroundRole)
            {
                if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getSectorRecord(3).getLapNumber() &&
                    dd.getDriverName() == EventData::getInstance().getSessionRecords().getSectorRecord(3).getDriverName())
                    return LTData::colors[LTData::VIOLET];

                if (ld.getLapNumber() == dd.getSessionRecords().getBestSectorLapNumber(3))
                    return LTData::colors[LTData::GREEN];                

                return LTData::colors[LTData::WHITE];
            }

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignCenter);

        case 7:
            if (role == Qt::DisplayRole)
            {
                if (ld.getTime().isValid())
                {
                    if (EventData::getInstance().getEventType() == LTData::QUALI_EVENT)
                        return QString("%1 (Q%2)").arg(ld.getLapNumber()).arg(ld.getQualiLapExtraData().getQualiPeriod());

                    return ld.getLapNumber();
                }
            }

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::WHITE];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter | Qt::AlignRight);
    }
    return QVariant();
}

QVariant FastestLapsModel::headerData(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0: return "P";
            case 1: return "Name";
            case 2: return "Time";
            case 3: return "Gap";
            case 4: return "S1";
            case 5: return "S2";
            case 6: return "S3";
            case 7: return "Lap";
        }
    }
    if (role == Qt::ForegroundRole)
        return LTData::colors[LTData::DEFAULT];

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() >= 2 && index.column() <= 6)
            return (int)(Qt::AlignCenter);

        if (index.column() == 0 || index.column() == 7)
            return (int)(Qt::AlignVCenter | Qt::AlignRight);
    }

    return QVariant();
}
