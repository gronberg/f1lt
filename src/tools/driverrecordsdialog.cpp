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


#include "driverrecordsdialog.h"
#include "ui_driverrecordsdialog.h"

#include "../main_gui/ltitemdelegate.h"

DriverRecordsDialog::DriverRecordsDialog(QWidget *parent) :
    QDialog(parent/*, Qt::Window*/),
    ui(new Ui::DriverRecordsDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setItemDelegate(new LTItemDelegate());
}

DriverRecordsDialog::~DriverRecordsDialog()
{
    delete ui;
}

void DriverRecordsDialog::exec(const TrackWeekendRecords &records, QString trackName)
{        
    driverRecords = records;

    this->trackName = trackName;
    loadRecords();

    QDialog::show();
}

void DriverRecordsDialog::loadRecords()
{    
    //local class used for proper sorting columns
    class MyTableWidgetItem : public QTableWidgetItem
    {
    public:
        MyTableWidgetItem() : QTableWidgetItem() { }
        MyTableWidgetItem(QString s) : QTableWidgetItem(s) { }
        virtual bool operator <(const QTableWidgetItem &other) const
        {
            if (text().isNull() || text() == "")
                return false;

            if (other.text().isNull() || other.text() == "")
                return true;

            return text() < other.text();
        }
    };

    setWindowTitle("Driver records: " + trackName);

    int currentIndex = ui->comboBox->currentIndex();
    int i = 0;
    for (; i < driverRecords.driverRecords.size(); ++i)
    {
        if (i <= ui->tableWidget->rowCount())
            ui->tableWidget->insertRow(i);

        MyTableWidgetItem *item = new MyTableWidgetItem(driverRecords.driverRecords[i].driver);
        item->setForeground(SeasonData::getInstance().getColor(LTPackets::WHITE));
        ui->tableWidget->setItem(i, 0, item);

        item = static_cast<MyTableWidgetItem *>(ui->tableWidget->item(i, 1));
        if (!item)
        {
            item = new MyTableWidgetItem();
            ui->tableWidget->setItem(i, 1, item);
        }
        item->setText(driverRecords.driverRecords[i].team);
        item->setForeground(SeasonData::getInstance().getColor(LTPackets::WHITE));


        QColor color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

        Record rec;

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(S1_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][S1_RECORD];


        if (rec.time == driverRecords.sessionRecords[S1_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[S1_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        QString text = rec.time.toString();
        if ((currentIndex == 3 || currentIndex == 5) && text != "")
            text += " (" + rec.session + ")";

        item = static_cast<MyTableWidgetItem *>(ui->tableWidget->item(i, 3));
        if (!item)
        {
            item = new MyTableWidgetItem();
            ui->tableWidget->setItem(i, 3, item);
        }
        item->setText(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);


        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(S2_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][S2_RECORD];

        if (rec.time == driverRecords.sessionRecords[S2_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[S2_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        else
            color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

        text = rec.time.toString();
        if ((currentIndex == 3 || currentIndex == 5) && text != "")
            text += " (" + rec.session + ")";

        item = static_cast<MyTableWidgetItem *>(ui->tableWidget->item(i, 4));
        if (!item)
        {
            item = new MyTableWidgetItem();
            ui->tableWidget->setItem(i, 4, item);
        }
        item->setText(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(S3_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][S3_RECORD];


        if (rec.time == driverRecords.sessionRecords[S3_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[S3_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        else
            color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

        text = rec.time.toString();
        if ((currentIndex == 3 || currentIndex == 5) && text != "")
            text += " (" + rec.session + ")";

        item = static_cast<MyTableWidgetItem *>(ui->tableWidget->item(i, 5));
        if (!item)
        {
            item = new MyTableWidgetItem();
            ui->tableWidget->setItem(i, 5, item);
        }
        item->setText(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(TIME_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][TIME_RECORD];


        if (rec.time == driverRecords.sessionRecords[TIME_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[TIME_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        else
            color = SeasonData::getInstance().getColor(LTPackets::GREEN);

        text = rec.time.toString();
        if ((currentIndex == 3 || currentIndex == 5) && text != "")
            text += " (" + rec.session + ")";

        item = static_cast<MyTableWidgetItem *>(ui->tableWidget->item(i, 2));
        if (!item)
        {
            item = new MyTableWidgetItem();
            ui->tableWidget->setItem(i, 2, item);
        }
        item->setText(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);
    }
    for (int j = ui->tableWidget->rowCount()-1; j >= i; --j)
        ui->tableWidget->removeRow(j);

    if (ui->comboBox_2->currentIndex() > 0)
    {
        int col = ui->comboBox_2->currentIndex() + 1;
        ui->tableWidget->sortByColumn(col, Qt::AscendingOrder);
    }
}

void DriverRecordsDialog::loadRecords(const TrackWeekendRecords &records, QString trackName)
{
    driverRecords = records;
    this->trackName = trackName;
    loadRecords();
}

void DriverRecordsDialog::setFont(const QFont &font)
{
    ui->tableWidget->setFont(font);
}

void DriverRecordsDialog::saveSettings(QSettings &settings)
{
    settings.setValue("ui/driver_records_geometry", saveGeometry());
    settings.setValue("ui/driver_records_table", ui->tableWidget->saveGeometry());
    settings.setValue("ui/driver_records_session", ui->comboBox->currentIndex());
    settings.setValue("ui/driver_records_sort", ui->comboBox_2->currentIndex());

    QList<QVariant> list;
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
        list.append(ui->tableWidget->columnWidth(i));

    settings.setValue("ui/driver_records_columns", list);
}

void DriverRecordsDialog::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/driver_records_geometry").toByteArray());
    ui->tableWidget->restoreGeometry(settings.value("ui/driver_records_table").toByteArray());
    ui->comboBox->setCurrentIndex(settings.value("ui/driver_records_session", 5).toInt());
    ui->comboBox_2->setCurrentIndex(settings.value("ui/driver_records_sort", 0).toInt());

    QList<QVariant> list;
    list = settings.value("ui/driver_records_columns").toList();

    if (list.size() > 0 && list[0].toInt() > 0)
    {
        for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
            ui->tableWidget->setColumnWidth(i, list[i].toInt());
    }
}

void DriverRecordsDialog::on_comboBox_currentIndexChanged(int)
{
    loadRecords();
}

void DriverRecordsDialog::on_comboBox_2_currentIndexChanged(int index)
{
    if (index > 0)
    {
        int col = index + 1;
        ui->tableWidget->sortByColumn(col, Qt::AscendingOrder);
    }
    else
        loadRecords();
}
