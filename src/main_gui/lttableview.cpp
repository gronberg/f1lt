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

        if (ltModel->indexInDriverRowsData(index))
            QTableView::mousePressEvent(event);

        if (index.row() == 0)
            emit headerClicked(index.column());
    }
}
