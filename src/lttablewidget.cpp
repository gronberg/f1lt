#include "lttablewidget.h"
#include <iostream>
#include <QLabel>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QWheelEvent>

LTTableWidget::LTTableWidget(QObject *parent) :
    QTableWidget(static_cast<QWidget*>(parent)), eventData(EventData::getInstance())
{    
    setItemDelegate(new LTTableItemDelegate(this));    

    upArrowIcon = QIcon(QPixmap(":/track/up-arrow.png").scaledToHeight(10, Qt::SmoothTransformation));
    downArrowIcon = QIcon(QPixmap(":/track/down-arrow.png").scaledToHeight(10, Qt::SmoothTransformation));    

    loadCarImages();
}

void LTTableWidget::loadCarImages()
{
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

        item = QTableWidget::item(i+1, 0);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            setItem(i+1, 0, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_POSITION]]);


        tmp = QString("%1").arg(driverList[i].number);

        if (driverList[i].number == -1)
            tmp = "";

        item = QTableWidget::item(i+1, 1);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(i+1, 1, item);
        }
        else
            item->setText(tmp);

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

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_NUMBER]]);


        if (driverList[i].number > 0 && driverList[i].number < 26)
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

        tmp = QString("%1").arg(driverList[i].driver);
        item = QTableWidget::item(i+1, 3);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            setItem(i+1, 3, item);
        }
        else        
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_DRIVER]]);


        tmp = driverList[i].lastLap.gap;
        item = QTableWidget::item(i+1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(i+1, 4, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_GAP]]);


        tmp = driverList[i].lastLap.interval;

        item = QTableWidget::item(i+1, 5);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(i+1, 5, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_INTERVAL]]);


            tmp = QString("%1").arg(driverList[i].lastLap.lapTime);

        item = QTableWidget::item(i+1, 6);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 6, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_LAP_TIME]]);


        tmp = driverList[i].lastLap.sector1;
        item = QTableWidget::item(i+1, 7);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 7, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_SECTOR_1]]);

        tmp = QString("%1").arg(driverList[i].lastLap.sector2);
        item = QTableWidget::item(i+1, 8);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 8, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_SECTOR_2]]);

        tmp = QString("%1").arg(driverList[i].lastLap.sector3);
        item = QTableWidget::item(i+1, 9);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 9, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_SECTOR_3]]);


        if (driverList[i].colorData[LTData::RACE_NUM_PITS] == LTData::DEFAULT)
            tmp = "";
        else
            tmp = QString("%1").arg(driverList[i].numPits);

        item = QTableWidget::item(i+1, 10);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 10, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::RACE_NUM_PITS]]);

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
        item = QTableWidget::item(rowCount()-1, 1);
        if (!item)
        {
            item = new QTableWidgetItem("Fastest lap:");            
            setItem(rowCount()-1, 1, item);
        }
        else
            item->setText("Fastest lap:");

        item->setFlags(Qt::NoItemFlags);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setSpan(rowCount()-1, 1, 1, 2);


        QString s = eventData.FLDriver;

        item = QTableWidget::item(rowCount()-1, 3);
        if (!item)
        {
            item = new QTableWidgetItem(s);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            setItem(rowCount()-1, 3, item);
        }
        else
            item->setText(s);               
        item->setTextColor(LTData::colors[LTData::VIOLET]);


        s = "lap";
        item = QTableWidget::item(rowCount()-1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(s);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            setItem(rowCount()-1, 4, item);
        }
        else
            item->setText(s);
        item->setTextColor(LTData::colors[LTData::DEFAULT]);


        s = QString("%1").arg(eventData.FLLap);
        item = QTableWidget::item(rowCount()-1, 5);
        if (!item)
        {
            item = new QTableWidgetItem(s);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(rowCount()-1, 5, item);
        }
        else
            item->setText(s);
        item->setTextColor(LTData::colors[LTData::VIOLET]);

        s = eventData.FLTime;
        item = QTableWidget::item(rowCount()-1, 6);
        if (!item)
        {
            item = new QTableWidgetItem(s);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            setItem(rowCount()-1, 6, item);
        }
        else
            item->setText(s);
        item->setTextColor(LTData::colors[LTData::VIOLET]);
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
        if (driverList[i].colorData[LTData::RACE_POSITION] == LTData::DEFAULT)
            tmp = "";
        else
            tmp = QString("%1.").arg(driverList[i].pos);

        item = QTableWidget::item(i+1, 0);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            setItem(i+1, 0, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_POSITION]]);


        tmp = QString("%1").arg(driverList[i].number);
        if (tmp == "-1") tmp = "";
        item = QTableWidget::item(i+1, 1);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(i+1, 1, item);
        }
        else
            item->setText(tmp);

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
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_NUMBER]]);        

        if (driverList[i].number > 0 && driverList[i].number < 26)
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

        tmp = QString("%1").arg(driverList[i].driver);
        item = QTableWidget::item(i+1, 3);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            setItem(i+1, 3, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_DRIVER]]);


        tmp = driverList[i].lastLap.lapTime.toString();
        item = QTableWidget::item(i+1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 4, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_BEST]]);

        tmp = QString("%1").arg(driverList[i].lastLap.gap);
        if (driverList[i].pos == 1)
            tmp = "";
        item = QTableWidget::item(i+1, 5);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(i+1, 5, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_GAP]]);


