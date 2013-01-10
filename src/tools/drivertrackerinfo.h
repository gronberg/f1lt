#ifndef DRIVERTRACKERINFO_H
#define DRIVERTRACKERINFO_H

#include <QWidget>

#include "../core/driverdata.h"

class DriverTrackerInfo : public QWidget
{
    Q_OBJECT
public:
    explicit DriverTrackerInfo(QWidget *parent = 0);

    void setDriverData(DriverData *dd)
    {
        driverData = dd;
        SeasonData::getInstance().getCarThumbnailsFactory().loadCarThumbnails(240, false);

        repaint();
    }

    void setup();

    void paintDriverInfo(QPainter *p);
    void paintLapsInfo(QPainter *p);
    void paintGapsInfo(QPainter *p);

protected:
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:

private:
    QPixmap labelBig;
    QPixmap labelInfoLong;

    DriverData *driverData;
    
};

#endif // DRIVERTRACKERINFO_H
