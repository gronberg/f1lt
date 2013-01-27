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


#include "sessiondatawidget.h"
#include "ui_sessiondatawidget.h"

#include <QClipboard>
#include <QDebug>
#include <QLabel>
#include <QPair>
#include <QResizeEvent>

#include "ltitemdelegate.h"

#include "../core/colorsmanager.h"
#include "../core/trackrecords.h"

SessionDataWidget::SessionDataWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SessionDataWidget), eventData(EventData::getInstance())
{
    ui->setupUi(this);


    ui->speedRecordsTable->setModel(&speedRecordsModel);
    ui->fastestLapsTable->setModel(&fastestLapsModel);
    ui->pitStopsTable->setModel(&pitStopsModel);    

    ui->speedRecordsTable->setItemDelegate(new LTItemDelegate(this));
    ui->fastestLapsTable->setItemDelegate(new LTItemDelegate(this));
    ui->pitStopsTable->setItemDelegate(new LTItemDelegate(this));    

    for (int i = 0; i < speedRecordsModel.rowCount(); ++i)
        ui->speedRecordsTable->setRowHeight(i, 22);

    for (int i = 0; i < fastestLapsModel.rowCount(); ++i)
        ui->fastestLapsTable->setRowHeight(i, 22);

    for (int i = 0; i < pitStopsModel.rowCount(); ++i)
        ui->pitStopsTable->setRowHeight(i, 22);    

    setupContents();    
}


SessionDataWidget::~SessionDataWidget()
{
    delete ui;
}

QTableWidgetItem* SessionDataWidget::setItem(QTableWidget *table, int row, int col, QString text, Qt::ItemFlags flags, int align,
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

void SessionDataWidget::setupContents()
{
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

void SessionDataWidget::updateData()
{
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    updateFastestLaps();
    switch (ui->tabWidget->currentIndex())
    {
        case 0:
            updateEventInfo();
        case 2:
            updateSpeedRecords();            
            break;

//        case 1:
//            updateFastestLaps(carId);
//            break;

        case 3:
            updatePitStops();
            break;
    }    
}

void SessionDataWidget::updateEventInfo()
{
    if (eventData.getEventInfo().eventNo < 1)
        return;

    LTEvent event = eventData.getEventInfo();

    QPalette palette;
    ui->eventNameLabel->setText(event.eventName);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::YELLOW));
    ui->eventNameLabel->setPalette(palette);

    ui->eventPlaceLabel->setText(event.eventPlace);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::GREEN));
    ui->eventPlaceLabel->setPalette(palette);

    ui->eventDateLabel->setText(event.fpDate.toString("dd.MM.yyyy") + " - " + event.raceDate.toString("dd.MM.yyyy"));
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::WHITE));
    ui->eventDateLabel->setPalette(palette);

    ui->eventLapsLabel->setText(QString::number(event.laps) + " laps");
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::CYAN));
    ui->eventLapsLabel->setPalette(palette);

