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

#include <QMouseEvent>

#include "driverradar.h"
#include <typeinfo>

#include "../core/colorsmanager.h"

DriverRadar::DriverRadar(QWidget *parent) :
    QWidget(parent), radarX(0), radarY(0), radarR(0.0), radarPitR(0.0), radarLappedR(0.0), selectedDriver(-2), dti(0)
{
    loadDriversList();
    QWidget::setMinimumSize(50, 200);
}

DriverRadar::~DriverRadar()
{
    for (int i = 0; i < drp.size(); ++i)
        delete drp[i];
}

void DriverRadar::checkSetupCorrect(int speed)
{
    if (drp.isEmpty() || selectedDriver == -2)
    {        
        setupDrivers(speed);
    }
}

void DriverRadar::loadDriversList()
{
    if (dti == 0)
    {
        dti = new DriverTrackerInfo(this);
        dti->setVisible(false);
    }

    for (int i = 0; i < drp.size(); ++i)
        delete drp[i];

    drp.resize(EventData::getInstance().getDriversData().size());

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i] = new DriverRadarPositioner(&EventData::getInstance().getDriversData()[i]);
    }
}

void DriverRadar::setupDrivers(int speed)
{
    if (selectedDriver != -2)
        selectedDriver = -1;
    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i]->setStartupPosition();
        drp[i]->setExcluded(false);
        drp[i]->setSpeed(speed);
    }

    resizeEvent(0);
    repaint();

    if (dti)
    {
        dti->setup();
        dti->repaint();
        dti->setVisible(false);
//        setMinimumSize(dti->minimumSize());
    }
}

void DriverRadar::update()
{
    if (selectedDriver == -2)
        selectedDriver = -1;

    for (int i = 0; i < drp.size(); ++i)
        drp[i]->update();

    repaint();

    if (dti)
        dti->repaint();
}

void DriverRadar::excludeDriver(int id, bool exclude)
{
    for (int i = 0; i < drp.size(); ++i)
    {
        if (drp[i]->getDriverId() == id)
            drp[i]->setExcluded(exclude);
    }
    repaint();

    if (dti)
        dti->repaint();
}

void DriverRadar::resizeEvent(QResizeEvent *)
{
    QSize size;

    if (dti)
    {
        size = dti->minimumSize();

        qDebug() << size.height();
    }
    radarR = width() < (height() - 30 - size.height()) ? (double)width() / 2.0 - 20.0 : (double)(height() - 30 - size.height()) / 2.0 - 20.0;
    radarX = width()/2;
    radarY = radarR + 30;

    radarLappedR = radarR * 0.75;
    radarPitR = radarR * 0.5;

    if (dti)
    {
        dti->setGeometry(0, radarR*2 + 60, width(), height() - (radarR*2 + 60));
        qDebug() << radarR*2 + 60 << height() - (radarR*2 + 60);
    }

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i]->setRadarCoords(radarX, radarY, radarR, radarPitR, radarLappedR);
    }

}

void DriverRadar::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QBrush(QColor(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND))));
    p.drawRect(0, 0, width(), height());

    QPainterPath path;

    QPen pen(QColor(255, 255, 255), 5);

    if (EventData::getInstance().getFlagStatus() == LTPackets::SAFETY_CAR_DEPLOYED)
        pen.setColor(ColorsManager::getInstance().getDefaultColor(LTPackets::YELLOW));

//    path.addEllipse(QPoint(radarX, radarY), radarR, radarR);
    p.setBrush(QBrush());
    p.setPen(pen);
    p.drawEllipse(QPoint(radarX, radarY), (int)radarR, (int)radarR);
//    p.drawPath(path);

//    path.addEllipse(QPoint(radarX, radarY), radarLappedR, radarLappedR);
    pen.setWidth(3);
    p.setPen(pen);
    p.drawEllipse(QPoint(radarX, radarY), (int)radarLappedR, (int)radarLappedR);
//    p.drawPath(path);

    pen.setColor(QColor(255, 255, 255));
//    path.addEllipse(QPoint(radarX, radarY), radarPitR, radarPitR);
    pen.setWidth(5);
    p.setPen(pen);
//    p.drawEllipse(QPoint(radarX, radarY), radarPitR, radarPitR);
    int x = radarX - radarPitR;
    int y = radarY - radarPitR;
    int w = radarX + radarPitR - x;
    int h = radarY + radarPitR - y;
    p.drawArc(x, y, w, h, 270*16, 180*16);
//    p.drawPath(path);

    pen.setWidth(2);
    p.setPen(pen);
    p.drawLine(radarX, radarY - radarR - 10, radarX, radarY - radarLappedR + 10);

//    p.drawPixmap(radarX - trackMap.width()/2, radarY - trackMap.height()/2, trackMap);

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

    p.end();
}

void DriverRadar::mousePressEvent(QMouseEvent *event)
{
    bool found = false;
    for (int i = 0; i < drp.size(); ++i)
    {
        if (drp[i]->isSelected(event->pos()))
        {
            if (selectedDriver != drp[i]->getDriverId())
                selectedDriver = drp[i]->getDriverId();
            else
                selectedDriver = -1;

            found = true;
            break;
        }
    }
    if (!found)
        selectedDriver = -1;

    selectDriver(selectedDriver);
    emit driverSelected(selectedDriver);
    repaint();
}
