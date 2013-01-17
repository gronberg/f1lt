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



#ifndef DRIVERRECORDSDIALOG_H
#define DRIVERRECORDSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "../core/trackrecords.h"

namespace Ui {
class DriverRecordsDialog;
}

class DriverRecordsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DriverRecordsDialog(QWidget *parent = 0);
    ~DriverRecordsDialog();

    void exec(const TrackWeekendRecords &records, QString trackName);

    void loadRecords();
    void loadRecords(const TrackWeekendRecords &records, QString trackName);

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);
    
private slots:
    void on_comboBox_currentIndexChanged(int);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::DriverRecordsDialog *ui;
    TrackWeekendRecords driverRecords;
    QString trackName;
};

#endif // DRIVERRECORDSDIALOG_H