//    if (ui->eventMapLabel->pixmap() == 0 || ui->eventMapLabel->pixmap()->isNull())
    {
        QPixmap trackImg = eventData.getEventInfo().trackImg;
        if (!trackImg.isNull())
        {
            QPixmap pix = trackImg;
            int w = ui->scrollArea_2->width() - 20;
            int h = ui->scrollArea_2->height() - 20;

            if (pix.width() > w)
                pix = trackImg.scaledToWidth(w, Qt::SmoothTransformation);

            if (pix.height() > h)
                pix = trackImg.scaledToHeight(h, Qt::SmoothTransformation);

            ui->eventMapLabel->setPixmap(pix);
        }
    }

    Track *track = 0;
    TrackVersion *tv = 0;
    TrackWeekendRecords *twr = 0;

    TrackRecords::getInstance().getCurrentTrackRecords(&track, &twr, &tv);

    if (tv != 0)
    {
        ui->qRTLabel->setText(tv->trackRecords[QUALI_RECORD].time.toString());
        ui->qRDLabel->setText(tv->trackRecords[QUALI_RECORD].driver);
        ui->qRDTLabel->setText(tv->trackRecords[QUALI_RECORD].team);
        ui->qRYLabel->setText(tv->trackRecords[QUALI_RECORD].year);

        ui->rRTLabel->setText(tv->trackRecords[RACE_RECORD].time.toString());
        ui->rRDLabel->setText(tv->trackRecords[RACE_RECORD].driver);
        ui->rRDTLabel->setText(tv->trackRecords[RACE_RECORD].team);
        ui->rRYLabel->setText(tv->trackRecords[RACE_RECORD].year);

        QPalette palette;
        palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::VIOLET));
        ui->qRTLabel->setPalette(palette);
        ui->rRTLabel->setPalette(palette);

        palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::WHITE));
        ui->qRDLabel->setPalette(palette);
        ui->qRDTLabel->setPalette(palette);
        ui->rRDLabel->setPalette(palette);
        ui->rRDTLabel->setPalette(palette);

        palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::YELLOW));
        ui->qRYLabel->setPalette(palette);
        ui->rRYLabel->setPalette(palette);
    }
    else
    {
        ui->qRTLabel->clear();
        ui->qRDLabel->clear();
        ui->qRDTLabel->clear();
        ui->qRYLabel->clear();

        ui->rRTLabel->clear();
        ui->rRDLabel->clear();
        ui->rRDTLabel->clear();
        ui->rRYLabel->clear();

    }
//    QTableWidgetItem *item;
//

//    item = setItem(ui->tableWidget_5, 0, 0, event.eventName, Qt::NoItemFlags, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::YELLOW));
//    item->setFont(QFont("Arial", 15, QFont::Bold));

//    item = setItem(ui->tableWidget_5, 1, 0, event.eventPlace, Qt::NoItemFlags, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::GREEN));
//    item->setFont(QFont("Arial", 12, QFont::Bold));

//    item = setItem(ui->tableWidget_5, 2, 0, event.fpDate.toString("dd.MM.yyyy") + " - " + event.raceDate.toString("dd.MM.yyyy"),
//                   Qt::NoItemFlags, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::WHITE));
//    item->setFont(QFont("Arial", 10, QFont::Bold));

//    item = setItem(ui->tableWidget_5, 3, 0, QString::number(event.laps) + " laps", Qt::NoItemFlags, Qt::AlignCenter, SeasonData::getInstance().getColor(LTPackets::CYAN));


//    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget_5->cellWidget(4, 0));
//    if (!lab)
//    {
//        lab = new QLabel(this);
//        ui->tableWidget_5->setCellWidget(4, 0, lab);
//    }
////    if (!lab->pixmap())
//    {
//        QPixmap trackImg = eventData.getEventInfo().trackImg;
//        if (!trackImg.isNull())
//        {
//        	QPixmap pix = trackImg;
//        	int w = width() - 20;
//        	int h = height() - 20;

//        	if (pix.width() > w)
//        		pix = trackImg.scaledToWidth(w, Qt::SmoothTransformation);

//        	if (pix.height() > h)
//                pix = trackImg.scaledToHeight(h, Qt::SmoothTransformation);

//        	lab->setPixmap(pix);

////            lab->resize(trackImg.size());
//            lab->setAlignment(Qt::AlignCenter);
//            if (ui->tableWidget_5->rowHeight(4) < trackImg.height())
//                ui->tableWidget_5->setRowHeight(4,trackImg.height());

//            ui->tableWidget_5->setColumnWidth(0, trackImg.width());
//        }
//    }
}

void SessionDataWidget::updateSpeedRecords()
{
    speedRecordsModel.update();
}

