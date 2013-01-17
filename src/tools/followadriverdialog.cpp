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


#include "followadriverdialog.h"
#include "ui_followadriverdialog.h"

#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include "../main_gui/ltitemdelegate.h"

FollowADriverDialog::FollowADriverDialog(QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::FollowADriverDialog), eventData(EventData::getInstance()), thumbnailsSize(180)
{
    ui->setupUi(this);

    loadDriversList();
    setupTables();

    ui->lapTimesTableWidget->setItemDelegate(new LTItemDelegate());
    ui->dataTableWidget->setItemDelegate(new LTItemDelegate());

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
}

FollowADriverDialog::~FollowADriverDialog()
{
    delete ui;
}

void FollowADriverDialog::loadDriversList()
{
    /*smallCarImg = */SeasonData::getInstance().getCarThumbnailsFactory().loadCarThumbnails(thumbnailsSize);

//    ui->comboBox->clear();

    clearData();
    if (ui->comboBox->itemText(1) == "")
    {
        QStringList list = SeasonData::getInstance().getDriversList();

        qDebug() << "loading drivers" << list.size();
        if (list.size() > 1)
            ui->comboBox->addItems(SeasonData::getInstance().getDriversList());
    }
}

void FollowADriverDialog::setupTables()
{
    if (ui->dataTableWidget->rowCount() == 0)
    {
        for (int i = 0; i < 6; ++i)
        {
            ui->dataTableWidget->insertRow(i);
            ui->dataTableWidget->setRowHeight(i, 22);
        }
    }

    if (ui->lapTimesTableWidget->rowCount() == 0)
    {
        for (int i = 0; i < 6; ++i)
        {
            ui->lapTimesTableWidget->insertRow(i);
            ui->lapTimesTableWidget->setRowHeight(i, 22);
        }
    }
    setItem(ui->dataTableWidget, 0, 0, "P", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    setItem(ui->dataTableWidget, 0, 1, "Name", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    setItem(ui->dataTableWidget, 0, 2, "Int.", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    setItem(ui->dataTableWidget, 0, 3, "Time", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    setItem(ui->dataTableWidget, 0, 4, "S1", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    setItem(ui->dataTableWidget, 0, 5, "S2", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    setItem(ui->dataTableWidget, 0, 6, "S3", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));

    setItem(ui->lapTimesTableWidget, 0, 0, "Lap", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
}

QTableWidgetItem* FollowADriverDialog::setItem(QTableWidget *table, int row, int col, QString text, Qt::ItemFlags flags, int align,
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

int FollowADriverDialog::exec(int currentDriverId)
{
//    loadDriversData();
//    if (ui->comboBox->itemText(1) == "")// && eventData.driversData[0].driver != "")
//    {
//        ui->comboBox->addItems(SeasonData::getInstance().getDriversList());
//    }
    setCurrentDriver(currentDriverId);

    updateData();

    return QDialog::exec();
}

void FollowADriverDialog::show(int currentDriverId)
{
//    loadDriversData();
//    if (ui->comboBox->itemText(1) == "")// && eventData.driversData[0].driver != "")
//    {
//        ui->comboBox->addItems(SeasonData::getInstance().getDriversList());
//    }
    setCurrentDriver(currentDriverId);

    updateData();

    QDialog::show();
}

void FollowADriverDialog::updateData()
{
    int no = getNumber();
    DriverData *dd = eventData.getDriverDataPtr(no);
    setWindowTitle("Follow a driver");
    if (dd != 0 && dd->getCarID() > 0)
    {        
        setWindowTitle("Follow a driver - " + dd->getDriverName());
        updateButtonsState(*dd);
        printDriverInfo(*dd);

        QList<DriverData*> drivers = getDriversArray(dd->getPosition());
        printDataTable(*dd, drivers);
        printLapTimesTable(*dd, drivers);
    }
    else
        clearData(false);
}

void FollowADriverDialog::printDriverInfo(const DriverData &dd)
{    
    ui->carImageLabel->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(dd.getNumber(), thumbnailsSize));
    ui->bestLapLabel->setText(dd.getSessionRecords().getBestLap().getTime().toString() + QString(" (L%1)").arg(dd.getSessionRecords().getBestLap().getLapNumber()));

    QPalette palette;

    if (eventData.getEventType() == LTPackets::RACE_EVENT)
    {
        if (dd.getPosition() > 1)
            ui->gapLabel->setText(dd.getLastLap().getGap());
        else
            ui->gapLabel->setText("");

        palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::PIT));
        ui->pitsLabel->setText(QString::number(dd.getPitStops().size()));
        ui->pitsLabel->setPalette(palette);

        ui->pitStopsLabel->setText("Pit stops:");

        QPalette palette = ui->pitStopsLabel->palette();
        palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
        ui->pitStopsLabel->setPalette(palette);
    }
    else
    {
        QString gap = "";
        if (dd.getPosition() != 1)
        {
            DriverData *fd = eventData.getDriverDataByPosPtr(1);

            if (fd)
                gap = DriverData::calculateGap(dd.getLastLap().getTime(), fd->getLastLap().getTime());
        }
        ui->gapLabel->setText(gap);

        ui->pitsLabel->setText("");


        palette = ui->pitStopsLabel->palette();
        if (dd.getColorData().numberColor() == LTPackets::PIT)
        {
            ui->pitStopsLabel->setText("In pits");
            palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::PIT));
        }
        else
        {
            ui->pitStopsLabel->setText("On track");
            palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::GREEN));
        }
        ui->pitStopsLabel->setPalette(palette);
    }

    palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::GREEN));
    ui->bestLapLabel->setPalette(palette);

    palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::YELLOW));
    ui->gapLabel->setPalette(palette);


}

