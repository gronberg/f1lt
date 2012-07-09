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

    QList< PitStopAtom > getPitstops(const QList<DriverData> &driversData);
};

#endif // SESSIONDATAWIDGET_H
