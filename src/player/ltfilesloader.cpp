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
            return loadV1File(stream, packets);

        if (sbuf == "F1LT2_LT")
            return loadV2File(stream, packets);
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
    ltEvent.eventName = QString(cbuf);

    stream >> cbuf;
    ltEvent.eventShortName = QString(cbuf);

    stream >> cbuf;
    ltEvent.eventPlace = QString(cbuf);

    stream >> cbuf;
    ltEvent.fpDate = QDate::fromString(QString(cbuf), "dd-MM-yyyy");

    stream >> cbuf;
    ltEvent.raceDate = QDate::fromString(QString(cbuf), "dd-MM-yyyy");

    stream >> ibuf;
    ltEvent.laps = ibuf;

    SeasonData::getInstance().loadSeasonData(ltEvent.fpDate.year());
    EventData::getInstance().setEventInfo(ltEvent);

    //load drivers data
    stream >> size;
    ltTeamList.resize(size);
    for (int i = 0; i < size; ++i)
    {
        stream >> cbuf; ltTeamList[i].teamName = QString(cbuf);
        qDebug() << "TEAM=" << ltTeamList[i].teamName;

        int dsize;
        stream >> dsize;

        ltTeamList[i].drivers.resize(dsize);
        for (int j = 0; j < dsize; ++j)
        {
            stream >> cbuf; ltTeamList[i].drivers[j].name = QString(cbuf);
            stream >> cbuf; ltTeamList[i].drivers[j].shortName = QString(cbuf);
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
        packets[i].second.longData = QByteArray(cbuf);
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
    stream << event.eventName.toStdString().c_str();
    stream << event.eventShortName.toStdString().c_str();
    stream << event.eventPlace.toStdString().c_str();
    stream << event.fpDate.toString("dd-MM-yyyy").toStdString().c_str();
    stream << event.raceDate.toString("dd-MM-yyyy").toStdString().c_str();
    stream << event.laps;

    QVector<LTTeam> teams = SeasonData::getInstance().getTeamsFromCurrentSession();
    stream << teams.size();
    for (int i = 0; i < teams.size(); ++i)
    {
        stream << teams[i].teamName.toStdString().c_str();
        stream << teams[i].drivers.size();

        for (int j = 0; j < teams[i].drivers.size(); ++j)
        {
            stream << teams[i].drivers[j].name.toStdString().c_str();
            stream << teams[i].drivers[j].shortName.toStdString().c_str();
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
        stream << packets[i].second.longData.constData();
    }
}
