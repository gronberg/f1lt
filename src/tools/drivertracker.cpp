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


#include "drivertracker.h"
#include "drivertrackerpositioner.h"

#include <QMouseEvent>

#include "../core/colorsmanager.h"

DriverTracker::DriverTracker(QWidget *parent) : DriverRadar(parent), drawClassification(true)
{
    loadDriversList();

    label = QPixmap(":/ui_icons/label.png");
    selectedLabel = QPixmap(":/ui_icons/label-sel.png");
}

void DriverTracker::setMinimumSize()
{
    trackMap = EventData::getInstance().getEventInfo().trackImg;
    QSize size = trackMap.size();
    int classificationWidth = drawClassification ? label.width() : 0;
    size.setWidth(size.width() + classificationWidth + 10);

    size.setHeight(size.height() + 120);// > EventData::getInstance().getDriversData().size() * 20 + 20 ? size.height() : EventData::getInstance().getDriversData().size() * 20 + 20);
    QWidget::setMinimumSize(size);
}

void DriverTracker::setupDrivers(int speed)
{
    if (selectedDriver != -2)
        selectedDriver = -1;

    excludedDrivers.clear();

    setMinimumSize();

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i]->setStartupPosition();
        drp[i]->setExcluded(false);
    }

    for (int i = 0; i < drp.size(); ++i)
    {
        DriverTrackerPositioner *dtp = static_cast<DriverTrackerPositioner*>(drp[i]);

        int labWidth = label.width() + 10;
        int px = labWidth + (width() - labWidth - trackMap.width())/2;
        int py = (height() - trackMap.height()-50)/2;
        int pitX = 15;
        int pitY = height()-60;
        int pitW = width() - 30;
        int pitH = 50;
        dtp->setMapCoords(px, py, pitX, pitY, pitW, pitH);
        dtp->setSpeed(speed);
    }

    repaint();
}

void DriverTracker::loadDriversList()
{
    dti = 0;

    for (int i = 0; i < drp.size(); ++i)
        delete drp[i];

    drp.resize(EventData::getInstance().getDriversData().size());

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i] = new DriverTrackerPositioner(&EventData::getInstance().getDriversData()[i]);
    }
}


void DriverTracker::resizeEvent(QResizeEvent *)
{
    for (int i = 0; i < drp.size(); ++i)
    {
        DriverTrackerPositioner *dtp = static_cast<DriverTrackerPositioner*>(drp[i]);

        int labWidth = drawClassification ? label.width() + 10 : 10;

        int px = labWidth + (width() - labWidth - trackMap.width())/2;
        int py = (height() - trackMap.height()-50)/2;
        int pitX = 15;
        int pitY = height()-60;
        int pitW = width() - 30;
        int pitH = 50;
        dtp->setMapCoords(px, py, pitX, pitY, pitW, pitH);
    }
}

void DriverTracker::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QBrush(QColor(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND))));
    p.drawRect(0, 0, width(), height());

    int labWidth = drawClassification ? label.width() + 10 : 10;
    int px = labWidth + (width() - labWidth - trackMap.width())/2;
    int py = (height() - trackMap.height()-50)/2;

    QPoint point(px, py);
    p.drawPixmap(point, trackMap);

    p.setPen(QColor(255, 0, 0));
    p.drawRect(15, height()-60, width()-30, 50);

    int sel = -1;
    for (int i = drp.size() - 1; i >= 0; --i)
    {
        if (drp[i]->getDriverId() != selectedDriver)
            drp[i]->paint(&p);

        else
            sel = i;
    }
    if (sel >= 0)
        drp[sel]->paint(&p, true);

    if (drawClassification)
        paintClassification(p);

    p.end();
}

void DriverTracker::paintClassification(QPainter &p)
{
    for (int i = 0; i < EventData::getInstance().getDriversData().size(); ++i)
    {
        DriverData *dd = EventData::getInstance().getDriverDataByPosPtr(i+1);
        if (dd != 0)
        {
            QString number = QString::number(dd->getNumber());

            if (dd->getNumber() < 10)
                number = "  " + number;

            QString txt = SeasonData::getInstance().getDriverShortName(dd->getDriverName());

            p.setFont(QFont("Arial", 10, 100));
            QColor drvColor = ColorsManager::getInstance().getCarColor(dd->getNumber());
            p.setBrush(drvColor);

            if (isExcluded(dd->getCarID()))
                p.setBrush(QColor(80, 80, 80));

            int x = 5;
            int y = 10 + i * 20;

            int numX = x + 35;
            int numY = y + p.fontMetrics().height()/2 + 8;

            p.setPen(QColor(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND)));
            p.drawRect(x, y, 70, 20);

            if (dd->getCarID() == selectedDriver)
                p.drawPixmap(x, y, selectedLabel);
            else
                p.drawPixmap(x, y, label);

            p.drawText(numX, numY, number);

            p.setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE));

            if (isExcluded(dd->getCarID()))
                p.setPen(QColor(80, 80, 80));

            p.drawText(x+60, numY, txt);

