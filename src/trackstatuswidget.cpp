#include "trackstatuswidget.h"
#include <QBuffer>
#include <QLabel>
#include <QTableWidgetItem>
#include <QDebug>

TrackStatusWidget::TrackStatusWidget(QWidget *parent) :
    QTableWidget(parent), eventData(EventData::getInstance())
{
//    document()->setDefaultStyleSheet(stylesheet);
//    setStyleSheet ( "text-align: center");

//    QByteArray bArray;
//    QBuffer buf(&bArray);
//    QPixmap pix;

//    pix = QPixmap(":/track/green-light.png").scaledToHeight(20, Qt::SmoothTransformation);
//    pix.save(&buf, "PNG");
//    icons[0] = "<img src=\"data:image/png;base64," + bArray.toBase64() + "\"/>";
//    bArray.clear();
//    buf.close();

//    buf.setBuffer(&bArray);
//    pix = QPixmap(":/track/red-light.png").scaledToHeight(20, Qt::SmoothTransformation);
//    pix.save(&buf, "PNG");
//    icons[1] = "<img src=\"data:image/png;base64," + bArray.toBase64() + "\"/>";
//    bArray.clear();
//    buf.close();

//    buf.setBuffer(&bArray);
//    pix = QPixmap(":/track/yellow-light.png").scaledToHeight(20, Qt::SmoothTransformation);
//    pix.save(&buf, "PNG");
//    icons[2] = "<img src=\"data:image/png;base64," + bArray.toBase64() + "\"/>";
//    bArray.clear();
//    buf.close();

//    buf.setBuffer(&bArray);
//    pix = QPixmap(":/track/sc.png").scaledToHeight(20, Qt::SmoothTransformation);
//    pix.save(&buf, "PNG");
//    icons[3] = "<img src=\"data:image/png;base64," + bArray.toBase64() + "\"/>";

    icons[0] = QPixmap(":/track/green-light.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[1] = QPixmap(":/track/red-light.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[2] = QPixmap(":/track/yellow-light.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[3] = QPixmap(":/track/sc.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[4] = QPixmap(":/track/flag.png").scaledToHeight(20, Qt::SmoothTransformation);

    //    for (int i = 0; i < 2; ++i)
//        insertRow(i);

//    setStyleSheet("QHeaderView::section:horizontal {margin-right: 20}");
}

void TrackStatusWidget::setupItems()
{
    insertRow(0);    

    QLabel *lab = new QLabel();
    lab->setAlignment(Qt::AlignCenter);
    lab->resize(20, 20);
    setCellWidget(0, 0, lab);

    QTableWidgetItem *item;

    QFont tFont = QTableWidget::font();
    tFont.setPointSize(tFont.pointSize() + 4);
    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setFont(tFont);
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    item->setTextColor(LTData::colors[LTData::WHITE]);
    setItem(0, 1, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    item->setTextColor(LTData::colors[LTData::WHITE]);
    setItem(0, 2, item);

    item = new QTableWidgetItem("Air temp:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    setItem(0, 3, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::VIOLET]);
    setItem(0, 4, item);

    item = new QTableWidgetItem("Track temp:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    setItem(0, 5, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::YELLOW]);
    setItem(0, 6, item);

    item = new QTableWidgetItem("Wind speed:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    setItem(0, 7, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::GREEN]);
    setItem(0, 8, item);

    item = new QTableWidgetItem("Pressure:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    setItem(0, 9, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::YELLOW]);
    setItem(0, 10, item);

    item = new QTableWidgetItem("Humidity:");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::DEFAULT]);
    setItem(0, 11, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::WHITE]);
    setItem(0, 12, item);

    item = new QTableWidgetItem("");
    item->setFlags(Qt::NoItemFlags);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setTextColor(LTData::colors[LTData::YELLOW]);
    setItem(0, 13, item);

    setRowHeight(1, 20);

}

void TrackStatusWidget::updateTrackStatus(const EventData &eventData)
{
//    if (eventData.notice != "")
//    {
//        QString html = "<html><body><p align=\"center\">" + eventData.notice + "</p></body></html>";
//        setHtml(html);
//        return;
//    }
//    QString eventName = LTData::getEvent(eventData.eventNo).eventName;
//    QString html = "<html><body><table cellspacing=\"10\">";
////    html += "<div class=\"eventName\">" + eventName + "</div>";
//    html += "<tr><td class=\"status\" width=\"50\">";
//    switch (eventData.flagStatus)
//    {
//        case LTData::GREEN_FLAG:
//            html += icons[0];//":/track/green-light.png";
//            break;

//        case LTData::RED_FLAG:
//            html += icons[1];//":/track/red-light.png";
//            break;

//        case LTData::YELLOW_FLAG:
//            html += icons[2];//":/track/yellow-light.png";
//            break;

//        case LTData::SAFETY_CAR_DEPLOYED:
//            html += icons[3];//":/track/sc.png";
//            break;
//    }
//    html += "</td>";

//    if (eventData.eventType != LTData::RACE_EVENT)
//        html += "<td class=\"remTime\">"+eventData.remainingTime.toString("h:mm:ss")+"</td>";
//    else
//    {
////        QString lapsCpl = eventData.lapsCompleted+1;
//        html += "<td class=\"remTime\">"+QString("%1 / %2").arg(eventData.lapsCompleted).arg(LTData::getEvent(eventData.eventNo).laps)+"</td>";
//    }

//    html += "<td class = \"desc\">Air temp:</td><td class=\"violet\" width=\"50\">" + QString("%1").arg(eventData.airTemp) + "°C</td>";
//    html += "<td class = \"desc\">Track temp:</td><td class=\"yellow\" width=\"50\">" + QString("%1").arg(eventData.trackTemp) + "°C</td>";
//    html += "<td class = \"desc\">Wind speed:</td><td class=\"green\" width=\"50\">" + QString("%1").arg(eventData.windSpeed) + "m/s</td>";
//    html += "<td class = \"desc\">Pressure:</td><td class=\"yellow\" width=\"65\">" + QString("%1").arg(eventData.pressure) + "mb</td>";
//    html += "<td class = \"desc\">Humidity:</td><td class=\"white\" width=\"50\">" + QString("%1").arg(eventData.humidity) + "%</td>";
//    html += "<td class = \"desc\"></td><td class=\"" + (eventData.wetdry == 0 ? QString("yellow\">Dry track") : QString("cyan\">Wet track")) + "</td>";

////    if (eventData.eventType != LTData::RACE_EVENT)

//    html += "</tr></table></body></html>";
//    setHtml(html);

    if (eventData.eventType != LTData::RACE_EVENT)
    {
        setColumnWidth(2, 0);
    }
    else
        setColumnWidth(2, 80);

    int i = 4;

    QString s = QString("%1").arg(eventData.airTemp);

    QTableWidgetItem *item = QTableWidget::item(0, i);
//    if (!item)
//    {
//        item = new QTableWidgetItem(s + "°C");
//        item->setFlags(Qt::NoItemFlags);
//        item->setTextAlignment(Qt::AlignRight);
//        item->setTextColor(LTData::colors[LTData::WHITE]);
//        setItem(0, 3, item);
//    }
//    else
        item->setText(s + "°C");


        i += 2;
    s = QString("%1").arg(eventData.trackTemp);
    item = QTableWidget::item(0, i);
//    if (!item)
//    {
//        item = new QTableWidgetItem(s + "°C");
//        item->setFlags(Qt::NoItemFlags);
//        item->setTextAlignment(Qt::AlignRight);
//        item->setTextColor(LTData::colors[LTData::WHITE]);
//        setItem(0, 5, item);
//        setRowHeight(0, 20);
//    }
//    else
        item->setText(s + "°C");




        i += 2;
    s = QString("%1").arg(eventData.windSpeed);
    item = QTableWidget::item(0, i);
//    if (!item)
//    {
//        item = new QTableWidgetItem(s + "m/s");
//        item->setFlags(Qt::NoItemFlags);
//        item->setTextAlignment(Qt::AlignRight);
//        item->setTextColor(LTData::colors[LTData::WHITE]);
//        setItem(0, 7, item);
//    }
//    else
        item->setText(s + "m/s");


        i += 2;
    s = QString("%1").arg(eventData.pressure);
    item = QTableWidget::item(0, i);
//    if (!item)
//    {
//        item = new QTableWidgetItem(s + "mb");
//        item->setFlags(Qt::NoItemFlags);
//        item->setTextAlignment(Qt::AlignRight);
//        item->setTextColor(LTData::colors[LTData::WHITE]);
//        setItem(1, 3, item);

//        setRowHeight(3, 20);
//    }
//    else
        item->setText(s + "mb");

        i += 2;
    s = QString("%1").arg(eventData.humidity);
    item = QTableWidget::item(0, i);
//    if (!item)
//    {
//        item = new QTableWidgetItem(s + "%");
//        item->setFlags(Qt::NoItemFlags);
//        item->setTextAlignment(Qt::AlignRight);
//        item->setTextColor(LTData::colors[LTData::WHITE]);
//        setItem(1, 5, item);
//    }
//    else
        item->setText(s + "%");

    s = (eventData.wetdry == 0 ? "Dry track" : "Wet track");
    item = QTableWidget::item(0, ++i);
//    if (!item)
//    {
//        item = new QTableWidgetItem(s);
//        item->setFlags(Qt::NoItemFlags);
//        item->setTextAlignment(Qt::AlignLeft);
//        item->setTextColor(LTData::colors[LTData::DEFAULT]);
//        setItem(1, 6, item);
//    }
//    else
        item->setText(s);
    item->setTextColor(eventData.wetdry == 0 ? LTData::colors[LTData::YELLOW] : LTData::colors[LTData::CYAN]);



    QLabel *lab = qobject_cast<QLabel*>(cellWidget(0, 0));
//    if (!lab)
//    {
//        lab = new QLabel();
//        lab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
//        lab->resize(20, 20);
//        setCellWidget(0, 1, lab);
//    }

    if (eventData.sessionStarted)
    {
        switch (eventData.flagStatus)
        {
            case LTData::GREEN_FLAG:
                lab->setPixmap(icons[0]);
                break;

            case LTData::RED_FLAG:
                lab->setPixmap(icons[1]);
                break;

            case LTData::YELLOW_FLAG:
                lab->setPixmap(icons[2]);
                break;

        case LTData::SAFETY_CAR_DEPLOYED:
            lab->setPixmap(icons[3]);
            break;
        }
    }
    else
        lab->clear();

    item = QTableWidget::item(0, 1);
    if (eventData.eventType != LTData::RACE_EVENT)
    {
        item->setText(eventData.remainingTime.toString("h:mm:ss"));

        if (eventData.remainingTime.toString("hh:mm:ss") == "00:00:00")
            lab->setPixmap(icons[4]);
    }
    else
    {        
        item->setText(QString("%1 / %2").arg(eventData.lapsCompleted).arg(eventData.eventInfo.laps));

        item = QTableWidget::item(0, 2);
        item->setText(eventData.remainingTime.toString("h:mm:ss"));
        if (eventData.lapsCompleted >= eventData.eventInfo.laps)
            lab->setPixmap(icons[4]);
    }

}

void TrackStatusWidget::setFont(const QFont &f)
{
    QTableWidget::setFont(f);
    QTableWidgetItem *item = QTableWidget::item(0, 1);
    QFont tFont = f;
    tFont.setPointSize(tFont.pointSize()+2);
    item->setFont(tFont);
}

void TrackStatusWidget::resizeEvent(QResizeEvent *event)
{
    int w;
    if (event)
        w = event->size().width() - 20;
    else
        w = width()-20;


    setColumnWidth(0, 0.05*w);  //track status
    setColumnWidth(1, 0.06*w);  //rem. time
    setColumnWidth(2, 0.06*w);  //rem. time
    setColumnWidth(3, 0.1*w);  //Air temp
    setColumnWidth(4, 0.04*w);  // -||-
    setColumnWidth(5, 0.1*w);  //Track temp
    setColumnWidth(6, 0.04*w);  // -||-
    setColumnWidth(7, 0.1*w);  //Wind speed
    setColumnWidth(8, 0.06*w);  // -||-
    setColumnWidth(9, 0.1*w);  //Pressure
    setColumnWidth(10, 0.05*w);  // -||-
    setColumnWidth(11, 0.1*w);  //Humidity
    setColumnWidth(12, 0.04*w);  // -||-
    setColumnWidth(13, 0.1*w);  //Dry/wet track
}
