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

LapTimeComparisonDialog::LapTimeComparisonDialog(bool rev, QWidget *parent) :
    QDialog(parent, Qt::Window), ui(new Ui::LapTimeComparisonDialog), reversedOrder(rev), eventData(EventData::getInstance())
{
    ui->setupUi(this);

    comboBox[0] = ui->comboBox1;
    comboBox[1] = ui->comboBox2;
    comboBox[2] = ui->comboBox3;
    comboBox[3] = ui->comboBox4;

    loadCarImages();

    color[0] = LTData::colors[LTData::GREEN];
    color[1] = LTData::colors[LTData::CYAN];
    color[2] = LTData::colors[LTData::YELLOW];
    color[3] = LTData::colors[LTData::VIOLET];
    lapCompChart = new LapCompChartWidget(color, this);


    connect(ui->comboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox3, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
    connect(ui->comboBox4, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));

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
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
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

void LapTimeComparisonDialog::loadCarImages()
{
    comboBox[0]->clear();
    comboBox[1]->clear();
    comboBox[2]->clear();
    comboBox[3]->clear();

//    comboBox[0]->addItems(LTData::getDriversList());
//    comboBox[1]->addItems(LTData::getDriversList());
//    comboBox[2]->addItems(LTData::getDriversList());
//    comboBox[3]->addItems(LTData::getDriversList());

    smallCarImg.clear();
    for (int i = 0; i < LTData::ltTeams.size(); ++i)
        smallCarImg.append(LTData::ltTeams[i].carImg.scaledToWidth(150, Qt::SmoothTransformation));
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
            wTitle += eventData.driversData[idx-1].driver;
            if(!eventData.driversData[idx-1].lapData.isEmpty())
            {
                if (eventData.driversData[idx-1].lapData[0].numLap < firstLap)
                    firstLap = eventData.driversData[idx-1].lapData[0].numLap;

                if (eventData.driversData[idx-1].lapData.last().numLap > lastLap)
                    lastLap = eventData.driversData[idx-1].lapData.last().numLap;
            }

            DriverData &dd = eventData.driversData[idx-1];
            int idx = (dd.number > 13 ? dd.number-2 : dd.number-1) / 2;
			QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
			lab->setPixmap(smallCarImg[idx]);//eventData.carImages[idx].scaledToWidth(120, Qt::SmoothTransformation));
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
        int lapNo = reversedOrder ? lastLap - k + firstLap : k;
        LapTime laps[4];

        if (ui->tableWidget->rowCount() <= j+1)
            ui->tableWidget->insertRow(j+1);

        item = ui->tableWidget->item(j+1, 0);
        if (!item)
        {
            item = new QTableWidgetItem(QString("%1.").arg(lapNo));
//                item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            ui->tableWidget->setItem(j+1, 0, item);
        }
        else
            item->setText(QString("%1.").arg(lapNo));


        for (int i = 0; i < 4; ++i)
        {
            int idx = eventData.getDriverId(getNumber(i));

            if (idx > 0 && !eventData.driversData[idx-1].lapData.isEmpty())
            {
                //int lapIndex = (reversedOrder ? eventData.driversData[idx-1].lapData.size() - index[i] - 1 : index[i]);
                DriverData dd = eventData.driversData[idx-1];
                LapData ld = dd.getLapData(lapNo);

//                if (j == 0)
//                {
//                    int idx = (dd.number > 13 ? dd.number-2 : dd.number-1) / 2;
//                    QLabel *lab = qobject_cast<QLabel*>(ui->tableWidget->cellWidget(0, i+1));
//                    lab->setPixmap(smallCarImg[idx]);//eventData.carImages[idx].scaledToWidth(120, Qt::SmoothTransformation));
//                }

                if (dd.lapData.size() > index[i] && ld.numLap == lapNo && ld.carID != -1)
                {
                    laps[i] = ld.lapTime;

                    item = ui->tableWidget->item(j+1, i+1);
                    if (!item)
                    {
                        item = new QTableWidgetItem(ld.lapTime);
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(j+1, i+1, item);
                    }
                    else
                        item->setText(ld.lapTime);

                    if (ld.lapTime.toString() == "IN PIT")
                        item->setText(item->text() + " (" + dd.getPitTime(ld.numLap) + ")");

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
                if (i != bestIdx && laps[i].toString() != "" && laps[i].toString() != "IN PIT" && laps[i].toString() != "RETIRED")
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
                                    msecs[ji++] = 999999999998;
                            }
                        }
                        double maxGap = std::max(std::max(msecs[0], msecs[1]), msecs[2]);
                        double minGap = std::min(std::min(msecs[0], msecs[1]), msecs[2]);

                        LTData::Colors color = LTData::YELLOW;

                        if (laps[i].toMsecs() == minGap)
                            color = LTData::WHITE;

                        else if (laps[i].toMsecs() == maxGap)
                            color = LTData::RED;

                        item->setTextColor(LTData::colors[color]);
                    }
                }
                else if (laps[i].toString() == "IN PIT" || laps[i].toString() == "RETIRED")
                {
                    item = ui->tableWidget->item(j+1, i+1);
                    item->setTextColor(LTData::colors[LTData::RED]);
                }
            }
            item = ui->tableWidget->item(j+1, bestIdx+1);
            if (item && laps[bestIdx].toString() != "IN PIT" && laps[bestIdx].toString() != "RETIRED")
                item->setTextColor(LTData::colors[LTData::GREEN]);
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
    DriverData driverData[4];
    QString driver;
    int carIdx;
    for (int i = 0; i < 4; ++i)
    {
        int idx = eventData.getDriverId(getNumber(i));
        if (idx > 0)
        {
            driver = eventData.driversData[idx-1].driver;
            driverData[i] = eventData.driversData[idx-1];
            carIdx = (eventData.driversData[idx-1].number > 13 ?
                             eventData.driversData[idx-1].number-2 :
                             eventData.driversData[idx-1].number-1) / 2;

            QTableWidgetItem *item = ui->chartsTableWidget->item(0, i);
            item->setText(driver);
            item->setTextColor(color[i]);

            if (carIdx >= 0)
            {
                QLabel *lab = qobject_cast<QLabel*>(ui->chartsTableWidget->cellWidget(1, i));
                if (!lab)
                {
                    lab = new QLabel();
                    lab->setAlignment(Qt::AlignCenter);
                    lab->setPixmap(smallCarImg[carIdx]);//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
                    ui->chartsTableWidget->setCellWidget(1, i, lab);
                }
                else
                    lab->setPixmap(smallCarImg[carIdx]);//eventData.carImages[carIdx].scaledToWidth(120, Qt::SmoothTransformation));
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

void LapTimeComparisonDialog::show()
{
    if (comboBox[0]->itemText(1) == "")
    {
        comboBox[0]->addItems(LTData::getDriversList());
        comboBox[1]->addItems(LTData::getDriversList());
        comboBox[2]->addItems(LTData::getDriversList());
        comboBox[3]->addItems(LTData::getDriversList());
    }
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

int LapTimeComparisonDialog::exec()
{
    if (comboBox[0]->itemText(1) == "")
    {
        comboBox[0]->addItems(LTData::getDriversList());
        comboBox[1]->addItems(LTData::getDriversList());
        comboBox[2]->addItems(LTData::getDriversList());
        comboBox[3]->addItems(LTData::getDriversList());
    }

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

