#ifndef DRIVERTRACKERWIDGET_H
#define DRIVERTRACKERWIDGET_H

#include <QSettings>
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

    void update();
    void setup();

    void exec();

protected:
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::DriverTrackerWidget *ui;
};

#endif // DRIVERTRACKERWIDGET_H
