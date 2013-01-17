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


#include "nosessionboardwidget.h"
#include "../core/seasondata.h"

#include <QDateTime>

NoSessionBoardWidget::NoSessionBoardWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

NoSessionBoardWidget::~NoSessionBoardWidget()
{

}

void NoSessionBoardWidget::showSessionBoard(QString msg)
{
	ui.noSessionWidget->setVisible(true);
	ui.startupWidget->setVisible(false);

	int year = 2000 + msg.left(2).toInt();
	int month = msg.mid(2, 2).toInt();
	int day = msg.mid(4, 2).toInt();
	int hour = msg.mid(6, 2).toInt() + 1;

    QDateTime dateTime(QDate(year, month, day), QTime(hour, 0), Qt::UTC);
    dateTime = dateTime.toLocalTime();

    LTEvent event = SeasonData::getInstance().getCurrentEvent();
	QString str = event.eventName +
            "\n\n" + dateTime.toString("hh:mm, dddd dd MMMM yyyy");
            /*QString::number(year) + "." + (month < 10 ? "0" + QString::number(month) : QString::number(month)) + "." +
        (day < 10 ? "0" + QString::number(day) : QString::number(day)) + " - " + QString::number(hour) + ":00 GMT";*/

	ui.sessionLabel->setText(str);
    QPixmap pix = event.trackImg.height() > 600 ? event.trackImg.scaledToHeight(600, Qt::SmoothTransformation) : event.trackImg;
    ui.trackMapLabel->setPixmap(pix);

}

void NoSessionBoardWidget::showStartupBoard()
{
	ui.noSessionWidget->setVisible(false);
	ui.startupWidget->setVisible(true);

    LTEvent event = SeasonData::getInstance().getNextEvent();
	QString str = event.eventName;

	ui.sessionLabel2->setText(str);
    QPixmap pix = event.trackImg.height() > 600 ? event.trackImg.scaledToHeight(600, Qt::SmoothTransformation) : event.trackImg;
    ui.trackMapLabel->setPixmap(pix);

}
