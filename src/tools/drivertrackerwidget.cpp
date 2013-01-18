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


#include "drivertrackerwidget.h"
#include "ui_drivertrackerwidget.h"

#include <QKeyEvent>

DriverTrackerWidget::DriverTrackerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriverTrackerWidget), speed(1)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui->driverTracker, SIGNAL(driverExcluded(int,bool)), ui->driverRadar, SLOT(excludeDriver(int,bool)));
    connect(ui->driverTracker, SIGNAL(driverSelected(int)), ui->driverRadar, SLOT(selectDriver(int)));
}

DriverTrackerWidget::~DriverTrackerWidget()
{
    delete ui;
}

void DriverTrackerWidget::update()
{
    ui->driverRadar->update();
    ui->driverTracker->update();
}

void DriverTrackerWidget::setup()
{    
    ui->driverRadar->setupDrivers(speed);
    ui->driverTracker->setupDrivers(speed);
}

void DriverTrackerWidget::loadSettings(QSettings *settings)
{
    ui->splitter->restoreState(settings->value("ui/tracker_splitter_pos").toByteArray());
    restoreGeometry(settings->value("ui/driver_tracker_geometry").toByteArray());
    ui->driverTracker->setDrawDriverClassification(settings->value("ui/draw_tracker_classification").toBool());
}

void DriverTrackerWidget::saveSettings(QSettings *settings)
{
    settings->setValue("ui/tracker_splitter_pos", ui->splitter->saveState());
    settings->setValue("ui/driver_tracker_geometry", saveGeometry());
    settings->setValue("ui/draw_tracker_classification", ui->driverTracker->drawDriverClassification());
}

void DriverTrackerWidget::on_pushButton_clicked()
{
    close();
}

void DriverTrackerWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        close();
}

void DriverTrackerWidget::exec()
{
    setWindowTitle("Driver tracker: " + EventData::getInstance().getEventInfo().eventName);

    ui->driverTracker->checkSetupCorrect(speed);
    ui->driverRadar->checkSetupCorrect(speed);

    show();
}

void DriverTrackerWidget::drawTrackerClassification(bool val)
{
    ui->driverTracker->setDrawDriverClassification(val);
}

void DriverTrackerWidget::startTimer(int s)
{
    if (timer->isActive())
        setTimerInterval(s);

    else
    {
        ui->driverTracker->checkSetupCorrect(speed);
        ui->driverRadar->checkSetupCorrect(speed);

        interval = s / speed;
        timer->start(interval);
    }
}
