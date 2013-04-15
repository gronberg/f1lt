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


#include "laptimecomparisondialog.h"
#include "ui_laptimecomparisondialog.h"

#include <algorithm>
#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include <QStringList>
#include <QList>
#include <QLabel>
#include <QResizeEvent>
#include <QScrollBar>
#include <cmath>

#include "../core/colorsmanager.h"
#include "../main_gui/ltitemdelegate.h"

LapTimeComparisonDialog::LapTimeComparisonDialog(QWidget *parent) :
    QDialog(parent, Qt::Window), ui(new Ui::LapTimeComparisonDialog), eventData(EventData::getInstance()), thumbnailsSize(150)
{
    ui->setupUi(this);

    comboBox[0] = ui->comboBox1;
    comboBox[1] = ui->comboBox2;
    comboBox[2] = ui->comboBox3;
    comboBox[3] = ui->comboBox4;

    loadDriversList();
    lapCompChart = new LapCompChart(this);


    connect(ui->comboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox3, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox4, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));

    ui->tableWidget->setItemDelegate(new LTItemDelegate(this));

    ui->tableWidget->setColumnWidth(0, 30);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 150);
    ui->tableWidget->setColumnWidth(4, 150);

    ui->chartsTableWidget->setColumnWidth(0, 170);
    ui->chartsTableWidget->setColumnWidth(1, 170);
    ui->chartsTableWidget->setColumnWidth(2, 170);
    ui->chartsTableWidget->setColumnWidth(3, 170);

    ui->chartsTableWidget->insertRow(0);
    ui->chartsTableWidget->setRowHeight(0, 20);
    ui->chartsTableWidget->insertRow(1);
    ui->chartsTableWidget->setRowHeight(1, 50);

    QTableWidgetItem *item;
    for (int j = 0; j < 4; ++j)
    {
        item = new QTableWidgetItem();
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        ui->chartsTableWidget->setItem(0, j, item);
    }

    ui->chartsTableWidget->insertRow(2);

    ui->chartsTableWidget->setCellWidget(2, 0, lapCompChart);
    ui->chartsTableWidget->setSpan(2, 0, 1, 4);
    ui->chartsTableWidget->setRowHeight(2, 500);

    ui->tableWidget->insertRow(0);
    item = new QTableWidgetItem("Lap");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(ColorsManager::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(0, 0, item);

    QLabel *lab = new QLabel();
    ui->tableWidget->setCellWidget(0, 1, lab);
    lab->setAlignment(Qt::AlignCenter);

    lab = new QLabel();
    ui->tableWidget->setCellWidget(0, 2, lab);
    lab->setAlignment(Qt::AlignCenter);

    lab = new QLabel();
    ui->tableWidget->setCellWidget(0, 3, lab);
    lab->setAlignment(Qt::AlignCenter);

    lab = new QLabel();
    ui->tableWidget->setCellWidget(0, 4, lab);
    lab->setAlignment(Qt::AlignCenter);

    ui->tableWidget->setRowHeight(0, 50);
}

LapTimeComparisonDialog::~LapTimeComparisonDialog()
{
    delete ui;
}

void LapTimeComparisonDialog::loadDriversList()
{
    comboBox[0]->clear();
    comboBox[1]->clear();
    comboBox[2]->clear();
    comboBox[3]->clear();

    if (comboBox[0]->itemText(1) == "")// && eventData.driversData[0].driver != "")
    {
        QStringList list = SeasonData::getInstance().getDriversList();
        if (list.size() > 1)
        {
            comboBox[0]->addItems(list);
            comboBox[1]->addItems(list);
            comboBox[2]->addItems(list);
            comboBox[3]->addItems(list);
        }
    }

//    comboBox[0]->addItems(SeasonData::getInstance().getDriversList());
//    comboBox[1]->addItems(SeasonData::getInstance().getDriversList());
//    comboBox[2]->addItems(SeasonData::getInstance().getDriversList());
//    comboBox[3]->addItems(SeasonData::getInstance().getDriversList());

    /*smallCarImg = */ImagesFactory::getInstance().getCarThumbnailsFactory().loadCarThumbnails(thumbnailsSize);
}

