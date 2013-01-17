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


#include "sessionanalysiswidget.h"

#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QTableWidgetItem>

#include "../main_gui/ltitemdelegate.h"


SessionAnalysisWidget::SessionAnalysisWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

    for (int i = 0; i < ui.gridLayout->columnCount(); i += 2)
    {
        QLabel *lab = (QLabel*)ui.gridLayout->itemAtPosition(0, i)->widget();
        QCheckBox *box = (QCheckBox*)ui.gridLayout->itemAtPosition(0, i+1)->widget();

        box->setChecked(true);
        driverLabels.append(lab);
        driverCheckBoxes.append(box);

        lab = (QLabel*)ui.gridLayout->itemAtPosition(1, i)->widget();
        box = (QCheckBox*)ui.gridLayout->itemAtPosition(1, i+1)->widget();

        box->setChecked(true);
        driverLabels.append(lab);
        driverCheckBoxes.append(box);
    }

    setupTables();
    ui.lapTimeTableWidget->setItemDelegate(new LTItemDelegate);
    ui.lapTimeTableWidgetFP->setItemDelegate(new LTItemDelegate);
    ui.lapTimeTableWidgetQuali->setItemDelegate(new LTItemDelegate);
    ui.lapTimeTableWidgetQ1->setItemDelegate(new LTItemDelegate);
    ui.lapTimeTableWidgetQ2->setItemDelegate(new LTItemDelegate);
    ui.lapTimeTableWidgetQ3->setItemDelegate(new LTItemDelegate);

    setupColors();



    ui.sessionLapTimesChartQ1->setQualiPeriod(1);


    ui.sessionLapTimesChartQ2->setQualiPeriod(2);

    ui.sessionLapTimesChartQ3->setQualiPeriod(3);
    selected = true;
//    ui.splitter->refresh();
//    QList<int> sizes;
//    sizes << 100 << 400;
//    ui.splitter->setSizes(sizes);
//    ui.lapTimeTableWidget->setGeometry(ui.lapTimeTableWidget->x(), ui.lapTimeTableWidget->y(), sizes[0], ui.lapTimeTableWidget->height());
//    ui.sessionLapTimesChart->setGeometry(ui.sessionLapTimesChart->x(), ui.sessionLapTimesChart->y(), 500, ui.sessionLapTimesChart->height());

    connect(ui.sessionLapTimesChart, SIGNAL(zoomChanged(int,int,double,double)), this, SLOT(onZoomChanged(int,int,double,double)));
    connect(ui.sessionLapTimesChartFP, SIGNAL(zoomChanged(int,int,double,double)), this, SLOT(onZoomChanged(int,int,double,double)));
    connect(ui.sessionLapTimesChartQuali, SIGNAL(zoomChanged(int,int,double,double)), this, SLOT(onZoomChanged(int,int,double,double)));
    connect(ui.sessionLapTimesChartQ1, SIGNAL(zoomChanged(int,int,double,double)), this, SLOT(onZoomChanged(int,int,double,double)));
    connect(ui.sessionLapTimesChartQ2, SIGNAL(zoomChanged(int,int,double,double)), this, SLOT(onZoomChanged(int,int,double,double)));
    connect(ui.sessionLapTimesChartQ3, SIGNAL(zoomChanged(int,int,double,double)), this, SLOT(onZoomChanged(int,int,double,double)));

    top10only = false;
    first = 0;
    last = 99;
    min = -1;
    max = -1;
}

SessionAnalysisWidget::~SessionAnalysisWidget()
{

}

