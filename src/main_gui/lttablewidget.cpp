#include "lttablewidget.h"
#include <iostream>
#include <QLabel>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QWheelEvent>

LTTableWidget::LTTableWidget(QObject *parent) :
    QTableWidget(static_cast<QWidget*>(parent)), eventData(EventData::getInstance()), drawCarThumbnails(true)
{    
    setItemDelegate(new LTTableItemDelegate(this));    

    upArrowIcon = QIcon(QPixmap(":/track/up-arrow.png").scaledToHeight(10, Qt::SmoothTransformation));
    downArrowIcon = QIcon(QPixmap(":/track/down-arrow.png").scaledToHeight(10, Qt::SmoothTransformation));    

    loadCarImages();
}

QTableWidgetItem* LTTableWidget::setItem(int row, int col, QString text, Qt::ItemFlags flags, int align,
             QColor textColor, QBrush background)
{
    QTableWidgetItem *item = QTableWidget::item(row, col);
    if (!item)
    {
        item = new QTableWidgetItem(text);
        item->setFlags(flags);
        QTableWidget::setItem(row, col, item);
    }
    item->setTextAlignment(align);
    item->setBackground(background);
    item->setText(text);
    item->setTextColor(textColor);

    return item;
}

void LTTableWidget::loadCarImages()
{
	showDiff = 0;
	currCar = 0;

    smallCarImg.clear();
    for (int i = 0; i < LTData::ltTeams.size(); ++i)
        smallCarImg.append(LTData::ltTeams[i].carImg.scaledToWidth(75, Qt::SmoothTransformation));
}

void LTTableWidget::updateLT()
{
    setHeader();
    switch(eventData.eventType)
    {
        case LTData::RACE_EVENT:
            updateRaceEvent();
            break;

        case LTData::PRACTICE_EVENT:
            updatePracticeEvent();
            break;

        case LTData::QUALI_EVENT:
            updateQualiEvent();
            break;
    }
}

void LTTableWidget::updateDriver(int driverIdx)
{
    switch(eventData.eventType)
    {
        case LTData::RACE_EVENT:
            updateRaceEvent(driverIdx);
            break;

        case LTData::PRACTICE_EVENT:
            updatePracticeEvent();
            break;

        case LTData::QUALI_EVENT:
            updateQualiEvent();
            break;
    }
}

void LTTableWidget::updateRaceEvent(int ddIdx)
{
    QList<DriverData> driverList = eventData.driversData;
    qSort(driverList);

    QTableWidgetItem *item;

    for (int i = 0; i < driverList.size(); ++i)
    {
        if (ddIdx != -1 && i != ddIdx)
            continue;

        if ((i+1) > rowCount()-1)
        {            
            insertRow(rowCount());                        
        }                
        setRowHeight(i+1, 22);

        QString tmp;
        if (driverList[i].colorData[LTData::RACE_POSITION] == LTData::DEFAULT || driverList[i].retired)
            tmp = "";
        else
            tmp = QString("%1.").arg(driverList[i].pos);

        setItem(i+1, 0, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::RACE_POSITION]]);


        tmp = QString("%1").arg(driverList[i].number);

        if (driverList[i].number == -1)
            tmp = "";

        item = setItem(i+1, 1, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::RACE_NUMBER]]);

        if (/*driverList[i].lastLap.sector2 != "" && driverList[i].lastLap.sector3 != "" &&*/ /*!driverList[i].lapData.isEmpty() && */!driverList[i].retired && driverList[i].number > 0)
