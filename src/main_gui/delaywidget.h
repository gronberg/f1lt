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
    void delayChanged(int previousDelay, int delay);

public slots:
    void synchronizingTimer(bool);
    
private slots:

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::DelayWidget *ui;
    int delay;
};

#endif // DELAYWIDGET_H
