#include "imagewidgetitem.h"

ImageWidgetItem::ImageWidgetItem(QObject *parent) :
    QTableWidgetItem(QTableWidgetItem::UserType)
{
    setTextAlignment(Qt::AlignCenter);
}

ImageWidgetItem::ImageWidgetItem(QImage i,QObject *parent) : QTableWidgetItem(QTableWidgetItem::UserType), img(i)
{
    setTextAlignment(Qt::AlignCenter);
}
