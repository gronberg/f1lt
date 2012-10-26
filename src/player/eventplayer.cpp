#include "eventplayer.h"
#include "ui_eventplayer.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>


EventPlayer::EventPlayer(QWidget *parent) :
    QWidget(parent), ui(new Ui::EventPlayer), currentPos(0), playing(false), paused(false), eventData(EventData::getInstance())
{
    ui->setupUi(this);
}

EventPlayer::~EventPlayer()
{
    delete ui;
}

bool EventPlayer::loadFromFile(QString fName)
{       
    fileName = fName;
    QFile file(fName);

    if (file.open(QIODevice::ReadOnly))
    {                
        QDataStream stream(&file);

        ltTeamList.clear();
        packets.clear();

        char *tab;
        stream >> tab;

        QString sbuf(tab);

        delete [] tab;
        if (sbuf != "F1LT")
            return false;

        int ibuf;
        QPixmap pixBuf;

        stream >> ibuf;
        ltEvent.eventNo = ibuf;

        stream >> sbuf;
        ltEvent.eventName = sbuf;

        stream >> sbuf;
        ltEvent.eventShortName = sbuf;

        stream >> sbuf;
        ltEvent.eventPlace = sbuf;

        stream >> sbuf;
        ltEvent.fpDate = QDate::fromString(sbuf, "dd-MM-yyyy");

        stream >> sbuf;
        ltEvent.raceDate = QDate::fromString(sbuf, "dd-MM-yyyy");

        stream >> ibuf;
        ltEvent.laps = ibuf;

        stream >> pixBuf;
        ltEvent.trackImg = pixBuf;

        eventData.setEventInfo(ltEvent);
        int size;
        stream >> size;
        ltTeamList.resize(size);
        for (int i = 0; i < size; ++i)
        {
            stream >> sbuf; ltTeamList[i].teamName = sbuf;
            stream >> sbuf; ltTeamList[i].driver1Name = sbuf;
            stream >> sbuf; ltTeamList[i].driver1ShortName = sbuf;
            stream >> ibuf; ltTeamList[i].driver1No = ibuf;
            stream >> sbuf; ltTeamList[i].driver2Name = sbuf;
            stream >> sbuf; ltTeamList[i].driver2ShortName = sbuf;
            stream >> ibuf; ltTeamList[i].driver2No = ibuf;
            stream >> ltTeamList[i].carImg;
        }

        stream >> size;
        packets.resize(size);
        for (int i = 0; i < size; ++i)
        {
            stream >> packets[i].first;
            stream >> packets[i].second.type;
            stream >> packets[i].second.carID;
            stream >> packets[i].second.data;
            stream >> packets[i].second.length;
            stream >> packets[i].second.longData;
        }   
        return true;
    }
    return false;
}

void EventPlayer::startPlaying()
{
    elapsedSeconds = 0;
    currentPos = 0;

    playing = false;
    paused = false;
    ui->playButton->setIcon(QIcon(":/ui_icons/play.png"));    
    //emit initial packets
    timeout();
    initialPacketsNum = currentPos;

    ui->seekSlider->setMaximum(packets.last().first);//packets.size() - 1 - initialPacketsNum);
    ui->seekSlider->setValue(0);
    setTimeLabel();
}

void EventPlayer::pausePlaying()
{
    paused = true;
    ui->playButton->setIcon(QIcon(":/ui_icons/play.png"));
    emit pauseClicked();
}

void EventPlayer::on_playButton_clicked()
{
    if (currentPos >= packets.size())
        return;

    timerInterval = 1000.0 / ui->speedBox->currentText().right(ui->speedBox->currentText().size()-2).toDouble();
    if (!paused && !playing)
    {
        playing = true;
        ui->playButton->setIcon(QIcon(":/ui_icons/pause.png"));
        emit playClicked(timerInterval);
    }
    else if (playing && !paused)
    {
        pausePlaying();
    }
    else if (playing && paused)
    {
        paused = false;
        ui->playButton->setIcon(QIcon(":/ui_icons/pause.png"));
        emit playClicked(timerInterval);
    }
}

void EventPlayer::timeout()
{    
    if (currentPos >= packets.size())
    {
        stopPlaying();
        return;
    }

//    QTime prevTime = packets[currentPos].first;
    QVector<Packet> LTpackets;

//    LTpackets.append(packets[currentPos].second);
//    bool appendPacket = true;


    while (currentPos < packets.size() && elapsedSeconds == packets[currentPos].first)
    {                
//        QTime currTime = packets[currentPos].first;

//        if (prevTime.second() == currTime.second())
            LTpackets.append(packets[currentPos].second);

//        else
//            break;
        ++currentPos;
    }
    emit nextPackets(LTpackets);
    ui->seekSlider->setValue(elapsedSeconds);//currentPos - initialPacketsNum);
    setTimeLabel();
    ++elapsedSeconds;
}


void EventPlayer::on_speedBox_currentIndexChanged(const QString &arg1)
{
    timerInterval = 1000.0 / arg1.right(arg1.size()-2).toDouble();
    if (playing && !paused)
        emit playClicked(timerInterval);
}

void EventPlayer::on_rewindToStartButton_clicked()
{
    currentPos = 0;
    elapsedSeconds = 0;
    ui->seekSlider->setValue(0);//initialPacketsNum);
    setTimeLabel();
    stopPlaying();

    emit rewindToStartClicked();
}

