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


#include "trackmapscoordinates.h"

#include "eventdata.h"

TrackMapsCoordinates::TrackMapsCoordinates()
{
}

bool TrackMapsCoordinates::loadTrackDataFile()
{
    QString fName = F1LTCore::trackDataFile();
    if (!fName.isNull())
    {
        QFile f(fName);
        if (f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);

            char *cbuf;

            stream >> cbuf;
            QString sbuf(cbuf);
            delete [] cbuf;

            if (sbuf != "F1LT_TD")
                return false;

            int size;
            stream >> size;

            for (int i = 0; i < size; ++i)
            {
                TrackCoordinates trackCoordinates;

                int coordSize;
                stream >> cbuf;
                trackCoordinates.name = QString(cbuf);
                delete [] cbuf;

                stream >> trackCoordinates.year;
                stream >> trackCoordinates.indexes[0];
                stream >> trackCoordinates.indexes[1];
                stream >> trackCoordinates.indexes[2];
                stream >> coordSize;

                for (int j = 0; j < coordSize; ++j)
                {
                    QPoint p;
                    stream >> p;
                    trackCoordinates.coordinates.append(p);
                }
                ltTrackCoordinates.append(trackCoordinates);

//                for (int j = 0; j < ltEvents.size(); ++j)
//                {
//                    if (ltEvents[j].eventShortName.toLower() == trackCoordinates.name.toLower())
//                    {
//                        ltEvents[j].trackCoordinates = trackCoordinates;
//                        break;
//                    }
//                }
            }
            return true;
        }
    }
    return false;
}

TrackCoordinates TrackMapsCoordinates::getCurrentTrackCoordinates() const
{
    EventData &ed = EventData::getInstance();
    for (int i = 0; i < ltTrackCoordinates.size(); ++i)
    {
        if (ltTrackCoordinates[i].name.toLower() == ed.getEventInfo().eventShortName.toLower())
        {
            return ltTrackCoordinates[i];
        }
    }
    return TrackCoordinates();
}
