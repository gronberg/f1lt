#include "sessionanalysiswidget.h"

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QTableWidgetItem>


SessionAnalysisWidget::SessionAnalysisWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

    QList<QColor> colors;
	for (int i = 0; i < ui.gridLayout->columnCount(); i += 2)
	{
        QLabel *lab = (QLabel*)ui.gridLayout->itemAtPosition(0, i)->widget();
        QCheckBox *box = (QCheckBox*)ui.gridLayout->itemAtPosition(0, i+1)->widget();

        box->setChecked(true);
		driverLabels.append(lab);
		driverCheckBoxes.append(box);

        colors.append(lab->palette().window().color());
        qDebug() << colors.last();

        lab = (QLabel*)ui.gridLayout->itemAtPosition(1, i)->widget();
        box = (QCheckBox*)ui.gridLayout->itemAtPosition(1, i+1)->widget();

        box->setChecked(true);
		driverLabels.append(lab);
		driverCheckBoxes.append(box);

        colors.append(lab->palette().window().color());
	}
    setupTables();
    setupIcons(colors);
    ui.sessionLapTimesChart->setColors(colors);
    ui.sessionPositionsChart->setColors(colors);
    ui.sessionGapsChart->setColors(colors);
    ui.sessionLapTimesChartFP->setColors(colors);
    ui.sessionLapTimesChartQuali->setColors(colors);

    ui.sessionLapTimesChartQ1->setColors(colors);
    ui.sessionLapTimesChartQ1->setQualiPeriod(1);
    ui.sessionLapTimesChartQ1->setSessionLength(20);

    ui.sessionLapTimesChartQ2->setColors(colors);
    ui.sessionLapTimesChartQ2->setQualiPeriod(2);
    ui.sessionLapTimesChartQ2->setSessionLength(15);

    ui.sessionLapTimesChartQ3->setColors(colors);
    ui.sessionLapTimesChartQ3->setQualiPeriod(3);
    ui.sessionLapTimesChartQ3->setSessionLength(10);
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
    setWindowTitle("Session analysis: " + EventData::getInstance().eventInfo.eventName);
    setupBoxes();
    switch(EventData::getInstance().eventType)
    {
        case LTData::RACE_EVENT:
            ui.raceTabWidget->setVisible(true);
            ui.fpTabWidget->setVisible(false);
            ui.qualiTabWidget->setVisible(false);
            break;

        case LTData::PRACTICE_EVENT:
            ui.raceTabWidget->setVisible(false);
            ui.fpTabWidget->setVisible(true);
            ui.qualiTabWidget->setVisible(false);
        break;

        case LTData::QUALI_EVENT:
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

	setItem(ui.lapTimeTableWidget, 0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
	setItem(ui.lapTimeTableWidget, 0, 1, "Driver", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
	setItem(ui.lapTimeTableWidget, 0, 2, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
	setItem(ui.lapTimeTableWidget, 0, 3, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
	setItem(ui.lapTimeTableWidget, 0, 4, "Lap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);

    ui.lapTimeTableWidget->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetFP->rowCount() == 0)
        ui.lapTimeTableWidgetFP->insertRow(0);

    setItem(ui.lapTimeTableWidgetFP, 0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetFP, 0, 1, "Driver", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetFP, 0, 2, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetFP, 0, 3, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetFP, 0, 4, "S. time", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);

    ui.lapTimeTableWidgetQuali->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQuali->rowCount() == 0)
        ui.lapTimeTableWidgetQuali->insertRow(0);

    setItem(ui.lapTimeTableWidgetQuali, 0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQuali, 0, 1, "Driver", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQuali, 0, 2, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQuali, 0, 3, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQuali, 0, 4, "S. time", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQuali, 0, 5, "Period", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);

    ui.lapTimeTableWidgetQuali->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQ1->rowCount() == 0)
        ui.lapTimeTableWidgetQ1->insertRow(0);

    setItem(ui.lapTimeTableWidgetQ1, 0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ1, 0, 1, "Driver", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ1, 0, 2, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ1, 0, 3, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ1, 0, 4, "S. time", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);

    ui.lapTimeTableWidgetQ1->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQ2->rowCount() == 0)
        ui.lapTimeTableWidgetQ2->insertRow(0);

    setItem(ui.lapTimeTableWidgetQ2, 0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ2, 0, 1, "Driver", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ2, 0, 2, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ2, 0, 3, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ2, 0, 4, "S. time", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);

    ui.lapTimeTableWidgetQ2->setRowHeight(0, 22);

    if (ui.lapTimeTableWidgetQ3->rowCount() == 0)
        ui.lapTimeTableWidgetQ3->insertRow(0);

    setItem(ui.lapTimeTableWidgetQ3, 0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ3, 0, 1, "Driver", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ3, 0, 2, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ3, 0, 3, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui.lapTimeTableWidgetQ3, 0, 4, "S. time", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);

    ui.lapTimeTableWidgetQ3->setRowHeight(0, 22);

    resizeTables();
}

void SessionAnalysisWidget::setupIcons(QList<QColor> colors)
{
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
    setWindowTitle("Session analysis: " + EventData::getInstance().eventInfo.eventName);
    setupBoxes();
    switch(EventData::getInstance().eventType)
    {
    case LTData::RACE_EVENT:
        ui.raceTabWidget->setVisible(true);
        ui.fpTabWidget->setVisible(false);
        ui.qualiTabWidget->setVisible(false);
        break;

    case LTData::PRACTICE_EVENT:
        ui.raceTabWidget->setVisible(false);
        ui.fpTabWidget->setVisible(true);
        ui.qualiTabWidget->setVisible(false);
        break;

    case LTData::QUALI_EVENT:
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
    QStringList driverList = LTData::getDriversListShort();
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

void SessionAnalysisWidget::on_buttonBox_clicked(QAbstractButton *button)
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
    lapDataArray.clear();
    for (int i = 0; i < EventData::getInstance().driversData.size(); ++i)
    {
        DriverData &dd = EventData::getInstance().driversData[i];

        if (driverChecked(dd.number))
        {
            for (int j = 0; j < dd.lapData.size(); ++j)
            {
                lapDataArray.append(dd.lapData[j]);
//                if (dd.lapData[j].numLap >= first && dd.lapData[j].numLap <= last)// &&
//                {
//                    if (((dd.lapData[j].lapTime.isValid() && dd.lapData[j].lapTime.toDouble() >= min) || min == -1) &&
//                         (dd.lapData[j].lapTime.isValid() && dd.lapData[j].lapTime.toDouble() <= max) || max == -1)
//                        lapDataArray.append(dd.lapData[j]);


//                    if (!dd.lapData[j].lapTime.isValid() && min != -1 && max != -1)
//                    {
//                        if (j > 0 && dd.lapData[j-1].lapTime.isValid() && dd.lapData[j-1].lapTime.toDouble() <= max && dd.lapData[j-1].lapTime.toDouble() >= min)
//                            lapDataArray.append(dd.lapData[j]);

//                        else if (j < dd.lapData.size()-1 && dd.lapData[j+1].lapTime.isValid() && dd.lapData[j+1].lapTime.toDouble() <= max && dd.lapData[j+1].lapTime.toDouble() >= min)
//                            lapDataArray.append(dd.lapData[j]);
//                    }

//                }
            }
        }

    }
    qSort(lapDataArray);
}

bool SessionAnalysisWidget::lapInWindow(int j)
{
    bool inTimeWindow = true;

    if (EventData::getInstance().eventType == LTData::RACE_EVENT)
        inTimeWindow = lapDataArray[j].numLap >= first && lapDataArray[j].numLap <= last;

    else if (EventData::getInstance().eventType == LTData::PRACTICE_EVENT)
    {
        int minute = LTData::getFPLength() - LTData::timeToMins(lapDataArray[j].sessionTime);
        inTimeWindow = minute >= first && minute <= last;
    }
    else if (EventData::getInstance().eventType == LTData::QUALI_EVENT)
    {
        int qPeriod = ui.qualiTabWidget->currentIndex();
        if (qPeriod == 0)
        {
            int minute = LTData::getQualiLength(lapDataArray[j].qualiPeriod) - LTData::timeToMins(lapDataArray[j].sessionTime);

            for (int k = 0; k < lapDataArray[j].qualiPeriod-1; ++k)
                minute += LTData::getQualiLength(k+1);

            inTimeWindow = (minute >= first && minute <= last);
        }
        else
        {
            int sessLength = LTData::getQualiLength(qPeriod);
            int minute = sessLength - LTData::timeToMins(lapDataArray[j].sessionTime);
            inTimeWindow = (minute >= first && minute <= last) && lapDataArray[j].qualiPeriod == qPeriod;
        }


        qDebug() <<"window: "<<first << " "<<last<<" "<<min <<" "<<max<< " " << inTimeWindow;
    }
    if (inTimeWindow)// &&
    {
        if (((lapDataArray[j].lapTime.isValid() && lapDataArray[j].lapTime.toDouble() >= min) || min == -1) &&
             ((lapDataArray[j].lapTime.isValid() && lapDataArray[j].lapTime.toDouble() <= max) || max == -1))
            return true;


        if ((EventData::getInstance().eventType == LTData::RACE_EVENT) &&
             (!lapDataArray[j].lapTime.isValid() && min != -1 && max != -1))
        {
            DriverData &dd = EventData::getInstance().driversData[lapDataArray[j].carID-1];
            LapData ld = dd.getLapData(lapDataArray[j].numLap-1);

            if (ld.carID == dd.carID)
            {
                if (ld.lapTime.isValid() && ld.lapTime.toDouble() <= max && ld.lapTime.toDouble() >= min)
                    return true;
            }
            ld = dd.getLapData(lapDataArray[j].numLap+1);
            if (ld.carID == dd.carID)
            {
                if (ld.lapTime.isValid() && ld.lapTime.toDouble() <= max && ld.lapTime.toDouble() >= min)
                    return true;
            }
        }
    }
    return false;
}

void SessionAnalysisWidget::update(bool repaintCharts)
{
    gatherData();
    qDebug() << "size=" << lapDataArray.size() ;

    int rows = 0;

    QTableWidget *table = ui.lapTimeTableWidget;
    if (EventData::getInstance().eventType == LTData::PRACTICE_EVENT)
        table = ui.lapTimeTableWidgetFP;

    if (EventData::getInstance().eventType == LTData::QUALI_EVENT)
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

            if (lapDataArray[i].carID < 0)
                continue;
            setItem(table, rows+1, 0, QString::number(rows+1)+".", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::CYAN]);

            QString s = EventData::getInstance().driversData[lapDataArray[i].carID-1].driver;
            QTableWidgetItem *item = setItem(table, rows+1, 1, s, Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::WHITE]);
            item->setIcon(getDriverIcon(EventData::getInstance().driversData[lapDataArray[i].carID-1].number));


            setItem(table, rows+1, 2, lapDataArray[i].lapTime.toString(), Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::WHITE]);

            s = (rows == 0 || !lapDataArray[i].lapTime.isValid()) ? "" : "+" + DriverData::calculateGap(lapDataArray[i].lapTime, lapDataArray[firstPlace].lapTime);
            setItem(table, rows+1, 3, s, Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::YELLOW]);

            s = QString::number(lapDataArray[i].numLap);
            if (EventData::getInstance().eventType == LTData::PRACTICE_EVENT)
                s = LTData::correctFPTime(lapDataArray[i].sessionTime).toString("h:mm:ss");//lapDataArray[i].sessionTime.toString("h:mm:ss") + " (" + QString::number(LTData::currentEventFPLength()-LTData::timeToMins(lapDataArray[i].sessionTime))+")";

            else if (EventData::getInstance().eventType == LTData::QUALI_EVENT)
                s = LTData::correctQualiTime(lapDataArray[i].sessionTime, lapDataArray[i].qualiPeriod).toString("mm:ss");

            setItem(table, rows+1, 4, s, Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::WHITE]);

            if (ui.qualiTabWidget->currentIndex() == 0)
                setItem(table, rows+1, 5, QString::number(lapDataArray[i].qualiPeriod), Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::WHITE]);

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
        if (EventData::getInstance().eventType == LTData::RACE_EVENT)
        {
            ui.sessionLapTimesChart->setData(lapDataArray);
            ui.sessionLapTimesChart->update();

            ui.sessionPositionsChart->setData(lapDataArray);
            ui.sessionPositionsChart->update();

            ui.sessionGapsChart->setData(lapDataArray);
            ui.sessionGapsChart->update();
        }
        if (EventData::getInstance().eventType == LTData::PRACTICE_EVENT)
        {
            ui.sessionLapTimesChartFP->setData(lapDataArray);
            ui.sessionLapTimesChartFP->update();
        }
        if (EventData::getInstance().eventType == LTData::QUALI_EVENT)
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

    lapDataArray.clear();
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

        if (drvNo == no)//LTData::getDriverShortName(driver) == driverCheckBoxes[i]->text())
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

        if (drvNo == no)//LTData::getDriverShortName(driver) == driverCheckBoxes[i]->text())
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