void EventPlayer::on_forwardToEndButton_clicked()
{
    QVector<Packet> LTpackets;
    while (currentPos < packets.size())
    {
        LTpackets.append(packets[currentPos].second);
        elapsedSeconds = packets[currentPos].first;
        ++currentPos;
    }
    ui->seekSlider->setValue(elapsedSeconds);//currentPos - initialPacketsNum);
    setTimeLabel();
    emit nextPackets(LTpackets);
    emit forwardToEndClicked();    
    stopPlaying();
}

void EventPlayer::on_forwardButton_clicked()
{
    if (elapsedSeconds <= packets.last().first)
    {
        elapsedSeconds += 10;
        if (elapsedSeconds > packets.last().first)
            elapsedSeconds = packets.last().first;

        QVector<Packet> LTpackets;

        while (currentPos < packets.size() && packets[currentPos].first <= elapsedSeconds)
        {
            LTpackets.append(packets[currentPos].second);
            ++currentPos;
        }
        ui->seekSlider->setValue(elapsedSeconds);//currentPos - initialPacketsNum);
        setTimeLabel();
        emit nextPackets(LTpackets);
    }
}

void EventPlayer::on_rewindButton_clicked()
{
    if (elapsedSeconds > 0)
    {
        emit rewindClicked();
        elapsedSeconds -= 10;

        if (elapsedSeconds < 0)
            elapsedSeconds = 0;

        //we have to back to the begining and re-emit all packets
        currentPos = 0;
        QVector<Packet> LTpackets;

        while (currentPos < packets.size() && packets[currentPos].first <= elapsedSeconds)
        {
            LTpackets.append(packets[currentPos].second);
            ++currentPos;
        }
        ui->seekSlider->setValue(elapsedSeconds);//currentPos - initialPacketsNum);
        setTimeLabel();
        emit nextPackets(LTpackets);
    }
}

void EventPlayer::stopPlaying()
{
    playing = paused = false;
    ui->playButton->setIcon(QIcon(":/ui_icons/play.png"));
    emit pauseClicked();
}

void EventPlayer::on_stopButton_clicked()
{
    stopPlaying();
    emit stopClicked();
}

void EventPlayer::on_seekSlider_valueChanged(int)
{

}

void EventPlayer::on_seekSlider_sliderMoved(int position)
{
//    int value = position + initialPacketsNum;
//    if (value == 0 || value == packets.size()-1)
//    {
//        currentPos = value;
//        stopPlaying();
//        return;
//    }

//    if (value > currentPos)
//    {
//        QList<Packet> LTpackets;
//        while (currentPos <= value)
//        {
//            LTpackets.append(packets[currentPos].second);
//            ++currentPos;
//            elapsedSeconds = packets[currentPos].first-1;
//        }
//        emit nextPackets(LTpackets);
//    }
//    else
//    {
//        emit rewindClicked();

//        //we have to back to the begining and re-emit all packets
//        currentPos = 0;
//        QList<Packet> LTpackets;

//        while (currentPos <= value)
//        {
//            LTpackets.append(packets[currentPos].second);
//            ++currentPos;
//            elapsedSeconds = packets[currentPos].first-1;
//        }
//        emit nextPackets(LTpackets);
//    }
//    setTimeLabel();

    int value = position;// + initialPacketsNum;

    if (value > elapsedSeconds)//packets[currentPos].first)
    {
        QVector<Packet> LTpackets;
        while (elapsedSeconds <= value && currentPos < packets.size()-1)
        {
            LTpackets.append(packets[currentPos].second);
            ++currentPos;
            elapsedSeconds = packets[currentPos].first-1;
        }
        emit nextPackets(LTpackets);
    }
    else
    {
        emit rewindClicked();

        //we have to back to the begining and re-emit all packets
        currentPos = 0;
        elapsedSeconds = 0;
        QVector<Packet> LTpackets;

        while (elapsedSeconds <= value)
        {
            LTpackets.append(packets[currentPos].second);
            ++currentPos;

            if (currentPos < packets.size())
                elapsedSeconds = packets[currentPos].first-1;
            else
                break;
        }
        emit nextPackets(LTpackets);
    }
    setTimeLabel();

    if (value == 0 || value == packets.last().first)
    {/*
        currentPos = (value == 0) ? 0 : packets.size()-1;
        elapsedSeconds = value;
        setTimeLabel();*/
        stopPlaying();
    }
}

void EventPlayer::setTimeLabel()
{
    int elSeconds = packets[currentPos == 0 ? 0 : currentPos-1].first;
    int hours = elSeconds / 3600;
    int mins = (elSeconds - hours * 3600)/60;
    int secs = elSeconds - hours * 3600 - mins * 60;

    QString sMins = QString::number(mins);
    QString sSecs = QString::number(secs);

    if (mins < 10)
        sMins = "0" + sMins;

    if (secs < 10)
        sSecs = "0" + sSecs;

    QString currTime = QString("%1:%2:%3").arg(hours).arg(sMins).arg(sSecs);

    int totalSeconds = packets.last().first;

    hours = totalSeconds / 3600, mins = totalSeconds / 60, secs = totalSeconds;
    mins = (totalSeconds - hours * 3600)/60;
    secs = totalSeconds - hours * 3600 - mins * 60;

    sMins = QString::number(mins);
    sSecs = QString::number(secs);

    if (mins < 10)
        sMins = "0" + sMins;

    if (secs < 10)
        sSecs = "0" + sSecs;

    QString totalTime = QString("/%1:%2:%3").arg(hours).arg(sMins).arg(sSecs);

    ui->timeLabel->setText(currTime + totalTime);

}