void SessionDataWidget::updateFastestLaps()
{
    fastestLapsModel.update();
    ui->fastestLapsTable->setMinimumSize(QSize(ui->fastestLapsTable->minimumWidth(), fastestLapsModel.rowCount() * 22));

    QPalette palette;
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::VIOLET));
    QList<LapData> fastestLaps = fastestLapsModel.getFastestLaps();

    QString str = eventData.getSessionRecords().getFastestLap().getTime();
    if (eventData.getEventType() != LTPackets::RACE_EVENT && !fastestLaps.isEmpty())
        str = fastestLaps[0].getTime().toString();

    ui->flTimeLabel->setText(str);
    ui->flTimeLabel->setPalette(palette);

    str = eventData.getSessionRecords().getFastestLap().getDriverName();
    if (eventData.getEventType() != LTPackets::RACE_EVENT && !fastestLaps.isEmpty() && fastestLaps[0].getCarID() != -1)
    {
        DriverData *dd = eventData.getDriverDataByIdPtr(fastestLaps[0].getCarID());
        if (dd  != 0)
            str = dd->getDriverName();
    }

    ui->flDriverLabel->setText(str);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::WHITE));
    ui->flDriverLabel->setPalette(palette);

    str = QString("L%1").arg(eventData.getSessionRecords().getFastestLap().getLapNumber());
    if (eventData.getSessionRecords().getFastestLap().getLapNumber() <= 0)
        str = "";

    if (eventData.getEventType() == LTPackets::PRACTICE_EVENT && !fastestLaps.isEmpty() && fastestLaps[0].getTime().isValid())
        str = SeasonData::getInstance().getSessionDefaults().correctFPTime(fastestLaps[0].getPracticeLapExtraData().getSessionTime()).toString("h:mm:ss");

    else if (eventData.getEventType() == LTPackets::QUALI_EVENT && !fastestLaps.isEmpty() && fastestLaps[0].getTime().isValid())
    {
        QString time = SeasonData::getInstance().getSessionDefaults().correctQualiTime(fastestLaps[0].getQualiLapExtraData().getSessionTime(), fastestLaps[0].getQualiLapExtraData().getQualiPeriod()).toString("mm:ss");
        str = QString("%1 (Q%2)").arg(time).arg(fastestLaps[0].getQualiLapExtraData().getQualiPeriod());
    }

    ui->flLapLabel->setText(str);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::YELLOW));
    ui->flLapLabel->setPalette(palette);

    ui->s1TimeLabel->setText(eventData.getSessionRecords().getSectorRecord(1).getTime());
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::VIOLET));
    ui->s1TimeLabel->setPalette(palette);


    ui->s1DriverLabel->setText(eventData.getSessionRecords().getSectorRecord(1).getDriverName());
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::WHITE));
    ui->s1DriverLabel->setPalette(palette);

    str = "";    
    if ((eventData.getEventType() == LTPackets::RACE_EVENT && eventData.getSessionRecords().getSectorRecord(1).getLapNumber() > -1))
        str = QString("L%1").arg(eventData.getSessionRecords().getSectorRecord(1).getLapNumber());

    else if (eventData.getSessionRecords().getSectorRecord(1).getSessionTime().isValid())
    {
        if (eventData.getEventType() == LTPackets::PRACTICE_EVENT)
            str = SeasonData::getInstance().getSessionDefaults().correctFPTime(eventData.getSessionRecords().getSectorRecord(1).getSessionTime()).toString("h:mm:ss");
        else
        {
            QString time = SeasonData::getInstance().getSessionDefaults().correctQualiTime(eventData.getSessionRecords().getSectorRecord(1).getSessionTime(),
                                                                      eventData.getSessionRecords().getSectorRecord(1).getQualiPeriod()).toString("mm:ss");
            str = QString("%1 (Q%2)").arg(time).arg(eventData.getSessionRecords().getSectorRecord(1).getQualiPeriod());
        }
    }

    ui->s1LapLabel->setText(str);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::YELLOW));
    ui->s1LapLabel->setPalette(palette);


    ui->s2TimeLabel->setText(eventData.getSessionRecords().getSectorRecord(2).getTime());
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::VIOLET));
    ui->s2TimeLabel->setPalette(palette);

    ui->s2DriverLabel->setText(eventData.getSessionRecords().getSectorRecord(2).getDriverName());
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::WHITE));
    ui->s2DriverLabel->setPalette(palette);

    str = "";
    if ((eventData.getEventType() == LTPackets::RACE_EVENT && eventData.getSessionRecords().getSectorRecord(2).getLapNumber() > -1))
        str = QString("L%1").arg(eventData.getSessionRecords().getSectorRecord(2).getLapNumber());

    else if (eventData.getSessionRecords().getSectorRecord(2).getSessionTime().isValid())
    {
        if (eventData.getEventType() == LTPackets::PRACTICE_EVENT)
            str = SeasonData::getInstance().getSessionDefaults().correctFPTime(eventData.getSessionRecords().getSectorRecord(2).getSessionTime()).toString("h:mm:ss");
        else
        {
            QString time = SeasonData::getInstance().getSessionDefaults().correctQualiTime(eventData.getSessionRecords().getSectorRecord(2).getSessionTime(),
                                                                      eventData.getSessionRecords().getSectorRecord(2).getQualiPeriod()).toString("mm:ss");
            str = QString("%1 (Q%2)").arg(time).arg(eventData.getSessionRecords().getSectorRecord(2).getQualiPeriod());
        }
    }

    ui->s2LapLabel->setText(str);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::YELLOW));
    ui->s2LapLabel->setPalette(palette);

    ui->s3TimeLabel->setText(eventData.getSessionRecords().getSectorRecord(3).getTime());
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::VIOLET));
    ui->s3TimeLabel->setPalette(palette);

    ui->s3DriverLabel->setText(eventData.getSessionRecords().getSectorRecord(3).getDriverName());
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::WHITE));
    ui->s3DriverLabel->setPalette(palette);

    str = "";
    if ((eventData.getEventType() == LTPackets::RACE_EVENT && eventData.getSessionRecords().getSectorRecord(3).getLapNumber() > -1))
        str = QString("L%1").arg(eventData.getSessionRecords().getSectorRecord(3).getLapNumber());

    else if (eventData.getSessionRecords().getSectorRecord(3).getSessionTime().isValid())
    {
        if (eventData.getEventType() == LTPackets::PRACTICE_EVENT)
            str = SeasonData::getInstance().getSessionDefaults().correctFPTime(eventData.getSessionRecords().getSectorRecord(3).getSessionTime()).toString("h:mm:ss");
        else
        {
            QString time = SeasonData::getInstance().getSessionDefaults().correctQualiTime(eventData.getSessionRecords().getSectorRecord(3).getSessionTime(),
                                                                      eventData.getSessionRecords().getSectorRecord(3).getQualiPeriod()).toString("mm:ss");
            str = QString("%1 (Q%2)").arg(time).arg(eventData.getSessionRecords().getSectorRecord(3).getQualiPeriod());
        }
    }

    ui->s3LapLabel->setText(str);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::YELLOW));
    ui->s3LapLabel->setPalette(palette);

    QString tL = "";
    if (eventData.getSessionRecords().getSectorRecord(1).getTime().toString() != "" &&
        eventData.getSessionRecords().getSectorRecord(2).getTime().toString() != "" &&
        eventData.getSessionRecords().getSectorRecord(3).getTime().toString() != "")
        tL = LapData::sumSectors(eventData.getSessionRecords().getSectorRecord(1).getTime(),
                                 eventData.getSessionRecords().getSectorRecord(2).getTime(),
                                 eventData.getSessionRecords().getSectorRecord(3).getTime());

    ui->theoreticalTimeLabel->setText(tL);
    palette.setBrush(QPalette::Foreground, ColorsManager::getInstance().getColor(LTPackets::CYAN));
    ui->theoreticalTimeLabel->setPalette(palette);
}