void SessionAnalysisWidget::resetView()
{
    lapDataArray.clear();
    setWindowTitle("Session analysis: " + EventData::getInstance().getEventInfo().eventName);
    setupBoxes();
    switch(EventData::getInstance().getEventType())
    {
        case LTPackets::RACE_EVENT:
            ui.raceTabWidget->setVisible(true);
            ui.fpTabWidget->setVisible(false);
            ui.qualiTabWidget->setVisible(false);
            break;

        case LTPackets::PRACTICE_EVENT:
            ui.raceTabWidget->setVisible(false);
            ui.fpTabWidget->setVisible(true);
            ui.qualiTabWidget->setVisible(false);
        break;

        case LTPackets::QUALI_EVENT:
            ui.raceTabWidget->setVisible(false);
            ui.fpTabWidget->setVisible(false);
            ui.qualiTabWidget->setVisible(true);
            break;

        default:
            ui.raceTabWidget->setVisible(true);
            ui.fpTabWidget->setVisible(false);
            ui.qualiTabWidget->setVisible(false);
            break;

    }
    ui.sessionLapTimesChart->resetZoom();
    ui.sessionPositionsChart->resetZoom();
    ui.sessionGapsChart->resetZoom();
    ui.sessionLapTimesChartFP->resetZoom();

    first = 0;
    last = 99;
    min = -1;
    max = -1;

    update();
}

void SessionAnalysisWidget::resizeTables()
{
    int w = ui.lapTimeTableWidget->width()-20;//sizes[0]-20;
    ui.lapTimeTableWidget->setColumnWidth(0, 0.15*w);
    ui.lapTimeTableWidget->setColumnWidth(1, 0.35*w);
    ui.lapTimeTableWidget->setColumnWidth(2, 0.2*w);
    ui.lapTimeTableWidget->setColumnWidth(3, 0.2*w);
    ui.lapTimeTableWidget->setColumnWidth(4, 0.1*w);    

    w = ui.lapTimeTableWidgetFP->width()-20;//sizes[0]-20;
    ui.lapTimeTableWidgetFP->setColumnWidth(0, 0.15*w);
    ui.lapTimeTableWidgetFP->setColumnWidth(1, 0.32*w);
    ui.lapTimeTableWidgetFP->setColumnWidth(2, 0.18*w);
    ui.lapTimeTableWidgetFP->setColumnWidth(3, 0.17*w);
    ui.lapTimeTableWidgetFP->setColumnWidth(4, 0.18*w);

    w = ui.lapTimeTableWidgetQuali->width()-20;//sizes[0]-20;
    ui.lapTimeTableWidgetQuali->setColumnWidth(0, 0.1*w);
    ui.lapTimeTableWidgetQuali->setColumnWidth(1, 0.3*w);
    ui.lapTimeTableWidgetQuali->setColumnWidth(2, 0.17*w);
    ui.lapTimeTableWidgetQuali->setColumnWidth(3, 0.16*w);
    ui.lapTimeTableWidgetQuali->setColumnWidth(4, 0.17*w);
    ui.lapTimeTableWidgetQuali->setColumnWidth(5, 0.1*w);

    w = ui.lapTimeTableWidgetQ1->width()-20;//sizes[0]-20;
    ui.lapTimeTableWidgetQ1->setColumnWidth(0, 0.15*w);
    ui.lapTimeTableWidgetQ1->setColumnWidth(1, 0.32*w);
    ui.lapTimeTableWidgetQ1->setColumnWidth(2, 0.18*w);
    ui.lapTimeTableWidgetQ1->setColumnWidth(3, 0.17*w);
    ui.lapTimeTableWidgetQ1->setColumnWidth(4, 0.18*w);

    w = ui.lapTimeTableWidgetQ2->width()-20;//sizes[0]-20;
    ui.lapTimeTableWidgetQ2->setColumnWidth(0, 0.15*w);
    ui.lapTimeTableWidgetQ2->setColumnWidth(1, 0.32*w);
    ui.lapTimeTableWidgetQ2->setColumnWidth(2, 0.18*w);
    ui.lapTimeTableWidgetQ2->setColumnWidth(3, 0.17*w);
    ui.lapTimeTableWidgetQ2->setColumnWidth(4, 0.18*w);

    w = ui.lapTimeTableWidgetQ3->width()-20;//sizes[0]-20;
    ui.lapTimeTableWidgetQ3->setColumnWidth(0, 0.15*w);
    ui.lapTimeTableWidgetQ3->setColumnWidth(1, 0.32*w);
    ui.lapTimeTableWidgetQ3->setColumnWidth(2, 0.18*w);
    ui.lapTimeTableWidgetQ3->setColumnWidth(3, 0.17*w);
    ui.lapTimeTableWidgetQ3->setColumnWidth(4, 0.18*w);
}

