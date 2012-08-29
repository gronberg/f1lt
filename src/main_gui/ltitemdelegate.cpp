#include "ltitemdelegate.h"

#include <QPainter>

#include "models/ltmodel.h"

void LTItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);

    QColor color = index.data(Qt::ForegroundRole).value<QColor>();
    if (color.isValid())
    {
        if (color != option.palette.color(QPalette::WindowText))
            viewOption.palette.setColor(QPalette::HighlightedText, color);
    }
    QItemDelegate::paint(painter, viewOption, index);
}

//=========================================

QPixmap LTMainItemDelegate::getCarImage(const DriverData &dd) const
{
    if (dd.getNumber() > 0 && dd.getNumber() < 26 && carImg)
    {
        int idx = (dd.getNumber() > 13 ? dd.getNumber()-2 : dd.getNumber()-1) / 2;

        return (*carImg)[idx];
    }
    return QPixmap();
}

void LTMainItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    LTItemDelegate::paint(painter, option, index);

    if (index.column() == 2 && drawCars)
    {
        DriverData dd = static_cast<const LTModel*>(index.model())->getDriverData(index);
        QPixmap pix = getCarImage(dd);

        int x = option.rect.x() + (option.rect.width() - pix.rect().width())/2;
        int y = option.rect.y() + (option.rect.height() - pix.rect().height())/2;
        painter->drawPixmap(x, y, pix.width(), pix.height(), pix);
    }
}