void FollowADriverDialog::printDataTable(const DriverData &dd, const QList<DriverData*> &drivers)
{
    for (int i = 0; i < drivers.size(); ++i)
    {
        if (drivers[i] != 0)
        {
            QBrush bg = QBrush();
            if (drivers[i]->getCarID() == dd.getCarID())
                bg = QBrush(QColor(50, 50, 50));

            QString interval = "";
            double dInterval = 0.0;
            bool ok;
            LapData ld = drivers[i]->getLastLap();
            LapData ldCurr = dd.getLastLap();

            if (eventData.getEventType() != LTPackets::RACE_EVENT)
            {
                ld = drivers[i]->getSessionRecords().getBestLap();
                ldCurr = dd.getSessionRecords().getBestLap();
            }
            if (eventData.getEventType() == LTPackets::QUALI_EVENT)
            {
                if (drivers[i]->getSessionRecords().getBestQualiLap(eventData.getQualiPeriod()).getTime().isValid())
                    ld = drivers[i]->getSessionRecords().getBestQualiLap(eventData.getQualiPeriod());

                if (dd.getSessionRecords().getBestQualiLap(eventData.getQualiPeriod()).getTime().isValid())
                    ldCurr = dd.getSessionRecords().getBestQualiLap(eventData.getQualiPeriod());
            }

            QString time = ld.getTime().toString();
            QColor colors[5];
            colors[0] = SeasonData::getInstance().getColor(LTPackets::VIOLET);

            if (drivers[i]->getCarID() != dd.getCarID())
            {                
                if (eventData.getEventType() == LTPackets::RACE_EVENT)
                    interval = eventData.calculateInterval(dd, *drivers[i], -1);//dd.getLapData().last().getLapNumber());
                else
                    interval = DriverData::calculateGap(ld.getTime(), ldCurr.getTime());

                dInterval = interval.toDouble(&ok);

                if (ld.getTime().isValid() && ldCurr.getTime().isValid())
                {
                    QString gap = DriverData::calculateGap(ld.getTime(), ldCurr.getTime());
                    colors[1] = SeasonData::getInstance().getColor(LTPackets::GREEN);
                    if (gap.size() > 0 && gap[0] != '-')
                    {
                        gap = "+" + gap;
                        colors[1] = SeasonData::getInstance().getColor(LTPackets::RED);
                    }
                    time += " ("+gap+")";
                }
                else if (!time.contains("IN PIT") && !time.contains("OUT"))
                    colors[1] = SeasonData::getInstance().getColor(LTPackets::GREEN);
                else if (time.contains("LAP"))
                    colors[1] = SeasonData::getInstance().getColor(LTPackets::RED);
                else
                {
                    time = QString("IN PIT (%1)").arg(drivers[i]->getPitTime(ld.getLapNumber())) ;
                    colors[1] = SeasonData::getInstance().getColor(LTPackets::RED);
                }

                for (int j = 1; j <= 3; ++j)
                {
                    if ((ld.getSectorTime(j).toDouble() < ldCurr.getSectorTime(j).toDouble()) ||
                         dd.getLastLap().getSectorTime(j).toDouble() == 0.0)
                        colors[j+1] = SeasonData::getInstance().getColor(LTPackets::GREEN);

                    else
                        colors[j+1] = SeasonData::getInstance().getColor(LTPackets::RED);

                    if (ok && eventData.getEventType() == LTPackets::RACE_EVENT &&
                            ld.getLapNumber() == ldCurr.getLapNumber() &&
                            ldCurr.getSectorTime(3).toDouble() == 0 && ld.getSectorTime(3).toDouble() == 0 &&
                            ldCurr.getSectorTime(j).toDouble() != 0 && ld.getSectorTime(j).toDouble() != 0)

                    {
                        dInterval += ldCurr.getSectorTime(j).toDouble() - ld.getSectorTime(j).toDouble();
                    }
                }
                if (ok)
                {
                    interval = QString::number(dInterval, 'f', 1);
                    if (dInterval > 0)
                        interval = "+" + interval;
                }

                if (interval.size() > 0 && interval[0] == '+')
                    colors[0] = SeasonData::getInstance().getColor(LTPackets::RED);
            }
            else
            {
                for (int j = 0; j < 5; ++j)
                    colors[j] = SeasonData::getInstance().getColor(LTPackets::WHITE);

                if (time.contains("IN PIT") || time.contains("OUT"))
                {
                    colors[1] = SeasonData::getInstance().getColor(LTPackets::RED);
                    time = QString("IN PIT (%1)").arg(drivers[i]->getPitTime(ld.getLapNumber())) ;
                }
            }


            setItem(ui->dataTableWidget, i+1, 0, QString::number(drivers[i]->getPosition()), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter,
                    SeasonData::getInstance().getColor(LTPackets::CYAN), bg);
            setItem(ui->dataTableWidget, i+1, 1, drivers[i]->getDriverName(), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter,
                    SeasonData::getInstance().getColor(LTPackets::WHITE), bg);
            setItem(ui->dataTableWidget, i+1, 2, interval, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                    colors[0], bg);
            setItem(ui->dataTableWidget, i+1, 3, time, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                    colors[1], bg);

            for (int j = 1; j <= 3; ++j)
            {
                setItem(ui->dataTableWidget, i+1, 3+j, ld.getSectorTime(j).toString(), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                        colors[j+1], bg);
            }
        }
        else
            clearRow(i+1);
    }
}
void FollowADriverDialog::printLapTimesTable(const DriverData &dd, const QList<DriverData*> &drivers)
{
//    int lap = eventData.getCompletedLaps();

    for (int i = 0; i < drivers.size(); ++i)
    {
        if (drivers[i] != 0)
            setItem(ui->lapTimesTableWidget, 0, i+1, QString("%1 %2").arg(drivers[i]->getPosition()).arg(drivers[i]->getDriverName()),
                    Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::WHITE));
    }
    int i, j, k;
