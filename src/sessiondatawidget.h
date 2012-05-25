#ifndef SESSIONDATAWIDGET_H
#define SESSIONDATAWIDGET_H

#include <QKeyEvent>
#include <QList>
#include <QPair>
#include <QWidget>
#include <QTableWidgetItem>

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

    QTableWidgetItem* setItem(QTableWidget *table, int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                 QColor textColor = LTData::colors[LTData::DEFAULT], QBrush background = QBrush());

    Ui::SessionDataWidget *ui;
    QList<LapData> bestLaps;

    EventData &eventData;
};

#endif // SESSIONDATAWIDGET_H
