#include "driverlaphistorymodel.h"
#include <QDebug>

DriverLapHistoryModel::DriverLapHistoryModel(QObject *parent) :
    QAbstractTableModel(parent), rows(1)
{    
}

int DriverLapHistoryModel::rowCount(const QModelIndex &) const
{
//    int rows = driverData.getLapData().size() + 1;
//    qDebug() << "rows=" << rows;

    return rows; //EventData::getInstance().getDriversData().size()+1;
}

int DriverLapHistoryModel::columnCount(const QModelIndex &) const
{
    return 8;
}

void DriverLapHistoryModel::update(const DriverData &dd)
{    
    driverData = dd;
    int laps = dd.getLapData().size();    

    if (laps >= rows)
    {
        insertRows(rows, laps - rows);
        rows = laps + 1;
    }
    else if (laps < rows)
    {
        removeRows(laps + 1, rows - laps - 1);
        rows = laps + 1;
    }

    QModelIndex topLeft = QAbstractTableModel::index(0, 0);
    QModelIndex bottomRight = QAbstractTableModel::index(rowCount()-1, columnCount()-1);

    emit dataChanged(topLeft, bottomRight);
}

QVariant DriverLapHistoryModel::data(const QModelIndex & index, int role) const
{
    if (index.row() == 0)
        return headerData(index, role);

    if (index.row() > driverData.getLapData().size())
        return QVariant();

    LapData ld = driverData.getLapData()[driverData.getLapData().size() - index.row()];
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)
                return ld.getLapNumber();

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::WHITE];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 1:
            if (role == Qt::DisplayRole)
                return ld.getPosition();

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::CYAN];

        case 2:
            if (role == Qt::DisplayRole)
            {
                if (EventData::getInstance().getEventType() == LTData::RACE_EVENT)
                {
                    if (ld.getPosition() != 1)
                        return ld.getGap();

                    return "";
                }
                else if (EventData::getInstance().getEventType() == LTData::QUALI_EVENT)
                    return QString("%1 (Q%2)").arg(LTData::correctQualiTime(ld.getQualiLapExtraData().getSessionTime(), ld.getQualiLapExtraData().getQualiPeriod()).toString("mm:ss")).
                            arg(ld.getQualiLapExtraData().getQualiPeriod());

                else if (EventData::getInstance().getEventType() == LTData::PRACTICE_EVENT)
                    return LTData::correctFPTime(ld.getPracticeLapExtraData().getSessionTime()).toString("h:mm:ss");
            }

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::YELLOW];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 3:
                return getLapTime(ld, role);

        case 4:
            if (role == Qt::DisplayRole)
            {
                if (ld.getTime() != driverData.getSessionRecords().getBestLap().getTime() && ld.getTime().isValid())
                   return DriverData::calculateGap(ld.getTime(), driverData.getSessionRecords().getBestLap().getTime());

                return "";
            }

            if (role == Qt::ForegroundRole)
                return LTData::colors[LTData::YELLOW];

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 5:
            return getSectorTime(ld, role, 1);

        case 6:
            return getSectorTime(ld, role, 2);

        case 7:
            return getSectorTime(ld, role, 3);
    }
    return QVariant();
}

QVariant DriverLapHistoryModel::getLapTime(const LapData &ld, int role) const
{
    QColor color = LTData::colors[LTData::WHITE];
    QString s = ld.getTime().toString();

    if (ld.getLapNumber() == driverData.getSessionRecords().getBestLap().getLapNumber())
        color = LTData::colors[LTData::GREEN];

    if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getFastestLap().getLapNumber() &&
        driverData.getDriverName() == EventData::getInstance().getSessionRecords().getFastestLap().getDriverName())
        color = LTData::colors[LTData::VIOLET];

    if (ld.getTime().toString() == "IN PIT")
    {
        color = LTData::colors[LTData::PIT];
        s = QString("IN PIT (%1)").arg(driverData.getPitTime(ld.getLapNumber()));
    }

    if (ld.getTime().toString() == "RETIRED")
        color = LTData::colors[LTData::PIT];

    if ((EventData::getInstance().getEventType() == LTData::PRACTICE_EVENT && ld.getPracticeLapExtraData().isApproxLap()) ||
        (EventData::getInstance().getEventType() == LTData::QUALI_EVENT && ld.getQualiLapExtraData().isApproxLap()))
        color = LTData::colors[LTData::CYAN];

    if (role == Qt::DisplayRole)
        return s;

    if (role == Qt::ForegroundRole)
        return color;

    if (role == Qt::TextAlignmentRole)
        return (int)(Qt::AlignCenter);

    return QVariant();
}
QVariant DriverLapHistoryModel::getSectorTime(const LapData &ld, int role, int sector) const
{
    QColor color = LTData::colors[LTData::WHITE];
    QString s = ld.getSectorTime(sector).toString();

    if (ld.getLapNumber() == driverData.getSessionRecords().getBestSectorLapNumber(sector))
    {
        color = LTData::colors[LTData::GREEN];
    }

    if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getSectorRecord(sector).getLapNumber() &&
        driverData.getDriverName() == EventData::getInstance().getSessionRecords().getSectorRecord(sector).getDriverName())
        color = LTData::colors[LTData::VIOLET];

    if (role == Qt::DisplayRole)
        return s;

    if (role == Qt::ForegroundRole)
        return color;

    if (role == Qt::TextAlignmentRole)
        return (int)(Qt::AlignCenter);

    return QVariant();
}

QVariant DriverLapHistoryModel::headerData(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0: return "Lap";
            case 1: return "P";
            case 2:
                if (EventData::getInstance().getEventType() == LTData::RACE_EVENT)
                    return "Gap";
                return "S. time";
            case 3: return "Time";
            case 4: return "Diff.";
            case 5: return "S1";
            case 6: return "S2";
            case 7: return "S3";
        }
    }
    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
            case 0: return (int)(Qt::AlignVCenter + Qt::AlignRight);
            case 1: return (int)(Qt::AlignVCenter + Qt::AlignRight);
            case 2: return (int)(Qt::AlignVCenter + Qt::AlignRight);
            case 3: return (int)(Qt::AlignCenter);
            case 4: return (int)(Qt::AlignVCenter + Qt::AlignRight);
            case 5: return (int)(Qt::AlignCenter);
            case 6: return (int)(Qt::AlignCenter);
            case 7: return (int)(Qt::AlignCenter);
        }
    }
    if (role == Qt::ForegroundRole)
        return LTData::colors[LTData::DEFAULT];

    return QVariant();
}
