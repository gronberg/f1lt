#include "seasondata.h"

#include "ltpackets.h"
#include <QDataStream>
#include <QFile>

#include "eventdata.h"

CarThumbnailsFactory::~CarThumbnailsFactory()
{
    QMap<int, QList<QPixmap*> >::Iterator iter = carThumbnails.begin();
    while (iter != carThumbnails.end())
    {
        QList<QPixmap*> *images = &iter.value();

        while (!images->isEmpty())
        {
            delete images->takeFirst();
        }
        ++iter;
    }
}

QList<QPixmap*> *CarThumbnailsFactory::loadCarThumbnails(int size, bool clear)
{
    QList<QPixmap*> *images = &carThumbnails[size];

    if (!images->isEmpty())
    {
        if (clear)
        {
            for (int i = 0; i < images->size(); ++i)
            {
                delete (*images)[i];
            }

            images->clear();
        }
        else
            return images;
    }

    for (int i = 0; i < SeasonData::getInstance().getTeams().size(); ++i)
    {
        images->append(new QPixmap(SeasonData::getInstance().getTeams()[i].carImg.scaledToWidth(size, Qt::SmoothTransformation)));
    }

    return images;
}

QPixmap &CarThumbnailsFactory::getCarThumbnail(int no, int size)
{
    if (no < 1)
        return nullPixmap;

    const QList<QPixmap*> *images = loadCarThumbnails(size, false);

    int idx = (no > 13 ? no-2 : no-1) / 2;

    if (idx >= 0 && idx < images->size())
        return *(*images)[idx];

    return nullPixmap;
}

SeasonData::SeasonData() : season(2012), baseEventId (7066), baseEventInc (6)
{
    fpLengths[0] = 90;
    fpLengths[1] = 90;
    fpLengths[2] = 60;

    qualiLengths[0] = 20;
    qualiLengths[1] = 15;
    qualiLengths[2] = 10;

    colors.clear();
    for (int i = 0; i < 10; ++i)
        colors.append(QColor());

    colors[LTPackets::DEFAULT] = QColor(150, 150, 150);
    colors[LTPackets::CYAN] = QColor(0, 255, 255);
    colors[LTPackets::GREEN] = QColor(0, 255, 0);
    colors[LTPackets::VIOLET] = QColor(255, 0, 255);
    colors[LTPackets::WHITE] = QColor(220, 220, 220);
    colors[LTPackets::YELLOW] = QColor(255, 255, 0);
    colors[LTPackets::RED] = QColor(231, 31, 31);
    colors[LTPackets::PIT] = QColor(231, 31, 31);
    colors[LTPackets::BACKGROUND] = QColor(20, 20, 20);
    colors[LTPackets::BACKGROUND2] = QColor(30, 30, 30);

    driverColors.append(QColor(0, 255, 255));
    driverColors.append(QColor(0, 0, 255));
    driverColors.append(QColor(128, 0, 128));
    driverColors.append(QColor(255, 0, 255));
    driverColors.append(QColor(229, 30, 27));
    driverColors.append(QColor(165, 25, 28));
    driverColors.append(QColor(255, 255, 255));
    driverColors.append(QColor(148, 148, 176));
    driverColors.append(QColor(255, 209, 38));
    driverColors.append(QColor(255, 104, 57));
    driverColors.append(QColor(124, 196, 236));
    driverColors.append(QColor(12, 163, 218));
    driverColors.append(QColor(19, 139, 65));
    driverColors.append(QColor(114, 183, 136));
    driverColors.append(QColor(94, 93, 91));
    driverColors.append(QColor(157, 155, 156));
    driverColors.append(QColor(94, 109, 157));
    driverColors.append(QColor(39, 72, 125));
    driverColors.append(QColor(0, 255, 0));
    driverColors.append(QColor(26, 98, 21));
    driverColors.append(QColor(128, 64, 0));
    driverColors.append(QColor(206, 103, 0));
    driverColors.append(QColor(234, 78, 115));
    driverColors.append(QColor(245, 146, 166));

    ltTeams.clear();
    fillEventNamesMap();
}

