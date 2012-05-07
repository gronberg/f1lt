#include "driverdatawidget.h"
#include "ui_driverdatawidget.h"

#include <QClipboard>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSplitter>

DriverDataWidget::DriverDataWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DriverDataWidget), currentDriver(0), reversedOrder (false), eventData(EventData::getInstance())
{        
    ui->setupUi(this);

    ui->tableWidget_4->setColumnWidth(0, 40);
    ui->tableWidget_4->setColumnWidth(1, 40);
    ui->tableWidget_4->setColumnWidth(2, 50);
    ui->tableWidget_4->setColumnWidth(3, 80);
    ui->tableWidget_4->setColumnWidth(4, 50);
    ui->tableWidget_4->setColumnWidth(5, 50);
    ui->tableWidget_4->setColumnWidth(6, 50);
    ui->tableWidget_4->setColumnWidth(7, 50);

    for (int i = 0; i < 9; ++i)
    {
        ui->tableWidget_4->insertRow(i);
    }

    posChart = new ChartWidget(1, 24, LTData::colors[LTData::CYAN], this);

    QColor colors[5];
    colors[0] = LTData::colors[LTData::RED];
    colors[1] = LTData::colors[LTData::WHITE];
    colors[2] = LTData::colors[LTData::GREEN];
    colors[3] = LTData::colors[LTData::VIOLET];
    colors[4] = LTData::colors[LTData::YELLOW];

    lapTimeChart = new LapTimeChartWidget(colors, this);

    gapChart = new GapChartWidget(LTData::colors[LTData::YELLOW], this);

    ui->chartsTableWidget->setColumnWidth(0, ui->chartsTableWidget->width());
    ui->lapTimeChartTableWidget->setColumnWidth(0, ui->lapTimeChartTableWidget->width());

//    ui->toolBox->insertItem(0, gapChart, QIcon(), "Position chart");
//    ui->stackedWidget->addWidget(gapChart);


//    QSplitter *splitter = new QSplitter(Qt::Vertical, ui->tab_3);
//    splitter->addWidget(posChart);
//    splitter->addWidget(lapTimeChart);


//    ui->verticalLayout_4->addWidget(splitter);

    ui->chartsTableWidget->insertRow(0);
    ui->chartsTableWidget->setRowHeight(0, 20);
    QTableWidgetItem *item = new QTableWidgetItem("Position chart");
    item->setTextAlignment(Qt::AlignCenter);
    ui->chartsTableWidget->setItem(0, 0, item);

    ui->chartsTableWidget->insertRow(1);
    ui->chartsTableWidget->setCellWidget(1, 0, posChart);
    ui->chartsTableWidget->setRowHeight(1, 200);

    ui->lapTimeChartTableWidget->insertRow(0);
    ui->lapTimeChartTableWidget->setRowHeight(0, 20);
    item = new QTableWidgetItem("Lap time chart");
    item->setTextAlignment(Qt::AlignCenter);
    ui->lapTimeChartTableWidget->setItem(0, 0, item);

    ui->lapTimeChartTableWidget->insertRow(1);
    ui->lapTimeChartTableWidget->setCellWidget(1, 0, lapTimeChart);
    ui->lapTimeChartTableWidget->setRowHeight(1, ui->lapTimeChartTableWidget->height()-20);

    ui->chartsTableWidget->insertRow(2);
    ui->chartsTableWidget->setRowHeight(2, 20);
    item = new QTableWidgetItem("Gap to leader chart");
    item->setTextAlignment(Qt::AlignCenter);
    ui->chartsTableWidget->setItem(2, 0, item);

    ui->chartsTableWidget->insertRow(3);
    ui->chartsTableWidget->setCellWidget(3, 0, gapChart);
    ui->chartsTableWidget->setRowHeight(3, 200);
}

DriverDataWidget::~DriverDataWidget()
{
    delete ui;
}

