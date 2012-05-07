#ifndef EVENTPLAYER_H
#define EVENTPLAYER_H

#include <QList>
#include <QPair>
#include <QTime>
#include <QWidget>
#include "datastreamreader.h"

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

signals:
    void rewindToStartClicked();
    void playClicked(int);
    void pauseClicked();
    void forwardToEndClicked();
    void rewindClicked();
    void stopClicked();

    void nextPackets(const QList<Packet> &);
    
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
    QList< QPair<int, Packet> > packets;

    LTEvent ltEvent;
    QList<LTTeam> ltTeamList;

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
