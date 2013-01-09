#include "headtoheaddialog.h"
#include "ui_headtoheaddialog.h"
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

#include "../main_gui/ltitemdelegate.h"



HeadToHeadDialog::HeadToHeadDialog(bool rev, QWidget *parent) :
    QDialog(parent, Qt::Window), ui(new Ui::HeadToHeadDialog), reversedOrder(rev), eventData(EventData::getInstance()), thumbnailsSize(150)
{
    ui->setupUi(this);

    QStringList ls;

//    ls.append("");
//    for (int i = 0; i < ed.driversData.size(); ++i)
//        ls.append(eventData.driversData[i].driver);

    comboBox[0] = ui->comboBox1;
    comboBox[1] = ui->comboBox2;
//    for (int i = 0; i < 4; ++i)
//    {
//        comboBox[i]->addItems(ls);
//        comboBox[i]->setCurrentIndex(0);
//    }

    loadDriversList();

    color[0] = SeasonData::getInstance().getDefaultColor(LTPackets::YELLOW);
    color[1] = SeasonData::getInstance().getDefaultColor(LTPackets::CYAN);
    lapCompChart = new LapCompChart(color, this);
    gapCompChart = new GapCompChart(color, this);
    posCompChart = new PosCompChart(color, this);

    posCompChart->setMinMax(1, SeasonData::getInstance().getTeams().size()*2);


    connect(ui->comboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));

//    ui->tableWidget->setColumnWidth(0, 30);
//    ui->tableWidget->setColumnWidth(1, 150);
//    ui->tableWidget->setColumnWidth(2, 150);
//    ui->tableWidget->setColumnWidth(3, 150);
//    ui->tableWidget->setColumnWidth(4, 150);

    ui->chartsTableWidget->setColumnWidth(0, 340);
    ui->chartsTableWidget->setColumnWidth(1, 340);

    ui->chartsTableWidget->insertRow(0);
    ui->chartsTableWidget->setRowHeight(0, 20);
    ui->chartsTableWidget->insertRow(1);
    ui->chartsTableWidget->setRowHeight(1, 50);

    ui->gapChartTableWidget->setColumnWidth(0, 340);
    ui->gapChartTableWidget->setColumnWidth(1, 340);

    ui->gapChartTableWidget->insertRow(0);
    ui->gapChartTableWidget->setRowHeight(0, 20);
    ui->gapChartTableWidget->insertRow(1);
    ui->gapChartTableWidget->setRowHeight(1, 50);

    ui->posChartTableWidget->setColumnWidth(0, 340);
    ui->posChartTableWidget->setColumnWidth(1, 340);

    ui->posChartTableWidget->insertRow(0);
    ui->posChartTableWidget->setRowHeight(0, 20);
    ui->posChartTableWidget->insertRow(1);
    ui->posChartTableWidget->setRowHeight(1, 50);

    QTableWidgetItem *item;
    for (int j = 0; j < 2; ++j)
    {
        item = new QTableWidgetItem();
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        ui->chartsTableWidget->setItem(0, j, item);

        item = new QTableWidgetItem();
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        ui->gapChartTableWidget->setItem(0, j, item);

        item = new QTableWidgetItem();
        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignCenter);
        ui->posChartTableWidget->setItem(0, j, item);
    }

    ui->chartsTableWidget->insertRow(2);

    ui->chartsTableWidget->setCellWidget(2, 0, lapCompChart);
    ui->chartsTableWidget->setSpan(2, 0, 1, 4);
    ui->chartsTableWidget->setRowHeight(2, 500);

    ui->gapChartTableWidget->insertRow(2);

    ui->gapChartTableWidget->setCellWidget(2, 0, gapCompChart);
    ui->gapChartTableWidget->setSpan(2, 0, 1, 4);
    ui->gapChartTableWidget->setRowHeight(2, 500);

    ui->posChartTableWidget->insertRow(2);

    ui->posChartTableWidget->setCellWidget(2, 0, posCompChart);
    ui->posChartTableWidget->setSpan(2, 0, 1, 4);
    ui->posChartTableWidget->setRowHeight(2, 500);

    ui->tableWidget->setItemDelegate(new LTItemDelegate(this));
    ui->tableWidget->insertRow(0);
    QLabel *lab = new QLabel();
    lab->setAlignment(Qt::AlignCenter);
    ui->tableWidget->setCellWidget(0, 2, lab);

    lab = new QLabel();
    lab->setAlignment(Qt::AlignCenter);
    ui->tableWidget->setCellWidget(0, 7, lab);

    ui->tableWidget->setSpan(0, 1  , 1, 5);
    ui->tableWidget->setSpan(0, 7, 1, 5);
    ui->tableWidget->setRowHeight(0, 50);

    ui->tableWidget->insertRow(1);
    item = new QTableWidgetItem("L");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 0, item);

    item = new QTableWidgetItem("P");
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 1, item);

