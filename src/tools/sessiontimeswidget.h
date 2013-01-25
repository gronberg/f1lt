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


#ifndef SESSIONTIMESWIDGET_H
#define SESSIONTIMESWIDGET_H

#include <QKeyEvent>
#include <QSettings>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QWidget>

#include "../core/colorsmanager.h"
#include "../core/driverdata.h"
#include "../core/seasondata.h"

namespace Ui {
class SessionTimesWidget;
}

class SessionTimesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SessionTimesWidget(QWidget *parent = 0);
    ~SessionTimesWidget();
    
    void update();
    void handleRaceEvent();
    void handleQualiEvent();
    void handlePracticeEvent();

    void loadDriversList();

    int getNumber(int row);
    QString getName(int row);
    void exec();

    void removeRows(int row);

    void setFont(const QFont &);
    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void saveCheckedArray();
    void restoreCheckedArray();

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void onHeaderClicked(int);
    void onHeaderDoubleClicked(int);

    void on_closeButton_clicked();

    void on_flagButton_clicked();

    void on_driversListWidget_clicked(const QModelIndex &index);

    void on_relativeButton_toggled(bool checked);

    void on_driversListWidget_doubleClicked(const QModelIndex &index);

    void on_top10Button_toggled(bool checked);

private:

    QTableWidgetItem* setItem(int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                     QColor textColor = ColorsManager::getInstance().getColor(LTPackets::DEFAULT), QBrush background = QBrush());

    Ui::SessionTimesWidget *ui;

    DriverData *selectedDriver;
    bool relativeTimes;
    QByteArray checkedArray;
};

#endif // SESSIONTIMESWIDGET_H