//        tmp = QString("%1").arg(driverList[i].lastLap.interval);
//        item = new QTableWidgetItem(tmp);
//        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
//        item->setTextColor(colors[driverList[i].colorData[LTData::PRACTICE_INTERVAL]]));
//        setItem(i+1, 5, item);


        tmp = driverList[i].lastLap.sector1;
        item = QTableWidget::item(i+1, 6);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 6, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_SECTOR_1]]);


        tmp = driverList[i].lastLap.sector2;
        item = QTableWidget::item(i+1, 7);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 7, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_SECTOR_2]]);


        tmp = driverList[i].lastLap.sector3;
        item = QTableWidget::item(i+1, 8);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 8, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_SECTOR_3]]);


        tmp = QString("%1").arg(driverList[i].lastLap.numLap);
        if (tmp == "-1") tmp = "";
        item = QTableWidget::item(i+1, 9);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 9, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::PRACTICE_LAP]]);
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

        item = QTableWidget::item(rowCount()-1, 3);
        if (!item)
        {
            item = new QTableWidgetItem("107% time");
            setItem(rowCount()-1, 3, item);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else
            item->setText("107% time");
        item->setTextColor(LTData::colors[LTData::RED]);

        QString time = driverList[0].bestLap.lapTime.calc107p().toString();
        item = QTableWidget::item(rowCount()-1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(time);
            setItem(rowCount()-1, 4, item);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);

        }
        else
            item->setText(time);
        item->setTextColor(LTData::colors[LTData::RED]);
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
        if (driverList[i].colorData[LTData::RACE_POSITION] == LTData::DEFAULT)
            tmp = "";
        else
            tmp = QString("%1.").arg(driverList[i].pos);

        item = QTableWidget::item(i+1, 0);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            setItem(i+1, 0, item);
        }
        else
            item->setText(tmp);

        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_POSITION]]);


        tmp = QString("%1").arg(driverList[i].number);
        if (tmp == "-1") tmp = "";
        item = QTableWidget::item(i+1, 1);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            setItem(i+1, 1, item);
        }
        else
            item->setText(tmp);

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
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_NUMBER]]);



        if (driverList[i].number > 0 && driverList[i].number < 26)
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

        tmp = QString("%1").arg(driverList[i].driver);
        item = QTableWidget::item(i+1, 3);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            setItem(i+1, 3, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_DRIVER]]);


        tmp = driverList[i].q1;
        item = QTableWidget::item(i+1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 4, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_PERIOD_1]]);


        tmp = driverList[i].q2;
        item = QTableWidget::item(i+1, 5);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 5, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_PERIOD_2]]);



//        tmp = QString("%1").arg(driverList[i].lastLap.interval);
//        item = new QTableWidgetItem(tmp);
//        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
//        item->setTextColor(colors[driverList[i].colorData[LTData::PRACTICE_INTERVAL]]));
//        setItem(i+1, 5, item);


        tmp = driverList[i].q3;
        item = QTableWidget::item(i+1, 6);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 6, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_PERIOD_3]]);


//        if (driverList[i].lastLap)
            tmp = driverList[i].lastLap.sector1;
//        else
//            tmp = "";

        item = QTableWidget::item(i+1, 7);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 7, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_SECTOR_1]]);


//        if (driverList[i].lastLap)
            tmp = driverList[i].lastLap.sector2;
//        else
//            tmp = "";

        item = QTableWidget::item(i+1, 8);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 8, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_SECTOR_2]]);



//        if (driverList[i].lastLap)
            tmp = driverList[i].lastLap.sector3;
//        else
//            tmp = "";
        item = QTableWidget::item(i+1, 9);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 9, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_SECTOR_3]]);


//        if (driverList[i].lastLap)
            tmp = QString("%1").arg(driverList[i].lastLap.numLap);
            if (tmp == "-1") tmp = "";
