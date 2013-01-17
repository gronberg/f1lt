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


#include "sessiontimeswidget.h"
#include "ui_sessiontimeswidget.h"


#include <QDebug>
#include <QStringList>
#include "../core/eventdata.h"
#include "../core/seasondata.h"
#include "../main_gui/ltitemdelegate.h"

SessionTimesWidget::SessionTimesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionTimesWidget), selectedDriver(0), relativeTimes(false)
{
    ui->setupUi(this);
    loadDriversList();

    ui->timesTableWidget->setItemDelegate(new LTItemDelegate());
    ui->driversListWidget->setItemDelegate(new LTItemDelegate());

    connect(ui->timesTableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));
    connect(ui->timesTableWidget->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(onHeaderDoubleClicked(int)));
}

SessionTimesWidget::~SessionTimesWidget()
{
    delete ui;
}

QTableWidgetItem* SessionTimesWidget::setItem(int row, int col, QString text, Qt::ItemFlags flags, int align,
             QColor textColor, QBrush background)
{
    if (ui->timesTableWidget->rowCount() <= row)
    {
        ui->timesTableWidget->insertRow(row);

//        for (int i = 0; i < ui->timesTableWidget->columnCount();  ++i)
//        {
//            if (i != col)
//                ui->timesTableWidget->setItem(row, i, new QTableWidgetItem());
//        }
    }

    QTableWidgetItem *item = ui->timesTableWidget->item(row, col);
    if (!item)
    {
        item = new QTableWidgetItem(text);
        item->setFlags(flags);
        ui->timesTableWidget->setItem(row, col, item);
    }
    item->setTextAlignment(align);
    item->setBackground(background);
    item->setText(text);
    item->setTextColor(textColor);

    return item;
}

void SessionTimesWidget::loadDriversList()
{
    QStringList list = SeasonData::getInstance().getDriversList();
    list.removeFirst();

    ui->driversListWidget->clear();
    ui->driversListWidget->addItems(list);

    for (int i = ui->timesTableWidget->columnCount(); i < list.size(); ++i)
    {
        ui->timesTableWidget->insertColumn(i);
    }

    if (list.size() < ui->timesTableWidget->columnCount())
    {
        for (int i = ui->timesTableWidget->columnCount(); i >= list.size(); --i)
            ui->timesTableWidget->removeColumn(i);
    }
    ui->timesTableWidget->clear();

    for (int i = 0; i < ui->driversListWidget->count(); ++i)
    {
        ui->driversListWidget->item(i)->setFlags(ui->driversListWidget->item(i)->flags() | Qt::ItemIsUserCheckable);

        QTableWidgetItem *item = ui->timesTableWidget->horizontalHeaderItem(i);
        if (item == 0)
        {
            item = new QTableWidgetItem();
            ui->timesTableWidget->setHorizontalHeaderItem(i, item);
        }
        item->setText(getName(i));
    }
    restoreCheckedArray();
}

void SessionTimesWidget::exec()
{
    setWindowTitle("Session times: " + EventData::getInstance().getEventInfo().eventName);
    loadDriversList();
    update();
    show();
}

void SessionTimesWidget::update()
{
    switch(EventData::getInstance().getEventType())
    {
        case LTPackets::RACE_EVENT:     handleRaceEvent(); break;
        case LTPackets::QUALI_EVENT:    handleQualiEvent(); break;
        case LTPackets::PRACTICE_EVENT: handlePracticeEvent(); break;
    }
}

