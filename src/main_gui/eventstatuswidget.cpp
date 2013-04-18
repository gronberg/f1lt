#include "eventstatuswidget.h"
#include "ui_eventstatuswidget.h"
#include <QDebug>
#include <QBuffer>
#include <QMatrix>

#include "../core/eventdata.h"

EventStatusWidget::EventStatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventStatusWidget)
{
    ui->setupUi(this);

    icons[0] = QPixmap(":/track/green-light.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[1] = QPixmap(":/track/red-light.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[2] = QPixmap(":/track/yellow-light.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[3] = QPixmap(":/track/sc.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[4] = QPixmap(":/track/flag.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[5] = QPixmap(":/ui_icons/weather_dry.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[6] = QPixmap(":/ui_icons/weather_wet.png").scaledToHeight(20, Qt::SmoothTransformation);
    icons[7] = QPixmap(":/ui_icons/weather_wind.png").scaledToHeight(18, Qt::SmoothTransformation);


    ui->airTempLabelIcon->setPixmap(QPixmap(":/ui_icons/weather_air_temp.png").scaledToHeight(20, Qt::SmoothTransformation));
    ui->trackTempLabelIcon->setPixmap(QPixmap(":/ui_icons/weather_track_temp.png").scaledToHeight(20, Qt::SmoothTransformation));
    ui->windSpeedLabelIcon->setPixmap(icons[7]);
    ui->pressureLabelIcon->setPixmap(QPixmap(":/ui_icons/weather_pressure.png").scaledToHeight(20, Qt::SmoothTransformation));
    ui->humidityLabelIcon->setPixmap(QPixmap(":/ui_icons/weather_humidity.png").scaledToHeight(20, Qt::SmoothTransformation));
    ui->weatherLabelIcon->setPixmap(icons[5]);
}

EventStatusWidget::~EventStatusWidget()
{
    delete ui;
}

void EventStatusWidget::updateEventStatus()
{
    EventData &eventData = EventData::getInstance();

    if (eventData.isSessionStarted())
    {
        switch (eventData.getFlagStatus())
        {
            case LTPackets::GREEN_FLAG:
                ui->statusLabelIcon->setPixmap(icons[0]);
                break;

            case LTPackets::RED_FLAG:
                ui->statusLabelIcon->setPixmap(icons[1]);
                break;

            case LTPackets::YELLOW_FLAG:
                ui->statusLabelIcon->setPixmap(icons[2]);
                break;

            case LTPackets::SAFETY_CAR_DEPLOYED:            
                ui->statusLabelIcon->setPixmap(icons[3]);
                break;

            case LTPackets::SAFETY_CAR_STANDBY:
                break;

            default:
                ui->statusLabelIcon->setPixmap(icons[0]);
                break;
        }
    }
    else
        ui->statusLabelIcon->clear();

    if (eventData.getEventType() != LTPackets::RACE_EVENT)
    {
        ui->timerLabel->setText(eventData.getRemainingTime().toString("h:mm:ss"));

        if (eventData.getRemainingTime().toString("hh:mm:ss") == "00:00:00")
            ui->statusLabelIcon->setPixmap(icons[4]);

        if (eventData.getEventType() == LTPackets::QUALI_EVENT)
            ui->infoLabel->setText("Q" + (eventData.getQualiPeriod() > 0 ? QString::number(eventData.getQualiPeriod()) : ""));
        else
            ui->infoLabel->setText("FP" + QString::number(EventData::getInstance().getFPNumber()));
    }
    else
    {
        int lapsCompleted = eventData.getCompletedLaps()+1;

        if (eventData.getCompletedLaps() == 0 && !eventData.isSessionStarted())
    		lapsCompleted = 0;

        if (lapsCompleted > eventData.getEventInfo().laps)
            lapsCompleted = eventData.getEventInfo().laps;

        ui->timerLabel->setText(QString("%1 / %2").arg(lapsCompleted).arg(eventData.getEventInfo().laps));

        ui->infoLabel->setText(eventData.getRemainingTime().toString("h:mm:ss"));
        if (eventData.getCompletedLaps() >= eventData.getEventInfo().laps)
            ui->statusLabelIcon->setPixmap(icons[4]);
    }

    QString s = QString("%1").arg(eventData.getWeather().getAirTemp().getValue());
    ui->airTempLabel->setText(s + "°C");

    s = QString::number(eventData.getWeather().getTrackTemp().getValue());
    ui->trackTempLabel->setText(s + "°C");

    s = QString::number(eventData.getWeather().getWindSpeed().getValue());
    ui->windSpeedLabel->setText(s + "m/s");

    s = QString::number(eventData.getWeather().getHumidity().getValue());
    ui->humidityLabel->setText(s + "%");

    s = QString::number(eventData.getWeather().getPressure().getValue());
    ui->pressureLabel->setText(s + "mb");

    ui->weatherLabelIcon->setPixmap(int(eventData.getWeather().getWetDry().getValue()) == 1 ? icons[6] : icons[5]);


    QMatrix matrix;
    matrix.rotate(eventData.getWeather().getWindDirection().getValue());

    ui->windSpeedLabelIcon->setPixmap(icons[7].transformed(matrix, Qt::SmoothTransformation));

//    QStringList sl = eventData.commentary.split("\n");
//    QString commentaryLine;
//    if (!sl.isEmpty())
//    {
//        if (sl.last() != "")
//            commentaryLine = sl.last();

//        else if (sl.size() > 1 && sl[sl.size()-2] != "")
//            commentaryLine = sl[sl.size()-2];

//        else if (sl.size() > 2 && sl[sl.size()-3] != "")
//            commentaryLine = sl[sl.size()-3];

//        ui->commentaryLabel->setText(commentaryLine);
//    }
}

void EventStatusWidget::setFont(const QFont &f)
{
    ui->airTempLabel->setFont(f);
    ui->trackTempLabel->setFont(f);
    ui->windSpeedLabel->setFont(f);
    ui->humidityLabel->setFont(f);
    ui->pressureLabel->setFont(f);
    ui->infoLabel->setFont(f);

    QFont tFont = f;
    tFont.setPointSize(tFont.pointSize()+2);
    ui->timerLabel->setFont(tFont);
}
