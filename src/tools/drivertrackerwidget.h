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
        interval = s / speed;
        timer->setInterval(interval);
    }

    void setup();

    void exec();

    void drawTrackerClassification(bool val);

public slots:
    void startTimer(int s=1000);

    void pauseTimer(bool pause)
    {
        if (pause)
            timer->stop();

        else
            timer->start(interval);
    }

    void stopTimer()
    {
        timer->stop();
    }


protected:
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_pushButton_clicked();    
    void update();

private:
    Ui::DriverTrackerWidget *ui;
    QTimer *timer;
    int speed;
    int interval;
};

#endif // DRIVERTRACKERWIDGET_H