void SessionAnalysisWidget::setupTables()
{
    if (ui.lapTimeTableWidget->rowCount() == 0)
        ui.lapTimeTableWidget->insertRow(0);

//    QList<int> sizes = ui.splitter->sizes();

    SeasonData &sd = SeasonData::getInstance();
    setItem(ui.lapTimeTableWidget, 0, 0, "P", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidget, 0, 1, "Driver", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidget, 0, 2, "Time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidget, 0, 3, "Gap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidget, 0, 4, "Lap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));

    ui.lapTimeTableWidget->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetFP->rowCount() == 0)
        ui.lapTimeTableWidgetFP->insertRow(0);

    setItem(ui.lapTimeTableWidgetFP, 0, 0, "P", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetFP, 0, 1, "Driver", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetFP, 0, 2, "Time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetFP, 0, 3, "Gap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetFP, 0, 4, "S. time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));

    ui.lapTimeTableWidgetQuali->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQuali->rowCount() == 0)
        ui.lapTimeTableWidgetQuali->insertRow(0);

    setItem(ui.lapTimeTableWidgetQuali, 0, 0, "P", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQuali, 0, 1, "Driver", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQuali, 0, 2, "Time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQuali, 0, 3, "Gap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQuali, 0, 4, "S. time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQuali, 0, 5, "Period", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));

    ui.lapTimeTableWidgetQuali->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQ1->rowCount() == 0)
        ui.lapTimeTableWidgetQ1->insertRow(0);

    setItem(ui.lapTimeTableWidgetQ1, 0, 0, "P", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ1, 0, 1, "Driver", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ1, 0, 2, "Time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ1, 0, 3, "Gap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ1, 0, 4, "S. time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));

    ui.lapTimeTableWidgetQ1->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQ2->rowCount() == 0)
        ui.lapTimeTableWidgetQ2->insertRow(0);

    setItem(ui.lapTimeTableWidgetQ2, 0, 0, "P", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ2, 0, 1, "Driver", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ2, 0, 2, "Time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ2, 0, 3, "Gap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ2, 0, 4, "S. time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));

    ui.lapTimeTableWidgetQ2->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQ3->rowCount() == 0)
        ui.lapTimeTableWidgetQ3->insertRow(0);

    setItem(ui.lapTimeTableWidgetQ3, 0, 0, "P", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ3, 0, 1, "Driver", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ3, 0, 2, "Time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ3, 0, 3, "Gap", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));
    setItem(ui.lapTimeTableWidgetQ3, 0, 4, "S. time", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::DEFAULT));

    ui.lapTimeTableWidgetQ3->setRowHeight(0, 22);

    resizeTables();
}

void SessionAnalysisWidget::setupColors()
{
    for (int i = 0; i < ui.gridLayout->columnCount(); i += 2)
    {
        QLabel *lab = (QLabel*)ui.gridLayout->itemAtPosition(0, i)->widget();

        QPalette palette = lab->palette();
        palette.setColor(QPalette::Window, SeasonData::getInstance().getDriverColors()[i]);
        lab->setPalette(palette);

        lab = (QLabel*)ui.gridLayout->itemAtPosition(1, i)->widget();

        palette = lab->palette();
        palette.setColor(QPalette::Window, SeasonData::getInstance().getDriverColors()[i+1]);
        lab->setPalette(palette);
    }
    setupIcons(SeasonData::getInstance().getDriverColors());

    update();
}

void SessionAnalysisWidget::setupIcons(const QList<QColor> &colors)
{
    driverIcons.clear();
    for (int i = 0; i < colors.size(); ++i)
    {
        QPixmap pix(10, 22);
        QPainter p;
        p.begin(&pix);

        p.setPen(colors[i]);
        p.setBrush(colors[i]);
        p.drawRect(0, 0, 10, 22);
        p.end();

        driverIcons.append(QPair<int, QIcon>(i+1, QIcon(pix)));
    }
}

