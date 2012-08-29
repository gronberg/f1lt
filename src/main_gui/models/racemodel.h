#ifndef RACEMODEL_H
#define RACEMODEL_H

#include <QIcon>

#include "ltmodel.h"
#include "../../core/eventdata.h"

class RaceModel : public LTModel
{
public:
    RaceModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    virtual QVariant driverRowData(const DriverData &dd, const QModelIndex &index, int role) const;
    virtual QVariant headerRowData(const QModelIndex &index, int role) const;
    virtual QVariant extraRowData(const QModelIndex &index, int role) const;

    QVariant gapToSelected(const DriverData &dd) const;

    virtual bool selectDriver(int id, int column)
    {
        LTModel::selectDriver(id, column);
        if (column == 0 || column == 5 || column == 6)
        {
            updateLT();
            return true;
        }

        return false;
    }

    QVariant getIcon(const DriverData &) const;

private:
    EventData &eventData;
    QIcon arrowIcons[2];
};

#endif // RACEMODEL_H