//           ((driverList[i].lapData.size > 1 && driverList[i].lapData.last() != driverList[i].lapData[driverList[i].lapData.size()-2])) ||
//            (driverList[i].lapData.size == 1 && driverList[i].lapData.last() != driverList[i].posHistory[0]))
        {
            int lastPos, prevPos;
            if (driverList[i].lapData.size() > 1)// && driverList[i].lapData.last() != driverList[i].lapData[driverList[i].lapData.size()-2])
            {
                lastPos = driverList[i].lastLap.pos;
                prevPos = driverList[i].lapData[driverList[i].lapData.size()-2].pos;
            }
            else if (!driverList[i].posHistory.isEmpty())//if (driverList[i].lapData.size() == 1)// && driverList[i].lapData.last() != driverList[i].posHistory[0])
            {
                lastPos = driverList[i].lastLap.pos;
                prevPos = driverList[i].posHistory[driverList[i].lastLap.numLap == 1 ? 0 : driverList[i].posHistory.size()-1];
            }

            if (lastPos != prevPos)
            {
                //QIcon arrowIcon = QIcon(QPixmap((lastPos > prevPos ? ":/track/up-arow.png" : ":/track/down-arrow.png")));
                item->setIcon(lastPos < prevPos ? upArrowIcon : downArrowIcon);
            }
            else
            {
                QIcon itemIcon = item->icon();
                if (!itemIcon.isNull())
                    item->setIcon(QIcon());
            }
        }
        else
        {
            QIcon itemIcon = item->icon();
            if (!itemIcon.isNull())
                item->setIcon(QIcon());
        }


        if (drawCarThumbnails && driverList[i].number > 0 && driverList[i].number < 26)
        {
            int idx = (driverList[i].number > 13 ? driverList[i].number-2 : driverList[i].number-1) / 2;

            QLabel *lab = qobject_cast<QLabel*>(cellWidget(i+1, 2));
            if (!lab)
            {
                lab = new QLabel();                
                setCellWidget(i+1, 2, lab);
            }
            lab->setPixmap(smallCarImg[idx]);//eventData.carImagesSmall[idx]);
            lab->setAlignment(Qt::AlignCenter);
//            setCellWidget(i+1, 2, lab);
//            lab->repaint();
        }
        else
        {
            QWidget *w = cellWidget(i+1, 2);
            if (w)
                removeCellWidget(i+1, 2);
        }

        tmp = QString("%1").arg(driverList[i].driver);

        setItem(i+1, 3, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::RACE_DRIVER]]);


        tmp = driverList[i].lastLap.gap;
        setItem(i+1, 4, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::RACE_GAP]]);


        tmp = driverList[i].lastLap.interval;

        if (showDiff == 4 && driverList[i].carID != currCar && currCar > 0)
        	tmp = EventData::getInstance().calculateInterval(driverList[i], EventData::getInstance().driversData[currCar-1], -1);//EventData.getInstance().lapsCompleted);//.driversData.get(currentCar-1).lastLap.numLap);

        else if (showDiff == 4 && driverList[i].carID == currCar && currCar > 0)
        {
        	tmp = "";
        }

        setItem(i+1, 5, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::RACE_INTERVAL]]);

        tmp = QString("%1").arg(driverList[i].lastLap.lapTime);

        if (showDiff == 1 && driverList[i].carID != currCar && currCar > 0 &&
        		driverList[i].lastLap.lapTime.isValid() && EventData::getInstance().driversData[currCar-1].lastLap.lapTime.isValid())
		{
			tmp = DriverData::calculateGap(driverList[i].lastLap.lapTime,
					EventData::getInstance().driversData[currCar-1].lastLap.lapTime);

			if (tmp != "" && tmp[0] != '-')
				tmp = "+"+tmp;
		}

        setItem(i+1, 6, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::RACE_LAP_TIME]]);


        tmp = driverList[i].lastLap.sector1;
        setItem(i+1, 7, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::RACE_SECTOR_1]]);

        tmp = QString("%1").arg(driverList[i].lastLap.sector2);
        setItem(i+1, 8, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::RACE_SECTOR_2]]);

        tmp = QString("%1").arg(driverList[i].lastLap.sector3);
        setItem(i+1, 9, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::RACE_SECTOR_3]]);


        if (driverList[i].colorData[LTData::RACE_NUM_PITS] == LTData::DEFAULT || driverList[i].numPits < 1)
            tmp = "";
        else
            tmp = QString("%1").arg(driverList[i].numPits);

        setItem(i+1, 10, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::RACE_NUM_PITS]]);

    }
    if (rowCount()-1 == driverList.size())
    {
        insertRow(rowCount());
        setRowHeight(rowCount()-1, 22);
        insertRow(rowCount());
        setRowHeight(rowCount()-1, 22);
    }

    if (eventData.FLLap != -1)
    {
        setRowHeight(rowCount()-2, 22);
        setItem(rowCount()-1, 1, "Fastest lap:", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
        setSpan(rowCount()-1, 1, 1, 2);


        QString s = eventData.FLDriver;

        setItem(rowCount()-1, 3, s, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignLeft, LTData::colors[LTData::VIOLET]);


        s = "lap";
        setItem(rowCount()-1, 4, s, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::DEFAULT]);


        s = QString("%1").arg(eventData.FLLap);
        setItem(rowCount()-1, 5, s, Qt::NoItemFlags, Qt::AlignVCenter | Qt::AlignRight, LTData::colors[LTData::VIOLET]);

        s = eventData.FLTime;
        setItem(rowCount()-1, 6, s, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::VIOLET]);
    }
    else if (rowCount() > driverList.size() + 1)
    {
        for (int i = 1; i <= 10; ++i)
        {
            item = QTableWidget::item(rowCount()-1, i);
            if (item)
                item->setText("");
        }
    }
}

