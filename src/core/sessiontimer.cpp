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


#include "sessiontimer.h"

#include "eventdata.h"

SessionTimer::SessionTimer(QObject *parent) :
    QObject(parent), interval(1000), timerDelay(0)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void SessionTimer::timerTimeout()
{
//    if (!counterMode)
    {
        EventData &eventData = EventData::getInstance();
        if (eventData.isSessionStarted())
        {
            if (timerDelay != 0)
            {
                --timerDelay;
                emit synchronizingTimer(timerDelay != 0 ? true : false);
            }

            else if (!(eventData.getEventType() == LTPackets::RACE_EVENT && eventData.getCompletedLaps() == eventData.getEventInfo().laps) &&
                !((eventData.getEventType() == LTPackets::QUALI_EVENT || eventData.getEventType() == LTPackets::RACE_EVENT) && eventData.getFlagStatus() == LTPackets::RED_FLAG))
            {
                int hours = eventData.getRemainingTime().hour();
                int mins = eventData.getRemainingTime().minute();
                int secs = eventData.getRemainingTime().second();
                --secs;
                if (secs < 0)
                {
                    secs = 59;
                    --mins;
                    eventData.saveWeather();

                    if (mins < 0)
                    {
                        --hours;
                        mins = 59;

                        if (hours < 0)
                        {
                            secs = mins = hours = 0;
                            eventData.setSessionFinished(true);
//                            counterMode = true;
                        }
                    }
                }
                eventData.setRemainingTime(QTime(hours, mins, secs));
            }
        }
    }
    emit timeout();
}

void SessionTimer::setDelay(int prevDelay, int delay)
{
    EventData &eventData = EventData::getInstance();

    if (!eventData.isSessionStarted() || eventData.isSessionFinished() || (prevDelay == delay))
        return;

    timerDelay -= prevDelay - delay;

    if (delay == 0 || timerDelay < 0)
    {
        emit synchronizingTimer(false);

        int hours = eventData.getRemainingTime().hour();
        int minutes = eventData.getRemainingTime().minute();
        int secs = eventData.getRemainingTime().second();

        if (timerDelay < 0)
        {
            secs += timerDelay;
        }

        else if (delay == 0)
            secs -= prevDelay;

        timerDelay = 0;

        if (secs < 0)
        {
            --minutes;
            secs = 60 + secs;

            if (minutes < 0)
            {
                --hours;
                minutes = 60 + minutes;

                if (hours < 0)
                    hours = minutes = secs = 0;

            }
        }
        eventData.setRemainingTime(QTime(hours, minutes, secs));
    }
}