//    item = new QTableWidgetItem("Gap");
//    item->setTextAlignment(Qt::AlignCenter);
//    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT]);
//    ui->tableWidget->setItem(1, 2, item);

    item = new QTableWidgetItem("Lap time");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 2, item);

    item = new QTableWidgetItem("S1");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 3, item);

    item = new QTableWidgetItem("S2");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 4, item);

    item = new QTableWidgetItem("S3");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 5, item);

    item = new QTableWidgetItem("Gap");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 6, item);

    item = new QTableWidgetItem("P");
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 7, item);

//    item = new QTableWidgetItem("Gap");
//    item->setTextAlignment(Qt::AlignCenter);
//    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT]);
//    ui->tableWidget->setItem(1, 9, item);

    item = new QTableWidgetItem("Lap time");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 8, item);

    item = new QTableWidgetItem("S1");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 9, item);

    item = new QTableWidgetItem("S2");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 10, item);

    item = new QTableWidgetItem("S3");
    item->setTextAlignment(Qt::AlignCenter);
    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
    ui->tableWidget->setItem(1, 11, item);

    ui->tableWidget->setRowHeight(1, 20);    
}

HeadToHeadDialog::~HeadToHeadDialog()
{
    delete ui;
}

void HeadToHeadDialog::loadDriversList()
{
    /*smallCarImg = */SeasonData::getInstance().getCarThumbnailsFactory().loadCarThumbnails(thumbnailsSize);

    comboBox[0]->clear();
    comboBox[1]->clear();

    if (comboBox[0]->itemText(1) == "")// && eventData.driversData[0].driver != "")
    {
        QStringList list = SeasonData::getInstance().getDriversList();
        if (list.size() > 1)
        {
            comboBox[0]->addItems(list);
            comboBox[1]->addItems(list);
        }
    }

//    comboBox[0]->addItem("");
//    comboBox[1]->addItem("");
}