//    for (i = lap, k = 0; i >= lap-5; --i, ++k)
    for (i = 0, k = dd.getLapData().size()-1; k >= 0 && i < 5; --k, ++i)
    {
        int lapNo = dd.getLapData()[k].getLapNumber();

        setItem(ui->lapTimesTableWidget, i+1, 0, QString("%1.").arg(lapNo), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                SeasonData::getInstance().getColor(LTPackets::DEFAULT));
        for (j = 0; j < drivers.size(); ++j)
        {
            if (drivers[j] == 0)
                continue;

            LapData ld = drivers[j]->getLapData(lapNo);

            QString time = ld.getTime().toString();
            QColor color = SeasonData::getInstance().getColor(LTPackets::WHITE);
            if (drivers[j]->getCarID() != dd.getCarID())
            {
                if (ld.getTime().isValid() && dd.getLapData()[k].getTime().isValid())
                {
                    QString gap = DriverData::calculateGap(ld.getTime(), dd.getLapData()[k].getTime());
                    color = SeasonData::getInstance().getColor(LTPackets::GREEN);
                    if (gap.size() > 0 && gap[0] != '-')
                    {
                        color = SeasonData::getInstance().getColor(LTPackets::RED);
                        gap = "+" + gap;
                    }
                    time += " (" + gap + ")";
                }
                else
                {
                    if (time == "IN PIT" || time == "OUT")
                    {
                        color = SeasonData::getInstance().getColor(LTPackets::RED);
                        time = QString("IN PIT (%1)").arg(drivers[j]->getPitTime(ld.getLapNumber())) ;
                    }
                    else if (time.contains("LAP"))
                        color = SeasonData::getInstance().getColor(LTPackets::RED);
                    else
                        color = SeasonData::getInstance().getColor(LTPackets::GREEN);
                }
            }
            else if (!dd.getLapData()[k].getTime().isValid())
            {
                color = SeasonData::getInstance().getColor(LTPackets::RED);
                if (time == "IN PIT" || time == "OUT")
                    time = QString("IN PIT (%1)").arg(drivers[j]->getPitTime(ld.getLapNumber())) ;
            }

            if (ld.getCarID() == drivers[j]->getCarID())
                setItem(ui->lapTimesTableWidget, i+1, j+1, time, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, color);
            else
                setItem(ui->lapTimesTableWidget, i+1, j+1, "", Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
    }
    for (; i < 5; ++i)
        for (j = 0; j < 6; ++j)
            setItem(ui->lapTimesTableWidget, i+1, j, "", Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void FollowADriverDialog::clearData(bool clearDriverList)
{
    for (int i = 0; i <= 5; ++i)
    {
        if (i > 0)
            clearRow(i);

        for (int j = 0; j < 6; ++j)
        {
            if (i > 0 || (i == 0 && j > 0))
                setItem(ui->lapTimesTableWidget, i, j, "", Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
        ui->carImageLabel->setPixmap(QPixmap());
        ui->bestLapLabel->setText("");
        ui->pitsLabel->setText("");
        ui->gapLabel->setText("");

        if (eventData.getEventType() != LTPackets::RACE_EVENT)
            ui->pitStopsLabel->setText("");
    }
    if (clearDriverList)
        ui->comboBox->clear();
}

void FollowADriverDialog::clearRow(int row)
{
    for (int i = 0; i < 7; ++i)
        setItem(ui->dataTableWidget, row, i, "", Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void FollowADriverDialog::comboBoxValueChanged(int)
{
    updateData();
}

void FollowADriverDialog::setFont(const QFont &font)
{
    ui->dataFrame->setFont(font);
    ui->dataTableWidget->setFont(font);
    ui->lapTimesTableWidget->setFont(font);
}

bool FollowADriverDialog::driverInRange(const DriverData &dd)
{
    int no = getNumber();
    DriverData *cd = eventData.getDriverDataPtr(no);

    if (cd != 0)
    {
        QList<DriverData*> drivers = getDriversArray(cd->getPosition());
        for (int i = 0; i < drivers.size(); ++i)
        {
            if (drivers[i] != 0 && drivers[i]->getCarID() == dd.getCarID())
                return true;
        }
    }
    return false;
}

QList<DriverData*> FollowADriverDialog::getDriversArray(int pos)
{
    int first = pos - 2;
    int last = pos + 2;

    if (first <= 0)
    {
        last = 5;
        first = 1;
    }
    if (last > eventData.getDriversData().size())
    {
        last = eventData.getDriversData().size();
        first = last - 4;
    }

    DriverData *cd = eventData.getDriverDataByPosPtr(pos);
    bool lastIsRetired = false;
    do
    {
        DriverData *dd = eventData.getDriverDataByPosPtr(last);
        if (dd != 0 && dd->isRetired() && !cd->isRetired() && first > 0)
        {
            lastIsRetired = true;
            --last;
            --first;
        }
        else
            lastIsRetired = false;

    } while(lastIsRetired);

    QList<DriverData*> drivers;
    int i, j;
    for (i = first, j = 0; i <= last; ++i, ++j)
    {
        drivers << eventData.getDriverDataByPosPtr(i);
    }

    return drivers;
}

void FollowADriverDialog::setCurrentDriver(int id)
{
    if (id != 0)
    {
        DriverData dd = eventData.getDriverDataById(id);
        if (dd.getCarID() > 0)
        {
            int idx = ui->comboBox->findText(QString("%1 %2").arg(dd.getNumber()).arg(dd.getDriverName()));
            if (idx != -1)
                ui->comboBox->setCurrentIndex(idx);
        }
    }
}

int FollowADriverDialog::getNumber()
{
    QString text = ui->comboBox->currentText();

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

void FollowADriverDialog::updateButtonsState(const DriverData &dd)
{
    if (dd.getPosition() == 1 && ui->leftButton->isEnabled())
        ui->leftButton->setEnabled(false);

    else if (dd.getPosition() != 1 && !ui->leftButton->isEnabled())
        ui->leftButton->setEnabled(true);

    if (dd.getPosition() == eventData.getDriversData().size() && ui->rightButton->isEnabled())
        ui->rightButton->setEnabled(false);

    else if (dd.getPosition() < eventData.getDriversData().size() && !ui->rightButton->isEnabled())
        ui->rightButton->setEnabled(true);
}

void FollowADriverDialog::resizeEvent(QResizeEvent *)
{
    int w = ui->dataTableWidget->width();

    ui->dataTableWidget->setColumnWidth(0, 0.06*w);
    ui->dataTableWidget->setColumnWidth(1, 0.25*w);
    ui->dataTableWidget->setColumnWidth(2, 0.1*w);
    ui->dataTableWidget->setColumnWidth(3, 0.29*w);
    ui->dataTableWidget->setColumnWidth(4, 0.1*w);
    ui->dataTableWidget->setColumnWidth(5, 0.1*w);
    ui->dataTableWidget->setColumnWidth(6, 0.1*w);

    w = ui->lapTimesTableWidget->width();

    ui->lapTimesTableWidget->setColumnWidth(0, 0.05*w);
    ui->lapTimesTableWidget->setColumnWidth(1, 0.19*w);
    ui->lapTimesTableWidget->setColumnWidth(2, 0.19*w);
    ui->lapTimesTableWidget->setColumnWidth(3, 0.19*w);
    ui->lapTimesTableWidget->setColumnWidth(4, 0.19*w);
    ui->lapTimesTableWidget->setColumnWidth(5, 0.19*w);
}

void FollowADriverDialog::on_leftButton_clicked()
{
    int no = getNumber();
    DriverData *dd = eventData.getDriverDataPtr(no);
    if (dd != 0 && dd->getPosition() > 1)
    {
        DriverData *ddPtr = eventData.getDriverDataByPosPtr(dd->getPosition()-1);
        if (ddPtr)
        {
            setCurrentDriver(ddPtr->getCarID());
//            updateButtonsState(*ddPtr);
        }
    }
}

void FollowADriverDialog::on_rightButton_clicked()
{
    int no = getNumber();
    DriverData *dd = eventData.getDriverDataPtr(no);
    if (dd->getPosition() < eventData.getDriversData().size())
    {
        DriverData *ddPtr = eventData.getDriverDataByPosPtr(dd->getPosition()+1);
        if (ddPtr)
        {
            setCurrentDriver(ddPtr->getCarID());
//            updateButtonsState(*ddPtr);
        }
    }
}

void FollowADriverDialog::on_dataTableWidget_cellDoubleClicked(int row, int)
{
    QTableWidgetItem *item = ui->dataTableWidget->item(row, 0);
    if (item)
    {
        DriverData *dd = eventData.getDriverDataByPosPtr(item->text().toInt());
        if (dd)
        {
            setCurrentDriver(dd->getCarID());
            QList<QTableWidgetItem *> items = ui->dataTableWidget->findItems(dd->getDriverName(), Qt::MatchExactly);
            if (!items.isEmpty())
                ui->dataTableWidget->setCurrentItem(items.first());
        }
    }
}

void FollowADriverDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
    {
//        QItemSelectionModel * selection = ui->dataTableWidget->selectionModel();
        QModelIndexList indexes[2];
        indexes[0] = ui->dataTableWidget->selectionModel()->selectedIndexes();
        indexes[1] = ui->lapTimesTableWidget->selectionModel()->selectedIndexes();

        QTableWidget *table[2];
        table[0] = ui->dataTableWidget;
        table[1] = ui->lapTimesTableWidget;

        QString selected_text;
        for (int i = 0; i < 2; ++i)
        {
            if (indexes[i].size() < 1)
                continue;

            qSort(indexes[i].begin(), indexes[i].end());

            QModelIndex previous = indexes[i].first();
            indexes[i].removeFirst();
            QModelIndex current;
            Q_FOREACH(current, indexes[i])
            {
                QVariant data = table[i]->model()->data(previous);
                QString text = data.toString();

                selected_text.append(text);

                if (current.row() != previous.row())
                    selected_text.append(QLatin1Char('\n'));

                else
                    selected_text.append(QLatin1Char('\t'));

                previous = current;
            }
            selected_text.append(table[i]->model()->data(current).toString());
            selected_text.append(QLatin1Char('\n'));
        }
        qApp->clipboard()->setText(selected_text);

    }
    if (event->key() == Qt::Key_Escape)
        close();
}