void DriverDataWidget::printDriverData(int driverIdx)
{
    if (driverIdx < 0)
        return;

    printDriverChart(driverIdx);

    int scrollBarPosition = ui->tableWidget_4->verticalScrollBar()->sliderPosition();

    QString s;
    QString bestL;
    QTableWidgetItem *item;
    bool bl = false;

    DriverData &driverData = eventData.driversData[driverIdx];

    ui->tableWidget_4->setRowHeight(0, 100);
    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget_4->cellWidget(0, 0));
    if (!lab)
    {
        lab = new QLabel();
        lab->setAlignment(Qt::AlignCenter);
        ui->tableWidget_4->setCellWidget(0, 0, lab);
        ui->tableWidget_4->setSpan(0, 0, 1, 8);
    }
    lab->setPixmap(LTData::getCarImg(driverData.number));//eventData.carImages[idx]);

    s = QString("%1  %2 (%3)").arg(driverData.number).arg(driverData.driver).arg(LTData::getTeamName(driverData.number));
    item = ui->tableWidget_4->item(1, 0);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(1, 0, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignHCenter);
        item->setTextColor(LTData::colors[LTData::WHITE]);
        ui->tableWidget_4->setSpan(1, 0, 1, 8);
        ui->tableWidget_4->setRowHeight(1, 20);
    }
    else
        item->setText(s);

    s = "Grid position:";
    if (eventData.eventType != LTData::RACE_EVENT)
        s = "Laps completed:";

    item = ui->tableWidget_4->item(2, 0);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(2, 0, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        ui->tableWidget_4->setSpan(2, 0, 1, 3);
    }
    else
        item->setText(s);


    if (eventData.eventType == LTData::RACE_EVENT)
    {
        if (!driverData.posHistory.isEmpty())
            s = QString("%1").arg(driverData.posHistory[0]);
        else
            s = "";
    }
    else
        s = QString("%1").arg((driverData.lastLap.numLap >= 0 ? driverData.lastLap.numLap : 0));

    item = ui->tableWidget_4->item(2, 3);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(2, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight);
        item->setTextColor(LTData::colors[LTData::YELLOW]);
        ui->tableWidget_4->setRowHeight(2, 20);
    }
    else
        item->setText(s);

    item = ui->tableWidget_4->item(3, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Current position:");
        ui->tableWidget_4->setItem(3, 0, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        ui->tableWidget_4->setSpan(3, 0, 1, 3);
    }
    else
        item->setText("Current position:");


    s = QString("%1").arg(driverData.pos);
    item = ui->tableWidget_4->item(3, 3);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(3, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight);
        item->setTextColor(LTData::colors[LTData::CYAN]);
        ui->tableWidget_4->setRowHeight(3, 20);
    }
    else
        item->setText(s);


    item = ui->tableWidget_4->item(4, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Gap to leader:");
        ui->tableWidget_4->setItem(4, 0, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        ui->tableWidget_4->setSpan(4, 0, 1, 3);
    }
    else
        item->setText("Gap to leader:");


    s = "";
//    if (!driverData.lapData.isEmpty())
        s = QString("%1").arg(driverData.lastLap.gap);

    if (driverData.pos == 1)
        s = "-";
    item = ui->tableWidget_4->item(4, 3);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(4, 3, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight);
        item->setTextColor(LTData::colors[LTData::YELLOW]);
        ui->tableWidget_4->setRowHeight(4, 20);
    }
    else
        item->setText(s);

    item = ui->tableWidget_4->item(5, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Last lap:");
        ui->tableWidget_4->setItem(5, 0, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        ui->tableWidget_4->setSpan(5, 0, 1, 2);
    }
    else
        item->setText("Last lap:");        


    if (driverData.lapData.isEmpty())
        s = driverData.lastLap.lapTime;
    else
        s = driverData.lapData.last().lapTime;


    QString gap;
    if (!driverData.lapData.isEmpty())
        gap = DriverData::calculateGap(driverData.lapData.last().lapTime, driverData.bestLap.lapTime);
    else
        gap = DriverData::calculateGap(driverData.lastLap.lapTime, driverData.bestLap.lapTime);

    if (gap != "0" && gap != "" && gap != "0.000")
        s += " (+"+gap +")";

    item = ui->tableWidget_4->item(5, 2);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(5, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight);
        item->setTextColor(LTData::colors[LTData::WHITE]);
        ui->tableWidget_4->setSpan(5, 2, 1, 2);
        ui->tableWidget_4->setRowHeight(5, 20);
    }
    else
        item->setText(s);

    if (driverData.lastLap.lapTime.toString() == "IN PIT")
        item->setTextColor(LTData::colors[LTData::PIT]);
    else
        item->setTextColor(LTData::colors[LTData::WHITE]);


    item = ui->tableWidget_4->item(6, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Best lap:");
        ui->tableWidget_4->setItem(6, 0, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        ui->tableWidget_4->setSpan(6, 0, 1, 2);
    }
    else
        item->setText("Best lap:");

    bestL = driverData.bestLap.lapTime;
    if (bestL == "0")
        bestL = "";

    item = ui->tableWidget_4->item(6, 2);
    if (!item)
    {
        item = new QTableWidgetItem(bestL);
        ui->tableWidget_4->setItem(6, 2, item);
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight);
        ui->tableWidget_4->setSpan(6, 2, 1, 2);
        ui->tableWidget_4->setRowHeight(6, 20);
    }
    if (bestL == eventData.FLTime)
    {
        item->setText(bestL);
        item->setTextColor(LTData::colors[LTData::VIOLET]);//QColor(255, 0, 255));
    }
    else
    {
        item->setText(bestL);
        item->setTextColor(LTData::colors[LTData::GREEN]);
    }

    if (eventData.eventType == LTData::RACE_EVENT)
    {
        item = new QTableWidgetItem(QString("Pit stops (%1)").arg(driverData.numPits));
        ui->tableWidget_4->setItem(2, 5, item);

        item->setFlags(Qt::NoItemFlags);
        ui->tableWidget_4->setSpan(2, 5, 1, 3);
        item->setTextAlignment(Qt::AlignCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);

        for (int i = 0; i < 6; ++i)
        {
            QString s1 = "", s2 = "";
            if (i < driverData.pitData.size())
            {
                s1 = QString("L%1").arg(driverData.pitData[i].pitLap);
                s2 = driverData.pitData[i].pitTime;
            }

            item = ui->tableWidget_4->item(3+2*(i/3), 5+i%3);
            if (!item)
            {
                item = new QTableWidgetItem(s1);
                ui->tableWidget_4->setItem(3+2*(i/3), 5+i%3, item);

                item->setFlags(Qt::NoItemFlags);
                item->setTextAlignment(Qt::AlignHCenter);
                item->setTextColor(LTData::colors[LTData::WHITE]);
            }
            else
                item->setText(s1);

            item = ui->tableWidget_4->item(4+2*(i/3), 5+i%3);
            if (!item)
            {
                item = new QTableWidgetItem(s2);
                ui->tableWidget_4->setItem(4+2*(i/3), 5+i%3, item);

                item->setFlags(Qt::NoItemFlags);
                item->setTextAlignment(Qt::AlignHCenter);
                item->setTextColor(LTData::colors[LTData::YELLOW]);
            }
            else
                item->setText(s2);

        }
    }    

    item = ui->tableWidget_4->item(7, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Lap history");
        ui->tableWidget_4->setItem(7, 0, item);

        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        ui->tableWidget_4->setSpan(7, 0, 1, 8);
    }


    item = ui->tableWidget_4->item(8, 0);
    if (!item)
    {
        item = new QTableWidgetItem("Lap");
        ui->tableWidget_4->setItem(8, 0, item);

    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }

    item = ui->tableWidget_4->item(8, 1);
    if (!item)
    {
        item = new QTableWidgetItem("P");
        ui->tableWidget_4->setItem(8, 1, item);

    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }

    s = "Gap";
    if (eventData.eventType != LTData::RACE_EVENT)
        s = "Ses. time";

    item = ui->tableWidget_4->item(8, 2);
    if (!item)
    {
        item = new QTableWidgetItem(s);
        ui->tableWidget_4->setItem(8, 2, item);

    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }
    else
        item->setText(s);

    item = ui->tableWidget_4->item(8, 3);
    if (!item)
    {
        item = new QTableWidgetItem("Time");
        ui->tableWidget_4->setItem(8, 3, item);

    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }

    item = ui->tableWidget_4->item(8, 4);
    if (!item)
    {
        item = new QTableWidgetItem("Diff.");
        ui->tableWidget_4->setItem(8, 4, item);


    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }

    item = ui->tableWidget_4->item(8, 5);
    if (!item)
    {
        item = new QTableWidgetItem("S1");
        ui->tableWidget_4->setItem(8, 5, item);

    //    item->setFlags(Qt::NoItemFlags);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
        item->setTextAlignment(Qt::AlignCenter);
    }

    item = ui->tableWidget_4->item(8, 6);
    if (!item)
    {
        item = new QTableWidgetItem("S2");
        ui->tableWidget_4->setItem(8, 6, item);
    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }


    item = ui->tableWidget_4->item(8, 7);
    if (!item)
    {
        item = new QTableWidgetItem("S3");
        ui->tableWidget_4->setItem(8, 7, item);

    //    item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);
    }

    int rows = 9;
