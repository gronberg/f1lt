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
//    EventData &eventData = EventData::getInstance();

//    int hours = eventData.getRemainingTime().hour();
//    int minutes = eventData.getRemainingTime().minute();
//    int secs = eventData.getRemainingTime().second();

//    secs -= prevDelay - delay;

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
            qDebug() << "TIMERDELAY" << timerDelay;
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


//    qDebug() << "SESSIONTIMER" << secs << timerDelay;

//    if (secs >= 60)
//    {
//        ++minutes;
//        secs = secs - 60;

//        if (minutes >= 60)
//        {
//            ++hours;
//            minutes = minutes - 60;
//        }
//    }



//    int mins = minutes + 60 * hours;
//    if (eventData.getEventType() == LTPackets::RACE_EVENT && hours > 2)
//    {
//        hours = 2;
//        minutes = secs = 0;
//    }
//    else if (eventData.getEventType() == LTPackets::PRACTICE_EVENT && mins >= SeasonData::getInstance().getFPLength())
//    {
//        hours = 1;
//        minutes = SeasonData::getInstance().getFPLength() - 60;
//        secs = 0;
//    }
//    else if (eventData.getEventType() == LTPackets::QUALI_EVENT && minutes >= SeasonData::getInstance().getQualiLength(eventData.getQualiPeriod()))
//    {
//        hours = 0;
//        minutes = SeasonData::getInstance().getQualiLength(eventData.getQualiPeriod());
//        secs = 0;
//    }
//    else
//        timerDelay = 0;

//    eventData.setRemainingTime(QTime(hours, minutes, secs));
}
