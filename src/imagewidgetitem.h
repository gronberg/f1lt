#ifndef IMAGEWIDGETITEM_H
#define IMAGEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QImage>

class ImageWidgetItem : public QTableWidgetItem
{
public:
    explicit ImageWidgetItem(QObject *parent = 0);
    ImageWidgetItem(QImage img, QObject *parent = 0);

    QVariant data(int role) const
    {
        if (role == Qt::DecorationRole || role == Qt::TextAlignmentRole)
            return img;
    }
    QBrush foreground()
    {
        return QBrush(img);
    }

private:
    QImage img;
    
};

#endif // IMAGEWIDGETITEM_H
