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



#ifndef SESSIONDATAWIDGET_H
#define SESSIONDATAWIDGET_H

#include <QKeyEvent>
#include <QList>
#include <QPair>
#include <QWidget>
#include <QTableWidgetItem>

#include "../core/colorsmanager.h"
#include "../core/eventdata.h"
#include "models/fastestlapsmodel.h"
#include "models/pitstopsmodel.h"
#include "models/speedrecordsmodel.h"


namespace Ui {
class SessionDataWidget;
}

class SessionDataWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SessionDataWidget(QWidget *parent = 0);
    ~SessionDataWidget();

    int currentIndex();
    void setCurrentIndex(int);

    void setFont(const QFont &);
    void setupContents();

    void updateData();
    void updateSpeedRecords();
    void updateFastestLaps();
    void updatePitStops(bool clear = false);
    void updateEventInfo();

    void resizeTrackMap();


    void clearData();
    void clearFastestLaps();


protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_tabWidget_currentChanged(int index);

private:

    QTableWidgetItem* setItem(QTableWidget *table, int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                 QColor textColor = ColorsManager::getInstance().getColor(LTPackets::DEFAULT), QBrush background = QBrush());

    Ui::SessionDataWidget *ui;    

    EventData &eventData;

    SpeedRecordsModel speedRecordsModel;
    FastestLapsModel fastestLapsModel;
    PitStopsModel pitStopsModel;

    LTEvent currentEvent;
};

#endif // SESSIONDATAWIDGET_H
