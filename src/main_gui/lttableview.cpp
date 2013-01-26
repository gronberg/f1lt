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


#include "lttableview.h"
#include "models/ltmodel.h"

#include <QKeyEvent>

LTTableView::LTTableView(QWidget *parent) :
    QTableView(parent)
{
}

void LTTableView::updateColumn(QModelIndex index)
{
    LTModel *ltModel = static_cast<LTModel*>(model());
    for (int i = ltModel->firstRow(); i <= ltModel->lastRow(); ++i)
    {
        QModelIndex idx = ltModel->index(i, index.column());
        update(idx);
    }
}

void LTTableView::keyPressEvent(QKeyEvent *event)
{
    QModelIndex index = currentIndex();
    LTModel *ltModel = static_cast<LTModel*>(model());

    if (event->key() == Qt::Key_Up && index.row() > ltModel->firstRow())
    {
        QModelIndex newIndex = ltModel->index(index.row()-1, 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
    if (event->key() == Qt::Key_Down && index.row() < ltModel->lastRow())
    {
        QModelIndex newIndex = ltModel->index(index.row()+1, 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
    if (event->key() == Qt::Key_Return)
    {
        QTableView::keyPressEvent(event);
        emit doubleClicked(index);
    }
    if (event->key() == Qt::Key_Home)
    {
        QModelIndex newIndex = ltModel->index(ltModel->firstRow(), 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
    if (event->key() == Qt::Key_End)
    {
        QModelIndex newIndex = ltModel->index(ltModel->lastRow(), 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }

    if (event->key() == Qt::Key_PageDown)
    {
        int row = index.row() + ltModel->driverRows()/2;
        if (row > ltModel->lastRow())
            row = ltModel->lastRow();

        QModelIndex newIndex = ltModel->index(row, 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
    if (event->key() == Qt::Key_PageUp)
    {
        int row = index.row() - ltModel->driverRows()/2;
        if (row < ltModel->firstRow())
            row = ltModel->firstRow();

        QModelIndex newIndex = ltModel->index(row, 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
}

void LTTableView::resizeEvent(QResizeEvent *)
{

}

void LTTableView::wheelEvent(QWheelEvent *event)
{
    QModelIndex index = currentIndex();
    LTModel *ltModel = static_cast<LTModel*>(model());

    if (event->delta() < 0 && index.row() < ltModel->lastRow())
    {
        QModelIndex newIndex = ltModel->index(index.row()+1, 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
    else if (event->delta() > 0 && index.row() > ltModel->firstRow())
    {
        QModelIndex newIndex = ltModel->index(index.row()-1, 0);
        setCurrentIndex(newIndex);
        emit clicked(newIndex);
    }
    QTableView::wheelEvent(event);
}

void LTTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        LTModel *ltModel = static_cast<LTModel*>(model());

        if (ltModel && ltModel->indexInDriverRowsData(index))
            QTableView::mousePressEvent(event);

        if (index.row() == 0)
            emit headerClicked(index.column());
    }
}
