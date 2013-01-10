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
