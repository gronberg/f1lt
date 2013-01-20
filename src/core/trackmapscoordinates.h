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


#ifndef TRACKMAPSCOORDINATES_H
#define TRACKMAPSCOORDINATES_H


#include <QPoint>
#include <QString>
#include <QVector>

/*!
 * \brief The TrackCoordinates struct stores coordinates of a one track map
 */
struct TrackCoordinates
{
    int indexes[3];     //index of S1, S2 and pit out
    QVector<QPoint> coordinates;
    QString name;       //track name
    int year;           //track version
};


/*!
 * \brief The TrackMapsCoordinates class stores track map coordinates loaded from the .dat file. It used in driver tracker.
 */
class TrackMapsCoordinates
{
public:
    TrackMapsCoordinates();
    TrackCoordinates getCurrentTrackCoordinates() const;

    bool loadTrackDataFile();

private:
    QVector<TrackCoordinates> ltTrackCoordinates;
};

#endif // TRACKMAPSCOORDINATES_H
