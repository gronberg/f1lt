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



#ifndef LTWIDGET_H
#define LTWIDGET_H

#include <QItemDelegate>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QWidget>

#include "../core/eventdata.h"
#include "ltitemdelegate.h"
#include "models/ltmodel.h"




namespace Ui {
class LTWidget;
}

class LTWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LTWidget(QWidget *parent = 0);
    ~LTWidget();

    void updateLT();

    virtual void setFont(const QFont &font);


//    void updateRaceEvent(int ddIdx = -1);
//    void updateDriver(int driverIdx);
//    void updatePracticeEvent(int ddIdx = -1);
//    void updateQualiEvent(int ddIdx = -1);

    void loadCarImages();

    void setDrawCarThumbnails(bool val)
    {
        drawCarThumbnails = val;
        itemDelegate->setDrawCarThumbnails(val);
        resizeEvent(0);
        updateLT();
    }

    void clearData();

    void clearModelsData()
    {
        if (ltModel != 0)
            ltModel->clearData();
    }
    int getCurrentDriverId() { return currDriverId; }


protected:
    virtual void resizeEvent(QResizeEvent *);
    
signals:
    void driverSelected(int id);
    void driverDoubleClicked(int id);

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_headerClicked(int column);

private:
    Ui::LTWidget *ui;

    EventData &eventData;
    LTPackets::EventType eventType;

    int currDriverId;

    bool drawCarThumbnails;
    int showDiff;	//1 - time (best, q1), 2 - q2, 3 - q3, 4 - interval

    LTModel *ltModel;
    LTMainItemDelegate *itemDelegate;
    int thumbnailsSize;
};

#endif // LTWIDGET_H
