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

//====================================================

HelmetsFactory::~HelmetsFactory()
{
    QMap<int, QList<QPixmap*> >::Iterator iter = helmets.begin();
    while (iter != helmets.end())
    {
        QList<QPixmap*> *images = &iter.value();

        while (!images->isEmpty())
        {
            delete images->takeFirst();
        }
        ++iter;
    }
}

QList<QPixmap*> *HelmetsFactory::loadHelmets(int size, bool clear)
{
    QList<QPixmap*> *images = &helmets[size];

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
        images->append(loadHelmet(SeasonData::getInstance().getTeams()[i].driver1No, size));
        images->append(loadHelmet(SeasonData::getInstance().getTeams()[i].driver2No, size));
    }

    return images;
}

QPixmap &HelmetsFactory::getHelmet(int no, int size)
{
    if (no < 1)
        return nullPixmap;

    const QList<QPixmap*> *images = loadHelmets(size, false);

    int idx = (no > 13 ? no-2 : no-1);

    if (idx >= 0 && idx < images->size())
        return *(*images)[idx];

    return nullPixmap;
}

QPixmap *HelmetsFactory::loadHelmet(int no, int size)
{
    QImage helmet = QImage(":/ui_icons/helmet.png").scaledToHeight(size, Qt::SmoothTransformation);
    QImage helmetMask = QImage(":/ui_icons/helmet_mask.png").scaledToHeight(size, Qt::SmoothTransformation);

    QImage hl(helmet.size(), helmet.format());
    QColor drvColor = SeasonData::getInstance().getCarColor(no);
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