//    if (refresh || currentDriver != driverData.carID)
//    {
//        for (int i = ui->tableWidget_4->rowCount()-1; i >= rows; --i)
//            ui->tableWidget_4->removeRow(i);

////        ui->tableWidget_4->insertRow(rows);
//    }
    for (int i = ui->tableWidget_4->rowCount()-1; i >= driverData.lapData.size()+rows; --i)
        ui->tableWidget_4->removeRow(i);

//    for (int i = 0; i < driverData.lapData.size(); ++i)
    for (int i = 0; i < driverData.lapData.size(); ++i)
    {
        int lapIndex = (reversedOrder ? driverData.lapData.size() - i - 1 : i);

        if (i >= ui->tableWidget_4->rowCount() - 9)
            ui->tableWidget_4->insertRow(rows);

        s = QString("%1").arg(driverData.lapData[lapIndex].numLap);
        if (eventData.eventType == LTData::QUALI_EVENT)
            s += QString(" (Q%1)").arg(driverData.lapData[lapIndex].qualiPeriod);

        item = ui->tableWidget_4->item(rows, 0);
        if (!item)
        {
            item = new QTableWidgetItem(s);

            ui->tableWidget_4->setItem(rows, 0, item);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::WHITE]);
        }
        else
            item->setText(s);

        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        item = ui->tableWidget_4->item(rows, 1);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(driverData.lapData[lapIndex].pos));
            ui->tableWidget_4->setItem(rows, 1, item);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::CYAN]);
        }
        else
            item->setText(QString("%1").arg(driverData.lapData[lapIndex].pos));

        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);


        QString s;
        if (eventData.eventType == LTData::RACE_EVENT)
        {
            s = driverData.lapData[lapIndex].gap;
            if (driverData.lapData[lapIndex].pos == 1)
                s = "";
        }
        else        
            s = driverData.lapData[lapIndex].sessionTime.toString("h:mm:ss");


        item = ui->tableWidget_4->item(rows, 2);
        if (!item)
        {
            item = new QTableWidgetItem(s);
            ui->tableWidget_4->setItem(rows, 2, item);

            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(s);
        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

        ui->tableWidget_4->setRowHeight(rows, 20);

        item = ui->tableWidget_4->item(rows, 3);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(driverData.lapData[lapIndex].lapTime));
            ui->tableWidget_4->setItem(rows, 3, item);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(QString("%1").arg(driverData.lapData[lapIndex].lapTime));

        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

