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


#include <QtGui/QApplication>
#include <QMessageBox>
#include "main_gui/ltwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok = SeasonData::getInstance().loadSeasonFile();
    if (!ok)
    {
        QMessageBox::critical(0, "Error!", "Could not load input data file (season.dat). Click OK to exit.");
        exit(-1);
    }

    LTWindow w;
    w.show();
    
    return a.exec();
}
