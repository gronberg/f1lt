#include "ltdata.h"
#include <QDataStream>
#include <QFile>

#include <QDebug>

#include "eventdata.h"

LTData::LTData()
{
}

void LTData::init()
{
    LTData::colors.clear();
    for (int i = 0; i < 10; ++i)
        LTData::colors.append(QColor());

    LTData::colors[LTData::DEFAULT] = QColor(150, 150, 150);
    LTData::colors[LTData::CYAN] = QColor(0, 255, 255);
    LTData::colors[LTData::GREEN] = QColor(0, 255, 0);
    LTData::colors[LTData::VIOLET] = QColor(255, 0, 255);
    LTData::colors[LTData::WHITE] = QColor(220, 220, 220);
    LTData::colors[LTData::YELLOW] = QColor(255, 255, 0);
    LTData::colors[LTData::RED] = QColor(231, 31, 31);
    LTData::colors[LTData::PIT] = QColor(231, 31, 31);
    LTData::colors[LTData::BACKGROUND] = QColor(20, 20, 20);
    LTData::colors[LTData::BACKGROUND2] = QColor(27, 27, 27);

    ltTeams.clear();
}

bool LTData::loadLTData()
{   
    init();
    QString fName = F1LTCore::seasonDataFile();
    if (!fName.isNull())
    {
        QFile f(fName);        
        if (f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);
            int ibuf, size;
            QString sbuf;
            QPixmap pixBuf;

            stream >> ibuf;
            season = ibuf;
//            ui->spinBox->setValue(ibuf);

            stream >> size;
            for (int i = 0; i < size; ++i)
            {
                LTEvent ltev;

                stream >> ibuf;
                ltev.eventNo = ibuf;

                stream >> sbuf;
                ltev.eventName = sbuf;

                stream >> sbuf;
                ltev.eventShortName = sbuf;

                stream >> sbuf;
                ltev.eventPlace = sbuf;

                stream >> ibuf;
                ltev.laps = ibuf;

                stream >> sbuf;
                ltev.fpDate = QDate::fromString(sbuf, "dd-MM-yyyy");

                stream >> sbuf;
                ltev.raceDate = QDate::fromString(sbuf, "dd-MM-yyyy");

                stream >> pixBuf;
                ltev.trackImg = pixBuf;

                ltEvents.append(ltev);
            }
            stream >> size;
            for (int i = 0; i < size; ++i)
            {
                LTTeam ltteam;

                stream >> sbuf;
                ltteam.teamName = sbuf;

                stream >> sbuf;
                ltteam.driver1Name = sbuf;

                stream >> sbuf;
                ltteam.driver1ShortName = sbuf;

                stream >> ibuf;
                ltteam.driver1No = ibuf;

                stream >> sbuf;
                ltteam.driver2Name = sbuf;

                stream >> sbuf;
                ltteam.driver2ShortName = sbuf;

                stream >> ibuf;
                ltteam.driver2No = ibuf;

                stream >> pixBuf;
                ltteam.carImg = pixBuf;

                ltTeams.append(ltteam);
            }
        }
        else
            return false;
    }
    else
        return false;
    qSort(ltTeams);    
    qSort(ltEvents);

    return true;
}

QPixmap LTData::getCarImg(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1No == no || ltTeams[i].driver2No == no)
            return ltTeams[i].carImg;
    }
    return QPixmap();
}

QString LTData::getDriverName(QString name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        QString buf1 = ltTeams[i].driver1Name.toUpper();
        QString buf2 = ltTeams[i].driver2Name.toUpper();

        if (buf1 == name)
            return ltTeams[i].driver1Name;

        if (buf2 == name)
            return ltTeams[i].driver2Name;
    }
    name = name.left(4) + name.right(name.size()-4).toLower();
    int idx = name.indexOf(" ");
    while (idx != -1)
    {
        name.replace(name[idx+1], name[idx+1].toUpper());
        idx = name.indexOf(" ", idx+1);
    }
    return name;
}

QString LTData::getDriverShortName(QString name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        QString buf1 = ltTeams[i].driver1Name.toUpper();
        QString buf2 = ltTeams[i].driver2Name.toUpper();

        if (buf1 == name.toUpper())
            return ltTeams[i].driver1ShortName;

        if (buf2 == name.toUpper())
            return ltTeams[i].driver2ShortName;
    }
    return name.toUpper().mid(3, 3);
//    name = name.left(4) + name.right(name.size()-4).toLower();
//    int idx = name.indexOf(" ");
//    while (idx != -1)
//    {
//        name.replace(name[idx+1], name[idx+1].toUpper());
//        idx = name.indexOf(" ", idx+1);
//    }
//    return name;
}

