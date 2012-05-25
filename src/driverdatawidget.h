#ifndef DRIVERDATAWIDGET_H
#define DRIVERDATAWIDGET_H

#include <QWidget>
#include "ltdata.h"
#include "chartwidget.h"
#include <QKeyEvent>
#include <QPixmap>
#include <QLabel>
#include <QTableWidgetItem>

namespace Ui {
class DriverDataWidget;
}

class DriverDataWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DriverDataWidget(QWidget *parent = 0);
    ~DriverDataWidget();

    void setFont(const QFont &);

    void printDriverData(int driverIdx);
    void printDriverChart(int driverIdx);
    void updateView();

    int currentIndex();
    void setCurrentIndex(int);

    void setReversedOrder(bool rev) { reversedOrder = rev; }
    bool isReversedOrder() { return reversedOrder; }

    void clearData();       

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_tabWidget_currentChanged(int index);

private:

    QTableWidgetItem* setItem(int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                 QColor textColor = LTData::colors[LTData::DEFAULT], QBrush background = QBrush());

    Ui::DriverDataWidget *ui;

    ChartWidget *posChart;
    LapTimeChartWidget *lapTimeChart;
    GapChartWidget *gapChart;   

    int currentDriver;

    bool reversedOrder;     //print lap data rows in reverse?
    EventData &eventData;
};

#endif // DRIVERDATAWIDGET_H
