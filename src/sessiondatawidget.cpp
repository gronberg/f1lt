#include "sessiondatawidget.h"
#include "ui_sessiondatawidget.h"

#include <QClipboard>
#include <QLabel>
#include <QPair>
#include <QResizeEvent>

SessionDataWidget::SessionDataWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SessionDataWidget), eventData(EventData::getInstance())
{
    ui->setupUi(this);

    int numDrivers = LTData::ltTeams.size()*2;
    for (int i = 0; i < numDrivers+7; ++i)
    {
        if (i < 15)
            ui->tableWidget_2->insertRow(i);

        ui->tableWidget_3->insertRow(i);
        ui->tableWidget_3->setRowHeight(i, 20);

        if (i < 1)
            ui->tableWidget_4->insertRow(i);

        if (i < numDrivers)
            bestLaps.append(LapData());

        if (i < 5)
            ui->tableWidget_5->insertRow(i);

    }
    setupContents();
}


SessionDataWidget::~SessionDataWidget()
{
    delete ui;
}

void SessionDataWidget::setupContents()
{
    ui->tableWidget_5->setColumnWidth(0, ui->tableWidget_5->width());

    QTableWidgetItem *item;



    item = ui->tableWidget_3->item(0, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Fastest lap:");
        ui->tableWidget_3->setItem(0, 0, item);
    }
    else
        item->setText("Fastest lap:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(0, 0, 1, 2);

    ui->tableWidget_3->setRowHeight(0, 20);

    item = ui->tableWidget_3->item(1, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Fastest sector 1:");
        ui->tableWidget_3->setItem(1, 0, item);
    }
    else
        item->setText("Fastest sector 1:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(1, 0, 1, 2);

    ui->tableWidget_3->setRowHeight(1, 20);

    item = ui->tableWidget_3->item(2, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Fastest sector 2:");
        ui->tableWidget_3->setItem(2, 0, item);
    }
    else
        item->setText("Fastest sector 2:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(2, 0, 1, 2);

    ui->tableWidget_3->setRowHeight(2, 20);

    item = ui->tableWidget_3->item(3, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Fastest sector 3:");
        ui->tableWidget_3->setItem(3, 0, item);
    }
    else
        item->setText("Fastest sector 3:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(3, 0, 1, 2);

    ui->tableWidget_3->setRowHeight(3, 20);

    item = ui->tableWidget_3->item(4, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Theoretical lap time:");
        ui->tableWidget_3->setItem(4, 0, item);
    }
    else
        item->setText("Theoretical lap time:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(4, 0, 1, 2);

    ui->tableWidget_3->setRowHeight(4, 20);

    item = ui->tableWidget_3->item(5, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Best laps");
        ui->tableWidget_3->setItem(5, 0, item);
    }
    else
        item->setText("Best laps");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(5, 0, 1, 7);

    item = ui->tableWidget_3->item(6, 0);
    if (!item)
    {
        item = new QTableWidgetItem("P");
        ui->tableWidget_3->setItem(6, 0, item);
    }
    else
        item->setText("P");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 1);
    if (!item)
    {
        item = new QTableWidgetItem("Name");
        ui->tableWidget_3->setItem(6, 1, item);
    }
    else
        item->setText("Name");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 2);
    if (!item)
    {
        item = new QTableWidgetItem("Time");
        ui->tableWidget_3->setItem(6, 2, item);
    }
    else
        item->setText("Time");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 3);
    if (!item)
    {
        item = new QTableWidgetItem("Gap");
        ui->tableWidget_3->setItem(6, 3, item);
    }
    else
        item->setText("Gap");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 4);
    if (!item)
    {
        item = new QTableWidgetItem("S1");
        ui->tableWidget_3->setItem(6, 4, item);
    }
    else
        item->setText("S1");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 5);
    if (!item)
    {
        item = new QTableWidgetItem("S2");
        ui->tableWidget_3->setItem(6, 5, item);
    }
    else
        item->setText("S2");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 6);
    if (!item)
    {
        item = new QTableWidgetItem("S3");
        ui->tableWidget_3->setItem(6, 6, item);
    }
    else
        item->setText("S3");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = ui->tableWidget_3->item(6, 7);
    if (!item)
    {
        item = new QTableWidgetItem("Lap");
        ui->tableWidget_3->setItem(6, 7, item);
    }
    else
        item->setText("Lap");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);

    item = new QTableWidgetItem("Sector 1");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(0, 0, item);

    item = new QTableWidgetItem("KPH");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(0, 1, item);

    item = new QTableWidgetItem("Sector 2");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(0, 3, item);

    item = new QTableWidgetItem("KPH");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(0, 4, item);

    item = new QTableWidgetItem("Sector 3");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(8, 0, item);

    item = new QTableWidgetItem("KPH");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(8, 1, item);

    item = new QTableWidgetItem("Speed trap");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(8, 3, item);

    item = new QTableWidgetItem("KPH");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_2->setItem(8, 4, item);


    item = new QTableWidgetItem("");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_4->setItem(0, 0, item);

    item = new QTableWidgetItem("Lap");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_4->setItem(0, 1, item);

