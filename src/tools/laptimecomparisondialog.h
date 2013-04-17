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



#ifndef LAPTIMECOMPARISONDIALOG_H
#define LAPTIMECOMPARISONDIALOG_H

#include <QComboBox>
#include <QDialog>

#include <QList>
#include "../core/seasondata.h"
#include "../charts/lapcompchart.h"
#include "../net/packetparser.h"

namespace Ui {
class LapTimeComparisonDialog;
}

class LapTimeComparisonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LapTimeComparisonDialog(QWidget *parent = 0);
    ~LapTimeComparisonDialog();
    
    void setFont(const QFont &);

    void loadDriversList();

    int getNumber(int);
    void setCurrentDriver(int id);

public slots:
    int exec(int currentCarId = 0);
    void show(int currentCarId = 0);
    void comboBoxValueChanged(int);
    void updateData();
    void updateCharts();
    void driverUpdated(const DriverData &dd)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (dd.getCarID() == eventData.getDriverId(getNumber(i)))
            {
                updateData();
                updateCharts();
                return;
            }
        }
    }

    void updateData(const DataUpdates &dataUpdates)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (dataUpdates.driverIds.contains(eventData.getDriverId(getNumber(i))))
            {
                updateData();
                updateCharts();
                return;
            }
        }
    }

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    Ui::LapTimeComparisonDialog *ui;

    QComboBox *comboBox[4];
    LapCompChart *lapCompChart;
    QColor color[4];
    QList<QPixmap> *smallCarImg;

    EventData &eventData;
    int thumbnailsSize;
};

#endif // LAPTIMECOMPARISONDIALOG_H
