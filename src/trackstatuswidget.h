#ifndef TRACKSTATUSWIDGET_H
#define TRACKSTATUSWIDGET_H

#include <QPixmap>
#include <QResizeEvent>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "eventdata.h"

class TrackStatusWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TrackStatusWidget(QWidget *parent = 0);
    void setupItems();
    void setFont(const QFont &);
    
signals:
    
public slots:
    void updateTrackStatus(const EventData &eventData);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QTableWidgetItem* setItem(int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                 QColor textColor = LTData::colors[LTData::DEFAULT], QBrush background = QBrush());

    QPixmap icons[5];

    EventData &eventData;
    
};

#endif // TRACKSTATUSWIDGET_H