//        else
//            tmp = "";
        item = QTableWidget::item(i+1, 10);
        if (!item)
        {
            item = new QTableWidgetItem(tmp);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            setItem(i+1, 10, item);
        }
        else
            item->setText(tmp);
        item->setTextColor(LTData::colors[driverList[i].colorData[LTData::QUALI_LAP]]);
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

        item = QTableWidget::item(rowCount()-1, 3);
        if (!item)
        {
            item = new QTableWidgetItem("107% Q1 time");
            setItem(rowCount()-1, 3, item);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else
            item->setText("107% Q1 time");
        item->setTextColor(LTData::colors[LTData::RED]);

        QString time = bestQ1.calc107p().toString();
        item = QTableWidget::item(rowCount()-1, 4);
        if (!item)
        {
            item = new QTableWidgetItem(time);
            setItem(rowCount()-1, 4, item);
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);

        }
        else
            item->setText(time);
        item->setTextColor(LTData::colors[LTData::RED]);
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


            item = new QTableWidgetItem("P");
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 0, item);

            item = new QTableWidgetItem("Name");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 3, item);

            item = new QTableWidgetItem("Gap");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 4, item);

            item = new QTableWidgetItem("Interval");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 5, item);

            item = new QTableWidgetItem("Time");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 6, item);

            item = new QTableWidgetItem("S1");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 7, item);

            item = new QTableWidgetItem("S2");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 8, item);

            item = new QTableWidgetItem("S3");
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 9, item);

            item = new QTableWidgetItem("Pit");
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
//            item->setBackgroundColor(QColor(80, 80, 80));
            setItem(0, 10, item);

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

            item = new QTableWidgetItem("P");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 0, item);

            item = new QTableWidgetItem();
            item->setFlags(Qt::NoItemFlags);
            setItem(0, 1, item);

            item = new QTableWidgetItem();
            item->setFlags(Qt::NoItemFlags);
            setItem(0, 2, item);

            item = new QTableWidgetItem("Name");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 3, item);

            item = new QTableWidgetItem("Best");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 4, item);

            item = new QTableWidgetItem("Gap");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 5, item);

            item = new QTableWidgetItem("S1");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 6, item);

            item = new QTableWidgetItem("S2");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 7, item);

            item = new QTableWidgetItem("S3");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 8, item);

            item = new QTableWidgetItem("Laps");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 9, item);

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

            item = new QTableWidgetItem("P");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 0, item);

            item = new QTableWidgetItem();
            item->setFlags(Qt::NoItemFlags);
            setItem(0, 1, item);

            item = new QTableWidgetItem();
            item->setFlags(Qt::NoItemFlags);
            setItem(0, 2, item);

            item = new QTableWidgetItem("Name");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 3, item);

            item = new QTableWidgetItem("Q1");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 4, item);

            item = new QTableWidgetItem("Q2");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 5, item);

            item = new QTableWidgetItem("Q3");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 6, item);

            item = new QTableWidgetItem("S1");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 7, item);

            item = new QTableWidgetItem("S2");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 8, item);

            item = new QTableWidgetItem("S3");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 9, item);

            item = new QTableWidgetItem("Laps");
            item->setFlags(Qt::NoItemFlags);
            item->setTextAlignment(Qt::AlignCenter);
            item->setTextColor(LTData::colors[LTData::DEFAULT]);
            setItem(0, 10, item);

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

void LTTableWidget::resizeEvent(QResizeEvent *)
{
    int w = viewport()->width()/*-viewport()->width()*0.0115*/;//event->size().width()-event->size().width()*0.0115;

    switch(eventData.eventType)
    {
        case LTData::RACE_EVENT:
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
            break;

        case LTData::PRACTICE_EVENT:
            setColumnWidth(0, 0.04 * w);
            setColumnWidth(1, 0.05 * w);
            setColumnWidth(2, 0.12 * w);
            setColumnWidth(3, 0.2 * w);
            setColumnWidth(4, 0.14 * w);
            setColumnWidth(5, 0.1 * w);
            setColumnWidth(6, 0.09 * w);
            setColumnWidth(7, 0.09 * w);
            setColumnWidth(8, 0.09 * w);
            setColumnWidth(9, 0.09 * w);
            setColumnWidth(10, 0);
            break;

        case LTData::QUALI_EVENT:
            setColumnWidth(0, 0.04 * w);
            setColumnWidth(1, 0.044 * w);
            setColumnWidth(2, 0.1 * w);
            setColumnWidth(3, 0.18 * w);
            setColumnWidth(4, 0.105 * w);
            setColumnWidth(5, 0.105 * w);
            setColumnWidth(6, 0.105 * w);
            setColumnWidth(7, 0.09 * w);
            setColumnWidth(8, 0.09 * w);
            setColumnWidth(9, 0.09 * w);
            setColumnWidth(10, 0.06 * w);
            break;
    }
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
            QTableWidget::mousePressEvent(event);
    }
}
