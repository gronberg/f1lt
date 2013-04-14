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



#ifndef SESSIONTIMER_H
#define SESSIONTIMER_H

#include <QDebug>
#include <QObject>
#include <QTime>
#include <QTimer>

/*!
 * \brief The SessionTimer class defines a session timer.
 */
class SessionTimer : public QObject
{
    Q_OBJECT
public:
    explicit SessionTimer(QObject *parent = 0);    

    bool isCounterMode() { return counterMode; }
    bool isActive() { return timer.isActive(); }

    bool isSynchronizing() { return timerDelay > 0; }
    
signals:
    void timeout();
    void synchronizingTimer(bool);
    
public slots:

    void setTime(const QTime &t)
    {
        sessionTime = t;
    }

    void start(int t = 1000)
    {
        qDebug() << "START TIMER!";
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
    void setDelay(int prevDelay, int delay);

private slots:
    void timerTimeout();    
    
private:
    int interval;
    QTimer timer;
    QTime sessionTime;
    bool counterMode;       //in this mode timer will still be running but will not change the session time
    int timerDelay;
};

#endif // SESSIONTIMER_H
