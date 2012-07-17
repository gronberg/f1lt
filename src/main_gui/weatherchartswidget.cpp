#include "weatherchartswidget.h"

#include <QDebug>
WeatherChartsWidget::WeatherChartsWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

    tempWidget = new TempChart(LTData::colors[LTData::VIOLET], LTData::colors[LTData::YELLOW], 0, 1, 5, this);
    windWidget = new WeatherChart(LTData::colors[LTData::GREEN], 2, 5, this);
    pressureWidget = new WeatherChart(LTData::colors[LTData::WHITE], 3, 5, this);
    humidityWidget = new WeatherChart(LTData::colors[LTData::RED], 4, 5, this);
    wetDryWidget = new WetDryChart(LTData::colors[LTData::CYAN], 5, 5, this);

	ui.tempLayout->addWidget(tempWidget);
	ui.windLayout->addWidget(windWidget);
	ui.pressureLayout->addWidget(pressureWidget);
	ui.humidityLayout->addWidget(humidityWidget);
	ui.wetDryLayout->addWidget(wetDryWidget);
}

WeatherChartsWidget::~WeatherChartsWidget()
{

}

void WeatherChartsWidget::updateCharts()
{
	switch (ui.tabWidget->currentIndex())
	{
		case 0: tempWidget->repaint(); break;
		case 1: windWidget->repaint(); break;
		case 2: pressureWidget->repaint(); break;
		case 3: humidityWidget->repaint(); break;
		case 4: wetDryWidget->repaint(); break;
	}
}
