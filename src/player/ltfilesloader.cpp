#include "ltfilesloader.h"
#include "../core/eventdata.h"

#include <QDebug>

LTFilesLoader::LTFilesLoader()
{
}

bool LTFilesLoader::loadFile(QString fName, QVector<QPair<int, Packet> > &packets)
{
    QFile file(fName);

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);

        char *tab;
        stream >> tab;

        QString sbuf(tab);

        delete [] tab;
        if (sbuf == "F1LT")
        {
            EventData::getInstance().clear();

            //old files didn't contain any info about FP number, try to guess it from the file name
            QFileInfo fInfo(fName);
            QRegExp reg("fp(\\d)");

            if (reg.indexIn(fInfo.fileName()) != -1)
                EventData::getInstance().setFPNumber(reg.cap(1).toInt());

            return loadV1File(stream, packets);                        
        }

        if (sbuf == "F1LT2_LT")
        {
            EventData::getInstance().clear();
            return loadV2File(stream, packets);                
        }
    }
    return false;
}

bool LTFilesLoader::loadV1File(QDataStream &stream, QVector<QPair<int, Packet> > &packets)
{
    LTEvent ltEvent;
    QVector<LTTeam> ltTeamList;

    int ibuf;
    int size;
    QPixmap pixBuf;
    QString sbuf;

    //load event data
    stream >> ibuf;
    ltEvent.eventNo = ibuf;

    stream >> sbuf;
    ltEvent.eventName = sbuf;

    stream >> sbuf;
    ltEvent.eventShortName = sbuf;

    stream >> sbuf;
    ltEvent.eventPlace = sbuf;

    stream >> sbuf;
    ltEvent.fpDate = QDate::fromString(sbuf, "dd-MM-yyyy");

    stream >> sbuf;
    ltEvent.raceDate = QDate::fromString(sbuf, "dd-MM-yyyy");

    stream >> ibuf;
    ltEvent.laps = ibuf;

    stream >> pixBuf;
    ltEvent.trackImg = pixBuf;

    EventData::getInstance().setEventInfo(ltEvent);

    //load drivers data
    stream >> size;
    ltTeamList.resize(size);
    for (int i = 0; i < size; ++i)
    {
        ltTeamList[i].drivers.resize(2);

        stream >> sbuf; ltTeamList[i].teamName = sbuf;

        stream >> sbuf; ltTeamList[i].drivers[0].name = sbuf;
        stream >> sbuf; ltTeamList[i].drivers[0].shortName = sbuf;
        stream >> ibuf; ltTeamList[i].drivers[0].no = ibuf;

        stream >> sbuf; ltTeamList[i].drivers[1].name = sbuf;
        stream >> sbuf; ltTeamList[i].drivers[1].shortName = sbuf;
        stream >> ibuf; ltTeamList[i].drivers[1].no = ibuf;

        stream >> ltTeamList[i].carImg;
    }
    SeasonData::getInstance().loadSeasonData(ltEvent.fpDate.year());
    SeasonData::getInstance().updateTeamList(ltTeamList);

    stream >> size;
    packets.resize(size);
    for (int i = 0; i < size; ++i)
    {
        stream >> packets[i].first;
        stream >> packets[i].second.type;
        stream >> packets[i].second.carID;
        stream >> packets[i].second.data;
        stream >> packets[i].second.length;
        stream >> packets[i].second.longData;
    }
    return true;
}

