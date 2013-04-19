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


#include "eventdata.h"
#include "seasondata.h"

#include <QDebug>

void Weather::saveWeatherData(const EventData &ed)
{
    WeatherData wd;
    wd.lap = ed.getCompletedLaps();
    wd.sessionTime = ed.getRemainingTime();
    wd.qPeriod = ed.getQualiPeriod();

    for (int i = 0; i < 7; ++i)
    {
        wd.value = currentWeather[i].value;

        if (!(weatherData[i].isEmpty() && wd.value == 0.0 && i != 5))
            weatherData[i].append(wd);
    }
}

EventData::EventData()
{
    eventId = 0;

    eventInfo.laps = 0;
    eventInfo.eventNo = 0;

    key = 0;
    frame = 0;
    remainingTime = QTime();
    lapsCompleted = 0;

    flagStatus = LTPackets::GREEN_FLAG;

    qualiPeriod = 0;
    fpNumber = 0;

    sessionStarted = false;
    sessionFinished = false;
    qualiBreak = false;

    baseEventId = 7066;
    baseEventInc = 6;
}

void EventData::clear()
{
//    eventInfo.laps = 0;
//    eventInfo.eventNo = 0;
    eventId = 0;
    key = 0;
    frame = 0;        

    fpNumber = 0;

    driversData.resize(SeasonData::getInstance().getTeams().size()*2);

    reset();


//    driversData.clear();
//    for (int i = 0; i < SeasonData::getInstance().getTeams().size(); ++i)
//    {
//        driversData.append(DriverData());
//        driversData.append(DriverData());
    //    }
}

/*!
 * \brief EventData::reset - resets only some parameters, can be used ie. during session playback
 */
void EventData::reset()
{
    lapsCompleted = 0;
    remainingTime = QTime();
    weather = Weather();
    sessionRecords = SessionRecords();

    flagStatus = LTPackets::GREEN_FLAG;

    sessionStarted = false;
    sessionFinished = false;
    qualiPeriod = 0;
    commentary = "";

    for (int i = 0; i < driversData.size(); ++i)
        driversData[i] = DriverData();
}



QString EventData::calculateInterval(const DriverData &d1, const DriverData &d2, int lap) const
{
	LapData ld1 = d1.getLapData(lap);
	LapData ld2 = d2.getLapData(lap);

    if (lap == -1 && !d1.getLapData().isEmpty() && !d2.getLapData().isEmpty())
	{
        ld1 = d1.getLastLap();//d1.lapData.get(d1.lapData.size()-1);
        ld2 = d2.getLastLap();//d2.lapData.get(d2.lapData.size()-1);
	}

    if (ld1.getCarID() == -1 || ld2.getCarID() == -1)
		return "";

    QString gap1 = ld1.getGap();
    QString gap2 = ld2.getGap();

    if ((ld1.getTime().toString() == "" && ld1.getGap() == "") ||
        (ld2.getTime().toString() == "" && ld2.getGap() == ""))
		return "";

    if (ld1.getPosition() == 1)
		return "-" + (gap2 == "" ? "1L <" : gap2) + (gap2.contains("L") ? " <" : "");

    if (ld2.getPosition() == 1)
		return "+" + (gap1 == "" ? "1L <" : gap1) + (gap1.contains("L") ? " <" : "");

	if ((gap1 != "" && gap2 != "" && gap1[gap1.size()-1] != 'L' &&
			gap2[gap2.size()-1] != 'L') ||
       ((ld1.getPosition() == 1 && gap1 == "") || (ld2.getPosition() == 1 && gap2 == "")))
	{
		double interval = 0.0;

		interval = gap1.toDouble() - gap2.toDouble();

		QString sInterval  = QString::number(interval, 'f', 1);
//            String sInterval = QString::number(interval, 'f', 1);
		if (interval > 0)
			sInterval = "+" + sInterval;

		return sInterval;
	}
	else if ((gap1 != "" && gap1.contains("L")) ||
			(gap2 != "" && gap2.contains("L")) ||
			(gap1 == "" || gap2 == ""))
	{
        int pos1 = ld1.getPosition();
        int pos2 = ld2.getPosition();

		bool neg = true;
		if (pos2 < pos1)
		{
			int tmp = pos1;
			pos1 = pos2;
			pos2 = tmp;
			neg = false;
		}

		QStringList intervals;
//            intervals.reserve(pos2 - pos1);
		for (int i = 0; i < driversData.size(); ++i)
		{
			LapData ld = driversData[i].getLapData(lap);

            if (lap == -1 && !driversData[i].getLapData().isEmpty())
                ld = driversData[i].getLastLap();//lapData.get(driversData.get(i).lapData.size()-1);

            if (ld.getCarID() == -1)
				continue;

            int pos = ld.getPosition();
			if (pos > pos1 && pos <= pos2)
			{
                if (ld.getInterval() != "" && ld.getInterval().contains("L"))
					return neg ? "-1L <" : "+1L <";

                intervals.append(ld.getInterval());
			}
		}
		double interval = 0.0;

		for (int i = 0; i < intervals.size(); ++i)
			interval += intervals[i].toDouble();

        if (neg && ld1.getTime().isValid() && interval > ld1.getTime().toDouble())
			return "-1L <";
        if (!neg && ld2.getTime().isValid() && interval > ld2.getTime().toDouble())
			return "+1L <";


		QString sInterval  = QString::number(interval, 'f', 1);

		if (neg)
			sInterval = "-" + sInterval;
		else
			sInterval = "+" + sInterval;

		return sInterval;
	}

	return "";
}

int EventData::getFPNumber() const
{
    if (fpNumber > 0)
        return fpNumber;

    if (getEventId() == 0)
        return 1;

    else
        return (getEventId() - baseEventId) % baseEventInc;
}

int EventData::correctPosition(const LapTime &ld) const
{
    QList<LapTime> timeList;
    timeList << ld;

    for (int i = 0; i < driversData.size(); ++i)
    {
        LapTime lt = driversData[i].getQualiTime(qualiPeriod);
        if (!lt.isValid())
            lt = driversData[i].getSessionRecords().getBestLap().getTime();

        if (lt != ld)
            timeList << lt;
    }
    qSort(timeList);

    return timeList.indexOf(ld) + 1;
}

bool EventData::isFridayBeforeFP1()
{
    //on friday before FP1 starts there is already next GP weekend data loaded, but server still sends race results from previous race
    if ((QDateTime::currentDateTimeUtc().date() == getEventInfo().fpDate) &&
        getEventType() == LTPackets::RACE_EVENT)
    {
        return true;
    }

    return false;
}

/*

EventData eventData;*/