//            if (!dd->isRetired())
            {
                p.setPen(QColor(0,0,0));

                if (dd->isInPits())
                    p.setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::PIT));
                QString pos = QString::number(i+1);

                if (!dd->isRetired())
                {
                    if (i+1 < 10)
                        pos = "  " + pos;
                    p.drawText(x+10, numY, pos);
                }

                QString gap = "";
                if (i > 0)
                {
                    if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
                    {
                        DriverData *first = EventData::getInstance().getDriverDataByPosPtr(1);
                        if (first)
                        {
                            int quali = EventData::getInstance().getQualiPeriod();
                            gap = DriverData::calculateGap(dd->getQualiTime(quali), first->getQualiTime(quali));

                            if (gap != "")
                                gap = "+" + gap;
                        }
                    }
                    else
                    {
                        gap = dd->getLastLap().getGap();
                        if (gap != "")
                            gap = "+" + gap;

                        if (!gap.contains("L") && gap.size() < 5)
                            gap = "  " + gap;
                    }
                }
                else
                {
                    if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
                    {
                        int laps = EventData::getInstance().getCompletedLaps() + 1;
                        if (laps > EventData::getInstance().getEventInfo().laps)
                            laps = EventData::getInstance().getEventInfo().laps;
                        gap = QString("LAP %1").arg(laps);
                    }

                    else if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
                        gap = dd->getLastLap().getTime().toString();

                    else
                        gap = dd->getQualiTime(EventData::getInstance().getQualiPeriod()).toString();
                }
                p.setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::YELLOW)));

                if (isExcluded(dd->getCarID()))
                    p.setPen(QColor(80, 80, 80));

                p.drawText(x+105, numY, gap);
            }
        }
    }
}

void DriverTracker::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton &&
        event->pos().x() >= 5 && event->pos().x() <= 5 + label.width() &&
        event->pos().y() >= 10 && event->pos().y() <= 10 + 20 * EventData::getInstance().getDriversData().size())
    {
        int pos = (event->pos().y() - 10) / 20 + 1;
        DriverData *dd = EventData::getInstance().getDriverDataByPosPtr(pos);

        if (dd != 0 && !isExcluded(dd->getCarID()))
        {
            if (dd->getCarID() == selectedDriver)
                selectedDriver = -1;

            else
                selectedDriver = dd->getCarID();
        }
        emit driverSelected(selectedDriver);
    }
    repaint();
}

void DriverTracker::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton &&
        event->pos().x() >= 5 && event->pos().x() <= 5 + label.width() &&
        event->pos().y() >= 10 && event->pos().y() <= 10 + 20 * EventData::getInstance().getDriversData().size())
    {
        int pos = (event->pos().y() - 10) / 20 + 1;
        DriverData *dd = EventData::getInstance().getDriverDataByPosPtr(pos);

        if (dd != 0)
        {
            if (dd->getCarID() == selectedDriver)
                selectedDriver = -1;

            bool found = false;
            for (int i = 0; i < excludedDrivers.size(); ++i)
            {
                if (dd->getCarID() == excludedDrivers[i])
                {
                    excludedDrivers.takeAt(i);

                    excludeDriver(dd->getCarID(), false);
                    emit driverExcluded(dd->getCarID(), false);

                    found = true;
                    break;
                }
            }

            if (!found)
            {
                excludedDrivers.append(dd->getCarID());
                emit driverExcluded(dd->getCarID(), true);

                excludeDriver(dd->getCarID(), true);
            }
        }
    }
    repaint();
}

bool DriverTracker::isExcluded(int id)
{
    for (int i = 0; i < excludedDrivers.size(); ++i)
    {
        if (excludedDrivers[i] == id)
            return true;
    }

    return false;
}
