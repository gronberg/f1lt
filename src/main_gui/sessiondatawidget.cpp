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
    ui->tableWidget_5->setColumnWidth(0, ui->tableWidget_5->width());    

    setItem(ui->tableWidget_3, 0, 0, "Fastest lap:", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(0, 0, 1, 2);
    ui->tableWidget_3->setRowHeight(0, 20);

    setItem(ui->tableWidget_3, 1, 0, "Fastest sector 1:", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(1, 0, 1, 2);
    ui->tableWidget_3->setRowHeight(1, 20);

    setItem(ui->tableWidget_3, 2, 0, "Fastest sector 2:", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(2, 0, 1, 2);
    ui->tableWidget_3->setRowHeight(2, 20);

    setItem(ui->tableWidget_3, 3, 0, "Fastest sector 3:", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(3, 0, 1, 2);
    ui->tableWidget_3->setRowHeight(3, 20);

    setItem(ui->tableWidget_3, 4, 0, "Theor. lap time:", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(4, 0, 1, 2);
    ui->tableWidget_3->setRowHeight(4, 20);

    setItem(ui->tableWidget_3, 5, 0, "Best laps", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    ui->tableWidget_3->setSpan(5, 0, 1, 7);

    setItem(ui->tableWidget_3, 6, 0, "P", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 1, "Name", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 2, "Time", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 3, "Gap", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 4, "S1", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 5, "S2", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 6, "S3", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_3, 6, 7, "Lap", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);

    setItem(ui->tableWidget_2, 0, 0, "Sector 1", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 0, 1, "KPH", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 0, 3, "Sector 2", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 0, 4, "KPH", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 8, 0, "Sector 3", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 8, 1, "KPH", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 8, 3, "Speed trap", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_2, 8, 4, "KPH", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);


    setItem(ui->tableWidget_4, 0, 0, "", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_4, 0, 1, "Lap", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_4, 0, 3, "Name", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_4, 0, 4, "Time", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
    setItem(ui->tableWidget_4, 0, 5, "Pos change", Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);

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

    item = setItem(ui->tableWidget_5, 0, 0, event.eventName, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::YELLOW]);
    item->setFont(QFont("Arial", 15, QFont::Bold));

    item = setItem(ui->tableWidget_5, 1, 0, event.eventPlace, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::GREEN]);
    item->setFont(QFont("Arial", 12, QFont::Bold));

    item = setItem(ui->tableWidget_5, 2, 0, event.fpDate.toString("dd.MM.yyyy") + " - " + event.raceDate.toString("dd.MM.yyyy"), Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::WHITE]);
    item->setFont(QFont("Arial", 10, QFont::Bold));

    item = setItem(ui->tableWidget_5, 3, 0, QString::number(event.laps) + " laps", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::CYAN]);


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
        	QPixmap pix = trackImg;
        	int w = width() - 20;
        	int h = height() - 20;

        	if (pix.width() > w)
        		pix = trackImg.scaledToWidth(w, Qt::SmoothTransformation);

        	if (pix.height() > h)
                pix = trackImg.scaledToHeight(h, Qt::SmoothTransformation);

        	lab->setPixmap(pix);

//            lab->resize(trackImg.size());
            lab->setAlignment(Qt::AlignCenter);
            if (ui->tableWidget_5->rowHeight(4) < trackImg.height())
                ui->tableWidget_5->setRowHeight(4,trackImg.height());

            ui->tableWidget_5->setColumnWidth(0, trackImg.width());
        }
    }
}