void LTTableWidget::updatePracticeEvent(int ddIdx)
{
    QList<DriverData> driverList = eventData.driversData;
    qSort(driverList);

    QTableWidgetItem *item;
    for (int i = 0; i < driverList.size(); ++i)
    {
        if (ddIdx != -1 && i != ddIdx)
            continue;

        if ((i+1) > rowCount()-1)
        {
            insertRow(rowCount());           
        }       
        setRowHeight(i+1, 22);

        QString tmp;
        if (driverList[i].colorData[LTData::PRACTICE_POSITION] == LTData::DEFAULT)
            tmp = "";
        else
            tmp = QString("%1.").arg(driverList[i].pos);

        setItem(i+1, 0, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_POSITION]]);

        tmp = QString("%1").arg(driverList[i].number);
        if (tmp == "-1")
            tmp = "";

        setItem(i+1, 1, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_NUMBER]]);

//        if (!driverList[i].retired && driverList[i].number > 0 && driverList[i].lapData.size() > 1)
//        {
//            int lastPos, prevPos;
//            if (driverList[i].lapData.size() > 1)// && driverList[i].lapData.last() != driverList[i].lapData[driverList[i].lapData.size()-2])
//            {
//                lastPos = driverList[i].lastLap.pos;
//                prevPos = driverList[i].lapData[driverList[i].lapData.size()-2].pos;
//            }
//            else if (!driverList[i].posHistory.isEmpty())//if (driverList[i].lapData.size() == 1)// && driverList[i].lapData.last() != driverList[i].posHistory[0])
//            {
//                lastPos = driverList[i].lastLap.pos;
//                prevPos = driverList[i].posHistory[driverList[i].lastLap.numLap == 1 ? 0 : driverList[i].posHistory.size()-1];
//            }

