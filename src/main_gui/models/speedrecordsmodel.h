#ifndef SPEEDRECORDSMODEL_H
#define SPEEDRECORDSMODEL_H

#include <QAbstractTableModel>

class SpeedRecordsModel : public QAbstractTableModel
{
public:
    SpeedRecordsModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void update();
};

#endif // SPEEDRECORDSMODEL_H