//        item->setFlags(Qt::NoItemFlags);

        if (eventData.FLTime == driverData.lapData[lapIndex].lapTime && !bl)
        {
            bl = true;
            item->setTextColor(LTData::colors[LTData::VIOLET]);
        }
        else if (driverData.bestLap.lapTime.toString() != "" && driverData.bestLap.lapTime == driverData.lapData[lapIndex].lapTime && !bl)
        {
            bl = true;
            item->setTextColor(LTData::colors[LTData::GREEN]);
        }
        else if (driverData.lapData[lapIndex].lapTime.toString() == "IN PIT")
        {
            item->setText(item->text() + " (" + driverData.getPitTime(driverData.lapData[lapIndex].numLap) + ")");
            item->setTextColor(LTData::colors[LTData::PIT]);
        }
        else if (driverData.lapData[lapIndex].lapTime.toString() == "RETIRED")
            item->setTextColor(LTData::colors[LTData::RED]);

        else if (driverData.lapData[lapIndex].scLap == true)
            item->setTextColor(LTData::colors[LTData::YELLOW]);

        else if (eventData.eventType != LTData::RACE_EVENT && driverData.lapData[lapIndex].approxLap == true)
            item->setTextColor(LTData::colors[LTData::CYAN]);
        else
            item->setTextColor(LTData::colors[LTData::WHITE]);        


        s = DriverData::calculateGap(driverData.lapData[lapIndex].lapTime, driverData.bestLap.lapTime);
        if (s == "0" || s == "" || s == "0.000")
            s = "";


        item = ui->tableWidget_4->item(rows, 4);
        if (!item)
        {
            item = new QTableWidgetItem(s);
            ui->tableWidget_4->setItem(rows, 4, item);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            item->setTextColor(LTData::colors[LTData::YELLOW]);
        }
        else
            item->setText(s);
        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