bool SeasonData::loadSeasonFile()
{
    ltTeams.clear();
    QString fName = F1LTCore::seasonDataFile();
    if (!fName.isNull())
    {
        QFile f(fName);
        if (f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);
            int size;
            QString sbuf;

            stream >> season;
//            ui->spinBox->setValue(ibuf);

            stream >> size;
            ltEvents.resize(size);
            for (int i = 0; i < size; ++i)
            {
                stream >> ltEvents[i].eventNo;
                stream >> ltEvents[i].eventName;
                stream >> ltEvents[i].eventShortName;
                stream >> ltEvents[i].eventPlace;
                stream >> ltEvents[i].laps;

                stream >> sbuf;
                ltEvents[i].fpDate = QDate::fromString(sbuf, "dd-MM-yyyy");

                stream >> sbuf;
                ltEvents[i].raceDate = QDate::fromString(sbuf, "dd-MM-yyyy");

                stream >> ltEvents[i].trackImg;

            }
            stream >> size;
            ltTeams.resize(size);
            for (int i = 0; i < size; ++i)
            {
                stream >> ltTeams[i].teamName;
                stream >> ltTeams[i].driver1Name;
                stream >> ltTeams[i].driver1ShortName;
                stream >> ltTeams[i].driver1No;
                stream >> ltTeams[i].driver2Name;
                stream >> ltTeams[i].driver2ShortName;
                stream >> ltTeams[i].driver2No;
                stream >> ltTeams[i].carImg;
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

QPixmap SeasonData::getCarImg(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1No == no || ltTeams[i].driver2No == no)
            return ltTeams[i].carImg;
    }
    return QPixmap();
}

QString SeasonData::getDriverName(QString &name)
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

QString SeasonData::getDriverLastName(const QString &name)
{
    if (name.size() > 3)
        return name.right(name.size()-3);

    return name;
}

QString SeasonData::getDriverShortName(const QString &name)
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
}

QString SeasonData::getDriverNameFromShort(const QString &name)
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

QString SeasonData::getTeamName(int no)
{
    for (int i = 0; i < ltTeams.size(); ++i)
    {
        if (ltTeams[i].driver1No == no || ltTeams[i].driver2No == no)
            return ltTeams[i].teamName;
    }
    return QString();
}

LTEvent SeasonData::getEvent(int ev)
{
    for (int i = 0; i < ltEvents.size(); ++i)
    {
        if (ltEvents[i].eventNo == ev)
            return ltEvents[i];
    }
    return LTEvent();
}

int SeasonData::getEventNo(QDate date)
{
    for (int i = 1; i < ltEvents.size(); ++i)
    {
        if (date >= ltEvents[i-1].fpDate && date < ltEvents[i].fpDate)
            return ltEvents[i-1].eventNo;
    }
    return 1;
}

const LTEvent &SeasonData::getEvent(const QDate &date) const
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

const LTEvent &SeasonData::getCurrentEvent()const
{
    return getEvent(QDate::currentDate());
}

const LTEvent &SeasonData::getNextEvent() const
{
    QDate date = QDate::currentDate();
    for (int i = 1; i < ltEvents.size(); ++i)
    {
        if (date > ltEvents[i-1].raceDate && date <= ltEvents[i].raceDate)
            return ltEvents[i];
    }

    return ltEvents[0];
}

int SeasonData::getDriverNo(const QString &name)
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