void SessionDataWidget::updatePitStops(bool clear)
{
    if (clear)
    {
    }


    pitStopsModel.update();
    for (int i = 0; i < pitStopsModel.rowCount(); ++i)
        ui->pitStopsTable->setRowHeight(i, 22);
}


void SessionDataWidget::on_tabWidget_currentChanged(int index)
{
    int w;
    switch (index)
    {
        case 0:
//            updateSpeedRecords(eventData);
            updateEventInfo();
            break;

        case 1:

            w = ui->fastestLapsTable->viewport()->width();
            ui->fastestLapsTable->setColumnWidth(0, 0.06 * w);
            ui->fastestLapsTable->setColumnWidth(1, 0.28 * w);
            ui->fastestLapsTable->setColumnWidth(2, 0.16 * w);
            ui->fastestLapsTable->setColumnWidth(3, 0.11 * w);
            ui->fastestLapsTable->setColumnWidth(4, 0.09 * w);
            ui->fastestLapsTable->setColumnWidth(5, 0.09 * w);
            ui->fastestLapsTable->setColumnWidth(6, 0.09 * w);
            ui->fastestLapsTable->setColumnWidth(7, 0.12 * w);

            break;

        case 2:
//            updatePitStops(eventData);
            updateSpeedRecords();

            w = ui->speedRecordsTable->viewport()->width();
            ui->speedRecordsTable->setColumnWidth(0, 0.34 * w);
            ui->speedRecordsTable->setColumnWidth(1, 0.11 * w);
            ui->speedRecordsTable->setColumnWidth(2, 0.09 * w);
            ui->speedRecordsTable->setColumnWidth(3, 0.34 * w);
            ui->speedRecordsTable->setColumnWidth(4, 0.11 * w);
            break;

        case 3:
            updatePitStops();

            w = ui->pitStopsTable->viewport()->width();
            ui->pitStopsTable->setColumnWidth(0, 0.1 * w);
            ui->pitStopsTable->setColumnWidth(1, 0.1 * w);
            ui->pitStopsTable->setColumnWidth(2, 0.1 * w);
            ui->pitStopsTable->setColumnWidth(3, 0.35 * w);
            ui->pitStopsTable->setColumnWidth(4, 0.2 * w);
            ui->pitStopsTable->setColumnWidth(5, 0.15 * w);
        default:
            break;
    }
}

