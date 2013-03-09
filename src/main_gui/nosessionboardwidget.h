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



#ifndef NOSESSIONBOARDWIDGET_H
#define NOSESSIONBOARDWIDGET_H

#include <QMouseEvent>
#include <QtGui/QWidget>
#include "ui_nosessionboardwidget.h"

class NoSessionBoardWidget : public QWidget
{
    Q_OBJECT

public:
    NoSessionBoardWidget(QWidget *parent = 0);
    ~NoSessionBoardWidget();

    void showSessionBoard(QString msg);
    void showStartupBoard();

protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void connectClicked();
    void playClicked();
    void loadClicked();

private:
    Ui::NoSessionBoardWidgetClass ui;
};

#endif // NOSESSIONBOARDWIDGET_H
