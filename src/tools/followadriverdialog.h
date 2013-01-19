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


#ifndef FOLLOWADRIVERDIALOG_H
#define FOLLOWADRIVERDIALOG_H

#include <QDialog>
#include <QList>
#include <QTableWidgetItem>

#include "../core/colorsmanager.h"
#include "../core/eventdata.h"

namespace Ui {
class FollowADriverDialog;
}

class FollowADriverDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FollowADriverDialog(QWidget *parent = 0);
    ~FollowADriverDialog();

    int getNumber();
    void setFont(const QFont &);
    void printDriverInfo(const DriverData &dd);
    void printDataTable(const DriverData &dd, const QList<DriverData*> &drivers);
    void printLapTimesTable(const DriverData &dd, const QList<DriverData*> &drivers);

    QList<DriverData*> getDriversArray(int pos);

    void loadDriversList();
//    void loadCarImages();

    void setupTables();
    void setCurrentDriver(int id);

    void clearData(bool clearDriverList = true);
    void clearRow(int row);

    bool driverInRange(const DriverData &dd);
    void updateButtonsState(const DriverData &dd);

public slots:
    void show(int currentDriverId = 0);
    int exec(int currentDriverId = 0);
    void comboBoxValueChanged(int);
    void updateData();
    void driverUpdated(const DriverData &dd)
    {
        if (driverInRange(dd))
            updateData();

    }

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_dataTableWidget_cellDoubleClicked(int row, int column);

private:

    QTableWidgetItem* setItem(QTableWidget *table, int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                     QColor textColor = ColorsManager::getInstance().getColor(LTPackets::DEFAULT), QBrush background = QBrush());

    Ui::FollowADriverDialog *ui;

    QList<QPixmap> *smallCarImg;
    EventData &eventData;
    int thumbnailsSize;
};

#endif // FOLLOWADRIVERDIALOG_H
