#ifndef LTMODEL_H
#define LTMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QList>
#include <QPixmap>
#include "../../core/eventdata.h"

class LessThan
{
public:
    bool operator()(DriverData *d1, DriverData *d2)
    {
        if (d1->getPosition() <= 0)
            return false;

        if (d2->getPosition() <= 0)
            return true;

        return (d1->getPosition() < d2->getPosition());
    }
};



class LTModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LTModel(QObject *parent = 0);

    virtual void updateLT();


    virtual const DriverData *getDriverData(const QModelIndex &index) const;
    virtual bool indexInDriverRowsData(const QModelIndex &index) const;
    virtual int firstRow() const
    {
        return 1;
    }
    virtual int lastRow() const
    {
        return EventData::getInstance().getDriversData().size();
    }
    virtual int driverRows() const
    {
        return EventData::getInstance().getDriversData().size();
    }
    virtual QVariant driverRowData(const DriverData &, const QModelIndex &index, int role) const = 0;
    virtual QVariant headerRowData(const QModelIndex &index, int role) const = 0;
    virtual QVariant extraRowData(const QModelIndex &index, int role) const = 0;
    virtual QModelIndex indexOf(int driverId);

    virtual bool selectDriver(int id, int column)
    {
        if (selectedDriver.first == id)
        {
            if (selectedDriver.second == column)
                selectedDriver.second = 0;
            else
                selectedDriver.second = column;
        }
        else
        {
            selectedDriver.first = id;
            selectedDriver.second = column;
        }
        return true;
    }
    virtual void headerClicked(int)
    {

    }
    void clearData()
    {
        driversData.clear();
    }
    
signals:
    
public slots:

protected:
    virtual void gatherDriversData();
    QList<DriverData *> driversData;

    QPair<int, int> selectedDriver;     //first - driver id, second - column code
    
};

#endif // LTMODEL_H