void HeadToHeadDialog::updateData()
{
    QString s1 = comboBox[0]->currentText() + " vs ";
    if (comboBox[0]->currentText() == "")
        s1 = "";
    setWindowTitle("Head to head: " + s1  + comboBox[1]->currentText());
    int scrollBarPosition = ui->tableWidget->verticalScrollBar()->sliderPosition();

    QItemSelectionModel * selection = ui->tableWidget->selectionModel();
//    for (int i = ui->tableWidget->rowCount()-1; i >= 2; --i)
//        ui->tableWidget->removeRow(i);

    QTableWidgetItem *item;

    int firstLap = 99, lastLap = 0;
    int index[2];
    for (int i = 0; i < 2; ++i)
    {
        index[i] = 0;
        int idx = eventData.getDriverId(getNumber(i));
        if (idx > 0)
        {
            if (!eventData.getDriversData()[idx-1].getLapData().isEmpty())
        	{
                if (eventData.getDriversData()[idx-1].getLapData()[0].getLapNumber() < firstLap)
                    firstLap = eventData.getDriversData()[idx-1].getLapData()[0].getLapNumber();

                if (eventData.getDriversData()[idx-1].getLapData().last().getLapNumber() >= lastLap)
				{
                    lastLap = eventData.getDriversData()[idx-1].getLapData().last().getLapNumber();

                    if (lastLap < eventData.getEventInfo().laps &&
                        !eventData.getDriversData()[idx-1].isRetired() &&
                        eventData.getDriversData()[idx-1].getLastLap().getSectorTime(3).toString() == "" &&
                        eventData.getDriversData()[idx-1].getLastLap().getTime().toString() != "IN PIT")
						lastLap++;
				}
        	}
            DriverData &dd = eventData.getDriversData()[idx-1];            
			QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, 2+5*i));
            lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(dd.getNumber(), thumbnailsSize));//eventData.carImages[idx].scaledToWidth(120, Qt::SmoothTransformation));
        }
        else
        {
        	QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, 2+5*i));
			lab->clear();
        }
    }

    int j = 0, k = firstLap;
    bool scLap = false;
    double interval = 0.0;
    for (; k <= lastLap; ++k, ++j)
    {
        int lapNo = reversedOrder ? lastLap - k + firstLap : k;
        LapTime laps[4];

        if (ui->tableWidget->rowCount() <= j+2)
            ui->tableWidget->insertRow(j+2);

        item = ui->tableWidget->item(j+2, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1.").arg(lapNo));
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(SeasonData::getInstance().getColor(LTPackets::DEFAULT));
            ui->tableWidget->setItem(j+2, 0, item);
        }
        else
            item->setText(QString("%1.").arg(lapNo));


        int driversIdx[2] = {-1, -1};
        bool newLap[2] = {false, false};
        scLap = false;
        for (int i = 0; i < 2; ++i)
        {
            int idx = eventData.getDriverId(getNumber(i));
            driversIdx[i] = idx-1;            

            if (idx > 0 && !eventData.getDriversData()[idx-1].getLapData().isEmpty())
            {
//                int lapIndex = (reversedOrder ? eventData.driversData[idx-1].lapData.size() - index[i] - 1 : index[i]);
                DriverData &dd = eventData.getDriversData()[idx-1];
                LapData ld = dd.getLapData(lapNo);
                if (ld.getCarID() == -1)
                    ld = dd.getLapData().last();
//                if (ld.carIDlapIndex >= 0 && lapIndex < dd.lapData.size())
//                    ld = dd.lapData[lapIndex];
//                else
//                    ld = dd.lapData.last();

                if (lapNo == (ld.getLapNumber()+1) && !dd.isRetired() && dd.getLastLap().getSectorTime(3).toString() == "" && dd.getLastLap().getTime().toString() != "IN PIT")
                {
                    newLap[i] = true;
                    ld = dd.getLastLap();
                    ld.setLapNumber(ld.getLapNumber()+1);
                }

//                if (j == 0)
//                {
//                    int idx = (dd.number > 13 ? dd.number-2 : dd.number-1) / 2;
//                    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, 2+5*i));
//                    lab->setPixmap(smallCarImg[idx]);//eventData.carImages[idx].scaledToWidth(120, Qt::SmoothTransformation));
//                }
                if (dd.getLapData().size() >= index[i] && ld.getLapNumber() == lapNo)
                {                    
                    item = ui->tableWidget->item(j+2, 1+i*6);
                    if (!item)
                    {
                        item = new QTableWidgetItem(QString::number(ld.getPosition()));
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::CYAN));
                        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                        ui->tableWidget->setItem(j+2, 1 + i*6, item);
                    }
                    else
                        item->setText(QString::number(ld.getPosition()));

//                    QString gap = dd.lapData[lapIndex].pos == 1 ? "" : ld.gap;
//                    item = new QTableWidgetItem(gap);
//                    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::YELLOW]);
//                    item->setTextAlignment(Qt::AlignCenter);
//                    ui->tableWidget->setItem(j+2, 2 + i*7, item);

                    item = ui->tableWidget->item(j+2, 2+i*6);
                    QString lTime = newLap[i] ? "" : ld.getTime().toString();
                    laps[i] = newLap[i] ? LapTime() : ld.getTime();
                    if (!item)
                    {
                        item = new QTableWidgetItem(lTime);
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(j+2, 2 + i*6, item);
                    }
                    else
                        item->setText(lTime);

                    if (lTime == "IN PIT")
                    {
                        QString pitTime = dd.getPitTime(ld.getLapNumber());
                        item->setText(item->text() + " (" + pitTime + ")");
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::RED));
                    }
                    else if (lTime == "RETIRED" || lTime.contains("LAP"))
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::RED));

                    else if (ld.getRaceLapExtraData().isSCLap())
                    {
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::YELLOW));
                        scLap = true;
                    }
                    else
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::GREEN));


                    item = ui->tableWidget->item(j+2, 3+i*6);
                    if (!item)
                    {
                        item = new QTableWidgetItem(ld.getSectorTime(1).toString());
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(j+2, 3 + i*6, item);
                    }
                    else
                        item->setText(ld.getSectorTime(1).toString());
                    if (scLap)
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::YELLOW));
                    else
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::WHITE));

                    item = ui->tableWidget->item(j+2, 4+i*6);
                    if (!item)
                    {
                        item = new QTableWidgetItem(ld.getSectorTime(2).toString());
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(j+2, 4 + i*6, item);
                    }
                    else
                        item->setText(ld.getSectorTime(2).toString());

                    if (scLap)
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::YELLOW));
                    else
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::WHITE));

                    item = ui->tableWidget->item(j+2, 5+i*6);
                    if (!item)
                    {
                        item = new QTableWidgetItem(ld.getSectorTime(3).toString());
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(j+2, 5 + i*6, item);
                    }
                    else
                        item->setText(ld.getSectorTime(3).toString());

                    if (scLap)
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::YELLOW));
                    else
                        item->setTextColor(SeasonData::getInstance().getColor(LTPackets::WHITE));

                    if (!newLap[i])
                        ++index[i];
                }                
                else
                {
                    for (int w = i*6+1; w <= i*6+5; ++w)
                    {
                        item = ui->tableWidget->item(j+2, w);
                        if (item)
                            item->setText("");
                    }
                }
            }
            else
            {
//                if (j == 0)
//                {
//                    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, 2+5*i));
//                    lab->clear();
//                }
                for (int w = i*6+1; w <= i*6+5; ++w)
                {
                    item = ui->tableWidget->item(j+2, w);
                    if (item)
                        item->setText("");
                }
            }
        }
        int bestIdx = DriverData::lapDiff(laps);
        int i = ((bestIdx+1)%2);
        if (bestIdx != -1 && laps[i].toString() != "")
        {
            item = ui->tableWidget->item(j+2, 2 + i*6);
            if (laps[i].toString() != "IN PIT" && laps[i].toString() != "RETIRED" && !laps[i].toString().contains("LAP"))
            {
                item->setText(item->text() + " (+"+QString::number(laps[i].toDouble(), 'f', 3)+")");

                if (!scLap)
                    item->setTextColor(SeasonData::getInstance().getColor(LTPackets::WHITE));
            }
        }

        //sectors
        if (!scLap)
        {
            double s1, s2;
            for (int i = 0; i < 3; ++i)
            {
                if (ui->tableWidget->item(j+2, 3+i) && ui->tableWidget->item(j+2, 9+i) &&
                    (!ui->tableWidget->item(j+2, 3+i)->text().isNull() && !ui->tableWidget->item(j+2, 9+i)->text().isNull()))
                {
                    s1 = ui->tableWidget->item(j+2, 3+i)->text().toDouble();
                    s2 = ui->tableWidget->item(j+2, 9+i)->text().toDouble();

                    int greenIdx = (s1 < s2) ? 3+i : 9+i;

                    if (s1 == s2)
                        greenIdx = laps[1] < laps[0] ? 3+i : 9+i;

                    ui->tableWidget->item(j+2, greenIdx)->setTextColor(SeasonData::getInstance().getColor(LTPackets::GREEN));
                }
            }
        }

        //interval        
        QString sInterval;

        if (driversIdx[0] >= 0 && driversIdx[1] >= 0 && !laps[0].toString().contains("LAP") && !laps[1].toString().contains("LAP"))
        {
            if (newLap[0] || newLap[1])
            {
                if (eventData.getEventType() == LTPackets::RACE_EVENT)
                    sInterval = eventData.calculateInterval(eventData.getDriversData()[driversIdx[0]], eventData.getDriversData()[driversIdx[1]], lapNo-1);
            	else
            	{
                    if (std::abs(eventData.getDriversData()[driversIdx[0]].getLastLap().getLapNumber() -
                            eventData.getDriversData()[driversIdx[1]].getLastLap().getLapNumber()) <= 1)
            			sInterval = "0.0";
            	}

                double sumS[2] = {0.0, 0.0};

                QString s1[2], s2[2];

                if (sInterval != "" && sInterval != "-1L >" && sInterval != "+1L >")
                {
                    for (int i = 0; i < 2; ++i)
                    {
                        item = ui->tableWidget->item(j+2, 3+i);
                        if (item)
                            s1[i] = item->text();

                        item = ui->tableWidget->item(j+2, 9+i);
                        if (item)
                            s2[i] = item->text();
                    }
                    if (s1[0] != "" && s2[0] != "")
                        sumS[0] = s1[0].toDouble() - s2[0].toDouble();

                    if (s1[1] != "" && s2[1] != "")
                        sumS[1] = s1[1].toDouble() - s2[1].toDouble();

                    interval = sInterval.toDouble() + sumS[0] + sumS[1];
                    sInterval = QString::number(interval, 'f', 1);
                    if (interval > 0)
                        sInterval = "+" + sInterval;
                }
            }
            else
                sInterval = eventData.calculateInterval(eventData.getDriversData()[driversIdx[0]], eventData.getDriversData()[driversIdx[1]], lapNo);
        }
        item = ui->tableWidget->item(j+2, 6);
        if (!item)
        {
            item = new QTableWidgetItem(sInterval);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(j+2, 6, item);
        }
        else
            item->setText(sInterval);

        if (sInterval[0] == '-')
            item->setTextColor(SeasonData::getInstance().getColor(LTPackets::VIOLET));
        else
            item->setTextColor(SeasonData::getInstance().getColor(LTPackets::RED));

        ui->tableWidget->setRowHeight(j+2, 20);
    }


    for (int i = ui->tableWidget->rowCount()-1; i >= j+2; --i)
        ui->tableWidget->removeRow(i);

    ui->tableWidget->setSelectionModel(selection);
    ui->tableWidget->verticalScrollBar()->setSliderPosition(scrollBarPosition);
}