void SessionAnalysisWidget::exec()
{
    setWindowTitle("Session analysis: " + EventData::getInstance().getEventInfo().eventName);
    setupBoxes();
    switch(EventData::getInstance().getEventType())
    {
    case LTPackets::RACE_EVENT:
        ui.raceTabWidget->setVisible(true);
        ui.fpTabWidget->setVisible(false);
        ui.qualiTabWidget->setVisible(false);
        break;

    case LTPackets::PRACTICE_EVENT:
        ui.raceTabWidget->setVisible(false);
        ui.fpTabWidget->setVisible(true);
        ui.qualiTabWidget->setVisible(false);
        break;

    case LTPackets::QUALI_EVENT:
        ui.raceTabWidget->setVisible(false);
        ui.fpTabWidget->setVisible(false);
        ui.qualiTabWidget->setVisible(true);
        break;

    default:
        ui.raceTabWidget->setVisible(true);
        ui.fpTabWidget->setVisible(false);
        ui.qualiTabWidget->setVisible(false);
        break;
    }

	update();
	show();
}

void SessionAnalysisWidget::setupBoxes()
{
    QStringList driverList = SeasonData::getInstance().getDriversListShort();
    for (int i = 0; i < driverCheckBoxes.size() && i < driverList.size(); ++i)
    {
        QString str = driverList[i];
        int idx = str.indexOf(" ");
        if (idx != 0)
            driverIcons[i].first = str.left(idx).toInt();

        driverCheckBoxes[i]->setVisible(true);
        driverLabels[i]->setVisible(true);
        driverCheckBoxes[i]->setText(driverList[i]);
    }

    for (int i = driverList.size(); i < driverCheckBoxes.size(); ++i)
    {
        driverLabels[i]->setVisible(false);
        driverCheckBoxes[i]->setVisible(false);
    }
}

void SessionAnalysisWidget::selectDriversClicked()
{
    selected = !selected;
	for (int i = 0; i < driverCheckBoxes.size(); ++i)
        driverCheckBoxes[i]->setChecked(selected);

    top10only = false;
    ui.top10pushButton->setChecked(top10only);

    update();
}

void SessionAnalysisWidget::on_buttonBox_clicked(QAbstractButton *)
{
    close();
}


QTableWidgetItem* SessionAnalysisWidget::setItem(QTableWidget *table, int row, int col, QString text, Qt::ItemFlags flags, int align,
             QColor textColor, QBrush background)
{
    QTableWidgetItem *item = table->item(row, col);
    if (!item)
    {
        item = new QTableWidgetItem(text);
        item->setFlags(flags);
        table->setItem(row, col, item);
    }
    item->setTextAlignment(align);
    item->setBackground(background);
    item->setText(text);
    item->setTextColor(textColor);

    return item;
}

void SessionAnalysisWidget::gatherData()
{
//    lapDataArray.clear();
    int lapsIn = 0;
    for (int i = 0; i < EventData::getInstance().getDriversData().size(); ++i)
    {
        DriverData &dd = EventData::getInstance().getDriversData()[i];

        if (driverChecked(dd.getNumber()))
        {
            for (int j = 0; j < dd.getLapData().size(); ++j)
            {
                if (lapsIn >= lapDataArray.size())
                    lapDataArray.append(dd.getLapData()[j]);
                else
                    lapDataArray[lapsIn] = dd.getLapData()[j];

                ++lapsIn;

            }
        }
    }
    for (int i = lapDataArray.size()-1; i >= lapsIn; --i)
        lapDataArray.removeAt(i);

    qSort(lapDataArray);
}

