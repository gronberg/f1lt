#ifndef TRACKSTATUSWIDGET_H
#define TRACKSTATUSWIDGET_H

#include <QPixmap>
#include <QResizeEvent>
#include <QString>
#include <QTableWidget>
//#include <QTextEdit>

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
    QPixmap icons[5];

    EventData &eventData;
    
};

#endif // TRACKSTATUSWIDGET_H