//            if (lastPos != prevPos)
//            {
//                //QIcon arrowIcon = QIcon(QPixmap((lastPos > prevPos ? ":/track/up-arow.png" : ":/track/down-arrow.png")));
//                item->setIcon(lastPos < prevPos ? upArrowIcon : downArrowIcon);
//            }
//            else
//            {
//                QIcon itemIcon = item->icon();
//                if (!itemIcon.isNull())
//                    item->setIcon(QIcon());
//            }
//        }
//        else
//        {
//            QIcon itemIcon = item->icon();
//            if (!itemIcon.isNull())
//                item->setIcon(QIcon());
//        }

        if (drawCarThumbnails && driverList[i].number > 0 && driverList[i].number < 26)
        {
            int idx = (driverList[i].number > 13 ? driverList[i].number-2 : driverList[i].number-1) / 2;

            QLabel *lab = qobject_cast<QLabel*>(cellWidget(i+1, 2));
            if (!lab)
            {
                lab = new QLabel();
                setCellWidget(i+1, 2, lab);
            }
            lab->setPixmap(smallCarImg[idx]);//eventData.carImagesSmall[idx]);
            lab->setAlignment(Qt::AlignCenter);
//            setCellWidget(i+1, 2, lab);
//            lab->repaint();
        }
        else
        {
            QWidget *w = cellWidget(i+1, 2);
            if (w)
                removeCellWidget(i+1, 2);
        }

        tmp = QString("%1").arg(driverList[i].driver);
        setItem(i+1, 3, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_DRIVER]]);

        tmp = driverList[i].lastLap.lapTime.toString();
        if (showDiff == 1 && driverList[i].carID != currCar && currCar > 0 &&
				driverList[i].lastLap.lapTime.isValid() && EventData::getInstance().driversData[currCar-1].lastLap.lapTime.isValid())
		{
			tmp = DriverData::calculateGap(driverList[i].lastLap.lapTime,
					EventData::getInstance().driversData[currCar-1].lastLap.lapTime);

			if (tmp != "" && tmp[0] != '-')
				tmp = "+"+tmp;
		}
        setItem(i+1, 4, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_BEST]]);

        tmp = QString("%1").arg(driverList[i].lastLap.gap);
        if (driverList[i].pos == 1)
            tmp = "";

        setItem(i+1, 5, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_GAP]]);

        tmp = driverList[i].lastLap.sector1;
        setItem(i+1, 6, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_SECTOR_1]]);

        tmp = driverList[i].lastLap.sector2;
        setItem(i+1, 7, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_SECTOR_2]]);

        tmp = driverList[i].lastLap.sector3;
        setItem(i+1, 8, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_SECTOR_3]]);


        tmp = QString("%1").arg(driverList[i].lastLap.numLap);
        if (tmp == "-1")
            tmp = "";

        setItem(i+1, 9, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::PRACTICE_LAP]]);
    }    

    if (driverList.size() > 0 && driverList[0].bestLap.lapTime.toString() != "")
    {
        if (rowCount()-1 == driverList.size())
        {
            insertRow(rowCount());
            setRowHeight(rowCount()-1, 22);
            insertRow(rowCount());
            setRowHeight(rowCount()-1, 22);
        }
        setRowHeight(rowCount()-2, 22);

        for (int i = 0; i <= 9; ++i)
        {
            setItem(rowCount()-1, i, "", Qt::NoItemFlags, Qt::AlignCenter);
            item = QTableWidget::item(rowCount()-1, i);
            if (item)
                item->setText("");

            if (i == 2)
            {
//                QLabel *lab = qobject_cast<QLabel*>(QTableWidget::cellWidget(rowCount()-1, 2));
//                if (lab)
                    removeCellWidget(rowCount()-1, 2);
            }
        }

        setItem(rowCount()-1, 3, "107% time", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::RED]);

        QString time = driverList[0].bestLap.lapTime.calc107p().toString();
        setItem(rowCount()-1, 4, time, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::RED]);
    }
    else if (rowCount() > driverList.size() + 1)
    {
        for (int i = 1; i <= 9; ++i)
        {
            item = QTableWidget::item(rowCount()-1, i);
            if (item)
                item->setText("");
        }
    }
}

void LTTableWidget::updateQualiEvent(int ddIdx)
{
    QList<DriverData> driverList = eventData.driversData;
    qSort(driverList);

    LapTime bestQ1;
    QTableWidgetItem *item;
    for (int i = 0; i < driverList.size(); ++i)
    {
        //std::cout<<"i="<<i<<std::endl;
        //if (!driverList[i].lastLap)
          //  continue;

        if (driverList[i].q1 < bestQ1)
            bestQ1 = driverList[i].q1;

        if (ddIdx != -1 && i != ddIdx)
            continue;

        if ((i+1) > rowCount()-1)
        {
            insertRow(rowCount());            
        }        
        setRowHeight(i+1, 22);


        QString tmp;
        if (driverList[i].colorData[LTData::QUALI_POSITION] == LTData::DEFAULT)
            tmp = "";
        else
            tmp = QString("%1.").arg(driverList[i].pos);

        setItem(i+1, 0, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_POSITION]]);

        tmp = QString("%1").arg(driverList[i].number);
        if (tmp == "-1")
            tmp = "";

        setItem(i+1, 1, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_NUMBER]]);

