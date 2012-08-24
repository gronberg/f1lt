#include "lapdata.h"

LapTime::LapTime(int ms)
{
    int msec = ms;
    int sec = ms / 1000;
    int min = 0;

    if (sec > 0)
        msec = ms % (sec * 1000);


    if (sec > 60)
    {
        min = sec / 60;

        if (min > 0)
            sec = sec % (min * 60);
    }
    time = LapTime(min, sec, msec).toString();
}

//----------------------------------

int LapTime::toMsecs() const
{
    if (!isValid())
        return 0;

    int idx = time.indexOf(":");
    int min=0, sec=0, msec=0;

    if (idx > -1)
        min = time.left(idx).toInt();

    int idx2 = time.indexOf(".", idx < 0 ? 0 : idx);
    if(idx2 > -1)
    {
        sec = time.mid(idx+1, idx2 - idx - 1).toInt();
        QString strMS = time.right(time.size() - idx2 - 1);

        msec = strMS.toInt() * (strMS.size() < 3 ? (strMS.size() < 2 ? 100 : 10) : 1);
    }

    msec += sec * 1000 + min * 60000;
    return msec;
}

//----------------------------------

QString LapTime::toSecs() const
{
    double sec = (double)(toMsecs() / 1000.0);
    return QString::number(sec, 'f', 3);
}

//----------------------------------

bool LapTime::operator < (const LapTime &lt) const
{
    if (!isValid())
        return false;

    if (!lt.isValid())
        return true;

    return toMsecs() < lt.toMsecs();
}

//----------------------------------

bool LapTime::operator <= (const LapTime &lt) const
{
    if (!isValid())
        return false;

    if (!lt.isValid())
        return true;

    return toMsecs() <= lt.toMsecs();
}

//----------------------------------

bool LapTime::operator == (const LapTime &lt) const
{
    if (isValid() && lt.isValid())
        return toMsecs() == lt.toMsecs();

    return false;
}

//----------------------------------

LapTime LapTime::operator - (const LapTime &lt) const
{
    return LapTime(toMsecs() - lt.toMsecs());
}

//----------------------------------

LapTime LapTime::operator + (const LapTime &lt) const
{
    return LapTime(toMsecs() + lt.toMsecs());
}

//----------------------------------

bool LapTime::isValid() const
{
    if (time == "")
        return false;

    bool ok;
    int idx = time.indexOf(":");

    if (idx > -1)
    {
        time.left(idx).toInt(&ok);
        if (!ok)
            return false;
    }

    int idx2 = time.indexOf(".", (idx < 0 ? 0 : idx));
    if (idx2 > -1)
    {
        time.mid(idx+1, idx2-idx-1).toInt(&ok);

        if (!ok)
            return false;

        time.right(time.size() - idx2 - 1).toInt(&ok);

        if (!ok)
            return false;
    }
    else
        return false;

    return true;
}
