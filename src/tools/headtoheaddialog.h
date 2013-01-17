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


#ifndef HEADTOHEADDIALOG_H
#define HEADTOHEADDIALOG_H

#include <QDialog>
#include <QList>
#include "../core/seasondata.h"
#include "../charts/lapcompchart.h"

#include <QComboBox>

namespace Ui {
class HeadToHeadDialog;
}

class HeadToHeadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HeadToHeadDialog(bool rev = false, QWidget *parent = 0);
    ~HeadToHeadDialog();

    void setFont(const QFont &);

    void setReversedOrder(bool rev) { reversedOrder = rev; }
    bool isReversedOrder() { return reversedOrder; }

    void loadDriversList();

    int getNumber(int);
    void setCurrentDriver(int id);  

public slots:
    int exec(int currentDriverId = 0);
    void show(int currentDriverId = 0);
    void comboBoxValueChanged(int);
    void updateData();
    void updateCharts();
    void driverUpdated(const DriverData &dd)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (dd.getCarID() == eventData.getDriverId(getNumber(i)))
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

private slots:
    void on_pushButton_clicked();

private:
    Ui::HeadToHeadDialog *ui;

    QComboBox *comboBox[2];
    LapCompChart *lapCompChart;
    GapCompChart *gapCompChart;
    PosCompChart *posCompChart;
    QList<QPixmap> *smallCarImg;

    bool reversedOrder;

    EventData &eventData;
    int thumbnailsSize;
};

#endif // HEADTOHEADDIALOG_H
