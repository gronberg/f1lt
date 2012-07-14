#ifndef EVENTSTATUSWIDGET_H
#define EVENTSTATUSWIDGET_H

#include <QWidget>

namespace Ui {
class EventStatusWidget;
}

class EventStatusWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EventStatusWidget(QWidget *parent = 0);
    ~EventStatusWidget();

    void updateEventStatus();

    void setFont(const QFont &);
    
private:
    Ui::EventStatusWidget *ui;
    QPixmap icons[8];
};

#endif // EVENTSTATUSWIDGET_H
