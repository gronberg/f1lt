#ifndef FASTESTLAPSMODEL_H
#define FASTESTLAPSMODEL_H

#include <QAbstractTableModel>

#include "../../core/eventdata.h"

class FastestLapsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FastestLapsModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void update();
    
signals:
    
public slots:

private:
    QList<LapData> fastestLaps;
    
};

#endif // FASTESTLAPSMODEL_H
