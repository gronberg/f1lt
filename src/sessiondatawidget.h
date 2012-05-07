#ifndef SESSIONDATAWIDGET_H
#define SESSIONDATAWIDGET_H

#include <QKeyEvent>
#include <QList>
#include <QPair>
#include <QWidget>

#include "eventdata.h"

namespace Ui {
class SessionDataWidget;
}

class SessionDataWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SessionDataWidget(QWidget *parent = 0);
    ~SessionDataWidget();

    int currentIndex();
    void setCurrentIndex(int);

    void setFont(const QFont &);
    void setupContents();

    void updateData();
    void updateSpeedRecords();
    void updateFastestLaps();
    void updatePitStops(bool clear = false);
    void updateEventInfo();

    void clearData();
    void clearFastestLaps();

    QList< QPair< QPair<double, int>, QString > > getPitstops(const QList<DriverData> &driversData);

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::SessionDataWidget *ui;
    QList<LapData> bestLaps;

    EventData &eventData;
};

#endif // SESSIONDATAWIDGET_H
