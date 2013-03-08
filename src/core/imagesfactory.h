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

class ImagesFactory;
class LTDriver;

/*!
 * \brief The CarThumbnailsFactory class stores car thumbnails of all required sizes. All objects have access to car thumbnails using this class.
 * Only constructor of CarThumbnailsFactory is private, access to object of this class is available by ImagesFactory singleton only.
 */
class CarThumbnailsFactory
{
public:
    ~CarThumbnailsFactory();
    QList<QPixmap*> *loadCarThumbnails(int size, bool clear = true);
    QPixmap &getCarThumbnail(int no, int size);

    void reloadCarThumbnails();

    friend class ImagesFactory;

private:
    CarThumbnailsFactory() { }
    QMap<int, QList<QPixmap*> > carThumbnails;
    QPixmap nullPixmap;
};

/*!
 * \brief The HelmetsFactory class stores helmet images of all required sizes. All objects have access to helmet images using this class.
 * Only constructor of CarThumbnailsFactory is private, access to object of this class is available by ImagesFactory singleton only.
 */
class HelmetsFactory
{
public:
    ~HelmetsFactory();
    QMap<int, QPixmap*> *loadHelmets(int size, bool clear = true);
    QPixmap &getHelmet(int no, int size);

    QPixmap *loadHelmet(const LTDriver &driver, int size);

    void reloadHelmets();

    friend class ImagesFactory;

private:
    HelmetsFactory() { }
    QMap<int, QMap<int, QPixmap*> > helmets;
    QPixmap nullPixmap;
};

/*!
 * \brief The ImagesFactory class is a singleton used only to give access to HelmetsFactory and CarThumbnailsFactory objects.
 */
class ImagesFactory : public QObject
{
    Q_OBJECT
public:
    static ImagesFactory &getInstance()
    {
        static ImagesFactory imFact;
        return imFact;
    }

    CarThumbnailsFactory &getCarThumbnailsFactory() { return carThumbnailsFactory; }
    HelmetsFactory &getHelmetsFactory() { return helmetsFactory; }

public slots:
    void reloadGraphics()
    {
        carThumbnailsFactory.reloadCarThumbnails();
        helmetsFactory.reloadHelmets();
    }

private:
    ImagesFactory() { }

    CarThumbnailsFactory carThumbnailsFactory;
    HelmetsFactory helmetsFactory;
};


#endif // IMAGESFACTORY_H