void HeadToHeadDialog::updateCharts()
{
    DriverData *driverData[4] = {0, 0, 0, 0};
    QString driver;
    for (int i = 0; i < 2; ++i)
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
            item->setTextColor(color[i]);

            item = ui->gapChartTableWidget->item(0, i);
            item->setText(driver);
            item->setTextColor(color[i]);

            item = ui->posChartTableWidget->item(0, i);
            item->setText(driver);
            item->setTextColor(color[i]);

//            if (carIdx >= 0)
            {
                QLabel *lab = qobject_cast<QLabel*>(ui->chartsTableWidget->cellWidget(1, i));
                if (!lab)
                {
                    lab = new QLabel();
                    lab->setAlignment(Qt::AlignCenter);
                    lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
                    ui->chartsTableWidget->setCellWidget(1, i, lab);
                }
                else
                    lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));

                lab = qobject_cast<QLabel*>(ui->gapChartTableWidget->cellWidget(1, i));
                if (!lab)
                {
                    lab = new QLabel();
                    lab->setAlignment(Qt::AlignCenter);
                    lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
                    ui->gapChartTableWidget->setCellWidget(1, i, lab);
                }
                else
                    lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));

                lab = qobject_cast<QLabel*>(ui->posChartTableWidget->cellWidget(1, i));
                if (!lab)
                {
                    lab = new QLabel();
                    lab->setAlignment(Qt::AlignCenter);
                    lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
                    ui->posChartTableWidget->setCellWidget(1, i, lab);
                }
                else
                    lab->setPixmap(SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData[i]->getNumber(), thumbnailsSize));
            }
        }
        else
        {
            QTableWidgetItem *item = ui->chartsTableWidget->item(0, i);
            item->setText("");

            QLabel *lab = qobject_cast<QLabel*>(ui->chartsTableWidget->cellWidget(1, i));
            if (lab)
                lab->clear();

            item = ui->gapChartTableWidget->item(0, i);
            item->setText("");

            lab = qobject_cast<QLabel*>(ui->gapChartTableWidget->cellWidget(1, i));
            if (lab)
                lab->clear();

            item = ui->posChartTableWidget->item(0, i);
            item->setText("");

            lab = qobject_cast<QLabel*>(ui->posChartTableWidget->cellWidget(1, i));
            if (lab)
                lab->clear();
        }
    }
    lapCompChart->setData(driverData);
    lapCompChart->repaint();

    int tab[2] = {0, 0};

    for (int i = 0; i < 2; ++i)
    {
        if (driverData[i] != 0)
            tab[i] = driverData[i]->getCarID()-1;
    }
    gapCompChart->setData(tab);
    gapCompChart->repaint();

    posCompChart->setData(driverData);
    posCompChart->repaint();
}