QString LTData::getDriverNameFromShort(QString name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        QString buf1 = ltTeams[i].driver1ShortName;
        QString buf2 = ltTeams[i].driver2ShortName;

        if (buf1 == name)
            return ltTeams[i].driver1Name;

        if (buf2 == name)
            return ltTeams[i].driver2Name;
    }
    return name.left(1) + name.right(name.size()-1).toLower();
}

QString LTData::getTeamName(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1No == no || ltTeams[i].driver2No == no)
            return ltTeams[i].teamName;
    }
    return QString();
}

LTEvent LTData::getEvent(int ev)
{
    for (int i = 0; i < ltEvents.size(); ++i)
    {
        if (ltEvents[i].eventNo == ev)
            return ltEvents[i];
    }
    return LTEvent();
}

int LTData::getEventNo(QDate date)
{
    for (int i = 1; i < ltEvents.size(); ++i)
    {        
        if (date >= ltEvents[i-1].fpDate && date < ltEvents[i].fpDate)     
            return ltEvents[i-1].eventNo;        
    }
    return 1;
}

LTEvent LTData::getEvent(QDate date)
{
    for (int i = 1; i < ltEvents.size(); ++i)
    {
        if (date >= ltEvents[i-1].fpDate && date < ltEvents[i].fpDate)
            return ltEvents[i-1];
    }
    if (date >= ltEvents.last().fpDate && date <= ltEvents.last().raceDate)
        return ltEvents.last();

    return ltEvents[0];
}

LTEvent LTData::getCurrentEvent()
{
	return getEvent(QDate::currentDate());
}

LTEvent LTData::getNextEvent()
{
	QDate date = QDate::currentDate();
	for (int i = 1; i < ltEvents.size(); ++i)
	{
		if (date > ltEvents[i-1].raceDate && date <= ltEvents[i].raceDate)
			return ltEvents[i];
	}

	return ltEvents[0];
}

int LTData::getDriverNo(QString name)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1Name == name)
            return ltTeams[i].driver1No;

        if (ltTeams[i].driver2Name == name)
            return ltTeams[i].driver2No;
    }
    return -1;
}

QStringList LTData::getDriversList()
{
    QStringList list;
    list.append("");

    for (int i = 0; i < 30; ++i)
    {
    	DriverData dd = EventData::getInstance().getDriverData(i);
    	if (dd.carID != -1)
    	{
    		list.append(QString::number(dd.number) + " " + dd.driver);
    	}
    }
    if (list.isEmpty())
    {
		for (int i = 0; i < ltTeams.size(); ++i)
		{
            list.append(QString::number(ltTeams[i].driver1No) + " " + ltTeams[i].driver1Name);
            list.append(QString::number(ltTeams[i].driver2No) + " " + ltTeams[i].driver2Name);
		}
    }

    return list;
}

QStringList LTData::getDriversListShort()
{
    QStringList list;

    for (int i = 0; i < 30; ++i)
    {
        DriverData dd = EventData::getInstance().getDriverData(i);
        qDebug() << "driver=" << dd.number <<" " << dd.driver;
        if (dd.carID != -1)
        {
            list.append(QString::number(dd.number) + " " + LTData::getDriverShortName(dd.driver));
        }
    }
    if (list.isEmpty())
    {
        for (int i = 0; i < ltTeams.size(); ++i)
        {
            list.append(QString::number(ltTeams[i].driver1No) + " " + ltTeams[i].driver1ShortName);
            list.append(QString::number(ltTeams[i].driver2No) + " " + ltTeams[i].driver2ShortName);
        }
    }

    return list;
}

int LTData::timeToMins(QTime time)
{
    int hour = time.hour();
    int min = time.minute();

    return hour * 60 + min;
}

int LTData::timeToSecs(QTime time)
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    return hour * 3600 + min * 60 + sec;
}

int LTData::currentEventFPLength()
{
//    LTEvent event = getCurrentEvent();
    return 90;

}

QTime LTData::correctFPTime(QTime time)
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    int t = currentEventFPLength() * 60 - hour * 3600 - min * 60 - sec;
    hour = t/3600;
    min = (t%3600)/60;
    sec = (t%3600)%60;
    QTime newTime(hour, min, sec);

    return newTime;
}

QTime LTData::correctQualiTime(QTime time, int qualiPeriod)
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    int sLength = 10 + (3-qualiPeriod)*5;

    int t = sLength * 60 - hour * 3600 - min * 60 - sec;

    hour = t/3600;
    min = (t%3600)/60;
    sec = (t%3600)%60;
    QTime newTime(hour, min, sec);

    return newTime;
}


QList<QColor> LTData::colors;
int LTData::season;
QList<LTEvent> LTData::ltEvents;
QList<LTTeam> LTData::ltTeams;

