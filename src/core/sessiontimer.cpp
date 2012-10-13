#include "sessiontimer.h"

#include "eventdata.h"

SessionTimer::SessionTimer(QObject *parent) :
    QObject(parent), interval(1000)
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
            if (!(eventData.getEventType() == LTPackets::RACE_EVENT && eventData.getCompletedLaps() == eventData.getEventInfo().laps) &&
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
                            eventData.setSessionStarted(false);
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