//        if (!driverList[i].retired && driverList[i].number > 0 && driverList[i].lapData.size() > 1)
//        {
//            int lastPos, prevPos;
//            if (driverList[i].lapData.size() > 1)// && driverList[i].lapData.last() != driverList[i].lapData[driverList[i].lapData.size()-2])
//            {
//                lastPos = driverList[i].lastLap.pos;
//                prevPos = driverList[i].lapData[driverList[i].lapData.size()-2].pos;
//            }
//            else if (!driverList[i].posHistory.isEmpty())//if (driverList[i].lapData.size() == 1)// && driverList[i].lapData.last() != driverList[i].posHistory[0])
//            {
//                lastPos = driverList[i].lastLap.pos;
//                prevPos = driverList[i].posHistory[driverList[i].lastLap.numLap == 1 ? 0 : driverList[i].posHistory.size()-1];
//            }

//            if (lastPos != prevPos)
//            {
//                //QIcon arrowIcon = QIcon(QPixmap((lastPos > prevPos ? ":/track/up-arow.png" : ":/track/down-arrow.png")));
//                item->setIcon(lastPos < prevPos ? upArrowIcon : downArrowIcon);
//            }
//            else
//            {
//                QIcon itemIcon = item->icon();
//                if (!itemIcon.isNull())
//                    item->setIcon(QIcon());
//            }
//        }
//        else
//        {
//            QIcon itemIcon = item->icon();
//            if (!itemIcon.isNull())
//                item->setIcon(QIcon());
//        }



        if (drawCarThumbnails && driverList[i].number > 0 && driverList[i].number < 26)
        {
            int idx = (driverList[i].number > 13 ? driverList[i].number-2 : driverList[i].number-1) / 2;

            QLabel *lab = qobject_cast<QLabel*>(cellWidget(i+1, 2));
            if (!lab)
            {
                lab = new QLabel();
                setCellWidget(i+1, 2, lab);
            }
            lab->setPixmap(smallCarImg[idx]);//eventData.carImagesSmall[idx]);
            lab->setAlignment(Qt::AlignCenter);
//            setCellWidget(i+1, 2, lab);
//            lab->repaint();
        }
        else
        {
            QWidget *w = cellWidget(i+1, 2);
            if (w)
                removeCellWidget(i+1, 2);
        }

        tmp = QString("%1").arg(driverList[i].driver);

        setItem(i+1, 3, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_DRIVER]]);

        tmp = driverList[i].q1;

        if (showDiff == 1 && driverList[i].carID != currCar && currCar > 0 &&
				LapTime(driverList[i].q1).isValid() && LapTime(EventData::getInstance().driversData[currCar-1].q1).isValid())
		{
			tmp = DriverData::calculateGap(LapTime(driverList[i].q1),
					LapTime(EventData::getInstance().driversData[currCar-1].q1));

			if (tmp != "" && tmp[0] != '-')
				tmp = "+"+tmp;
		}
        setItem(i+1, 4, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_PERIOD_1]]);

        tmp = driverList[i].q2;
        if (showDiff == 2 && driverList[i].carID != currCar && currCar > 0 &&
				LapTime(driverList[i].q2).isValid() && LapTime(EventData::getInstance().driversData[currCar-1].q2).isValid())
		{
			tmp = DriverData::calculateGap(LapTime(driverList[i].q2),
					LapTime(EventData::getInstance().driversData[currCar-1].q2));

			if (tmp != "" && tmp[0] != '-')
				tmp = "+"+tmp;
		}
        setItem(i+1, 5, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_PERIOD_2]]);

        tmp = driverList[i].q3;
        if (showDiff == 3 && driverList[i].carID != currCar && currCar > 0 &&
				LapTime(driverList[i].q3).isValid() && LapTime(EventData::getInstance().driversData[currCar-1].q3).isValid())
		{
			tmp = DriverData::calculateGap(LapTime(driverList[i].q3),
					LapTime(EventData::getInstance().driversData[currCar-1].q3));

			if (tmp != "" && tmp[0] != '-')
				tmp = "+"+tmp;
		}
        setItem(i+1, 6, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_PERIOD_3]]);

        tmp = driverList[i].lastLap.sector1;
        setItem(i+1, 7, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_SECTOR_1]]);

        tmp = driverList[i].lastLap.sector2;
        setItem(i+1, 8, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_SECTOR_2]]);

        tmp = driverList[i].lastLap.sector3;
        setItem(i+1, 9, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_SECTOR_3]]);


        tmp = QString("%1").arg(driverList[i].lastLap.numLap);
        if (tmp == "-1")
            tmp = "";

        setItem(i+1, 10, tmp, Qt::ItemIsSelectable | Qt::ItemIsEnabled, Qt::AlignCenter, LTData::colors[driverList[i].colorData[LTData::QUALI_LAP]]);
    }

    if (driverList.size() > 0 && bestQ1.toString() != "")
    {
        if (rowCount()-1 == driverList.size())
        {
            insertRow(rowCount());
            setRowHeight(rowCount()-1, 22);
            insertRow(rowCount());
            setRowHeight(rowCount()-1, 22);
        }
        setRowHeight(rowCount()-2, 22);

        for (int i = 0; i <= 10; ++i)
        {
            item = QTableWidget::item(rowCount()-1, i);
            if (item)
                item->setText("");

            if (i == 2)
            {
//                QLabel *lab = qobject_cast<QLabel*>(QTableWidget::cellWidget(rowCount()-1, 2));
//                if (lab)
                    removeCellWidget(rowCount()-1, 2);
            }
        }

        setItem(rowCount()-1, 3, "Q1 107% time", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::RED]);

        QString time = bestQ1.calc107p().toString();
        setItem(rowCount()-1, 4, time, Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::RED]);
    }
    else if (rowCount() > driverList.size() + 1)
    {
        for (int i = 1; i <= 10; ++i)
        {
            item = QTableWidget::item(rowCount()-1, i);
            if (item)
                item->setText("");
        }
    }
}

