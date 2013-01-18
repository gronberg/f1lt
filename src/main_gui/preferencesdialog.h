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



#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QNetworkProxy>
#include <QSettings>
#include <QToolButton>

#include "drivercolorsdialog.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

    void setFonts(const QFont &f1, const QFont &f2);

    void setMainFont(const QFont &f);
    void setCommentaryFont(const QFont &f);

    QFont getMainFont() { return mainFont; }
    QFont getCommentaryFont() { return commentaryFont; }

    void setSplitterOpaqueResize(bool);
    bool isSplitterOpaqueResize();

    void setDrawCarThumbnails(bool);
    bool drawCarThumbnails();

    void setAutoRecord(bool);
    bool isAutoRecord();

    void setAutoStopRecord(int);
    int getAutoStopRecord();

    void setAutoSaveRecord(int);
    int getAutoSaveRecord();

    void setAutoConnect(bool);
    bool isAutoConnect();

    bool drawTrackerClassification();
    void setDrawTrackerClassification(bool);

    QNetworkProxy getProxy();
    bool proxyEnabled();

signals:
    void driversColorsChanged();

public slots:
    int exec(QSettings *);
    
private slots:
    void on_buttonBox_accepted();

    void on_mainFontButton_clicked();

    void on_commentaryFontButton_clicked();

    void on_autoStopRecordBox_toggled(bool checked);

    void on_proxyCheckBox_toggled(bool checked);

    void on_pushButton_clicked();

    void on_colorWhiteButton_clicked();

    void on_colorCyanButton_clicked();

    void on_colorYellowButton_clicked();

    void on_colorRedButton_clicked();

    void on_colorGreenButton_clicked();

    void on_colorVioletButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_autoSaveRecordBox_toggled(bool checked);

private:

    void setButtonColor(QToolButton *button, QColor color);

    Ui::PreferencesDialog *ui;

    QFont mainFont;
    QFont commentaryFont;
    QSettings *settings;
    QList<QColor> colors;

    DriverColorsDialog *dcDialog;

};

#endif // PREFERENCESDIALOG_H
