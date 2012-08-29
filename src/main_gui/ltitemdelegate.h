#ifndef LTITEMDELEGATE_H
#define LTITEMDELEGATE_H

#include <QItemDelegate>

#include "../core/eventdata.h"

class LTItemDelegate: public QItemDelegate
{
public:
    LTItemDelegate(QObject* parent = 0) : QItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& rOption, const QModelIndex& rIndex) const;
};

class LTMainItemDelegate : public LTItemDelegate
{
public:
    LTMainItemDelegate(QObject* parent = 0, QList<QPixmap> *img = 0, bool draw = true) : LTItemDelegate(parent), carImg(img), drawCars(draw)
    {
    }

    void setDrawCarThumbnails(bool val)
    {
        drawCars = val;
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& rOption, const QModelIndex& rIndex) const;

    QPixmap getCarImage(const DriverData &dd) const;

private:
    QList<QPixmap> *carImg;
    bool drawCars;
};

#endif // LTITEMDELEGATE_H
