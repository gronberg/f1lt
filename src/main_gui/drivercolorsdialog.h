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



#ifndef DRIVERCOLORSDIALOG_H
#define DRIVERCOLORSDIALOG_H

#include <QDialog>
#include <QToolButton>

namespace Ui {
class DriverColorsDialog;
}

class DriverColorsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DriverColorsDialog(QWidget *parent = 0);
    ~DriverColorsDialog();
    int exec();

public slots:
    void onColorButtonClicked();
    void onResetButtonClicked();
    
private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    void setButtonColor(QToolButton *button, QColor color);

    Ui::DriverColorsDialog *ui;
    QList<QColor> colors;
};

#endif // DRIVERCOLORSDIALOG_H