//    item = new QTableWidgetItem("No");
//    item->setFlags(Qt::NoItemFlags);
//    item->setTextAlignment(Qt::AlignRight);
//    item->setTextColor(LTData::colors[LTData::DEFAULT]);
//    ui->tableWidget_4->setItem(0, 2, item);

    item = new QTableWidgetItem("Name");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_4->setItem(0, 3, item);

    item = new QTableWidgetItem("Time");
//    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_4->setItem(0, 4, item);

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
    if (eventData.eventInfo.eventNo < 1)
        return;
    QTableWidgetItem *item;
    LTEvent event = eventData.eventInfo;

    item = ui->tableWidget_5->item(0, 0);
    if (!item)
    {
        item = new QTableWidgetItem(event.eventName);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Arial", 15, QFont::Bold));
        item->setFlags(Qt::NoItemFlags);
        item->setTextColor(LTData::colors[LTData::YELLOW]);
        ui->tableWidget_5->setItem(0, 0, item);
    }
    else
        item->setText(event.eventName);

    item = ui->tableWidget_5->item(1, 0);
    if (!item)
    {
        item = new QTableWidgetItem(event.eventPlace);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Arial", 12, QFont::Bold));
        item->setFlags(Qt::NoItemFlags);
        item->setTextColor(LTData::colors[LTData::GREEN]);
        ui->tableWidget_5->setItem(1, 0, item);
    }
    else
        item->setText(event.eventPlace);

    item = ui->tableWidget_5->item(2, 0);
    if (!item)
    {
        item = new QTableWidgetItem(event.fpDate.toString("dd.MM.yyyy") + " - " + event.raceDate.toString("dd.MM.yyyy"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Arial", 10, QFont::Bold));
        item->setFlags(Qt::NoItemFlags);
        item->setTextColor(LTData::colors[LTData::WHITE]);
        ui->tableWidget_5->setItem(2, 0, item);
    }
    else
        item->setText(event.fpDate.toString("dd.MM.yyyy") + " - " + event.raceDate.toString("dd.MM.yyyy"));

    item = ui->tableWidget_5->item(3, 0);
    if (!item)
    {
        item = new QTableWidgetItem(QString::number(event.laps) + " laps");
        item->setTextAlignment(Qt::AlignCenter);
//        item->setFont(QFont("Arial", 15, QFont::Bold));
        item->setFlags(Qt::NoItemFlags);
        item->setTextColor(LTData::colors[LTData::CYAN]);
        ui->tableWidget_5->setItem(3, 0, item);
    }
    else
        item->setText(QString::number(event.laps) + " laps");

    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget_5->cellWidget(4, 0));
    if (!lab)
    {
        lab = new QLabel(this);
        ui->tableWidget_5->setCellWidget(4, 0, lab);
    }
//    if (!lab->pixmap())
    {
        QPixmap trackImg = eventData.eventInfo.trackImg;
        if (!trackImg.isNull())
        {
            lab->setPixmap(trackImg);
            lab->resize(trackImg.size());
            lab->setAlignment(Qt::AlignCenter);
            if (ui->tableWidget_5->rowHeight(4) < trackImg.height())
                ui->tableWidget_5->setRowHeight(4,trackImg.height());

            ui->tableWidget_5->setColumnWidth(0, trackImg.width());
        }
    }
}