bool SessionAnalysisWidget::lapInWindow(int j)
{
    bool inTimeWindow = true;

    if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
        inTimeWindow = lapDataArray[j].getLapNumber() >= first && lapDataArray[j].getLapNumber() <= last;

    else if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
    {
        int minute = SeasonData::getInstance().getFPLength() - SeasonData::getInstance().timeToMins(lapDataArray[j].getPracticeLapExtraData().getSessionTime());
        inTimeWindow = minute >= first && minute <= last;
    }
    else if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
    {
        int qPeriod = ui.qualiTabWidget->currentIndex();
        if (qPeriod == 0)
        {
            int minute = SeasonData::getInstance().getQualiLength(lapDataArray[j].getQualiLapExtraData().getQualiPeriod()) -
                    SeasonData::getInstance().timeToMins(lapDataArray[j].getQualiLapExtraData().getSessionTime());

            for (int k = 0; k < lapDataArray[j].getQualiLapExtraData().getQualiPeriod()-1; ++k)
                minute += SeasonData::getInstance().getQualiLength(k+1);

            inTimeWindow = (minute >= first && minute <= last);
        }
        else
        {
            int sessLength = SeasonData::getInstance().getQualiLength(qPeriod);
            int minute = sessLength - SeasonData::getInstance().timeToMins(lapDataArray[j].getQualiLapExtraData().getSessionTime());
            inTimeWindow = (minute >= first && minute <= last) && lapDataArray[j].getQualiLapExtraData().getQualiPeriod() == qPeriod;
        }
    }
    if (inTimeWindow)// &&
    {
        if (((lapDataArray[j].getTime().isValid() && lapDataArray[j].getTime().toDouble() >= min) || min == -1) &&
             ((lapDataArray[j].getTime().isValid() && lapDataArray[j].getTime().toDouble() <= max) || max == -1))
            return true;


        if ((EventData::getInstance().getEventType() == LTPackets::RACE_EVENT) &&
             (!lapDataArray[j].getTime().isValid() && min != -1 && max != -1))
        {
            DriverData &dd = EventData::getInstance().getDriversData()[lapDataArray[j].getCarID()-1];
            LapData ld = dd.getLapData(lapDataArray[j].getLapNumber()-1);

            if (ld.getCarID() == dd.getCarID())
            {
                if (ld.getTime().isValid() && ld.getTime().toDouble() <= max && ld.getTime().toDouble() >= min)
                    return true;
            }
            ld = dd.getLapData(lapDataArray[j].getLapNumber()+1);
            if (ld.getCarID() == dd.getCarID())
            {
                if (ld.getTime().isValid() && ld.getTime().toDouble() <= max && ld.getTime().toDouble() >= min)
                    return true;
            }
        }
    }
    return false;
}

