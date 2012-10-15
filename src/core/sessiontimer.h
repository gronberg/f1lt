#ifndef SESSIONTIMER_H
#define SESSIONTIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class SessionTimer : public QObject
{
    Q_OBJECT
public:
    explicit SessionTimer(QObject *parent = 0);    

    bool isCounterMode() { return counterMode; }
    bool isActive() { return timer.isActive(); }
    
signals:
    void timeout();
    
public slots:

    void setTime(const QTime &t)
    {
        sessionTime = t;
    }

    void start(int t = 1000)
    {
        interval = t;
        timer.stop();
        counterMode = false;
        timer.start(interval);
    }

    void stop()
    {
        timer.stop();
    }
    void setCounterMode(bool m)
    {
        counterMode = m;
    }

private slots:
    void timerTimeout();
    
private:
    int interval;
    QTimer timer;
    QTime sessionTime;
    bool counterMode;       //in this mode timer will still be running but will not change the session time
};

#endif // SESSIONTIMER_H
