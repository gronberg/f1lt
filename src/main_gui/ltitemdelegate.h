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
    LTMainItemDelegate(QObject* parent = 0, bool draw = true, int size = 75) : LTItemDelegate(parent), drawCars(draw), thumbnailsSize(size)
    {
    }

    void setDrawCarThumbnails(bool val)
    {
        drawCars = val;
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& rOption, const QModelIndex& rIndex) const;


private:
    bool drawCars;
    int thumbnailsSize;
};

#endif // LTITEMDELEGATE_H
