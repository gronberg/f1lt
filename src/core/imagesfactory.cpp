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


#include "colorsmanager.h"
#include "imagesfactory.h"
#include "seasondata.h"

#include <QPainter>

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

void CarThumbnailsFactory::reloadCarThumbnails()
{
    QList<int> keys = carThumbnails.keys();

    for (int i = 0; i < keys.size(); ++i)
        loadCarThumbnails(keys[i], true);
}

//====================================================

HelmetsFactory::~HelmetsFactory()
{
    QMap<int, QMap<int, QPixmap*> >::Iterator iter = helmets.begin();
    while (iter != helmets.end())
    {
        QList<int> keys = iter.value().keys();

        for (int i = 0; i < keys.size(); ++i)
        {
            delete iter.value().take(keys[i]);
        }

        ++iter;
    }
}

QMap<int, QPixmap *> *HelmetsFactory::loadHelmets(int size, bool clear)
{
    QMap<int, QPixmap *> *images = &helmets[size];

    if (!images->isEmpty())
    {
        if (clear)
        {
            QList<int> keys = images->keys();

            for (int i = 0; i < keys.size(); ++i)
            {
                delete images->take(keys[i]);
            }
        }
        else
            return images;
    }

    for (int i = 0; i < SeasonData::getInstance().getTeams().size(); ++i)
    {

        QList<LTDriver> mainDrivers = SeasonData::getInstance().getMainDrivers(SeasonData::getInstance().getTeams()[i]);
        qSort(mainDrivers);

        for (int j = 0; j < mainDrivers.size(); ++j)        
        {
            images->insert(mainDrivers[j].no, loadHelmet(mainDrivers[j], size));
        }

//        for (int j = )
//        images->append(loadHelmet(SeasonData::getInstance().getTeams()[i].driver1No, size));
//        images->append(loadHelmet(SeasonData::getInstance().getTeams()[i].driver2No, size));
    }

    return images;
}

QPixmap &HelmetsFactory::getHelmet(int no, int size)
{
    if (no < 1)
        return nullPixmap;

    const QMap<int, QPixmap *> *images = loadHelmets(size, false);

    if (images->contains(no))
        return *(*images)[no];

    return nullPixmap;
}

QPixmap *HelmetsFactory::loadHelmet(const LTDriver &driver, int size)
{
    if (!driver.helmet.isNull())            
        return new QPixmap(driver.helmet.scaledToHeight(size, Qt::SmoothTransformation));


    QImage helmet = QImage(":/ui_icons/helmet.png").scaledToHeight(size, Qt::SmoothTransformation);
    QImage helmetMask = QImage(":/ui_icons/helmet_mask.png").scaledToHeight(size, Qt::SmoothTransformation);

    QImage hl(helmet.size(), helmet.format());
    QColor drvColor = ColorsManager::getInstance().getCarColor(driver.no);
    QPainter phl;
    phl.begin(&hl);
    phl.setBrush(QBrush(drvColor));
    phl.drawRect(0, 0, hl.width(), hl.height());
    phl.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    phl.drawImage(0, 0, helmetMask);
    phl.setCompositionMode(QPainter::CompositionMode_SourceOver);
    phl.drawImage(0, 0, helmet);
    phl.end();

    return new QPixmap(QPixmap::fromImage(hl));
}

void HelmetsFactory::reloadHelmets()
{
    QList<int> keys = helmets.keys();

    for (int i = 0; i < keys.size(); ++i)
        loadHelmets(keys[i], true);
}
