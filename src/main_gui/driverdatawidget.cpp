#include "driverdatawidget.h"
#include "ui_driverdatawidget.h"

#include <QClipboard>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSplitter>

#include "ltitemdelegate.h"

DriverDataWidget::DriverDataWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DriverDataWidget), currentDriver(0), eventData(EventData::getInstance())
{        
    ui->setupUi(this);

    posChart = new DriverDataChart(1, 24, SeasonData::getInstance().getColor(LTPackets::CYAN), this);

    QColor colors[5];
    colors[0] = SeasonData::getInstance().getColor(LTPackets::RED);
    colors[1] = SeasonData::getInstance().getColor(LTPackets::WHITE);
    colors[2] = SeasonData::getInstance().getColor(LTPackets::GREEN);
    colors[3] = SeasonData::getInstance().getColor(LTPackets::VIOLET);
    colors[4] = SeasonData::getInstance().getColor(LTPackets::YELLOW);

    lapTimeChart = new LapTimeChart(colors, this);

    gapChart = new GapChart(SeasonData::getInstance().getColor(LTPackets::YELLOW), this);

    ui->chartsTableWidget->setColumnWidth(0, ui->chartsTableWidget->width());
    ui->lapTimeChartTableWidget->setColumnWidth(0, ui->lapTimeChartTableWidget->width());

    ui->infoWidget->setVisible(false);

    driverLapHistoryModel = new DriverLapHistoryModel(this);
    ui->tableView->setModel(driverLapHistoryModel);
    ui->tableView->setItemDelegate(new LTItemDelegate(this));    
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
    delete driverLapHistoryModel;
    delete ui;
}

void DriverDataWidget::updateDriverInfo(const DriverData &driverData)
{
    if (!ui->infoWidget->isVisible())
        ui->infoWidget->setVisible(true);

    QString s;

    s = QString("%1  %2 (%3)").arg(driverData.getNumber()).arg(driverData.getDriverName()).arg(SeasonData::getInstance().getTeamName(driverData.getNumber()));
    ui->driverNameLabel->setText(s);

    ui->carImageLabel->setPixmap(SeasonData::getInstance().getCarImg(driverData.getNumber()));

    if (eventData.getEventType() == LTPackets::RACE_EVENT)
    {
        ui->gridLabel->setText("Grid position");
        if (!driverData.getPositionHistory().isEmpty())
            s = QString("%1").arg(driverData.getPositionHistory()[0]);
        else
            s = "";
    }
    else
    {
        ui->gridLabel->setText("Laps completed:");
        int laps = driverData.getLastLap().getLapNumber();
        if (laps < 0)
            laps = 0;

        s = QString::number(laps);
    }
    ui->gridPositionLabel->setText(s);

    ui->currentPositionLabel->setText(QString::number(driverData.getPosition()));

    s = driverData.getLastLap().getGap();
    if (driverData.getPosition() == 1)
        s = "";

    ui->gapLabel->setText(s);

    if (eventData.getEventType() == LTPackets::RACE_EVENT)
    {
        LapTime lt = driverData.getLastLap().getTime();
        s = lt.toString();
        if (lt != driverData.getSessionRecords().getBestLap().getTime() &&
            lt.isValid())
            s += " (+" + DriverData::calculateGap(lt, driverData.getSessionRecords().getBestLap().getTime()) + ")";

        QPalette palette = ui->lastLapLabel->palette();
        if (lt.toString() == "RETIRED" || lt.toString() == "IN PIT" || lt.toString() == "OUT")
            palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::RED));
        else
            palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::WHITE));

        ui->lastLapLabel->setPalette(palette);
    }
    else
    {
        LapTime lt = driverData.getLastLap().getTime();
        if (!driverData.getLapData().isEmpty())
            lt = driverData.getLapData().last().getTime();

        s = lt.toString();

        if (lt.isValid() && lt != driverData.getSessionRecords().getBestLap().getTime())
            s += " (+" + DriverData::calculateGap(lt, driverData.getSessionRecords().getBestLap().getTime()) + ")";        
    }

    ui->lastLapLabel->setText(s);
    ui->bestLapLabel->setText(driverData.getSessionRecords().getBestLap().getTime().toString());

    QPalette palette = ui->bestLapLabel->palette();

    if (driverData.getDriverName() == eventData.getSessionRecords().getFastestLap().getDriverName() &&
        driverData.getSessionRecords().getBestLap().getTime() == eventData.getSessionRecords().getFastestLap().getTime())
        palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::VIOLET));

    else
        palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::GREEN));

    ui->bestLapLabel->setPalette(palette);

    if (driverData.getSessionRecords().getBestSectorTime(1).isValid() &&
        driverData.getSessionRecords().getBestSectorTime(2).isValid() &&
        driverData.getSessionRecords().getBestSectorTime(3).isValid())
    {
        ui->approxLapLabel->setText(LapData::sumSectors(driverData.getSessionRecords().getBestSectorTime(1),
                                    driverData.getSessionRecords().getBestSectorTime(2), driverData.getSessionRecords().getBestSectorTime(3)));
    }
    else
        ui->approxLapLabel->setText("");

    if (eventData.getEventType() == LTPackets::RACE_EVENT)
    {
        ui->pitStopsLabel->setText("Pit stops:");
        palette = ui->pitStopsLabel->palette();
        palette.setBrush(QPalette::Foreground, SeasonData::getInstance().getColor(LTPackets::DEFAULT));
        ui->pitStopsLabel->setPalette(palette);

        ui->numPitsLabel->setText(QString::number(driverData.getNumPits()));

        if (!ui->numPitsLabel->isVisible())
            ui->numPitsLabel->setVisible(true);
    }
    else
    {
        palette = ui->pitStopsLabel->palette();
        if (driverData.getColorData().numberColor() == LTPackets::PIT)
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

        if (ui->numPitsLabel->isVisible())
            ui->numPitsLabel->setVisible(false);
    }
}

