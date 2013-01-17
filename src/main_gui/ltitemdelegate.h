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
