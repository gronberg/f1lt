#ifndef DRIVERINFOLABEL_H
#define DRIVERINFOLABEL_H

#include <QWidget>
#include "../core/eventdata.h"

class DriverInfoLabel : public QWidget
{
    Q_OBJECT
public:
    explicit DriverInfoLabel(QWidget *parent = 0);
    QSize sizeHint();
    QSize minimumSize();
    
signals:
    
public slots:
    void setDriver(const DriverData *dd)
    {
        driverData = dd;
    }

    void update();
    void clear()
    {
        driverData = 0;
    }

protected:
    void paintEvent(QPaintEvent *);

private:
    const DriverData *driverData;
    QPixmap backgroundPixmap;
    
};

#endif // DRIVERINFOLABEL_H
