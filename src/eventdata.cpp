#include "eventdata.h"
#include "ltdata.h"

EventData::EventData()
{
    FLLap = -1;
    sec1Record[2] = "-1";
    sec2Record[2] = "-1";
    sec3Record[2] = "-1";

    eventInfo.laps = 0;
    eventInfo.eventNo = 0;

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
    }

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

/*

EventData eventData;*/