void SessionDataWidget::updateSpeedRecords()
{
    QTableWidgetItem *item;


    for (int i = 0, j = 0; i < 6; i+=1, j += 2)
    {
        item = ui->tableWidget_2->item(1+i, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(eventData.sec1Speed[j]));
            ui->tableWidget_2->setItem(1+i, 0, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(eventData.sec1Speed[j]);


        item = ui->tableWidget_2->item(1+i, 1);
        if (!item)
        {
            item = new QTableWidgetItem(eventData.sec1Speed[j+1]);
            ui->tableWidget_2->setItem(1+i, 1, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(eventData.sec1Speed[j+1]);


        item = ui->tableWidget_2->item(1+i, 3);
        if (!item)
        {            
            item = new QTableWidgetItem(QString("%1").arg(eventData.sec2Speed[j]));

            ui->tableWidget_2->setItem(1+i, 3, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(eventData.sec2Speed[j]);


        item = ui->tableWidget_2->item(1+i, 4);
        if (!item)
        {
            item = new QTableWidgetItem(eventData.sec2Speed[j+1]);
            ui->tableWidget_2->setItem(1+i, 4, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(eventData.sec2Speed[j+1]);

        ui->tableWidget_2->setRowHeight(1+i, 20);

        item = ui->tableWidget_2->item(9+i, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(eventData.sec3Speed[j]));
            ui->tableWidget_2->setItem(9+i, 0, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(eventData.sec3Speed[j]);


        item = ui->tableWidget_2->item(9+i, 1);
        if (!item)
        {
            item = new QTableWidgetItem(eventData.sec3Speed[j+1]);
            ui->tableWidget_2->setItem(9+i, 1, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(eventData.sec3Speed[j+1]);


        item = ui->tableWidget_2->item(9+i, 3);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(eventData.speedTrap[j]));
            ui->tableWidget_2->setItem(9+i, 3, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(eventData.speedTrap[j]);


        item = ui->tableWidget_2->item(9+i, 4);
        if (!item)
        {
            item = new QTableWidgetItem(eventData.speedTrap[j+1]);
            ui->tableWidget_2->setItem(9+i, 4, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(eventData.speedTrap[j+1]);

        ui->tableWidget_2->setRowHeight(9+i, 20);
    }   
}

void SessionDataWidget::updateFastestLaps()
{
    for (int i = 0; i < eventData.driversData.size() && i < bestLaps.size(); ++i)
    {
        if (eventData.driversData[i].bestLap.lapTime.isValid())
            bestLaps[i] = eventData.driversData[i].bestLap;
        else
            bestLaps[i] = LapData();

//        if (bestLaps[i].carID > 0 && eventData.driversData[bestLaps[i].carID-1].driver == eventData.FLDriver && bestLaps[i].lapTime != eventData.FLTime)
//        {
//            bestLaps[i].lapTime = eventData.FLTime;
//            bestLaps[i].numLap = eventData.FLLap;
////            bestLaps[i].sector1 = LapTime();
////            bestLaps[i].sector2 = LapTime();
////            bestLaps[i].sector3 = LapTime();
//        }
    }
    qSort(bestLaps);


    QTableWidgetItem *item;

    QString str = eventData.FLTime;
    if (eventData.eventType != LTData::RACE_EVENT && !bestLaps.isEmpty())
        str = bestLaps[0].lapTime.toString();

    item = ui->tableWidget_3->item(0, 2);
    if (!item)
    {
        item = new QTableWidgetItem(str);
        ui->tableWidget_3->setItem(0, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
    }
    else
        item->setText(str);

    item->setTextColor(LTData::colors[LTData::WHITE]);


    str = eventData.FLDriver;
    if (eventData.eventType != LTData::RACE_EVENT && !bestLaps.isEmpty() && bestLaps[0].carID != -1)
        str = eventData.driversData[bestLaps[0].carID-1].driver;

    item = ui->tableWidget_3->item(0, 3);
    if (!item)
    {
        item = new QTableWidgetItem(str);
        ui->tableWidget_3->setItem(0, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
        item->setText(str);

    item->setTextColor(LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(0, 3, 1, 3);

    str = QString("L%1").arg(eventData.FLLap);
    if (eventData.FLLap <= 0)
        str = "";
    if (eventData.eventType != LTData::RACE_EVENT && !bestLaps.isEmpty())
        str = bestLaps[0].sessionTime.toString("h:mm:ss");


    if (str.toInt() != -1)
    {
        item = ui->tableWidget_3->item(0, 6);
        if (!item)
        {
            item = new QTableWidgetItem(str);
            ui->tableWidget_3->setItem(0, 6, item);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else
            item->setText(str);

        item->setTextColor(LTData::colors[LTData::WHITE]);
        ui->tableWidget_3->setSpan(0, 6, 1, 2);
    }

    item = ui->tableWidget_3->item(1, 2);
    if (!item)
    {
        item = new QTableWidgetItem(eventData.sec1Record[1]);
        ui->tableWidget_3->setItem(1, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
    }
    else
        item->setText(eventData.sec1Record[1]);

    item->setTextColor(LTData::colors[LTData::WHITE]);

    item = ui->tableWidget_3->item(1, 3);
    if (!item)
    {
        item = new QTableWidgetItem(eventData.sec1Record[0]);
        ui->tableWidget_3->setItem(1, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
        item->setText(eventData.sec1Record[0]);

    item->setTextColor(LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(1, 3, 1, 3);

    str = "";
    bool ok;
    if ((eventData.eventType == LTData::RACE_EVENT && eventData.sec1Record[2].toInt(&ok) > -1) && ok)
        str = QString("L%1").arg(eventData.sec1Record[2]);

    else if (eventData.eventType != LTData::RACE_EVENT && QTime::fromString(eventData.sec1Record[3], "h:mm:ss").isValid())
        str = eventData.sec1Record[3];

    item = ui->tableWidget_3->item(1, 6);
    if (!item)
    {
        item = new QTableWidgetItem(str);
        ui->tableWidget_3->setItem(1, 6, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidget_3->setSpan(1, 6, 1, 2);
    }
    else
        item->setText(str);

    item->setTextColor(LTData::colors[LTData::WHITE]);


    item = ui->tableWidget_3->item(2, 2);
    if (!item)
    {
        item = new QTableWidgetItem(eventData.sec2Record[1]);
        ui->tableWidget_3->setItem(2, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
    }
    else
        item->setText(eventData.sec2Record[1]);

    item->setTextColor(LTData::colors[LTData::WHITE]);

    item = ui->tableWidget_3->item(2, 3);
    if (!item)
    {
        item = new QTableWidgetItem(eventData.sec2Record[0]);
        ui->tableWidget_3->setItem(2, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
        item->setText(eventData.sec2Record[0]);

    item->setTextColor(LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(2, 3, 1, 3);

    str = "";
    if ((eventData.eventType == LTData::RACE_EVENT && eventData.sec2Record[2].toInt(&ok) > -1) && ok)
        str = QString("L%1").arg(eventData.sec2Record[2]);

    else if (eventData.eventType != LTData::RACE_EVENT && QTime::fromString(eventData.sec2Record[3], "h:mm:ss").isValid())
        str = eventData.sec2Record[3];

    item = ui->tableWidget_3->item(2, 6);
    if (!item)
    {
        item = new QTableWidgetItem(str);
        ui->tableWidget_3->setItem(2, 6, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidget_3->setSpan(2, 6, 1, 2);
    }
    else
        item->setText(str);

    item->setTextColor(LTData::colors[LTData::WHITE]);

    item = ui->tableWidget_3->item(3, 2);
    if (!item)
    {
        item = new QTableWidgetItem(eventData.sec3Record[1]);
        ui->tableWidget_3->setItem(3, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
    }
    else
        item->setText(eventData.sec3Record[1]);

    item->setTextColor(LTData::colors[LTData::WHITE]);

    item = ui->tableWidget_3->item(3, 3);
    if (!item)
    {
        item = new QTableWidgetItem(eventData.sec3Record[0]);
        ui->tableWidget_3->setItem(3, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
        item->setText(eventData.sec3Record[0]);

    item->setTextColor(LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(3, 3, 1, 3);

    str = "";
    if ((eventData.eventType == LTData::RACE_EVENT && eventData.sec3Record[2].toInt(&ok) > -1) && ok)
        str = QString("L%1").arg(eventData.sec3Record[2]);

    else if (eventData.eventType != LTData::RACE_EVENT && QTime::fromString(eventData.sec3Record[3], "h:mm:ss").isValid())
        str = eventData.sec3Record[3];

    item = ui->tableWidget_3->item(3, 6);
    if (!item)
    {
        item = new QTableWidgetItem(str);
        ui->tableWidget_3->setItem(3, 6, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidget_3->setSpan(3, 6, 1, 2);
    }
    else
        item->setText(str);

    item->setTextColor(LTData::colors[LTData::WHITE]);

    QString tL = "";
    if (eventData.sec1Record[1] != "" && eventData.sec2Record[1] != "" && eventData.sec3Record[1] != "")
        tL = LapData::sumSectors(eventData.sec1Record[1], eventData.sec2Record[1], eventData.sec3Record[1]);

    item = ui->tableWidget_3->item(4, 2);
    if (!item)
    {
        item = new QTableWidgetItem(tL);
        ui->tableWidget_3->setItem(4, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
    }
    else
        item->setText(tL);

    item->setTextColor(LTData::colors[LTData::WHITE]);

    for (int i = 0, j = 0; i < bestLaps.size(); ++i)
    {
        if (bestLaps[i].lapTime.toString() == "" || bestLaps[i].carID == -1)
            continue;

        item = ui->tableWidget_3->item(7+j, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1.").arg(j+1));
            ui->tableWidget_3->setItem(7+j, 0, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        }
        else
            item->setText(QString("%1.").arg(j+1));
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        item->setTextColor(LTData::colors[LTData::DEFAULT]);


        item = ui->tableWidget_3->item(7+j, 1);
        if (!item)
        {
            item = new QTableWidgetItem(eventData.driversData[bestLaps[i].carID-1].driver);
            ui->tableWidget_3->setItem(7+j, 1, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        }
        else
            item->setText(eventData.driversData[bestLaps[i].carID-1].driver);
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        if (j == 0)
            item->setTextColor(LTData::colors[LTData::VIOLET]);
        else
            item->setTextColor(LTData::colors[LTData::WHITE]);


        item = ui->tableWidget_3->item(7+j, 2);
        if (!item)
        {
            item = new QTableWidgetItem(bestLaps[i].lapTime);
            ui->tableWidget_3->setItem(7+j, 2, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(bestLaps[i].lapTime);
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        if (j == 0)
            item->setTextColor(LTData::colors[LTData::VIOLET]);
        else
            item->setTextColor(LTData::colors[LTData::GREEN]);

        if (i != 0)
        {
            QString gap = DriverData::calculateGap(bestLaps[i].lapTime, bestLaps[0].lapTime);
            item = ui->tableWidget_3->item(7+j, 3);
            if (!item)
            {
                item = new QTableWidgetItem(gap);
                ui->tableWidget_3->setItem(7+j, 3, item);
//                item->setFlags(Qt::NoItemFlags);
                item->setTextAlignment(Qt::AlignCenter);
            }
            else
                item->setText(gap);
            if (i % 2 == 0 )
                item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
            else
                item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }


        item = ui->tableWidget_3->item(7+j, 4);
        if (!item)
        {
            item = new QTableWidgetItem(bestLaps[i].sector1);
            ui->tableWidget_3->setItem(7+j, 4, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(bestLaps[i].sector1);
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        if (eventData.driversData[bestLaps[i].carID-1].driver == eventData.sec1Record[0] && bestLaps[i].numLap == eventData.sec1Record[2].toInt())
            item->setTextColor(LTData::colors[LTData::VIOLET]);

        else if (eventData.driversData[bestLaps[i].carID-1].bestSectors[0].second == bestLaps[i].numLap)
            item->setTextColor(LTData::colors[LTData::GREEN]);

        else
            item->setTextColor(LTData::colors[LTData::WHITE]);

        item = ui->tableWidget_3->item(7+j, 5);
        if (!item)
        {
            item = new QTableWidgetItem(bestLaps[i].sector2);
            ui->tableWidget_3->setItem(7+j, 5, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(bestLaps[i].sector2);
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        if (eventData.driversData[bestLaps[i].carID-1].driver == eventData.sec2Record[0] && bestLaps[i].numLap == eventData.sec2Record[2].toInt())
            item->setTextColor(LTData::colors[LTData::VIOLET]);

        else if (eventData.driversData[bestLaps[i].carID-1].bestSectors[1].second == bestLaps[i].numLap)
            item->setTextColor(LTData::colors[LTData::GREEN]);

        else
            item->setTextColor(LTData::colors[LTData::WHITE]);

        item = ui->tableWidget_3->item(7+j, 6);
        if (!item)
        {
            item = new QTableWidgetItem(bestLaps[i].sector3);
            ui->tableWidget_3->setItem(7+j, 6, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(bestLaps[i].sector3);
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        if (eventData.driversData[bestLaps[i].carID-1].driver == eventData.sec3Record[0] && bestLaps[i].numLap == eventData.sec3Record[2].toInt())
            item->setTextColor(LTData::colors[LTData::VIOLET]);

        else if (eventData.driversData[bestLaps[i].carID-1].bestSectors[2].second == bestLaps[i].numLap)
            item->setTextColor(LTData::colors[LTData::GREEN]);

        else
            item->setTextColor(LTData::colors[LTData::WHITE]);


        str = QString("%1").arg(bestLaps[i].numLap);

        if (eventData.eventType == LTData::QUALI_EVENT)
                str += QString(" (Q%1)").arg(bestLaps[i].qualiPeriod);
        item = ui->tableWidget_3->item(7+j, 7);
        if (!item)
        {
            item = new QTableWidgetItem(str);
            ui->tableWidget_3->setItem(7+j, 7, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
        }
        else
            item->setText(str);
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        item->setTextColor(LTData::colors[LTData::WHITE]);

        ++j;
    }
}

QList< QPair< QPair<double, int>, QString > > SessionDataWidget::getPitstops(const QList<DriverData> &driversData)
{
    QList< QPair< QPair<double, int>, QString > > pitData;

    for (int i = 0; i < driversData.size(); ++i)
    {
        for (int j = 0; j < driversData[i].pitData.size(); ++j)
        {
            QPair< QPair<double, int>, QString > pitAtom(QPair<double, int>(driversData[i].pitData[j].pitTime.toDouble(), driversData[i].pitData[j].pitLap), driversData[i].driver);
            pitData.append(pitAtom);
        }
    }
    qSort(pitData);

    return pitData;
}

void SessionDataWidget::updatePitStops(bool clear)
{
    if (clear)
    {
        for (int i = ui->tableWidget_4->rowCount()-1; i > 0; --i)
            ui->tableWidget_4->removeRow(i);
    }
    QList< QPair< QPair<double, int>, QString > > pitData = getPitstops(eventData.driversData);
    QTableWidgetItem *item;

    for (int i = 0; i < pitData.size(); ++i)
    {
        if (ui->tableWidget_4->rowCount() - 1 < pitData.size())
            ui->tableWidget_4->insertRow(ui->tableWidget_4->rowCount());

        item = ui->tableWidget_4->item(i+1, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1.").arg(i+1));
            ui->tableWidget_4->setItem(i+1, 0, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
        }
        else
            item->setText(QString("%1.").arg(i+1));
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);


        item = ui->tableWidget_4->item(i+1, 1);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(pitData[i].first.second));
            ui->tableWidget_4->setItem(i+1, 1, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(QString("%1").arg(pitData[i].first.second));
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        item = ui->tableWidget_4->item(i+1, 2);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(LTData::getDriverNo(pitData[i].second)));
            ui->tableWidget_4->setItem(i+1, 2, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(QString("%1").arg(LTData::getDriverNo(pitData[i].second)));
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);


        item = ui->tableWidget_4->item(i+1, 3);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(pitData[i].second));
            ui->tableWidget_4->setItem(i+1, 3, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(QString("%1").arg(pitData[i].second));
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);


        item = ui->tableWidget_4->item(i+1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(pitData[i].first.first, 0, 'f', 1));
            ui->tableWidget_4->setItem(i+1, 4, item);
//            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(QString("%1").arg(pitData[i].first.first, 0, 'f', 1));
        if (i % 2 == 0)
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);


        ui->tableWidget_4->setRowHeight(i+1, 20);
    }
}

void SessionDataWidget::on_tabWidget_currentChanged(int index)
{
    int w;
    switch (index)
    {
        case 0:
//            updateSpeedRecords(eventData);
            updateEventInfo();
            w = ui->tableWidget_5->viewport()->width();
            ui->tableWidget_5->setColumnWidth(0, (w));
            break;

        case 1:
//            updateFastestLaps(eventData);
            w = ui->tableWidget_3->viewport()->width();
            ui->tableWidget_3->setColumnWidth(0, 0.06 * w);
            ui->tableWidget_3->setColumnWidth(1, 0.24 * w);
            ui->tableWidget_3->setColumnWidth(2, 0.16 * w);
            ui->tableWidget_3->setColumnWidth(3, 0.12 * w);
            ui->tableWidget_3->setColumnWidth(4, 0.1 * w);
            ui->tableWidget_3->setColumnWidth(5, 0.1 * w);
            ui->tableWidget_3->setColumnWidth(6, 0.1 * w);
            ui->tableWidget_3->setColumnWidth(7, 0.12 * w);
            break;

        case 2:
//            updatePitStops(eventData);
            updateSpeedRecords();
            w = ui->tableWidget_2->viewport()->width();
            ui->tableWidget_2->setColumnWidth(0, 0.34 * w);
            ui->tableWidget_2->setColumnWidth(1, 0.11 * w);
            ui->tableWidget_2->setColumnWidth(2, 0.09 * w);
            ui->tableWidget_2->setColumnWidth(3, 0.34 * w);
            ui->tableWidget_2->setColumnWidth(4, 0.11 * w);
            break;

        case 3:
            updatePitStops();
            w = ui->tableWidget_4->viewport()->width();
            ui->tableWidget_4->setColumnWidth(0, 0.1 * w);
            ui->tableWidget_4->setColumnWidth(1, 0.1 * w);
            ui->tableWidget_4->setColumnWidth(2, 0.1 * w);
            ui->tableWidget_4->setColumnWidth(3, 0.4 * w);
            ui->tableWidget_4->setColumnWidth(4, 0.3 * w);
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
    ui->tableWidget_2->setFont(font);
    ui->tableWidget_3->setFont(font);
    ui->tableWidget_4->setFont(font);
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
        QTableWidget *tWidget;
        if (ui->tabWidget->currentIndex() == 1)
            tWidget = ui->tableWidget_3;

        else if (ui->tabWidget->currentIndex() == 2)
            tWidget = ui->tableWidget_2;

        else if (ui->tabWidget->currentIndex() == 3)
            tWidget = ui->tableWidget_4;

        else
            return;

        QItemSelectionModel * selection = tWidget->selectionModel();
        QModelIndexList indexes = selection->selectedIndexes();

        if(indexes.size() < 1)
            return;

        // QModelIndex::operator < sorts first by row, then by column.
        // this is what we need
        std::sort(indexes.begin(), indexes.end());

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
    ui->tableWidget_2->clear();
    ui->tableWidget_3->clear();
    ui->tableWidget_4->clear();

    for (int i = 0; i < bestLaps.size(); ++i)
        bestLaps[i] = LapData();

    setupContents();
}

void SessionDataWidget::clearFastestLaps()
{
    for (int i = 7; i < ui->tableWidget_3->rowCount(); ++i)
    {
        for (int j = 0; j < ui->tableWidget_3->columnCount(); ++j)
        {
            QTableWidgetItem *item = ui->tableWidget_3->item(i, j);
            if (item)
                item->setText("");
        }
    }
}