void SessionDataWidget::updateSpeedRecords()
{
    for (int i = 0, j = 0; i < 6; i+=1, j += 2)
    {
        setItem(ui->tableWidget_2, 1+i, 0, QString("%1").arg(eventData.sec1Speed[j]), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
        setItem(ui->tableWidget_2, 1+i, 1, eventData.sec1Speed[j+1], Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::YELLOW]);

        setItem(ui->tableWidget_2, 1+i, 3, QString("%1").arg(eventData.sec2Speed[j]), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
        setItem(ui->tableWidget_2, 1+i, 4, eventData.sec2Speed[j+1], Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::YELLOW]);

        ui->tableWidget_2->setRowHeight(1+i, 20);

        setItem(ui->tableWidget_2, 9+i, 0, QString("%1").arg(eventData.sec3Speed[j]), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
        setItem(ui->tableWidget_2, 9+i, 1, eventData.sec3Speed[j+1], Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::YELLOW]);

        setItem(ui->tableWidget_2, 9+i, 3, QString("%1").arg(eventData.speedTrap[j]), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
        setItem(ui->tableWidget_2, 9+i, 4, eventData.speedTrap[j+1], Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::YELLOW]);

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

    QString str = eventData.FLTime;
    if (eventData.eventType != LTData::RACE_EVENT && !bestLaps.isEmpty())
        str = bestLaps[0].lapTime.toString();

    setItem(ui->tableWidget_3, 0, 2, str, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::VIOLET]);


    str = eventData.FLDriver;
    if (eventData.eventType != LTData::RACE_EVENT && !bestLaps.isEmpty() && bestLaps[0].carID != -1)
        str = eventData.driversData[bestLaps[0].carID-1].driver;

    setItem(ui->tableWidget_3, 0, 3, str, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(0, 3, 1, 3);

    str = QString("L%1").arg(eventData.FLLap);
    if (eventData.FLLap <= 0)
        str = "";
    if (eventData.eventType != LTData::RACE_EVENT && !bestLaps.isEmpty())
        str = bestLaps[0].sessionTime.toString("h:mm:ss");


    if (str.toInt() != -1)
    {
        setItem(ui->tableWidget_3, 0, 6, str, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::YELLOW]);
        ui->tableWidget_3->setSpan(0, 6, 1, 2);
    }

    setItem(ui->tableWidget_3, 1, 2, eventData.sec1Record[1], Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::VIOLET]);

    setItem(ui->tableWidget_3, 1, 3, eventData.sec1Record[0], Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(1, 3, 1, 3);

    str = "";
    bool ok;
    if ((eventData.eventType == LTData::RACE_EVENT && eventData.sec1Record[2].toInt(&ok) > -1) && ok)
        str = QString("L%1").arg(eventData.sec1Record[2]);

    else if (eventData.eventType != LTData::RACE_EVENT && QTime::fromString(eventData.sec1Record[3], "h:mm:ss").isValid())
        str = eventData.sec1Record[3];

    setItem(ui->tableWidget_3, 1, 6, str, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::YELLOW]);
    ui->tableWidget_3->setSpan(1, 6, 1, 2);


    setItem(ui->tableWidget_3, 2, 2, eventData.sec2Record[1], Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::VIOLET]);

    setItem(ui->tableWidget_3, 2, 3, eventData.sec2Record[0], Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(2, 3, 1, 3);

    str = "";
    if ((eventData.eventType == LTData::RACE_EVENT && eventData.sec2Record[2].toInt(&ok) > -1) && ok)
        str = QString("L%1").arg(eventData.sec2Record[2]);

    else if (eventData.eventType != LTData::RACE_EVENT && QTime::fromString(eventData.sec2Record[3], "h:mm:ss").isValid())
        str = eventData.sec2Record[3];

    setItem(ui->tableWidget_3, 2, 6, str, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::YELLOW]);
    ui->tableWidget_3->setSpan(2, 6, 1, 2);

    setItem(ui->tableWidget_3, 3, 2, eventData.sec3Record[1], Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::VIOLET]);

    setItem(ui->tableWidget_3, 3, 3, eventData.sec3Record[0], Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::WHITE]);
    ui->tableWidget_3->setSpan(3, 3, 1, 3);

    str = "";
    if ((eventData.eventType == LTData::RACE_EVENT && eventData.sec3Record[2].toInt(&ok) > -1) && ok)
        str = QString("L%1").arg(eventData.sec3Record[2]);

    else if (eventData.eventType != LTData::RACE_EVENT && QTime::fromString(eventData.sec3Record[3], "h:mm:ss").isValid())
        str = eventData.sec3Record[3];

    setItem(ui->tableWidget_3, 3, 6, str, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::YELLOW]);
    ui->tableWidget_3->setSpan(3, 6, 1, 2);

    QString tL = "";
    if (eventData.sec1Record[1] != "" && eventData.sec2Record[1] != "" && eventData.sec3Record[1] != "")
        tL = LapData::sumSectors(eventData.sec1Record[1], eventData.sec2Record[1], eventData.sec3Record[1]);

    setItem(ui->tableWidget_3, 4, 2, tL, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::CYAN]);

    for (int i = 0, j = 0; i < bestLaps.size(); ++i)
    {
        if (bestLaps[i].lapTime.toString() == "" || bestLaps[i].carID == -1)
            continue;

        setItem(ui->tableWidget_3, 7+j, 0, QString("%1.").arg(j+1), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft,
                       LTData::colors[LTData::DEFAULT], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);


        setItem(ui->tableWidget_3, 7+j, 1, eventData.driversData[bestLaps[i].carID-1].driver, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft,
                       LTData::colors[j == 0 ? LTData::VIOLET : LTData::WHITE], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        setItem(ui->tableWidget_3, 7+j, 2, bestLaps[i].lapTime, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                       LTData::colors[j == 0 ? LTData::VIOLET : LTData::GREEN], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        if (i != 0)
        {
            QString gap = DriverData::calculateGap(bestLaps[i].lapTime, bestLaps[0].lapTime);

            setItem(ui->tableWidget_3, 7+j, 3, gap, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                           LTData::colors[LTData::YELLOW], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);
        }
        QColor color = LTData::colors[LTData::WHITE];
        if (eventData.driversData[bestLaps[i].carID-1].driver == eventData.sec1Record[0] && bestLaps[i].numLap == eventData.sec1Record[2].toInt())
            color = LTData::colors[LTData::VIOLET];

        else if (eventData.driversData[bestLaps[i].carID-1].bestSectors[0].second == bestLaps[i].numLap)
            color = LTData::colors[LTData::GREEN];

        setItem(ui->tableWidget_3, 7+j, 4, bestLaps[i].sector1, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                       color, LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);


        color = LTData::colors[LTData::WHITE];
        if (eventData.driversData[bestLaps[i].carID-1].driver == eventData.sec2Record[0] && bestLaps[i].numLap == eventData.sec2Record[2].toInt())
            color = LTData::colors[LTData::VIOLET];

        else if (eventData.driversData[bestLaps[i].carID-1].bestSectors[1].second == bestLaps[i].numLap)
            color = LTData::colors[LTData::GREEN];

        setItem(ui->tableWidget_3, 7+j, 5, bestLaps[i].sector2, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                       color, LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        color = LTData::colors[LTData::WHITE];
        if (eventData.driversData[bestLaps[i].carID-1].driver == eventData.sec3Record[0] && bestLaps[i].numLap == eventData.sec3Record[2].toInt())
            color = LTData::colors[LTData::VIOLET];

        else if (eventData.driversData[bestLaps[i].carID-1].bestSectors[2].second == bestLaps[i].numLap)
            color = LTData::colors[LTData::GREEN];

        setItem(ui->tableWidget_3, 7+j, 6, bestLaps[i].sector3, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                       color, LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);



        str = QString("%1").arg(bestLaps[i].numLap);

        if (eventData.eventType == LTData::QUALI_EVENT)
                str += QString(" (Q%1)").arg(bestLaps[i].qualiPeriod);

        setItem(ui->tableWidget_3, 7+j, 7, str, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight,
                       LTData::colors[LTData::WHITE], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        ++j;
    }
}

QList< SessionDataWidget::PitStopAtom > SessionDataWidget::getPitstops(const QList<DriverData> &driversData)
{
    QList< SessionDataWidget::PitStopAtom > pitData;

    for (int i = 0; i < driversData.size(); ++i)
    {
        for (int j = 0; j < driversData[i].pitData.size(); ++j)
        {
        	PitStopAtom pitAtom;
        	pitAtom.driver = driversData[i].driver;
        	pitAtom.lap = driversData[i].pitData[j].pitLap;
        	pitAtom.time = driversData[i].pitData[j].pitTime.toDouble();

        	LapData ld1 = driversData[i].getLapData(driversData[i].pitData[j].pitLap);
			LapData ld2 = driversData[i].getLapData(driversData[i].pitData[j].pitLap+1);

			pitAtom.pos = 0;
			if (ld1.carID != -1 && ld2.carID != -1)
				pitAtom.pos = ld1.pos - ld2.pos;

//            QPair< QPair<double, int>, QString > pitAtom(QPair<double, int>(driversData[i].pitData[j].pitTime.toDouble(), driversData[i].pitData[j].pitLap), driversData[i].driver);
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
    QList< PitStopAtom > pitData = getPitstops(eventData.driversData);

    for (int i = 0; i < pitData.size(); ++i)
    {
        if (ui->tableWidget_4->rowCount() - 1 < pitData.size())
            ui->tableWidget_4->insertRow(ui->tableWidget_4->rowCount());

        setItem(ui->tableWidget_4, i+1, 0, QString("%1.").arg(i+1), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight,
                       LTData::colors[LTData::DEFAULT], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        setItem(ui->tableWidget_4, i+1, 1, QString("%1").arg(pitData[i].lap), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight,
                       LTData::colors[LTData::YELLOW], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        setItem(ui->tableWidget_4, i+1, 2, QString("%1").arg(LTData::getDriverNo(pitData[i].driver)), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignRight,
                       LTData::colors[LTData::WHITE], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        setItem(ui->tableWidget_4, i+1, 3, QString("%1").arg(pitData[i].driver), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignVCenter | Qt::AlignLeft,
                       LTData::colors[LTData::WHITE], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        setItem(ui->tableWidget_4, i+1, 4, QString("%1").arg(pitData[i].time, 0, 'f', 1), Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                       LTData::colors[LTData::YELLOW], LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

        QColor color = LTData::colors[LTData::WHITE];
        QString str = QString("%1").arg(pitData[i].pos);
        if (pitData[i].pos > 0)
        {
        	str = "+" + str;
        	color = LTData::colors[LTData::GREEN];
        }
        else if (pitData[i].pos < 0)
        	color = LTData::colors[LTData::RED];
        setItem(ui->tableWidget_4, i+1, 5, str, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter,
                               color, LTData::colors[i % 2 == 0 ? LTData::BACKGROUND : LTData::BACKGROUND2]);

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
            ui->tableWidget_4->setColumnWidth(3, 0.3 * w);
            ui->tableWidget_4->setColumnWidth(4, 0.2 * w);
            ui->tableWidget_4->setColumnWidth(5, 0.2 * w);
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