void SessionAnalysisWidget::on_top10pushButton_clicked()
{
    top10only = !top10only;
    ui.top10pushButton->setChecked(top10only);

    if (top10only)
    {
        for (int k = 0; k < EventData::getInstance().driversData.size(); ++k)
        {
            if (EventData::getInstance().driversData[k].pos <= 10)
                setDriverChecked(EventData::getInstance().driversData[k].number, true);

            else
                setDriverChecked(EventData::getInstance().driversData[k].number, false);
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

    qDebug() << " first=" << first<<", " << last << " " << min << " " << max;
    update();
}


void SessionAnalysisWidget::saveSettings(QSettings &settings)
{
    settings.setValue("ui/session_analysis_geometry", saveGeometry());
    settings.setValue("ui/session_analysis_splitter", ui.splitter->saveState());
    settings.setValue("ui/session_analysis_tab", ui.raceTabWidget->currentIndex());
    settings.setValue("ui/session_analysis_splitter_fp", ui.splitterFP->saveState());
    settings.setValue("ui/session_analysis_splitter_quali", ui.splitterQuali->saveState());
    settings.setValue("ui/session_analysis_splitter_q1", ui.splitterQ1->saveState());
    settings.setValue("ui/session_analysis_splitter_q2", ui.splitterQ2->saveState());
    settings.setValue("ui/session_analysis_splitter_q3", ui.splitterQ3->saveState());
}

void SessionAnalysisWidget::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/session_analysis_geometry").toByteArray());
    ui.splitter->restoreState(settings.value("ui/session_analysis_splitter").toByteArray());
    ui.raceTabWidget->setCurrentIndex(settings.value("ui/session_analysis_tab").toInt());
    ui.splitterFP->restoreState(settings.value("ui/session_analysis_splitter_fp").toByteArray());
    ui.splitterQuali->restoreState(settings.value("ui/session_analysis_splitter_quali").toByteArray());
    ui.splitterQ1->restoreState(settings.value("ui/session_analysis_splitter_q1").toByteArray());
    ui.splitterQ2->restoreState(settings.value("ui/session_analysis_splitter_q2").toByteArray());
    ui.splitterQ3->restoreState(settings.value("ui/session_analysis_splitter_q3").toByteArray());

    setupTables();

//    ui.splitter->refresh();
}

void SessionAnalysisWidget::onSplitterMoved(int pos, int index)
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

//    qDebug() << " gaag" << chart->getFirst() << " " << chart->getLast();
    first = chart->getFirst();
    last = chart->getLast();
    min = chart->getMin();
    max = chart->getMax();


    update();
}
