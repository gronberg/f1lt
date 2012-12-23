#include "driverlaphistorymodel.h"
#include <QDebug>

DriverLapHistoryModel::DriverLapHistoryModel(QObject *parent) :
    QAbstractTableModel(parent), rows(1)
{    
}

int DriverLapHistoryModel::rowCount(const QModelIndex &) const
{
//    int rows = driverData->getLapData().size() + 1;

    return rows; //EventData::getInstance().getDriversData().size()+1;
}

int DriverLapHistoryModel::columnCount(const QModelIndex &) const
{
    return 8;
}

void DriverLapHistoryModel::update(DriverData *dd)
{    
    driverData = dd;
    int laps = dd->getLapData().size();

//    if ((laps+1) > rows)
//    {
//        insertRows(rows, laps + 1 - rows);
//        rows = laps + 1;
//    }
//    else if ((laps+1) < rows)
//    {
//        removeRows(laps + 1, rows - laps - 1);
//        rows = laps + 1;
//    }

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
    if (driverData == 0)
        return QVariant();

    if (index.row() == 0)
        return headerData(index, role);

    if (index.row() > driverData->getLapData().size())
        return QVariant();

    const LapData &ld = driverData->getLapData()[driverData->getLapData().size() - index.row()];
    switch (index.column())
    {
        case 0:
            if (role == Qt::DisplayRole)
                return ld.getLapNumber();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::WHITE);

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 1:
            if (role == Qt::DisplayRole)
                return ld.getPosition();

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::CYAN);

        case 2:
            if (role == Qt::DisplayRole)
            {
                if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
                {
                    if (ld.getPosition() != 1)
                        return ld.getGap();

                    return "";
                }
                else if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
                    return QString("%1 (Q%2)").arg(SeasonData::getInstance().correctQualiTime(ld.getQualiLapExtraData().getSessionTime(), ld.getQualiLapExtraData().getQualiPeriod()).toString("mm:ss")).
                            arg(ld.getQualiLapExtraData().getQualiPeriod());

                else if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
                    return SeasonData::getInstance().correctFPTime(ld.getPracticeLapExtraData().getSessionTime()).toString("h:mm:ss");
            }

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::YELLOW);

            if (role == Qt::TextAlignmentRole)
                return (int)(Qt::AlignVCenter + Qt::AlignRight);

        case 3:
                return getLapTime(ld, role);

        case 4:
            if (role == Qt::DisplayRole)
            {
                if (ld.getTime() != driverData->getSessionRecords().getBestLap().getTime() && ld.getTime().isValid())
                   return DriverData::calculateGap(ld.getTime(), driverData->getSessionRecords().getBestLap().getTime());

                return "";
            }

            if (role == Qt::ForegroundRole)
                return SeasonData::getInstance().getColor(LTPackets::YELLOW);

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
    QColor color = SeasonData::getInstance().getColor(LTPackets::WHITE);
    QString s = ld.getTime().toString();

    if (ld.getLapNumber() == driverData->getSessionRecords().getBestLap().getLapNumber())
        color = SeasonData::getInstance().getColor(LTPackets::GREEN);

    if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getFastestLap().getLapNumber() &&
        driverData->getDriverName() == EventData::getInstance().getSessionRecords().getFastestLap().getDriverName())
        color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

    if (ld.getTime().toString() == "IN PIT")
    {
        color = SeasonData::getInstance().getColor(LTPackets::PIT);
        s = QString("IN PIT (%1)").arg(driverData->getPitTime(ld.getLapNumber()));
    }

    else if (ld.getTime().toString() == "RETIRED" || ld.getTime().toString().contains("LAP"))
        color = SeasonData::getInstance().getColor(LTPackets::PIT);

    else if (ld.getRaceLapExtraData().isSCLap())
        color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

    if ((EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT && ld.getPracticeLapExtraData().isApproxLap()) ||
        (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT && ld.getQualiLapExtraData().isApproxLap()))
        color = SeasonData::getInstance().getColor(LTPackets::CYAN);


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
    QColor color = SeasonData::getInstance().getColor(LTPackets::WHITE);
    QString s = ld.getSectorTime(sector).toString();

    if (ld.getLapNumber() == driverData->getSessionRecords().getBestSectorLapNumber(sector))
    {
        color = SeasonData::getInstance().getColor(LTPackets::GREEN);
    }

    if (ld.getLapNumber() == EventData::getInstance().getSessionRecords().getSectorRecord(sector).getLapNumber() &&
        driverData->getDriverName() == EventData::getInstance().getSessionRecords().getSectorRecord(sector).getDriverName())
        color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

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
                if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
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
        return SeasonData::getInstance().getColor(LTPackets::DEFAULT);

    return QVariant();
}
