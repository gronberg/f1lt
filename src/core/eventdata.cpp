#include "eventdata.h"
#include "ltdata.h"

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

    flagStatus = LTData::GREEN_FLAG;

    qualiPeriod = 0;

    sessionStarted = false;
}

void EventData::clear()
{
//    eventInfo.laps = 0;
//    eventInfo.eventNo = 0;
    eventId = 0;
    key = 0;
    frame = 0;
    remainingTime = QTime();
    lapsCompleted = 0;

    weather = Weather();
    sessionRecords = SessionRecords();

    flagStatus = LTData::GREEN_FLAG;

    sessionStarted = false;
    qualiPeriod = 0;

    commentary = "";
    driversData.clear();
    for (int i = 0; i < LTData::ltTeams.size(); ++i)
    {
        driversData.append(DriverData());
        driversData.append(DriverData());        
    }
}

int EventData::getDriverId(QString name) const
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].getDriverName() == name)
            return driversData[i].getCarID();
    }
    return -1;
}

int EventData::getDriverId(int no) const
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].getNumber() == no)
            return driversData[i].getCarID();
    }
    return -1;
}

DriverData EventData::getDriverData(int no) const
{
	int id = getDriverId(no);
	if (id > 0 && id <= driversData.size())
		return driversData[id-1];

    return DriverData();
}
DriverData EventData::getDriverDataByPos(int pos) const
{
	for (int i = 0; i < driversData.size(); ++i)
	{
        if (driversData[i].getPosition() == pos)
			return driversData[i];
	}

    return DriverData();
}

DriverData EventData::getDriverDataById(int id) const
{
    if (id > 0 && id <= driversData.size())
        return driversData[id-1];

    return DriverData();
}

QString EventData::calculateInterval(DriverData d1, DriverData d2, int lap) const
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

/*

EventData eventData;*/