bool LTFilesLoader::loadV2File(QDataStream &stream, QVector<QPair<int, Packet> > &packets)
{
    LTEvent ltEvent;
    QVector<LTTeam> ltTeamList;

    int ibuf;
    int size;
    char *cbuf;

    //load event data
    stream >> ibuf;
    ltEvent.eventNo = ibuf;

    stream >> cbuf;
    ltEvent.eventName = encrypt(QString(cbuf));

    stream >> cbuf;
    ltEvent.eventShortName = encrypt(QString(cbuf));

    stream >> cbuf;
    ltEvent.eventPlace = encrypt(QString(cbuf));

    stream >> cbuf;
    ltEvent.fpDate = QDate::fromString(encrypt(QString(cbuf)), "dd-MM-yyyy");

    stream >> cbuf;
    ltEvent.raceDate = QDate::fromString(encrypt(QString(cbuf)), "dd-MM-yyyy");

    stream >> ibuf;
    ltEvent.laps = ibuf;

    stream >> ibuf;
    EventData::getInstance().setEventType((LTPackets::EventType)ibuf);

    stream >> ibuf;
    if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
        EventData::getInstance().setFPNumber(ibuf);


    SeasonData::getInstance().loadSeasonData(ltEvent.fpDate.year());
    EventData::getInstance().setEventInfo(ltEvent);

    //load drivers data
    stream >> size;
    ltTeamList.resize(size);
    for (int i = 0; i < size; ++i)
    {
        stream >> cbuf; ltTeamList[i].teamName = encrypt(QString(cbuf));

        int dsize;
        stream >> dsize;

        ltTeamList[i].drivers.resize(dsize);
        for (int j = 0; j < dsize; ++j)
        {
            stream >> cbuf; ltTeamList[i].drivers[j].name = encrypt(QString(cbuf));
            stream >> cbuf; ltTeamList[i].drivers[j].shortName = encrypt(QString(cbuf));
            stream >> ibuf; ltTeamList[i].drivers[j].no = ibuf;
        }
    }
    SeasonData::getInstance().updateTeamList(ltTeamList);

    stream >> size;
    packets.resize(size);
    for (int i = 0; i < size; ++i)
    {
        stream >> packets[i].first;
        stream >> packets[i].second.type;
        stream >> packets[i].second.carID;
        stream >> packets[i].second.data;
        stream >> packets[i].second.length;
        stream >> cbuf;
        packets[i].second.longData.clear();
//        packets[i].second.longData.append(encrypt(std::string(cbuf)).c_str());
//        packets[i].second.longData.append(encrypt(sbuf));
        packets[i].second.longData.append(encrypt(QString(cbuf)));
    }
    return true;
}

void LTFilesLoader::saveFile(QString fName, const QVector<QPair<int, Packet> > &packets)
{
    QFile file(fName);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);

        const char tab[9] = "F1LT2_LT";

        stream << tab;

        //save the teams and events info
        saveMainData(stream);
        savePackets(stream, packets);
    }
}

void LTFilesLoader::saveMainData(QDataStream &stream)
{
    LTEvent event = EventData::getInstance().getEventInfo();

    stream << event.eventNo;
    stream << encrypt(event.eventName).toStdString().c_str();
    stream << encrypt(event.eventShortName).toStdString().c_str();
    stream << encrypt(event.eventPlace).toStdString().c_str();
    stream << encrypt(event.fpDate.toString("dd-MM-yyyy")).toStdString().c_str();
    stream << encrypt(event.raceDate.toString("dd-MM-yyyy")).toStdString().c_str();
    stream << event.laps;
    stream << EventData::getInstance().getEventType();
    stream << EventData::getInstance().getFPNumber();

    QVector<LTTeam> teams = SeasonData::getInstance().getTeamsFromCurrentSession();
    stream << teams.size();
    for (int i = 0; i < teams.size(); ++i)
    {
        stream << encrypt(teams[i].teamName).toStdString().c_str();
        stream << teams[i].drivers.size();

        for (int j = 0; j < teams[i].drivers.size(); ++j)
        {
            stream << encrypt(teams[i].drivers[j].name).toStdString().c_str();
            stream << encrypt(teams[i].drivers[j].shortName).toStdString().c_str();
            stream << teams[i].drivers[j].no;
        }
    }
}

void LTFilesLoader::savePackets(QDataStream &stream, const QVector<QPair<int, Packet> > &packets)
{
    stream << packets.size();

    for (int i = 0; i < packets.size(); ++i)
    {
        stream << packets[i].first;
        stream << packets[i].second.type;
        stream << packets[i].second.carID;
        stream << packets[i].second.data;
        stream << packets[i].second.length;

//        qDebug() << encrypt(QString(packets[i].second.longData)).size() << QString(packets[i].second.longData);
//        stream << encrypt(QString(packets[i].second.longData).toStdString()).c_str();
        stream << encrypt(QString(packets[i].second.longData)).toStdString().c_str();
//        stream << QString(packets[i].second.longData).toStdString().c_str();
    }
}
