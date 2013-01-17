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


#include "ltitemdelegate.h"
#include <QDebug>

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

void LTMainItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    LTItemDelegate::paint(painter, option, index);

    if (index.column() == 2 && drawCars)
    {
        const DriverData *dd = static_cast<const LTModel*>(index.model())->getDriverData(index);

        if (dd == 0 || dd->getCarID() <= 0)
            return;

        QPixmap &pix = SeasonData::getInstance().getCarThumbnailsFactory().getCarThumbnail(dd->getNumber(), thumbnailsSize);//getCarImage(dd);

        if (!pix.isNull())
        {
            int x = option.rect.x() + (option.rect.width() - pix.rect().width())/2;
            int y = option.rect.y() + (option.rect.height() - pix.rect().height())/2;
            painter->drawPixmap(x, y, pix.width(), pix.height(), pix);
        }
    }
}
