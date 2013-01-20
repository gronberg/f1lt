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


#ifndef SESSIONDEFAULTS_H
#define SESSIONDEFAULTS_H

#include <QList>
#include <QTime>

/*!
 * \brief The SessionDefaults class holds default settings of Formula 1 sessions, like practice and quali lengths
 */

class SessionDefaults
{
public:
    SessionDefaults();

    int getFPLength() const;
    int getFPLength(int fp) const;
    int getQualiLength(int q) const;

    QTime correctFPTime(const QTime &time) const;
    QTime correctQualiTime(const QTime &time, int qualiPeriod) const;

    int timeToMins(const QTime &time) const;
    int timeToSecs(const QTime &time) const;

private:
    QList<int> fpLengths;
    QList<int> qualiLengths;
};

#endif // SESSIONDEFAULTS_H
