#include "weatherchartswidget.h"

#include <QDebug>
WeatherChartsWidget::WeatherChartsWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	tempWidget = new TempChartWidget(LTData::colors[LTData::VIOLET], LTData::colors[LTData::YELLOW], 0, 1, 5, this);
	windWidget = new WeatherChartWidget(LTData::colors[LTData::GREEN], 2, 5, this);
	pressureWidget = new WeatherChartWidget(LTData::colors[LTData::RED], 3, 5, this);
	humidityWidget = new WeatherChartWidget(LTData::colors[LTData::CYAN], 4, 5, this);

	ui.tempLayout->addWidget(tempWidget);
	ui.windLayout->addWidget(windWidget);
	ui.pressureLayout->addWidget(pressureWidget);
	ui.humidityLayout->addWidget(humidityWidget);
}

WeatherChartsWidget::~WeatherChartsWidget()
{

}

void WeatherChartsWidget::updateCharts()
{
	tempWidget->repaint();
	windWidget->repaint();
	pressureWidget->repaint();
	humidityWidget->repaint();
}
