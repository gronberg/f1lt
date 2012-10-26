#ifndef PITSTOPSMODEL_H
#define PITSTOPSMODEL_H

#include <QAbstractTableModel>

#include "../../core/eventdata.h"

class PitStopsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PitStopsModel(QObject *parent = 0);

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

    QVariant headerData(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void update();

signals:
    
public slots:

private:
    int rows;

    struct PitStopAtom
    {
        double time;
        int lap;
        QString driver;
        int pos;

        bool operator <(const PitStopAtom &psa) const
        {
            return time < psa.time;
        }
    };

    void getPitstops(const QVector<DriverData> &driversData);
    QList< PitStopAtom > pitStops;
    
};

#endif // PITSTOPSMODEL_H