void LTTableWidget::setHeader()
{
    if (eventData.eventType == 0)
        return;

    if (eventType != eventData.eventType)
    {
        for (int i = rowCount()-1; i >= 0; --i)
            removeRow(i);

        clear();
        eventType = eventData.eventType;
    }

    if (rowCount() == 0)
        insertRow(0);
    else
    {
        QTableWidgetItem *item = QTableWidget::item(0, 0);
        if (item)
            return;
    }

    QTableWidgetItem *item;
    switch(eventData.eventType)
    {
        case LTData::RACE_EVENT:
//            setColumnWidth(0, 30);
//            setColumnWidth(1, 40);
//            setColumnWidth(2, 90);
//            setColumnWidth(3, 150);
//            setColumnWidth(4, 50);
//            setColumnWidth(5, 80);
//            setColumnWidth(6, 100);
//            setColumnWidth(7, 80);
//            setColumnWidth(8, 80);
//            setColumnWidth(9, 80);
//            setColumnWidth(10, 50);


            setItem(0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 1, "", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 2, "", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 3, "Name", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 4, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 5, "Interval", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 6, "Time", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 7, "S1", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 8, "S2", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 9, "S3", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 10, "Pit", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);

            resizeEvent(0);
            break;                       

        case LTData::PRACTICE_EVENT:       
//            setColumnWidth(0, 30);
//            setColumnWidth(1, 30);
//            setColumnWidth(2, 90);
//            setColumnWidth(3, 150);
//            setColumnWidth(4, 100);
//            setColumnWidth(5, 80);
//            setColumnWidth(6, 80);
//            setColumnWidth(7, 80);
//            setColumnWidth(8, 80);
//            setColumnWidth(9, 50);

            setItem(0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 1, "", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 2, "", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 3, "Name", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 4, "Best", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 5, "Gap", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 6, "S1", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 7, "S2", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 8, "S3", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 9, "Laps", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);

            resizeEvent(0);
            break;

        case LTData::QUALI_EVENT:
//            setColumnWidth(0, 30);
//            setColumnWidth(1, 30);
//            setColumnWidth(2, 90);
//            setColumnWidth(3, 150);
//            setColumnWidth(4, 100);
//            setColumnWidth(5, 100);
//            setColumnWidth(6, 100);
//            setColumnWidth(7, 80);
//            setColumnWidth(8, 80);
//            setColumnWidth(9, 80);
//            setColumnWidth(10, 50);

            setItem(0, 0, "P", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 1, "", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 2, "", Qt::NoItemFlags, Qt::AlignRight | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 3, "Name", Qt::NoItemFlags, Qt::AlignLeft | Qt::AlignVCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 4, "Q1", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 5, "Q2", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 6, "Q3", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 7, "S1", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 8, "S2", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 9, "S3", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);
            setItem(0, 10, "Laps", Qt::NoItemFlags, Qt::AlignCenter, LTData::colors[LTData::DEFAULT]);

            resizeEvent(0);
            break;
    }
}



