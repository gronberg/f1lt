#ifndef DRIVERTRACKERWIDGET_H
#define DRIVERTRACKERWIDGET_H

#include <QSettings>
#include <QTimer>
#include <QWidget>

namespace Ui {
class DriverTrackerWidget;
}

class DriverTrackerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DriverTrackerWidget(QWidget *parent = 0);
    ~DriverTrackerWidget();

    void loadSettings(QSettings *);
    void saveSettings(QSettings *);

    void setTimerInterval(int s=1000)
    {
        timer->setInterval(s / speed);
    }
    void startTimer(int s=1000)
    {
        if (timer->isActive())
            setTimerInterval(s);
        else
            timer->start(s/speed);
    }

    void stopTimer()
    {
        timer->stop();
    }

    void setup();

    void exec();

    void drawTrackerClassification(bool val);



protected:
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_pushButton_clicked();    
    void update();

private:
    Ui::DriverTrackerWidget *ui;
    QTimer *timer;
    int speed;
};

#endif // DRIVERTRACKERWIDGET_H
