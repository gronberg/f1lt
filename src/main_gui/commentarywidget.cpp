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

#include "commentarywidget.h"
#include "ui_commentarywidget.h"

#include <QScrollBar>

#include "../core/eventdata.h"

CommentaryWidget::CommentaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentaryWidget)
{
    ui->setupUi(this);
}

CommentaryWidget::~CommentaryWidget()
{
    delete ui;
}

void CommentaryWidget::saveSettings(QSettings &settings)
{
    settings.setValue("ui/commentary_autoscroll", ui->autoScrollBox->isChecked());
}

void CommentaryWidget::loadSettings(QSettings &settings)
{
    ui->autoScrollBox->setChecked(settings.value("ui/commentary_autoscroll", true).toBool());
}

void CommentaryWidget::setFont(const QFont &font)
{
    ui->commentaryEdit->setFont(font);
}

QString CommentaryWidget::getCommentary()
{
    return ui->commentaryEdit->toPlainText();
}

void CommentaryWidget::clear()
{
    ui->commentaryEdit->clear();
}

void CommentaryWidget::update()
{
    int position = ui->commentaryEdit->verticalScrollBar()->sliderPosition();
    ui->commentaryEdit->setText(EventData::getInstance().getCommentary());

    if (ui->autoScrollBox->isChecked())
    {
        QTextCursor c = ui->commentaryEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->commentaryEdit->setTextCursor(c);
    }
    else
    {
        ui->commentaryEdit->verticalScrollBar()->setSliderPosition(position);
    }

}