void SessionTimesWidget::handleRaceEvent()
{
    for (int i = 0; i < ui->timesTableWidget->columnCount(); ++i)
    {
        int driverNo = getNumber(i);

        DriverData *dd = EventData::getInstance().getDriverDataPtr(driverNo);

        if (dd != 0)
        {
            LapTime bestTime = dd->getSessionRecords().getBestLap().getTime();
            int bestLapNo = dd->getSessionRecords().getBestLap().getLapNumber();

            for (int j = 1; j <= EventData::getInstance().getCompletedLaps(); ++j)
            {
                LapData ld = dd->getLapData(j);
                QColor color = ((!relativeTimes || bestLapNo == j) && ld.getTime().isValid()) ? SeasonData::getInstance().getColor(LTPackets::WHITE) : SeasonData::getInstance().getColor(LTPackets::YELLOW);

                if (ld.getCarID() == dd->getCarID())
                {
                    QString time = ld.getTime().toString();
                    if (time == "IN PIT")
                        time = QString("IN PIT (%1)").arg(dd->getPitTime(j));

                    if (selectedDriver != 0 && ui->relativeButton->isChecked())
                    {
                        color = (ld.getTime().isValid()) ? SeasonData::getInstance().getColor(LTPackets::WHITE) : SeasonData::getInstance().getColor(LTPackets::RED);
                        if (selectedDriver != dd)
                        {
                            LapData sld = selectedDriver->getLapData(j);

                            if (sld.getCarID() == selectedDriver->getCarID() && sld.getTime().isValid() && ld.getTime().isValid())
                            {
                                color = (ld < sld) ? SeasonData::getInstance().getColor(LTPackets::GREEN) : SeasonData::getInstance().getColor(LTPackets::RED);

                                if (relativeTimes)
                                    time = DriverData::calculateGap(ld.getTime(), sld.getTime());
                            }
                        }

                    }
                    else
                    {
                        if (j == bestLapNo)
                            color = SeasonData::getInstance().getColor(LTPackets::GREEN);

                        if (!ld.getTime().isValid())
                            color = SeasonData::getInstance().getColor(LTPackets::RED);

                        if (relativeTimes && bestLapNo != j && ld.getTime().isValid())
                            time = DriverData::calculateGap(ld.getTime(), bestTime);
                    }
                    setItem(j-1, i, time, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, color);


                    QTableWidgetItem *item = ui->timesTableWidget->verticalHeaderItem(j-1);

                    if (item == 0)
                    {
                        item = new QTableWidgetItem();
                        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                        ui->timesTableWidget->setVerticalHeaderItem(j-1, item);
                    }
                    item->setText(QString::number(j));
                }
                else
                    setItem(j-1, i, "", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, color);

            }
        }
    }
    removeRows(EventData::getInstance().getCompletedLaps());
}

void SessionTimesWidget::handleQualiEvent()
{
    int row = 0;
    for (int q = 1; q <= 3; ++q)
    {
        for (int i = 0; i < ui->timesTableWidget->columnCount(); ++i)
            setItem(row, i, QString("Q%1").arg(q), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                    SeasonData::getInstance().getColor(LTPackets::BACKGROUND), /*SeasonData::getInstance().getColor(LTPackets::YELLOW)*/QBrush(QColor(123, 123, 123)));

        QTableWidgetItem *item = ui->timesTableWidget->verticalHeaderItem(row);

        if (item == 0)
        {
            item = new QTableWidgetItem();
            ui->timesTableWidget->setVerticalHeaderItem(row, item);
        }
        item->setText(QString("Q%1").arg(q));

        ++row;

        for (int j = 1; j <= SeasonData::getInstance().getQualiLength(q); ++j)
        {
            bool rowInserted = false;

            LapData sld;
            if (selectedDriver != 0 && ui->relativeButton->isChecked())
                sld = selectedDriver->getQLapData(j, q);

            for (int i = 0; i < ui->timesTableWidget->columnCount(); ++i)
            {
                int driverNo = getNumber(i);

                DriverData *dd = EventData::getInstance().getDriverDataPtr(driverNo);

                if (dd != 0)
                {
                    LapTime bestTime = dd->getSessionRecords().getBestQualiLap(3).getTime();
                    int bestQ = 3;

                    while (!bestTime.isValid() && bestQ > 1)
                    {
                        --bestQ;
                         bestTime = dd->getSessionRecords().getBestQualiLap(bestQ).getTime();
                    }

                    int bestLapNo = dd->getSessionRecords().getBestQualiLap(bestQ).getLapNumber();


                    LapData ld = dd->getQLapData(j, q);
                    QColor color = ((!relativeTimes || bestLapNo == ld.getLapNumber()) && ld.getTime().isValid()) ?
                                SeasonData::getInstance().getColor(LTPackets::WHITE) : SeasonData::getInstance().getColor(LTPackets::YELLOW);

                    if (ld.getTime().toString().contains("LAP"))
                        color = SeasonData::getInstance().getColor(LTPackets::RED);

                    if (ld.getCarID() == dd->getCarID())
                    {
                        QString time = ld.getTime().toString();


                        if (selectedDriver != 0 && ui->relativeButton->isChecked())
                        {
                            color = ld.getTime().isValid() ? SeasonData::getInstance().getColor(LTPackets::WHITE) : SeasonData::getInstance().getColor(LTPackets::RED);
                            if (selectedDriver != dd && sld.getCarID() == selectedDriver->getCarID() && sld.getTime().isValid() && ld.getTime().isValid())
                            {
                                color = (ld < sld) ? SeasonData::getInstance().getColor(LTPackets::GREEN) : SeasonData::getInstance().getColor(LTPackets::RED);

                                if (relativeTimes)
                                    time = DriverData::calculateGap(ld.getTime(), sld.getTime());
                            }

                        }
                        else
                        {
                            if (ld.getLapNumber() == bestLapNo)
                                color = SeasonData::getInstance().getColor(LTPackets::GREEN);

                            if (relativeTimes && bestLapNo != ld.getLapNumber() && ld.getTime().isValid())
                                time = DriverData::calculateGap(ld.getTime(), bestTime);
                        }

                        setItem(row, i, time, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, color);
                        rowInserted = true;
                    }
                }
            }
            if (rowInserted)
            {
                QTableWidgetItem *item = ui->timesTableWidget->verticalHeaderItem(row);

                if (item == 0)
                {
                    item = new QTableWidgetItem();
                    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                    ui->timesTableWidget->setVerticalHeaderItem(row, item);
                }
                item->setText(QString::number(j));

                ++row;
            }
        }
    }
    removeRows(row);
}