//        item->setFlags(Qt::NoItemFlags);

        item = ui->tableWidget_4->item(rows, 5);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(driverData.lapData[lapIndex].sector1));
            ui->tableWidget_4->setItem(rows, 5, item);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(QString("%1").arg(driverData.lapData[lapIndex].sector1));

        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

//        item->setFlags(Qt::NoItemFlags);


        if (driverData.driver == eventData.sec1Record[0] && driverData.lapData[lapIndex].numLap == eventData.sec1Record[2].toInt())
            item->setTextColor(LTData::colors[LTData::VIOLET]);
        else
            item->setTextColor(LTData::colors[LTData::WHITE]);


        item = ui->tableWidget_4->item(rows, 6);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(driverData.lapData[lapIndex].sector2));
            ui->tableWidget_4->setItem(rows, 6, item);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(QString("%1").arg(driverData.lapData[lapIndex].sector2));

        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

//        item->setFlags(Qt::NoItemFlags);


        if (driverData.driver == eventData.sec2Record[0] && driverData.lapData[lapIndex].numLap == eventData.sec2Record[2].toInt())
            item->setTextColor(LTData::colors[LTData::VIOLET]);
        else
            item->setTextColor(LTData::colors[LTData::WHITE]);

        item = ui->tableWidget_4->item(rows, 7);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1").arg(driverData.lapData[lapIndex].sector3));
            ui->tableWidget_4->setItem(rows, 7, item);
            item->setTextAlignment(Qt::AlignCenter);
        }
        else
            item->setText(QString("%1").arg(driverData.lapData[lapIndex].sector3));

        if (i % 2 == 0 )
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND]);
        else
            item->setBackgroundColor(LTData::colors[LTData::BACKGROUND2]);

//        item->setFlags(Qt::NoItemFlags);


        if (driverData.driver == eventData.sec3Record[0] && driverData.lapData[lapIndex].numLap == eventData.sec3Record[2].toInt())
            item->setTextColor(LTData::colors[LTData::VIOLET]);
        else
            item->setTextColor(LTData::colors[LTData::WHITE]);

        ++rows;
    }
    currentDriver = driverData.carID;
    ui->tableWidget_4->verticalScrollBar()->setSliderPosition(scrollBarPosition);
}

void DriverDataWidget::printDriverChart(int driverIdx)
{
    if (driverIdx < 0)
        return;

    DriverData &driverData = eventData.driversData[driverIdx];

    QList<double> pos;
    for (int i = 0; i < driverData.posHistory.size(); ++i)
    {
        int iPos = driverData.posHistory[i];
        if (i > 0 && iPos == 0)
            iPos = driverData.posHistory[i-1];

        pos.append((double)(iPos));
    }

    posChart->setData(driverData);
    posChart->repaint();

    lapTimeChart->setData(driverData);
    lapTimeChart->repaint();

//    if (eventData.eventType == LTData::RACE_EVENT)
    {
        gapChart->setData(driverData);
        gapChart->repaint();
    }
//    else
    {
//        ui->chartsTableWidget->insertRow(4);
//        ui->chartsTableWidget->setRowHeight(4, 20);
//        item = new QTableWidgetItem("Gap to leader chart");
//        item->setTextAlignment(Qt::AlignCenter);
//        ui->chartsTableWidget->setItem(4, 0, item);

//        ui->chartsTableWidget->insertRow(5);
//        ui->chartsTableWidget->setCellWidget(5, 0, gapChart);
//        ui->chartsTableWidget->setRowHeight(4, 0);
//        ui->chartsTableWidget->setRowHeight(5, 0);
    }
}