void LTTableWidget::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Up && currentRow() > 1) ||
        (event->key() == Qt::Key_Down && currentRow() < eventData.driversData.size()))
    {
        QTableWidget::keyPressEvent(event);
        emit cellClicked(currentRow(), currentColumn());
    }
    if (event->key() == Qt::Key_Return)
    {
        QTableWidget::keyPressEvent(event);
        emit cellDoubleClicked(currentRow(), currentColumn());
    }

    if (event->key() == Qt::Key_Home)
    {
//        QTableWidget::keyPressEvent(event);
        setCurrentCell(1, currentColumn());
        emit cellClicked(1, currentColumn());
    }

    if (event->key() == Qt::Key_End)
    {
//        QTableWidget::keyPressEvent(event);
        setCurrentCell(eventData.driversData.size(), currentColumn());
        emit cellClicked(eventData.driversData.size(), currentColumn());
    }

    if (event->key() == Qt::Key_PageDown)
    {
        int row = currentRow() + eventData.driversData.size()/2;
        if (row > eventData.driversData.size())
            row = eventData.driversData.size();

        setCurrentCell(row, currentColumn());
        emit cellClicked(row, currentColumn());
//        QTableWidget::keyPressEvent(event);
    }

    if (event->key() == Qt::Key_PageUp)
    {
        int row = currentRow() - eventData.driversData.size()/2;
        if (row < 1)
            row = 1;

        setCurrentCell(row, currentColumn());
        emit cellClicked(row, currentColumn());
//        QTableWidget::keyPressEvent(event);
    }
}

void LTTableWidget::resizeEvent(QResizeEvent *e)
{
    int w = viewport()->width()/*-viewport()->width()*0.0115*/;//event->size().width()-event->size().width()*0.0115;

    switch(eventData.eventType)
    {
        case LTData::RACE_EVENT:
            if (drawCarThumbnails)
            {
                setColumnWidth(0, 0.04 * w);
                setColumnWidth(1, 0.06 * w);
                setColumnWidth(2, 0.115 * w);
                setColumnWidth(3, 0.18 * w);
                setColumnWidth(4, 0.06 * w);
                setColumnWidth(5, 0.1 * w);
                setColumnWidth(6, 0.12 * w);
                setColumnWidth(7, 0.09 * w);
                setColumnWidth(8, 0.09 * w);
                setColumnWidth(9, 0.09 * w);
                setColumnWidth(10, 0.06 * w);
            }
            else
            {
                setColumnWidth(0, 0.05 * w);
                setColumnWidth(1, 0.07 * w);
                setColumnWidth(2, 0);
                setColumnWidth(3, 0.22 * w);
                setColumnWidth(4, 0.1 * w);
                setColumnWidth(5, 0.11 * w);
                setColumnWidth(6, 0.11 * w);
                setColumnWidth(7, 0.1 * w);
                setColumnWidth(8, 0.1 * w);
                setColumnWidth(9, 0.1 * w);
                setColumnWidth(10, 0.04 * w);
            }
            break;

        case LTData::PRACTICE_EVENT:
            if (drawCarThumbnails)
            {
                setColumnWidth(0, 0.04 * w);
                setColumnWidth(1, 0.05 * w);
                setColumnWidth(2, 0.12 * w);
                setColumnWidth(3, 0.2 * w);
                setColumnWidth(4, 0.14 * w);
                setColumnWidth(5, 0.11 * w);
                setColumnWidth(6, 0.09 * w);
                setColumnWidth(7, 0.09 * w);
                setColumnWidth(8, 0.09 * w);
                setColumnWidth(9, 0.09 * w);
                setColumnWidth(10, 0);
            }
            else
            {
                setColumnWidth(0, 0.05 * w);
                setColumnWidth(1, 0.07 * w);
                setColumnWidth(2, 0);
                setColumnWidth(3, 0.2 * w);
                setColumnWidth(4, 0.14 * w);
                setColumnWidth(5, 0.18 * w);
                setColumnWidth(6, 0.09 * w);
                setColumnWidth(7, 0.09 * w);
                setColumnWidth(8, 0.09 * w);
                setColumnWidth(9, 0.09 * w);
                setColumnWidth(10, 0);
            }
            break;

        case LTData::QUALI_EVENT:
            if (drawCarThumbnails)
            {
                setColumnWidth(0, 0.04 * w);
                setColumnWidth(1, 0.044 * w);
                setColumnWidth(2, 0.12 * w);
                setColumnWidth(3, 0.18 * w);
                setColumnWidth(4, 0.1 * w);
                setColumnWidth(5, 0.1 * w);
                setColumnWidth(6, 0.1 * w);
                setColumnWidth(7, 0.09 * w);
                setColumnWidth(8, 0.09 * w);
                setColumnWidth(9, 0.09 * w);
                setColumnWidth(10, 0.06 * w);
            }
            else
            {
                setColumnWidth(0, 0.05 * w);
                setColumnWidth(1, 0.05 * w);
                setColumnWidth(2, 0);
                setColumnWidth(3, 0.21 * w);
                setColumnWidth(4, 0.11 * w);
                setColumnWidth(5, 0.11 * w);
                setColumnWidth(6, 0.11 * w);
                setColumnWidth(7, 0.1 * w);
                setColumnWidth(8, 0.1 * w);
                setColumnWidth(9, 0.1 * w);
                setColumnWidth(10, 0.06 * w);
            }
            break;
    }
    QTableWidget::resizeEvent(e);
}