void SessionTimesWidget::handlePracticeEvent()
{
    int row = 0;
    for (int j = 1; j <= SeasonData::getInstance().getFPLength(); ++j)
    {
        bool rowInserted = false;

        LapData sld;
        if (selectedDriver != 0 && ui->relativeButton->isChecked())
            sld = selectedDriver->getFPLapData(j);

        for (int i = 0; i < ui->timesTableWidget->columnCount(); ++i)
        {
            int driverNo = getNumber(i);

            DriverData *dd = EventData::getInstance().getDriverDataPtr(driverNo);

            if (dd != 0)
            {
                LapTime bestTime = dd->getSessionRecords().getBestLap().getTime();
                int bestLapNo = dd->getSessionRecords().getBestLap().getLapNumber();


                LapData ld = dd->getFPLapData(j);
                QColor color = ((!relativeTimes || bestLapNo == ld.getLapNumber()) && ld.getTime().isValid()) ?
                            SeasonData::getInstance().getColor(LTPackets::WHITE) : SeasonData::getInstance().getColor(LTPackets::YELLOW);

                if (ld.getTime().toString().contains("LAP"))
                    color = SeasonData::getInstance().getColor(LTPackets::RED);

                if (ld.getCarID() == dd->getCarID())
                {
                    QString time = ld.getTime().toString();

                    if (selectedDriver != 0 && ui->relativeButton->isChecked())
                    {
                        color = ld.getTime().isValid() ? SeasonData::getInstance().getColor(LTPackets::WHITE) : SeasonData::getInstance().getColor(LTPackets::RED);
                        if (selectedDriver != dd && sld.getCarID() == selectedDriver->getCarID() && sld.getTime().isValid() && ld.getTime().isValid())
                        {
                            color = (ld < sld) ? SeasonData::getInstance().getColor(LTPackets::GREEN) : SeasonData::getInstance().getColor(LTPackets::RED);

                            if (relativeTimes)
                                time = DriverData::calculateGap(ld.getTime(), sld.getTime());
                        }

                    }
                    else
                    {
                        if (ld.getLapNumber() == bestLapNo)
                            color = SeasonData::getInstance().getColor(LTPackets::GREEN);

                        if (relativeTimes && bestLapNo != ld.getLapNumber() && ld.getTime().isValid())
                            time = DriverData::calculateGap(ld.getTime(), bestTime);
                    }

                    setItem(row, i, time, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, color);
                    rowInserted = true;
                }
            }
        }
        if (rowInserted)
        {
            QTableWidgetItem *item = ui->timesTableWidget->verticalHeaderItem(row);

            if (item == 0)
            {
                item = new QTableWidgetItem();
                item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                ui->timesTableWidget->setVerticalHeaderItem(row, item);
            }
            item->setText(QString::number(j));

            ++row;
        }
    }
    removeRows(row);
}

int SessionTimesWidget::getNumber(int row)
{
    QListWidgetItem *item = ui->driversListWidget->item(row);
    if (item)
    {
        QString text = item->text();

        int no = -1;
        int idx = text.indexOf(" ");
        if (idx != -1)
        {
            bool ok;
            no = text.left(idx).toInt(&ok);

            if (!ok)
                no = -1;
        }
        return no;
    }
    return -1;
}

