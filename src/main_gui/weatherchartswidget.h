#ifndef WEATHERCHARTSWIDGET_H
#define WEATHERCHARTSWIDGET_H

#include <QtGui/QWidget>
#include "ui_weatherchartswidget.h"

#include "../charts/weatherchart.h"

class WeatherChartsWidget : public QWidget
{
    Q_OBJECT

public:
    WeatherChartsWidget(QWidget *parent = 0);
    ~WeatherChartsWidget();

    void updateCharts();

private:
    Ui::WeatherChartsWidgetClass ui;

    TempChart *tempWidget;
    WeatherChart *windWidget;
    WeatherChart *pressureWidget;
    WeatherChart *humidityWidget;
    WetDryChart *wetDryWidget;

};

#endif // WEATHERCHARTSWIDGET_H