QStringList SeasonData::getDriversList()
{
    QStringList list;
    list.append("");

    for (int i = 0; i < 30; ++i)
    {
        DriverData dd = EventData::getInstance().getDriverData(i);
        if (dd.getCarID() != -1)
        {
            list.append(QString::number(dd.getNumber()) + " " + dd.getDriverName());
        }
    }
    if (list.size() == 1)
    {
        for (int i = 0; i < ltTeams.size(); ++i)
        {
            list.append(QString::number(ltTeams[i].driver1No) + " " + ltTeams[i].driver1Name);
            list.append(QString::number(ltTeams[i].driver2No) + " " + ltTeams[i].driver2Name);
        }
    }

    return list;
}

QStringList SeasonData::getDriversListShort()
{
    QStringList list;

    for (int i = 0; i < 30; ++i)
    {
        DriverData dd = EventData::getInstance().getDriverData(i);
        if (dd.getCarID() != -1)
        {
            list.append(QString::number(dd.getNumber()) + " " + SeasonData::getDriverShortName(dd.getDriverName()));
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

QString SeasonData::getEventNameFromShort(const QString &shortName)
{
    return eventNamesMap[shortName.toLower()] + " Grand Prix";
//    for (int i = 0; i < ltEvents.size(); ++i)
//    {
//        if (ltEvents[i].eventShortName.toLower() == shortName.toLower())
//            return ltEvents[i].eventName;
//    }
//    return shortName;
}

int SeasonData::timeToMins(const QTime &time)
{
    int hour = time.hour();
    int min = time.minute();

    return hour * 60 + min;
}

int SeasonData::timeToSecs(const QTime &time)
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    return hour * 3600 + min * 60 + sec;
}

int SeasonData::getFPNumber()
{
    if (EventData::getInstance().getEventId() == 0)
        return 1;

    else
        return (EventData::getInstance().getEventId() - 7066)%6;
}

int SeasonData::getFPLength()
{
    return getFPLength(getFPNumber());
}

int SeasonData::getFPLength(int fp)
{
    if (fp >= 1 && fp <= 3)
        return fpLengths[fp-1];

    return 0;
}

QTime SeasonData::correctFPTime(const QTime &time)
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    int t = getFPLength() * 60 - hour * 3600 - min * 60 - sec;
    hour = t/3600;
    min = (t%3600)/60;
    sec = (t%3600)%60;
    QTime newTime(hour, min, sec);

    return newTime;
}

QTime SeasonData::correctQualiTime(const QTime &time, int qualiPeriod)
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

int SeasonData::getQualiLength(int q)
{
    if (q >= 1 && q <= 3)
        return qualiLengths[q-1];

    return 0;
}

QColor SeasonData::getCarColor(const DriverData &dd)
{
    QColor color = getColor(LTPackets::BACKGROUND);
    if (dd.getCarID() > 0)
    {
        int no = dd.getNumber();

        if (no > 0 && no < driverColors.size()+2)
            color = driverColors[no <= 12 ? no-1 : no -2];
    }
    return color;
}

void SeasonData::fillEventNamesMap()
{
    eventNamesMap.insert("aus", "Australian");
    eventNamesMap.insert("mal", "Malaysian");
    eventNamesMap.insert("chn", "Chinese");
    eventNamesMap.insert("bah", "Bahrain");
    eventNamesMap.insert("spa", "Spanish");
    eventNamesMap.insert("mon", "Monaco");
    eventNamesMap.insert("can", "Canadian");
    eventNamesMap.insert("eur", "European");
    eventNamesMap.insert("val", "European");
    eventNamesMap.insert("gbr", "British");
    eventNamesMap.insert("ger", "German");
    eventNamesMap.insert("hun", "Hungarian");
    eventNamesMap.insert("bel", "Belgian");
    eventNamesMap.insert("ita", "Italian");
    eventNamesMap.insert("sgp", "Singapore");
    eventNamesMap.insert("jpn", "Japanese");
    eventNamesMap.insert("kor", "Korean");
    eventNamesMap.insert("ind", "Indian");
    eventNamesMap.insert("abu", "Abu Dhabi");
    eventNamesMap.insert("usa", "US");
    eventNamesMap.insert("bra", "Brazilian");
}


