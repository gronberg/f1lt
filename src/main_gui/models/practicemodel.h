#ifndef PRACTICEMODEL_H
#define PRACTICEMODEL_H

#include "ltmodel.h"
#include "../../core/eventdata.h"

class PracticeModel : public LTModel
{
public:
    PracticeModel(QObject *parent = 0);

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
        if (column == 0 || column == 4)
        {
            updateLT();
            return true;
        }

        return false;
    }

private:
    EventData &eventData;
};

#endif // PRACTICEMODEL_H
