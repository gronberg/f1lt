#include "eventdata.h"
#include "ltdata.h"

#include <QDebug>

EventData::EventData()
{
    FLLap = -1;
    sec1Record[2] = "-1";
    sec2Record[2] = "-1";
    sec3Record[2] = "-1";

    eventInfo.laps = 0;
    eventInfo.eventNo = 0;

    timeStamp = 0;

    trackTemp = 0;
    airTemp = 0;
    windSpeed = 0;
    humidity = 0;
    pressure = 0;
    windDirection = 0;    

    qualiPeriod = 0;

    sessionStarted = false;
}

void EventData::clear()
{
//    eventInfo.laps = 0;
//    eventInfo.eventNo = 0;
    key = 0;
    frame = 0;
    remainingTime = QTime();
    lapsCompleted = 0;

    flagStatus = LTData::GREEN_FLAG;

    timeStamp = 0;

    trackTemp = 0;
    airTemp = 0;
    windSpeed = 0;
    humidity = 0;
    pressure = 0;
    windDirection = 0;
    sessionStarted = false;
    qualiPeriod = 0;

    commentary = "";
    driversData.clear();
    for (int i = 0; i < LTData::ltTeams.size(); ++i)
    {
        driversData.append(DriverData());
        driversData.append(DriverData());

        if (i < 6)
        	weatherData[i].clear();
    }
    qDebug() << "CLEAR";

    FLNumber = 0;
    FLDriver.clear();
    FLTime = LapTime();
    FLLap = 0;

    for (int i = 0; i < 12; ++i)
    {
        sec1Speed[i].clear();
        sec2Speed[i].clear();
        sec3Speed[i].clear();
        speedTrap[i].clear();

        if (i < 4)
        {
            sec1Record[i].clear();
            sec2Record[i].clear();
            sec3Record[i].clear();
        }
    }
}

int EventData::getDriverId(QString name)
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].driver == name)
            return driversData[i].carID;
    }
    return -1;
}

int EventData::getDriverId(int no)
{
    for (int i = 0; i < driversData.size(); ++i)
    {
        if (driversData[i].number == no)
            return driversData[i].carID;
    }
    return -1;
}

DriverData EventData::getDriverData(int no)
{
	int id = getDriverId(no);
	if (id > 0 && id <= driversData.size())
		return driversData[id-1];


    return DriverData();
}
DriverData EventData::getDriverDataFromPos(int pos)
{
	for (int i = 0; i < driversData.size(); ++i)
	{
		if (driversData[i].pos == pos)
			return driversData[i];
	}

    return DriverData();
}

QString EventData::calculateInterval(DriverData d1, DriverData d2, int lap)
{
	LapData ld1 = d1.getLapData(lap);
	LapData ld2 = d2.getLapData(lap);

	if (lap == -1 && !d1.lapData.isEmpty() && !d2.lapData.isEmpty())
	{
		ld1 = d1.lastLap;//d1.lapData.get(d1.lapData.size()-1);
		ld2 = d2.lastLap;//d2.lapData.get(d2.lapData.size()-1);
	}

	if (ld1.carID == -1 || ld2.carID == -1)
		return "";

	QString gap1 = ld1.gap;
	QString gap2 = ld2.gap;

	if ((ld1.lapTime.toString() == "" && ld1.gap == "") ||
		(ld2.lapTime.toString() == "" && ld2.gap == ""))
		return "";

	if (ld1.pos == 1)
		return "-" + (gap2 == "" ? "1L <" : gap2) + (gap2.contains("L") ? " <" : "");

	if (ld2.pos == 1)
		return "+" + (gap1 == "" ? "1L <" : gap1) + (gap1.contains("L") ? " <" : "");

	if ((gap1 != "" && gap2 != "" && gap1[gap1.size()-1] != 'L' &&
			gap2[gap2.size()-1] != 'L') ||
	   ((ld1.pos == 1 && gap1 == "") || (ld2.pos == 1 && gap2 == "")))
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
		int pos1 = ld1.pos;
		int pos2 = ld2.pos;

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

			if (lap == -1 && !driversData[i].lapData.isEmpty())
				ld = driversData[i].lastLap;//lapData.get(driversData.get(i).lapData.size()-1);

			if (ld.carID == -1)
				continue;

			int pos = ld.pos;
			if (pos > pos1 && pos <= pos2)
			{
				if (ld.interval != "" && ld.interval.contains("L"))
					return neg ? "-1L <" : "+1L <";

				intervals.append(ld.interval);
			}
		}
		double interval = 0.0;

		for (int i = 0; i < intervals.size(); ++i)
			interval += intervals[i].toDouble();

		if (neg && ld1.lapTime.isValid() && interval > ld1.lapTime.toDouble())
			return "-1L <";
		if (!neg && ld2.lapTime.isValid() && interval > ld2.lapTime.toDouble())
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