void DriverDataWidget::resizeEvent(QResizeEvent *event)
{
    int h = event->size().height() - 110;

//    int w = ui->chartsTableWidget->viewport()->width();//event->size().width() - 40;

//    ui->chartsTableWidget->setColumnWidth(0, w);
//    if (eventData.eventType == LTData::RACE_EVENT)
    {
//        int div = eventData.eventType == LTData::RACE_EVENT ? 3 : 2;
//        ?int end = eventData.eventType == LTData::RACE_EVENT ? ui->chartsTableWidget->rowCount() : ui->chartsTableWidget->rowCount() - 2;
        for (int i = 1; i < ui->chartsTableWidget->rowCount(); i += 2)
        {
            int rH = (h/2 < 150 ? 150 : h/2);
            ui->chartsTableWidget->setRowHeight(i, rH);
        }
    }
    ui->lapTimeChartTableWidget->setRowHeight(1, h > 150 ? h : 150);
//    else
//    {
//        int rH = (2*h/5 < 150 ? 150 : 2*h/5);
//        ui->chartsTableWidget->setRowHeight(1, rH);

//        rH = (3*h/5 < 150 ? 150 : 3*h/5);
//        ui->chartsTableWidget->setRowHeight(3, rH);


//        ui->chartsTableWidget->setRowHeight(4, 0);
//        ui->chartsTableWidget->setRowHeight(5, 0);
//    }
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

void DriverDataWidget::on_tabWidget_currentChanged(int index)
{
    int w;
    switch (index)
    {
        case 0:
            w = ui->tableWidget_4->viewport()->width();
            ui->tableWidget_4->setColumnWidth(0, 0.135*w);
            ui->tableWidget_4->setColumnWidth(1, 0.06*w);
            ui->tableWidget_4->setColumnWidth(2, 0.15*w);
            ui->tableWidget_4->setColumnWidth(3, 0.23*w);
            ui->tableWidget_4->setColumnWidth(4, 0.13*w);
            ui->tableWidget_4->setColumnWidth(5, 0.1*w);
            ui->tableWidget_4->setColumnWidth(6, 0.1*w);
            ui->tableWidget_4->setColumnWidth(7, 0.1*w);
            break;
        case 1:
            w = ui->lapTimeChartTableWidget->viewport()->width();
            ui->lapTimeChartTableWidget->setColumnWidth(0, w);
            break;

        case 2:
            w = ui->chartsTableWidget->viewport()->width();
            ui->chartsTableWidget->setColumnWidth(0, w);
            break;
    }
}

void DriverDataWidget::updateView()
{
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

void DriverDataWidget::setFont(const QFont &font)
{
    ui->tableWidget_4->setFont(font);
}

int DriverDataWidget::currentIndex()
{
    return ui->tabWidget->currentIndex();
}

void DriverDataWidget::setCurrentIndex(int i)
{
    ui->tabWidget->setCurrentIndex(i);
}

void DriverDataWidget::keyPressEvent(QKeyEvent *event)
{
    if (ui->tabWidget->currentIndex() == 0 && event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
    {
            QItemSelectionModel * selection = ui->tableWidget_4->selectionModel();
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
                QVariant data = ui->tableWidget_4->model()->data(previous);
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
            selected_text.append(ui->tableWidget_4->model()->data(current).toString());
            selected_text.append(QLatin1Char('\n'));
            qApp->clipboard()->setText(selected_text);
//        QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();
//        QString text;
//        int prevRow = items.size() > 0 ? items[0]->row() : 0;
//        for (int i = 0; i < items.size(); ++i)
//        {
//            if (i > 0 && prevRow == items[i]->row())
//                text += "\t";
//            else  if (i > 0 && prevRow != items[i]->row())
//                text += "\n";

//            text += items[i]->text();
//            prevRow = items[i]->row();
//        }
//        qApp->clipboard()->setText(text);
    }
}

void DriverDataWidget::clearData()
{
    for (int i = ui->tableWidget_4->rowCount()-1; i >= 9; --i)
        ui->tableWidget_4->removeRow(i);
}
