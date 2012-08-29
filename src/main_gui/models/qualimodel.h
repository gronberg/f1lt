#ifndef QUALIMODEL_H
#define QUALIMODEL_H

#include "ltmodel.h"
#include "../../core/eventdata.h"

class QualiLessThan
{
public:
    QualiLessThan(int period) : qPeriod(period) { }
    bool operator()(DriverData *d1, DriverData *d2)
    {        
        if (qPeriod > 0 && d1->getQualiTime(qPeriod).isValid() && d2->getQualiTime(qPeriod).isValid())
        {
            if (d1->getQualiTime(qPeriod) == d2->getQualiTime(qPeriod))
                return (d1->getPosition() < d2->getPosition());

            return (d1->getQualiTime(qPeriod) < d2->getQualiTime(qPeriod));
        }

//        if (qPeriod == 0 && EventData::getInstance().isSessionStarted() && d1->getLastLap().getTime().isValid() && d2->getLastLap().getTime().isValid())
//            return d1->getLastLap().getTime() < d2->getLastLap().getTime();

        return (d1->getPosition() < d2->getPosition());
    }

private:
    int qPeriod;
};

class QualiModel : public LTModel
{
public:
    QualiModel(QObject *parent = 0);

    virtual void updateLT();

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
        if (column == 0 || column == 4 || column == 5 || column == 6)
        {
            updateLT();
            return true;
        }

        return false;
    }
    LapTime findBestQ1Time() const;

    virtual void headerClicked(int column);

private:
    EventData &eventData;
    int qualiPeriodSelected;
};

#endif // QUALIMODEL_H