void LapTimeComparisonDialog::updateData()
{
    int scrollBarPosition = ui->tableWidget->verticalScrollBar()->sliderPosition();

    QItemSelectionModel * selection = ui->tableWidget->selectionModel();
//    for (int i = ui->tableWidget->rowCount()-1; i >= 0; --i)
//        ui->tableWidget->removeRow(i);

    QTableWidgetItem *item;

    int firstLap = 99, lastLap = 0;
    int index[4];

    QString wTitle = "Lap time comparison: ";
    for (int i = 0; i < 4; ++i)
    {
        index[i] = 0;
        int idx = eventData.getDriverId(getNumber(i));
        if (idx > 0)
        {
            if (i > 0)
                wTitle += " - ";
            wTitle += eventData.getDriversData()[idx-1].getDriverName();
            if(!eventData.getDriversData()[idx-1].getLapData().isEmpty())
            {
                if (eventData.getDriversData()[idx-1].getLapData()[0].getLapNumber() < firstLap)
                    firstLap = eventData.getDriversData()[idx-1].getLapData()[0].getLapNumber();

                if (eventData.getDriversData()[idx-1].getLapData().last().getLapNumber() > lastLap)
                    lastLap = eventData.getDriversData()[idx-1].getLapData().last().getLapNumber();
            }

            DriverData &dd = eventData.getDriversData()[idx-1];
			QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
            lab->setPixmap(ImagesFactory::getInstance().getCarThumbnailsFactory().getCarThumbnail(dd.getNumber(), thumbnailsSize));//eventData.carImages[idx].scaledToWidth(120, Qt::SmoothTransformation));
        }
        else
        {
        	QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
        	lab->clear();
        }
    }
    setWindowTitle(wTitle);

//    ui->tableWidget->insertRow(0);
//    ui->tableWidget->setRowHeight(0, 50);

    int j = 0, k = firstLap;
    for (; k <= lastLap; ++k, ++j)
    {
        int lapNo = lastLap - k + firstLap;
        LapTime laps[4];

        if (ui->tableWidget->rowCount() <= j+1)
            ui->tableWidget->insertRow(j+1);

        item = ui->tableWidget->item(j+1, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1.").arg(lapNo));
//                item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(ColorsManager::getInstance().getColor(LTPackets::DEFAULT));
            ui->tableWidget->setItem(j+1, 0, item);
        }
        else
            item->setText(QString("%1.").arg(lapNo));


        for (int i = 0; i < 4; ++i)
        {
            int idx = eventData.getDriverId(getNumber(i));

            if (idx > 0 && !eventData.getDriversData()[idx-1].getLapData().isEmpty())
            {
                //int lapIndex = (reversedOrder ? eventData.driversData[idx-1].lapData.size() - index[i] - 1 : index[i]);
                DriverData &dd = eventData.getDriversData()[idx-1];
                LapData ld = dd.getLapData(lapNo);

//                if (j == 0)
//                {
//                    int idx = (dd.number > 13 ? dd.number-2 : dd.number-1) / 2;
//                    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
//                    lab->setPixmap(smallCarImg[idx]);//eventData.carImages[idx].scaledToWidth(120, Qt::SmoothTransformation));
//                }

                if (dd.getLapData().size() > index[i] && ld.getLapNumber() == lapNo && ld.getCarID() != -1)
                {
                    laps[i] = ld.getTime();

                    item = ui->tableWidget->item(j+1, i+1);
                    if (!item)
                    {
                        item = new QTableWidgetItem(ld.getTime());
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(j+1, i+1, item);
                    }
                    else
                        item->setText(ld.getTime());

                    if (ld.getTime().toString() == "IN PIT")
                        item->setText(item->text() + " (" + dd.getPitTime(ld.getLapNumber()) + ")");

                    ++index[i];
                }
                else
                {
                    item = ui->tableWidget->item(j+1, i+1);
                    if (item)
                        item->setText("");
                }
            }
            else
            {
                if (j == 0)
                {
//                    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
//                    if (lab)
//                        lab->clear();
                }
                item = ui->tableWidget->item(j+1, i+1);
                if (item)
                    item->setText("");
            }
        }
        int bestIdx = DriverData::lapDiff(laps);
        if (bestIdx != -1)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (i != bestIdx && laps[i].toString() != "" && laps[i].toString() != "IN PIT" && laps[i].toString() != "RETIRED" && !laps[i].toString().contains("LAP"))
                {
                    item = ui->tableWidget->item(j+1, i+1);

                    if (item)
                    {
                       item->setText(item->text() + " (+"+QString::number(laps[i].toDouble(), 'f', 3)+")");

                        double msecs[3];
                        int ji = 0;

                        for (int j = 0; j < 4; ++j)
                        {
                            if (j != bestIdx)
                            {
                                if (laps[j].toString() != "")
                                    msecs[ji++] = laps[j].toMsecs();
                                else
                                    msecs[ji++] = 1000000;
                            }
                        }
                        double maxGap = std::max(std::max(msecs[0], msecs[1]), msecs[2]);
                        double minGap = std::min(std::min(msecs[0], msecs[1]), msecs[2]);

                        LTPackets::Colors color = LTPackets::YELLOW;

                        if (laps[i].toMsecs() == minGap)
                            color = LTPackets::WHITE;

                        else if (laps[i].toMsecs() == maxGap)
                            color = LTPackets::RED;

                        item->setTextColor(ColorsManager::getInstance().getColor(color));
                    }
                }
                else if (laps[i].toString() == "IN PIT" || laps[i].toString() == "RETIRED" ||  laps[i].toString().contains("LAP"))
                {
                    item = ui->tableWidget->item(j+1, i+1);
                    item->setTextColor(ColorsManager::getInstance().getColor(LTPackets::RED));
                }
            }
            item = ui->tableWidget->item(j+1, bestIdx+1);
            if (item && laps[bestIdx].toString() != "IN PIT" && laps[bestIdx].toString() != "RETIRED" && !laps[bestIdx].toString().contains("LAP"))
                item->setTextColor(ColorsManager::getInstance().getColor(LTPackets::GREEN));
        }
        ui->tableWidget->setRowHeight(j+1, 20);
    }
    for (int i = ui->tableWidget->rowCount()-1; i >= j+1; --i)
        ui->tableWidget->removeRow(i);

    ui->tableWidget->setSelectionModel(selection);
    ui->tableWidget->verticalScrollBar()->setSliderPosition(scrollBarPosition);
}