void SessionAnalysisWidget::update(bool repaintCharts)
{
    gatherData();

    int rows = 0;

    QTableWidget *table = ui.lapTimeTableWidget;
    if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
        table = ui.lapTimeTableWidgetFP;

    if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
    {
        switch (ui.qualiTabWidget->currentIndex())
        {
            case 0: table = ui.lapTimeTableWidgetQuali; break;
            case 1: table = ui.lapTimeTableWidgetQ1; break;
            case 2: table = ui.lapTimeTableWidgetQ2; break;
            case 3: table = ui.lapTimeTableWidgetQ3; break;
        }
    }

    int firstPlace = 0;
	for (int i = 0; i < lapDataArray.size(); ++i)
	{
        if (lapInWindow(i))
        {
            if (rows == 0)
                firstPlace = i;
            if (rows + 1 >= table->rowCount())
            {
                table->insertRow(rows+1);
                table->setRowHeight(rows+1, 22);
            }

            if (lapDataArray[i].getCarID() < 0)
                continue;

            SeasonData &sd = SeasonData::getInstance();
            setItem(table, rows+1, 0, QString::number(rows+1)+".", Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::CYAN));

            QString s = EventData::getInstance().getDriversData()[lapDataArray[i].getCarID()-1].getDriverName();
            QTableWidgetItem *item = setItem(table, rows+1, 1, s, Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignLeft | Qt::AlignVCenter, sd.getColor(LTPackets::WHITE));
            item->setIcon(getDriverIcon(EventData::getInstance().getDriversData()[lapDataArray[i].getCarID()-1].getNumber()));


            setItem(table, rows+1, 2, lapDataArray[i].getTime().toString(), Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignCenter, sd.getColor(LTPackets::WHITE));

            s = (rows == 0 || !lapDataArray[i].getTime().isValid()) ? "" : "+" + DriverData::calculateGap(lapDataArray[i].getTime(), lapDataArray[firstPlace].getTime());
            setItem(table, rows+1, 3, s, Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::YELLOW));

            s = QString::number(lapDataArray[i].getLapNumber());
            if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
                s = sd.correctFPTime(lapDataArray[i].getPracticeLapExtraData().getSessionTime()).toString("h:mm:ss");//lapDataArray[i].sessionTime.toString("h:mm:ss") + " (" + QString::number(LTPackets::currentEventFPLength()-LTPackets::timeToMins(lapDataArray[i].sessionTime))+")";

            else if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
                s = sd.correctQualiTime(lapDataArray[i].getQualiLapExtraData().getSessionTime(), lapDataArray[i].getQualiLapExtraData().getQualiPeriod()).toString("mm:ss");

            setItem(table, rows+1, 4, s, Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::WHITE));

            if (ui.qualiTabWidget->currentIndex() == 0)
                setItem(table, rows+1, 5, QString::number(lapDataArray[i].getQualiLapExtraData().getQualiPeriod()), Qt::ItemIsEnabled | Qt::ItemIsSelectable, Qt::AlignRight | Qt::AlignVCenter, sd.getColor(LTPackets::WHITE));

            ++rows;
        }
	}
    if (rows < table->rowCount()-1)
    {
        for (int i = table->rowCount()-1; i > rows; --i)
            table->removeRow(i);
    }

    if (repaintCharts)
    {
        if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
        {
            ui.sessionLapTimesChart->setData(lapDataArray);
            ui.sessionLapTimesChart->update();

            ui.sessionPositionsChart->setData(lapDataArray);
            ui.sessionPositionsChart->update();

            ui.sessionGapsChart->setData(lapDataArray);
            ui.sessionGapsChart->update();
        }
        if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
        {
            ui.sessionLapTimesChartFP->setData(lapDataArray);
            ui.sessionLapTimesChartFP->update();
        }
        if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
        {
            switch (ui.qualiTabWidget->currentIndex())
            {
                case 0:
                    ui.sessionLapTimesChartQuali->setData(lapDataArray);
                    ui.sessionLapTimesChartQuali->update();
                break;

                case 1:
                    ui.sessionLapTimesChartQ1->setData(lapDataArray);
                    ui.sessionLapTimesChartQ1->update();
                    break;

                case 2:
                    ui.sessionLapTimesChartQ2->setData(lapDataArray);
                    ui.sessionLapTimesChartQ2->update();
                    break;

                case 3:
                    ui.sessionLapTimesChartQ3->setData(lapDataArray);
                    ui.sessionLapTimesChartQ3->update();
                    break;
            }
        }
    }
}

void SessionAnalysisWidget::on_pushButton_2_clicked()
{
//    for (int i = ui.lapTimeTableWidget->rowCount(); i > 0; --i)
//        ui.lapTimeTableWidget->removeRow(i);

    top10only = false;
    ui.top10pushButton->setChecked(top10only);

//    lapDataArray.clear();
    update();
}

bool SessionAnalysisWidget::driverChecked(int no)
{
    for (int i = 0; i < driverCheckBoxes.size(); ++i)
    {
        QString str = driverCheckBoxes[i]->text();
        int drvNo = -1;
        int idx = str.indexOf(" ");
        if (idx != 0)
            drvNo = str.left(idx).toInt();

        if (drvNo == no)//LTPackets::getDriverShortName(driver) == driverCheckBoxes[i]->text())
        {
            if (driverCheckBoxes[i]->isChecked())
                return true;
            return false;
        }
    }
    return false;
}

void SessionAnalysisWidget::setDriverChecked(int no, bool checked)
{
    for (int i = 0; i < driverCheckBoxes.size(); ++i)
    {
        QString str = driverCheckBoxes[i]->text();
        int drvNo = -1;
        int idx = str.indexOf(" ");
        if (idx != 0)
            drvNo = str.left(idx).toInt();

        if (drvNo == no)//LTPackets::getDriverShortName(driver) == driverCheckBoxes[i]->text())
        {
            driverCheckBoxes[i]->setChecked(checked);
        }
    }
}