void DriverDataWidget::printDriverData(int id)
{
    if (id <= 0)
        return;

    printDriverChart(id);
    printDriverRelatedCommentary(id);

    currentDriver = id;

    const DriverData &driverData = eventData.getDriverDataById(id);

    updateDriverInfo(driverData);
    driverLapHistoryModel->update(driverData);    

    for (int i = 0; i < driverLapHistoryModel->rowCount(); ++i)
        ui->tableView->setRowHeight(i, 22);

    ui->tableView->setMinimumSize(QSize(ui->tableView->minimumWidth(), driverLapHistoryModel->rowCount() * 22));
    return;
}

void DriverDataWidget::printDriverChart(int id)
{
    if (id <= 0)
        return;

    const DriverData &driverData = eventData.getDriverDataById(id);

    QList<double> pos;
    for (int i = 0; i < driverData.getPositionHistory().size(); ++i)
    {
        int iPos = driverData.getPositionHistory()[i];
        if (i > 0 && iPos == 0)
            iPos = driverData.getPositionHistory()[i-1];

        pos.append((double)(iPos));
    }

    posChart->setData(driverData);
    posChart->repaint();

    lapTimeChart->setData(driverData);
    lapTimeChart->repaint();

//    if (eventData.eventType == LTPackets::RACE_EVENT)
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

void DriverDataWidget::printDriverRelatedCommentary(int id)
{
    if (id <= 0)
        return;

    int idx = 0;

    if (id == currentDriver)
    {
        QString prevLast;
        QString text = ui->textEdit->toPlainText();

        idx = text.lastIndexOf("\n", -2);
        if (idx != -1)
        {
            prevLast = text.mid(idx+1, text.size() - idx - 2);

            idx = eventData.getCommentary().lastIndexOf(prevLast, -2, Qt::CaseInsensitive);
            if (idx != -1)
                idx += prevLast.size();
        }
        else
        {
            ui->textEdit->clear();
            idx = 0;
        }
    }
    else
        ui->textEdit->clear();

    const DriverData &driverData = eventData.getDriverDataById(id);
    QRegExp searchT(/*"[ ,.?!(]" +*/ SeasonData::getInstance().getDriverLastName(driverData.getDriverName()) + "[ \',.?!);:]", Qt::CaseInsensitive);
    QString lastEvent;

    idx = eventData.getCommentary().indexOf(searchT, idx);

    while (idx != -1)
    {
        int beg = eventData.getCommentary().lastIndexOf("\n", idx);
        int end = eventData.getCommentary().indexOf("\n", idx);

        QString ev = eventData.getCommentary().mid(beg+1, end-beg-1);

        if (ev != lastEvent)
        {
            lastEvent = ev;
            ui->textEdit->append(ev + "\n");
        }
        idx = eventData.getCommentary().indexOf(searchT, idx+1);
    }
}

void DriverDataWidget::resizeEvent(QResizeEvent *event)
{
    int h = event->size().height() - 110;

//    int w = ui->chartsTableWidget->viewport()->width();//event->size().width() - 40;

//    ui->chartsTableWidget->setColumnWidth(0, w);
//    if (eventData.eventType == LTPackets::RACE_EVENT)
    {
//        int div = eventData.eventType == LTPackets::RACE_EVENT ? 3 : 2;
//        ?int end = eventData.eventType == LTPackets::RACE_EVENT ? ui->chartsTableWidget->rowCount() : ui->chartsTableWidget->rowCount() - 2;
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
            w = ui->tableView->viewport()->width();
            if (w < ui->scrollAreaWidgetContents_5->width()/2)
                w = ui->scrollAreaWidgetContents_5->width() - 10;

            ui->tableView->setColumnWidth(0, 0.1*w);
            ui->tableView->setColumnWidth(1, 0.06*w);
            ui->tableView->setColumnWidth(2, 0.185*w);
            ui->tableView->setColumnWidth(3, 0.23*w);
            ui->tableView->setColumnWidth(4, 0.13*w);
            ui->tableView->setColumnWidth(5, 0.1*w);
            ui->tableView->setColumnWidth(6, 0.1*w);
            ui->tableView->setColumnWidth(7, 0.1*w);
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

void DriverDataWidget::setFont(const QFont &font, const QFont &cFont)
{
    ui->infoWidget->setFont(font);
    ui->tableView->setFont(font);
    ui->textEdit->setFont(cFont);
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
        QItemSelectionModel * selection = ui->tableView->selectionModel();
        QModelIndexList indexes = selection->selectedIndexes();

        if(indexes.size() < 1)
            return;

        qSort(indexes.begin(), indexes.end());

        QModelIndex previous = indexes.first();
        indexes.removeFirst();
        QString selected_text;
        QModelIndex current;
        Q_FOREACH(current, indexes)
        {
            QVariant data = ui->tableView->model()->data(previous);
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
        selected_text.append(ui->tableView->model()->data(current).toString());
        selected_text.append(QLatin1Char('\n'));
        qApp->clipboard()->setText(selected_text);
    }
}

void DriverDataWidget::clearData()
{
    ui->textEdit->clear();
}
