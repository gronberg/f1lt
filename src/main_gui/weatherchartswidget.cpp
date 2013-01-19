/*******************************************************************************
 *                                                                             *
 *   F1LT - unofficial Formula 1 live timing application                       *
 *   Copyright (C) 2012-2013  Mariusz Pilarek (pieczaro@gmail.com)             *
 *                                                                             *
 *   This program is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation, either version 3 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                             *
 *******************************************************************************/


#include "weatherchartswidget.h"

#include "../core/colorsmanager.h"

#include <QDebug>
WeatherChartsWidget::WeatherChartsWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

    ColorsManager &sd = ColorsManager::getInstance();
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
