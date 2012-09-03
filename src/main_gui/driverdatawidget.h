#ifndef DRIVERDATAWIDGET_H
#define DRIVERDATAWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QPixmap>
#include <QLabel>
#include <QTableWidgetItem>


#include "../core/seasondata.h"
#include "../charts/chartwidget.h"

#include "models/driverlaphistorymodel.h"

namespace Ui {
class DriverDataWidget;
}

class DriverDataWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DriverDataWidget(QWidget *parent = 0);
    ~DriverDataWidget();

    void setFont(const QFont &, const QFont &);

    void updateView();

    int currentIndex();
    void setCurrentIndex(int);

    void setReversedOrder(bool rev) { reversedOrder = rev; }
    bool isReversedOrder() { return reversedOrder; }

    void clearData();       

public slots:
    void updateDriverData()
    {
        printDriverData(currentDriver);
    }

    void printDriverData(int id);
    void printDriverChart(int id);
    void printDriverMainEvents(int id);

    void updateDriverInfo(const DriverData &);

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_tabWidget_currentChanged(int index);

private:

    Ui::DriverDataWidget *ui;

    ChartWidget *posChart;
    LapTimeChart *lapTimeChart;
    GapChart *gapChart;

    int currentDriver;

    bool reversedOrder;     //print lap data rows in reverse?
    EventData &eventData;

    DriverLapHistoryModel *driverLapHistoryModel;
};

#endif // DRIVERDATAWIDGET_H
