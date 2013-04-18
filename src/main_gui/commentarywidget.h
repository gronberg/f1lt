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

#ifndef COMMENTARYWIDGET_H
#define COMMENTARYWIDGET_H

#include <QSettings>
#include <QWidget>

namespace Ui {
class CommentaryWidget;
}

class CommentaryWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CommentaryWidget(QWidget *parent = 0);
    ~CommentaryWidget();

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);

    QString getCommentary();
    void clear();

public slots:
    void update();
    
private:
    Ui::CommentaryWidget *ui;
};

#endif // COMMENTARYWIDGET_H
