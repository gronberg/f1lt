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

#ifndef COLORSMANAGER_H
#define COLORSMANAGER_H

#include <QColor>
#include <QList>

#include "ltpackets.h"

/*!
 * \brief This class is responsible for holding interace and driver colors. This is a singleton.
 */
class ColorsManager
{
public:    

    static ColorsManager &getInstance()
    {
        static ColorsManager instance;
        return instance;
    }

    QColor getColor(LTPackets::Colors color)
    {
        return colors[color];
    }

    QColor getDefaultColor(LTPackets::Colors color)
    {
        return defaultColors[color];
    }

    QList<QColor> getColors()
    {
        return colors;
    }

    QList<QColor> getDefaultColors()
    {
        return defaultColors;
    }

    void setColors(QList<QColor> col)
    {
        colors = col;
    }

    void setColor(LTPackets::Colors colorCode, QColor color)
    {
        colors[colorCode] = color;
    }
    void setDefaultColor(LTPackets::Colors colorCode)
    {
        colors[colorCode] = defaultColors[colorCode];
    }
    void setAllDefaultColors()
    {
        colors = defaultColors;
    }

    QColor getCarColor(int no);


    QList<QColor> getDriverColors()
    {
        return driverColors;
    }
    void setDriverColors(QList<QColor> colors)
    {
        driverColors = colors;
    }
    QList<QColor> getDefaultDriverColors()
    {
        return defaultDriverColors;
    }

private:
    ColorsManager();

    QList<QColor> colors;
    QList<QColor> defaultColors;

    QList<QColor> driverColors;
    QList<QColor> defaultDriverColors;
};

#endif // COLORSMANAGER_H
