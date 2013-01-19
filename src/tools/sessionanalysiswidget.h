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



#ifndef SESSIONANALYSISWIDGET_H
#define SESSIONANALYSISWIDGET_H

#include <QtGui/QWidget>
#include <QIcon>
#include <QPair>
#include <QSettings>

#include "ui_sessionanalysiswidget.h"

#include "../core/colorsmanager.h"
#include "../core/eventdata.h"

class SessionAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    SessionAnalysisWidget(QWidget *parent = 0);
    ~SessionAnalysisWidget();

    void setupTables();    
    void resizeTables();

protected:

    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);


public slots:
	void selectDriversClicked();
    void exec();

    void setupColors();

    void update(bool repaintCharts = true);
    void gatherData();
    bool driverChecked(int no);
    void setDriverChecked(int no, bool checked);
    void onZoomChanged(int, int, double, double);
    void setupBoxes();
    void setupIcons(const QList<QColor> &colors);
    QIcon getDriverIcon(int no);

    void resetView();

    bool lapInWindow(int i);

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void onSplitterMoved(int pos, int index);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_2_clicked();

    void on_top10pushButton_clicked();

    void on_qualiTabWidget_currentChanged(int index);

private:

    QTableWidgetItem* setItem(QTableWidget *table, int row, int col, QString text = "", Qt::ItemFlags flags = Qt::NoItemFlags, int align = Qt::AlignCenter,
                     QColor textColor = ColorsManager::getInstance().getColor(LTPackets::DEFAULT), QBrush background = QBrush());

    Ui::SessionAnalysisWidgetClass ui;
    QList<QCheckBox*> driverCheckBoxes;
    QList<QLabel*> driverLabels;

    bool selected;
    int first, last;
    double min, max;
    QList<LapData> lapDataArray;
    QList< QPair<int, QIcon> > driverIcons;
    bool top10only;
};

#endif // SESSIONANALYSISWIDGET_H