bool LTTableWidget::printDiff(int row, int col)
{
	if (col < 4 || col > 6)
		return false;

	QList<DriverData> driverList = eventData.driversData;
	qSort(driverList);

	DriverData dd = driverList[row-1];
	int id = dd.carID;

	if (col == 4 && EventData::getInstance().eventType != LTData::RACE_EVENT)
	{
		if (showDiff == 1 && id == currCar)
		{
			showDiff = 0;
			currCar = 0;
		}
		else
		{
			showDiff = 1;
			currCar = id;
		}
		updateLT();
		return true;
	}

	if (col == 5 && EventData::getInstance().eventType == LTData::RACE_EVENT)
	{
		if (showDiff == 4 && id == currCar)
		{
			showDiff = 0;
			currCar = 0;
		}
		else
		{
			showDiff = 4;
			currCar = id;
		}
		updateLT();
		return true;
	}

	if (col == 5 && EventData::getInstance().eventType == LTData::QUALI_EVENT)
	{
		if (showDiff == 2 && id == currCar)
		{
			showDiff = 0;
			currCar = 0;
		}
		else
		{
			showDiff = 2;
			currCar = id;
		}
		updateLT();
		return true;
	}

	if (col == 6 && EventData::getInstance().eventType == LTData::QUALI_EVENT)
	{
		if (showDiff == 3 && id == currCar)
		{
			showDiff = 0;
			currCar = 0;
		}
		else
		{
			showDiff = 3;
			currCar = id;
		}
		updateLT();
		return true;
	}

	if (col == 6 && EventData::getInstance().eventType == LTData::RACE_EVENT)
	{
		if (showDiff == 1 && id == currCar)
		{
			showDiff = 0;
			currCar = 0;
		}
		else
		{
			showDiff = 1;
			currCar = id;
		}
		updateLT();
		return true;
	}

	return false;
}

void LTTableWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() < 0 && currentRow() < eventData.driversData.size())
    {
        setCurrentCell(currentRow()+1, currentColumn());
        emit cellClicked(currentRow(), currentColumn());
    }
    else if (event->delta() > 0 && currentRow() > 1)
    {
        setCurrentCell(currentRow()-1, currentColumn());
        emit cellClicked(currentRow(), currentColumn());
    }
    QTableWidget::wheelEvent(event);
}

void LTTableWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        int row = index.row();

        if (row > 0 && row <= eventData.driversData.size())
        {
        	int col = index.column();

			printDiff(row, col);

            QTableWidget::mousePressEvent(event);
        }
    }
}
