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


#ifndef TRACKRECORDSDIALOG_H
#define TRACKRECORDSDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QSettings>

#include "driverrecordsdialog.h"

namespace Ui {
class TrackRecordsDialog;
}

class TrackRecordsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TrackRecordsDialog(QWidget *parent = 0);
    ~TrackRecordsDialog();

    void loadTrackRecords(int year = 0);
    void updateRecordsLabels();

    void exec();

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);

    void update();

    
private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

    void on_trackVersionBox_activated(const QString &arg1);

    void on_yearBox_currentIndexChanged(const QString &arg1);

private:
    Ui::TrackRecordsDialog *ui;

    int currentIndex;
    TrackVersion *currentTV;
    TrackWeekendRecords *currentTWR;

    DriverRecordsDialog *drDialog;
};

#endif // TRACKRECORDSDIALOG_H
