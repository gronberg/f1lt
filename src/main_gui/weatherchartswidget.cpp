#include "weatherchartswidget.h"

#include <QDebug>
WeatherChartsWidget::WeatherChartsWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

    SeasonData &sd = SeasonData::getInstance();
    tempWidget = new TempChart(sd.getColor(LTPackets::VIOLET), sd.getColor(LTPackets::YELLOW), 0, 1, 5, this);
    windWidget = new WeatherChart(sd.getColor(LTPackets::GREEN), 2, 5, this);
    pressureWidget = new WeatherChart(sd.getColor(LTPackets::WHITE), 3, 5, this);
    pressureWidget->setAllowedMin(900);
    humidityWidget = new WeatherChart(sd.getColor(LTPackets::RED), 4, 5, this);
    wetDryWidget = new WetDryChart(sd.getColor(LTPackets::CYAN), 5, 5, this);

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
