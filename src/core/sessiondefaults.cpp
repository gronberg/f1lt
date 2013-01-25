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


#include "sessiondefaults.h"

#include "eventdata.h"

SessionDefaults::SessionDefaults()
{
    fpLengths.append(90);
    fpLengths.append(90);
    fpLengths.append(60);

    qualiLengths.append(20);
    qualiLengths.append(15);
    qualiLengths.append(10);
}

int SessionDefaults::timeToMins(const QTime &time) const
{
    int hour = time.hour();
    int min = time.minute();

    return hour * 60 + min + 1;
}

int SessionDefaults::timeToSecs(const QTime &time) const
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    return hour * 3600 + min * 60 + sec;
}



int SessionDefaults::getFPLength() const
{
    return getFPLength(EventData::getInstance().getFPNumber());
}

int SessionDefaults::getFPLength(int fp) const
{
    if (fp >= 1 && fp <= fpLengths.size())
        return fpLengths[fp-1];

    return 0;
}

QTime SessionDefaults::correctFPTime(const QTime &time) const
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

QTime SessionDefaults::correctQualiTime(const QTime &time, int qualiPeriod) const
{
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    int sLength = 10 + (qualiLengths.size() - qualiPeriod)*5;

    int t = sLength * 60 - hour * 3600 - min * 60 - sec;

    hour = t/3600;
    min = (t%3600)/60;
    sec = (t%3600)%60;
    QTime newTime(hour, min, sec);

    return newTime;
}

int SessionDefaults::getQualiLength(int q) const
{
    if (q >= 1 && q <= qualiLengths.size())
        return qualiLengths[q-1];

    return 0;
}
