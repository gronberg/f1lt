/*******************************************************************************
 *                                                                             *
 *   F1LT - unofficial Formula 1 live timing application                       *
 *   Copyright (C) 2012-2013  Mariusz Pilarek (pieczaro@gmail.com)             *
 *                                                                             *
 *   This program is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation, either version 3 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                             *
 *******************************************************************************/



#ifndef EVENTPLAYER_H
#define EVENTPLAYER_H

#include <QList>
#include <QPair>
#include <QTime>
#include <QWidget>
#include "../net/datastreamreader.h"

namespace Ui {
class EventPlayer;
}

class EventPlayer : public QWidget
{
    Q_OBJECT
    
public:
    explicit EventPlayer(QWidget *parent = 0);
    ~EventPlayer();

    bool loadFromFile(QString fName);

    bool isPlaying() { return playing; }
    bool isPaused() { return paused; }

    void setTimeLabel();
    QString playedFile() { return fileName; }

public slots:
    void timeout();
    void startPlaying();
    void stopPlaying();
    void pausePlaying();

signals:
    void rewindToStartClicked();
    void playClicked(int);
    void pauseClicked();
    void forwardToEndClicked();
    void rewindClicked();
    void stopClicked();

    void nextPackets(const QVector<Packet> &);
    
private slots:
    void on_playButton_clicked();

    void on_speedBox_currentIndexChanged(const QString &arg1);

    void on_rewindToStartButton_clicked();

    void on_forwardToEndButton_clicked();

    void on_forwardButton_clicked();

    void on_rewindButton_clicked();

    void on_stopButton_clicked();

    void on_seekSlider_valueChanged(int value);

    void on_seekSlider_sliderMoved(int position);

private:
    Ui::EventPlayer *ui;
    QVector< QPair<int, Packet> > packets;

    int currentPos;
    bool playing;
    bool paused;

    int elapsedSeconds;
    int timerInterval;
    int initialPacketsNum;

    EventData &eventData;
    QString fileName;
};

#endif // EVENTPLAYER_H