void LapTimeComparisonDialog::updateCharts()
{
    DriverData *driverData[4] = {0, 0, 0, 0};
    QString driver;
    for (int i = 0; i < 4; ++i)
    {
        int idx = eventData.getDriverId(getNumber(i));
        if (idx > 0)
        {
            driver = eventData.getDriversData()[idx-1].getDriverName();
            driverData[i] = &eventData.getDriversData()[idx-1];
//            carIdx = (eventData.getDriversData()[idx-1].getNumber() > 13 ?
//                             eventData.getDriversData()[idx-1].getNumber() - 2 :
//                             eventData.getDriversData()[idx-1].getNumber() - 1) / 2;

            QTableWidgetItem *item = ui->chartsTableWidget->item(0, i);
            item->setText(driver);
            item->setTextColor(ColorsManager::getInstance().getCarColor(driverData[i]->getNumber()));

//            if (carIdx >= 0)
            {
                QLabel *lab = qobject_cast<QLabel*>(ui->chartsTableWidget->cellWidget(1, i));
                if (!lab)
                {
                    lab = new QLabel();
                    lab->setAlignment(Qt::AlignCenter);
                    lab->setPixmap(ImagesFactory::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
                    ui->chartsTableWidget->setCellWidget(1, i, lab);
                }
                else
                    lab->setPixmap(ImagesFactory::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
            }
        }
        else
        {
            QTableWidgetItem *item = ui->chartsTableWidget->item(0, i);
            item->setText("");

            QLabel *lab = qobject_cast<QLabel*>(ui->chartsTableWidget->cellWidget(1, i));
            if (lab)
                lab->clear();
        }
    }
    lapCompChart->setData(driverData);
    lapCompChart->repaint();
}

void LapTimeComparisonDialog::show(int currentCarId)
{
//    if (comboBox[0]->itemText(1) == "")
//    {
//        comboBox[0]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[1]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[2]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[3]->addItems(SeasonData::getInstance().getDriversList());
//    }
    setCurrentDriver(currentCarId);
    for (int i = 0; i < 4; ++i)
    {
        QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
        if (lab)
            lab->clear();
    }

    updateData();
    updateCharts();

    QDialog::show();
}

int LapTimeComparisonDialog::exec(int currentCarId)
{
//    if (comboBox[0]->itemText(1) == "")
//    {
//        comboBox[0]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[1]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[2]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[3]->addItems(SeasonData::getInstance().getDriversList());
//    }
    setCurrentDriver(currentCarId);

    updateData();
    updateCharts();

    return QDialog::exec();
}

void LapTimeComparisonDialog::comboBoxValueChanged(int)
{
    updateData();
    updateCharts();
}

void LapTimeComparisonDialog::resizeEvent(QResizeEvent *event)
{
    for (int i = 0; i < 4; ++i)
    {
        ui->chartsTableWidget->setColumnWidth(i, (event->size().width()-40) / 4-5);
        ui->tableWidget->setColumnWidth(i+1, (event->size().width()-50) / 4 - 10);
    }

    int h = /*ui->chartsTableWidget->viewport()->height()-80;*/event->size().height() - 250;
    if (h < 200)
        h = 200;

    ui->chartsTableWidget->setRowHeight(2, h);
}

void LapTimeComparisonDialog::keyPressEvent(QKeyEvent *event)
{
    if (ui->tabWidget->currentIndex() == 0 && event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
    {
            QItemSelectionModel * selection = ui->tableWidget->selectionModel();
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
                QVariant data = ui->tableWidget->model()->data(previous);
                QString text = data.toString();
                // At this point `text` contains the text in one cell
                selected_text.append(text);
                // If you are at the start of the row the row number of the previous index
                // isn't the same.  Text is followed by a row separator, which is a newline.
                if (current.row() != previous.row())
                {
                    selected_text.append(QLatin1Char('\n'));
                }
                // Otherwise it's the same row, so append a column separator, which is a tab.
                else
                {
                    selected_text.append(QLatin1Char('\t'));
                }
                previous = current;
            }
            selected_text.append(ui->tableWidget->model()->data(current).toString());
            selected_text.append(QLatin1Char('\n'));
            qApp->clipboard()->setText(selected_text);
    }
    if (event->key() == Qt::Key_Escape)
        close();
}

void LapTimeComparisonDialog::setFont(const QFont &font)
{
    ui->tableWidget->setFont(font);
}

void LapTimeComparisonDialog::setCurrentDriver(int id)
{
    if (id != 0)
    {
        DriverData *dd = eventData.getDriverDataByIdPtr(id);
        if (dd != 0 && dd->getCarID() > 0)
        {
            int idx = comboBox[0]->findText(QString("%1 %2").arg(dd->getNumber()).arg(dd->getDriverName()));
            if (idx != -1)
                comboBox[0]->setCurrentIndex(idx);
        }
    }
}

int LapTimeComparisonDialog::getNumber(int i)
{
	QString text = comboBox[i]->currentText();

	int no = -1;
	int idx = text.indexOf(" ");
	if (idx != 0)
	{
		bool ok;
		no = text.left(idx).toInt(&ok);

		if (!ok)
			no = -1;
	}
	return no;
}