QString SessionTimesWidget::getName(int row)
{
    QListWidgetItem *item = ui->driversListWidget->item(row);
    if (item)
    {
        QString text = item->text();

        int idx = text.indexOf(" ");
        if (idx != -1)
        {
            return text.right(text.size()-idx-1);
        }
    }
    return QString();
}

void SessionTimesWidget::removeRows(int row)
{
    for (int i = ui->timesTableWidget->rowCount()-1; i >= row; --i)
        ui->timesTableWidget->removeRow(i);
}

void SessionTimesWidget::setFont(const QFont &font)
{
    ui->timesTableWidget->setFont(font);
}

void SessionTimesWidget::saveSettings(QSettings &settings)
{
    settings.setValue("ui/session_times_geometry", saveGeometry());
    settings.setValue("ui/session_times_splitter", ui->splitter->saveState());
    settings.setValue("ui/session_times_table", ui->timesTableWidget->saveGeometry());

    settings.setValue("ui/session_times_columns", checkedArray);
}

void SessionTimesWidget::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/session_times_geometry").toByteArray());
    ui->splitter->restoreState(settings.value("ui/session_times_splitter").toByteArray());
    ui->timesTableWidget->restoreGeometry(settings.value("ui/session_times_table").toByteArray());

    checkedArray = settings.value("ui/session_times_columns", QByteArray(ui->driversListWidget->count(), 2)).toByteArray();
}

void SessionTimesWidget::saveCheckedArray()
{
    checkedArray.resize(ui->driversListWidget->count());
    for (int i = 0; i < ui->driversListWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->driversListWidget->item(i);
        checkedArray[i] = (int)item->checkState();
    }
}

void SessionTimesWidget::restoreCheckedArray()
{
    for (int i = 0; i < ui->driversListWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->driversListWidget->item(i);

        if (i < checkedArray.size())
        {
            int t = checkedArray[i];
            item->setCheckState((Qt::CheckState)t);

            if ((Qt::CheckState)t == Qt::Unchecked)
                ui->timesTableWidget->setColumnHidden(i, true);
        }
        else
            item->setCheckState(Qt::Checked);
    }
}

void SessionTimesWidget::on_closeButton_clicked()
{
    saveCheckedArray();
    close();
}

void SessionTimesWidget::on_flagButton_clicked()
{
    relativeTimes = !relativeTimes;

    if (relativeTimes)
        ui->flagButton->setText("Absolute times");
    else
        ui->flagButton->setText("Relative times");

    update();
}

void SessionTimesWidget::on_driversListWidget_clicked(const QModelIndex &index)
{
    ui->relativeButton->setEnabled(true);
    int row = index.row();

    int no = getNumber(row);

    QListWidgetItem *item = ui->driversListWidget->item(row);
    if (item)
    {
        if (item->checkState() == Qt::Unchecked)
        {
            ui->timesTableWidget->setColumnHidden(row, true);

            if (ui->relativeButton->isChecked())
            {
                ui->relativeButton->setChecked(false);
                update();
            }
        }

        else
        {
            ui->timesTableWidget->selectColumn(row);
            ui->timesTableWidget->setColumnHidden(row, false);

            selectedDriver = EventData::getInstance().getDriverDataPtr(no);

            if (ui->relativeButton->isChecked())
            {
                update();
            }
        }
    }
}

void SessionTimesWidget::on_driversListWidget_doubleClicked(const QModelIndex &index)
{
    ui->relativeButton->setChecked(true);
    on_driversListWidget_clicked(index);
}

void SessionTimesWidget::on_relativeButton_toggled(bool)
{
    update();
}

void SessionTimesWidget::onHeaderClicked(int col)
{
    ui->relativeButton->setEnabled(true);
    ui->driversListWidget->clearSelection();
    QListWidgetItem *item = ui->driversListWidget->item(col);
    if (item)
        ui->driversListWidget->setCurrentItem(item);

    int no = getNumber(col);
    selectedDriver = EventData::getInstance().getDriverDataPtr(no);
    if (ui->relativeButton->isChecked())
        update();

}

void SessionTimesWidget::onHeaderDoubleClicked(int col)
{
    ui->relativeButton->setChecked(true);
    onHeaderClicked(col);
}


