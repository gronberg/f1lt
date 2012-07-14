#ifndef WEATHERCHARTSWIDGET_H
#define WEATHERCHARTSWIDGET_H

#include <QtGui/QWidget>
#include "ui_weatherchartswidget.h"

#include "chartwidget.h"

class WeatherChartsWidget : public QWidget
{
    Q_OBJECT

public:
    WeatherChartsWidget(QWidget *parent = 0);
    ~WeatherChartsWidget();

    void updateCharts();

private:
    Ui::WeatherChartsWidgetClass ui;

    TempChartWidget *tempWidget;
    WeatherChartWidget *windWidget;
    WeatherChartWidget *pressureWidget;
    WeatherChartWidget *humidityWidget;
    WetDryChartWidget *wetDryWidget;

};

#endif // WEATHERCHARTSWIDGET_H