QIcon SessionAnalysisWidget::getDriverIcon(int no)
{
    for (int i = 0; i < driverIcons.size(); ++i)
    {
        if (driverIcons[i].first == no)
            return driverIcons[i].second;
    }
    return QIcon();
}

void SessionAnalysisWidget::resizeEvent(QResizeEvent *e)
{
    resizeTables();

    QWidget::resizeEvent(e);
}

void SessionAnalysisWidget::keyPressEvent(QKeyEvent *k)
{
    if (k->key() == Qt::Key_Escape)
        close();

    if (k->key() == Qt::Key_C && k->modifiers() == Qt::ControlModifier)
    {
        QTableWidget *table = ui.lapTimeTableWidget;
        if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
            table = ui.lapTimeTableWidgetFP;

        if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
        {
            switch (ui.qualiTabWidget->currentIndex())
            {
                case 0: table = ui.lapTimeTableWidgetQuali; break;
                case 1: table = ui.lapTimeTableWidgetQ1; break;
                case 2: table = ui.lapTimeTableWidgetQ2; break;
                case 3: table = ui.lapTimeTableWidgetQ3; break;
            }
        }
        QItemSelectionModel * selection = table->selectionModel();
        QModelIndexList indexes = selection->selectedIndexes();

        if(indexes.size() < 1)
            return;

        // QModelIndex::operator < sorts first by row, then by column.
        // this is what we need
        qSort(indexes.begin(), indexes.end());

        // You need a pair of indexes to find the row changes
        QModelIndex previous = indexes.first();
        indexes.removeFirst();
        QString selected_text;
        QModelIndex current;
        Q_FOREACH(current, indexes)
        {
            QVariant data = table->model()->data(previous);
            QString text = data.toString();

            selected_text.append(text);

            if (current.row() != previous.row())
            {
                selected_text.append(QLatin1Char('\n'));
            }
            else
            {
                selected_text.append(QLatin1Char('\t'));
            }
            previous = current;
        }
        selected_text.append(table->model()->data(current).toString());
        selected_text.append(QLatin1Char('\n'));
        qApp->clipboard()->setText(selected_text);

    }

    QWidget::keyPressEvent(k);
}

void SessionAnalysisWidget::on_top10pushButton_clicked()
{
    top10only = !top10only;
    ui.top10pushButton->setChecked(top10only);

    if (top10only)
    {
        for (int k = 0; k < EventData::getInstance().getDriversData().size(); ++k)
        {
            if (EventData::getInstance().getDriversData()[k].getPosition() <= 10)
                setDriverChecked(EventData::getInstance().getDriversData()[k].getNumber(), true);

            else
                setDriverChecked(EventData::getInstance().getDriversData()[k].getNumber(), false);
        }
    }
    else
    {
        for (int i = 0; i < driverCheckBoxes.size(); ++i)
            driverCheckBoxes[i]->setChecked(true);
    }
    update();
}

void SessionAnalysisWidget::onZoomChanged(int first, int last, double min,double max)
{
    this->first = first;
    this->last = last;
    this->min = min;
    this->max = max;

    update();
}


void SessionAnalysisWidget::saveSettings(QSettings &settings)
{
    settings.setValue("ui/session_analysis_geometry", saveGeometry());
    settings.setValue("ui/session_analysis_splitter", ui.splitter->saveState());
    settings.setValue("ui/session_analysis_tab", ui.raceTabWidget->currentIndex());
    settings.setValue("ui/session_analysis_quali_tab", ui.qualiTabWidget->currentIndex());
    settings.setValue("ui/session_analysis_splitter_fp", ui.splitterFP->saveState());
    settings.setValue("ui/session_analysis_splitter_quali", ui.splitterQuali->saveState());
    settings.setValue("ui/session_analysis_splitter_q1", ui.splitterQ1->saveState());
    settings.setValue("ui/session_analysis_splitter_q2", ui.splitterQ2->saveState());
    settings.setValue("ui/session_analysis_splitter_q3", ui.splitterQ3->saveState());
    settings.setValue("ui/session_analysis_race_laptime", ui.lapTimeTableWidget->width());
    settings.setValue("ui/session_analysis_fp_laptime", ui.lapTimeTableWidgetFP->width());
    settings.setValue("ui/session_analysis_quali_laptime", ui.lapTimeTableWidgetQuali->width());
    settings.setValue("ui/session_analysis_q1_laptime", ui.lapTimeTableWidgetQ1->width());
    settings.setValue("ui/session_analysis_q2_laptime", ui.lapTimeTableWidgetQ2->width());
    settings.setValue("ui/session_analysis_q3_laptime", ui.lapTimeTableWidgetQ3->width());
}

