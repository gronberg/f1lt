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



#ifndef IMAGESFACTORY_H
#define IMAGESFACTORY_H

#include <QList>
#include <QMap>
#include <QPixmap>

class CarThumbnailsFactory
{
public:
    ~CarThumbnailsFactory();
    QList<QPixmap*> *loadCarThumbnails(int size, bool clear = true);
    QPixmap &getCarThumbnail(int no, int size);

private:
    QMap<int, QList<QPixmap*> > carThumbnails;
    QPixmap nullPixmap;
};

class HelmetsFactory
{
public:
    ~HelmetsFactory();
    QList<QPixmap*> *loadHelmets(int size, bool clear = true);
    QPixmap &getHelmet(int no, int size);

    QPixmap *loadHelmet(int no, int size);

    void reloadHelmets();

private:
    QMap<int, QList<QPixmap*> > helmets;
    QPixmap nullPixmap;
};


#endif // IMAGESFACTORY_H