void SessionDataWidget::resizeEvent(QResizeEvent *)
{
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

void SessionDataWidget::setFont(const QFont &font)
{
    ui->speedRecordsTable->setFont(font);
    ui->fastestLapsTable->setFont(font);
    ui->pitStopsTable->setFont(font);
    ui->infoWidget->setFont(font);

    ui->recordsLabel->setFont(font);
    for (int i = 0; i < ui->recordsLayout->count(); ++i)
        ui->recordsLayout->itemAt(i)->widget()->setFont(font);
}

int SessionDataWidget::currentIndex()
{
    return ui->tabWidget->currentIndex();
}
void SessionDataWidget::setCurrentIndex(int i)
{
    ui->tabWidget->setCurrentIndex(i);
}

void SessionDataWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
    {
        QTableView *tWidget;
        if (ui->tabWidget->currentIndex() == 1)
            tWidget = ui->fastestLapsTable;

        else if (ui->tabWidget->currentIndex() == 2)
            tWidget = ui->speedRecordsTable;

        else if (ui->tabWidget->currentIndex() == 3)
            tWidget = ui->pitStopsTable;

        else
            return;

        QItemSelectionModel * selection = tWidget->selectionModel();
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
            QVariant data = tWidget->model()->data(previous);
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
        selected_text.append(tWidget->model()->data(current).toString());
        selected_text.append(QLatin1Char('\n'));
        qApp->clipboard()->setText(selected_text);
    }
}

void SessionDataWidget::clearData()
{
    setupContents();
}

void SessionDataWidget::clearFastestLaps()
{

}