void SessionAnalysisWidget::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/session_analysis_geometry").toByteArray());
    ui.splitter->restoreState(settings.value("ui/session_analysis_splitter").toByteArray());
    ui.raceTabWidget->setCurrentIndex(settings.value("ui/session_analysis_tab").toInt());
    ui.qualiTabWidget->setCurrentIndex(settings.value("ui/session_analysis_quali_tab").toInt());
    ui.splitterFP->restoreState(settings.value("ui/session_analysis_splitter_fp").toByteArray());
    ui.splitterQuali->restoreState(settings.value("ui/session_analysis_splitter_quali").toByteArray());
    ui.splitterQ1->restoreState(settings.value("ui/session_analysis_splitter_q1").toByteArray());
    ui.splitterQ2->restoreState(settings.value("ui/session_analysis_splitter_q2").toByteArray());
    ui.splitterQ3->restoreState(settings.value("ui/session_analysis_splitter_q3").toByteArray());

    int w = settings.value("ui/session_analysis_race_laptime").toInt();
    ui.lapTimeTableWidget->setGeometry(ui.lapTimeTableWidget->x(), ui.lapTimeTableWidget->y(), w == 0 ? 300 : w,ui.lapTimeTableWidget->height());

    w = settings.value("ui/session_analysis_fp_laptime").toInt();
    ui.lapTimeTableWidgetFP->setGeometry(ui.lapTimeTableWidgetFP->x(), ui.lapTimeTableWidgetFP->y(), w == 0 ? 300 : w,ui.lapTimeTableWidgetFP->height());

    w = settings.value("ui/session_analysis_quali_laptime").toInt();
    ui.lapTimeTableWidgetQuali->setGeometry(ui.lapTimeTableWidgetQuali->x(), ui.lapTimeTableWidgetQuali->y(), w == 0 ? 300 : w,ui.lapTimeTableWidgetQuali->height());

    w = settings.value("ui/session_analysis_q1_laptime").toInt();
    ui.lapTimeTableWidgetQ1->setGeometry(ui.lapTimeTableWidgetQ1->x(), ui.lapTimeTableWidgetQ1->y(), w == 0 ? 300 : w,ui.lapTimeTableWidgetQ1->height());

    w = settings.value("ui/session_analysis_q2_laptime").toInt();
    ui.lapTimeTableWidgetQ2->setGeometry(ui.lapTimeTableWidgetQ2->x(), ui.lapTimeTableWidgetQ2->y(), w == 0 ? 300 : w,ui.lapTimeTableWidgetQ2->height());

    w = settings.value("ui/session_analysis_q3_laptime").toInt();
    ui.lapTimeTableWidgetQ3->setGeometry(ui.lapTimeTableWidgetQ3->x(), ui.lapTimeTableWidgetQ3->y(), w == 0 ? 300 : w,ui.lapTimeTableWidgetQ3->height());

    setupTables();
    setupColors();
}

void SessionAnalysisWidget::onSplitterMoved(int, int)
{
    resizeTables();
}

void SessionAnalysisWidget::on_qualiTabWidget_currentChanged(int index)
{
    QualiLapTimesChart *chart = ui.sessionLapTimesChartQuali;

    if (index == 1)
        chart = ui.sessionLapTimesChartQ1;

    else if (index == 2)
        chart = ui.sessionLapTimesChartQ2;

    else if (index == 3)
        chart = ui.sessionLapTimesChartQ3;

    first = chart->getFirst();
    last = chart->getLast();
    min = chart->getMin();
    max = chart->getMax();


    update();
}