void HeadToHeadDialog::show(int currentDriverId)
{
//    if (comboBox[0]->itemText(1) == "")// && eventData.driversData[0].driver != "")
//    {
//        comboBox[0]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[1]->addItems(SeasonData::getInstance().getDriversList());
//    }
    setCurrentDriver(currentDriverId);

//    if (j == 0)
    {
        QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, 7));
        if (lab)
            lab->clear();

        lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, 12));
        if (lab)
            lab->clear();
    }

    updateData();
    updateCharts();

    QDialog::show();
}

int HeadToHeadDialog::exec(int currentDriverId)
{
//    if (comboBox[0]->itemText(1) == "")// && eventData.driversData[0].driver != "")
//    {
//        comboBox[0]->addItems(SeasonData::getInstance().getDriversList());
//        comboBox[1]->addItems(SeasonData::getInstance().getDriversList());
//    }
    setCurrentDriver(currentDriverId);

    updateData();
    updateCharts();

    return QDialog::exec();
}

void HeadToHeadDialog::comboBoxValueChanged(int)
{
    updateData();
    updateCharts();
}

void HeadToHeadDialog::on_pushButton_clicked()
{
    accept();
}

void HeadToHeadDialog::resizeEvent(QResizeEvent *event)
{
    for (int i = 0; i < 2; ++i)
    {
        ui->chartsTableWidget->setColumnWidth(i, (event->size().width()-40) / 2-5);
        ui->gapChartTableWidget->setColumnWidth(i, (event->size().width()-40) / 2-5);
        ui->posChartTableWidget->setColumnWidth(i, (event->size().width()-40) / 2-5);
    }

    int w = /*(ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->viewport()->width() :*/ event->size().width()-20;
    ui->tableWidget->setColumnWidth(0, 0.05*w);
    ui->tableWidget->setColumnWidth(1, 0.05*w);
//    ui->tableWidget->setColumnWidth(2, 0.06*w);
    ui->tableWidget->setColumnWidth(2, 0.2*w);
    ui->tableWidget->setColumnWidth(3, 0.05*w);
    ui->tableWidget->setColumnWidth(4, 0.05*w);
    ui->tableWidget->setColumnWidth(5, 0.05*w);
    ui->tableWidget->setColumnWidth(6, 0.09*w);
    ui->tableWidget->setColumnWidth(7, 0.05*w);
//    ui->tableWidget->setColumnWidth(9, 0.06*w);
    ui->tableWidget->setColumnWidth(8, 0.2*w);
    ui->tableWidget->setColumnWidth(9, 0.05*w);
    ui->tableWidget->setColumnWidth(10, 0.05*w);
    ui->tableWidget->setColumnWidth(11, 0.05*w);

    int h = /*ui->chartsTableWidget->viewport()->height()-80;*/event->size().height() - 250;
    if (h < 200)
        h = 200;

    ui->chartsTableWidget->setRowHeight(2, h);
    ui->gapChartTableWidget->setRowHeight(2, h);
    ui->posChartTableWidget->setRowHeight(2, h);
}

void HeadToHeadDialog::keyPressEvent(QKeyEvent *event)
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
        selected_text.append(ui->tableWidget->model()->data(current).toString());
        selected_text.append(QLatin1Char('\n'));
        qApp->clipboard()->setText(selected_text);

    }
    if (event->key() == Qt::Key_Escape)
        close();
}

void HeadToHeadDialog::setFont(const QFont &font)
{
    ui->tableWidget->setFont(font);
}

void HeadToHeadDialog::setCurrentDriver(int id)
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

int HeadToHeadDialog::getNumber(int i)
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
