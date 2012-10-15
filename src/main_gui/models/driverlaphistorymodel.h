#ifndef DRIVERLAPHISTORYMODEL_H
#define DRIVERLAPHISTORYMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "../../core/eventdata.h"

class DriverLapHistoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DriverLapHistoryModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex())
    {
        beginInsertRows(parent, row, row + count);
        endInsertRows();
        return true;
    }

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex())
    {
        beginRemoveRows(parent, row, row + count - 1);
        endRemoveRows();
        return true;
    }

    QVariant getLapTime(const LapData &, int role) const;
    QVariant getSectorTime(const LapData &, int role, int sector) const;

    QVariant headerData(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void update(DriverData *dd);
    void clear()
    {
        driverData = 0;
        rows = 0;
        reset();
    }
    
signals:
    
public slots:

private:
    DriverData *driverData;
    int rows;
    
};

#endif // DRIVERLAPHISTORYMODEL_H
