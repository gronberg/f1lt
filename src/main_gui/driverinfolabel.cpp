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


#include "driverinfolabel.h"
#include <QPainter>

#include <QDebug>

DriverInfoLabel::DriverInfoLabel(QWidget *parent) :
    QWidget(parent), driverData(0)
{
    backgroundPixmap = QPixmap(":/ui_icons/label-small.png");
}

QSize DriverInfoLabel::sizeHint()
{
    return backgroundPixmap.size();
}

QSize DriverInfoLabel::minimumSize()
{
    return backgroundPixmap.size();
}

void DriverInfoLabel::update()
{
    if (height() < sizeHint().height())
    {
        QSize size = backgroundPixmap.size();
        size.setHeight(size.height() + 10);
        setMinimumSize(size);
        updateGeometry();
    }
    repaint();
}

void DriverInfoLabel::paintEvent(QPaintEvent *)
{
    if (driverData == 0)
        return;

    QPainter p;
    p.begin(this);
    int x = (width() - backgroundPixmap.width())/2;

    QColor color = /*QColor(170, 170, 170);*/SeasonData::getInstance().getCarColor(driverData->getNumber());
    int numX = x + 105;
    int numY = 1;
    p.setBrush(color);
    p.drawRect(numX, numY, 30, 20);
    numX = x + 20;
    numY = 30;

    p.drawPixmap(x, 0, backgroundPixmap);

    QString txt = QString::number(driverData->getPosition());

    if (driverData->getPosition() < 10)
        txt = " " + txt;

    p.setFont(QFont("Arial", 20, 100));
    p.setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::BACKGROUND)));

    if (driverData->isInPits())
        p.setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::PIT)));

    p.drawText(numX, numY, txt);

    txt = driverData->getDriverName();
    p.setPen(QColor(SeasonData::getInstance().getDefaultColor(LTPackets::WHITE)));
    p.setFont(QFont("Arial", 10, 100));
    numX = x + 140;
    numY = 15;
    p.drawText(numX, numY, txt);

    numX = x + 120;
    numY = 35;
    txt = SeasonData::getInstance().getTeamName(driverData->getNumber());
    p.drawText(numX, numY, txt);



    p.setPen(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND)));
    txt = QString::number(driverData->getNumber());
    if (driverData->getNumber() < 10)
        txt = " " + txt;
    numX = x + 115;
    numY = 15;
    p.drawText(numX, numY, txt);


    numX = x + 63;
    numY = 4;
    p.drawPixmap(numX, numY, SeasonData::getInstance().getHelmetsFactory().getHelmet(driverData->getNumber(), 30));
}
