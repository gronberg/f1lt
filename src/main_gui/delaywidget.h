#ifndef DELAYWIDGET_H
#define DELAYWIDGET_H

#include <QWidget>

namespace Ui {
class DelayWidget;
}

class DelayWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DelayWidget(QWidget *parent = 0);
    ~DelayWidget();

signals:
    void delayChanged(int);
    
private:
    Ui::DelayWidget *ui;
};

#endif // DELAYWIDGET_H
