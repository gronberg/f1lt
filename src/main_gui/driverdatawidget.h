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



#ifndef DRIVERDATAWIDGET_H
#define DRIVERDATAWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QPixmap>
#include <QLabel>
#include <QTableWidgetItem>


#include "../core/seasondata.h"
#include "../charts/driverdatachart.h"
#include "../net/packetparser.h"

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

    void clearData();       

public slots:
    void updateDriverData(int id)
    {
        if (id == currentDriver)
            printDriverData(currentDriver);
    }

    void updateDriverData(const DataUpdates &dataUpdates)
    {
        if (dataUpdates.driverIds.contains(currentDriver))
            printDriverData(currentDriver);
    }


    void printDriverData(int id);
    void printDriverChart(int id);
    void printDriverRecords(int id);
    void printDriverRelatedCommentary(int id);

    void updateDriverInfo(const DriverData &);

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_tabWidget_currentChanged(int index);

private:

    Ui::DriverDataWidget *ui;

    DriverDataChart *posChart;
    LapTimeChart *lapTimeChart;
    GapChart *gapChart;

    int currentDriver;

    EventData &eventData;

    DriverLapHistoryModel *driverLapHistoryModel;    
};

#endif // DRIVERDATAWIDGET_H
