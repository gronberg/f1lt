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



#ifndef LTWINDOW_H
#define LTWINDOW_H

#include <QMainWindow>

#include <QByteArray>
#include <QIcon>
#include <QList>
#include <QProgressDialog>
#include <QSettings>

#include "aboutdialog.h"
#include "../net/datastreamreader.h"
#include "../player/eventplayer.h"
#include "../player/eventrecorder.h"
#include "../tools/drivertrackerwidget.h"
#include "../tools/followadriverdialog.h"
#include "../tools/headtoheaddialog.h"
#include "../tools/laptimecomparisondialog.h"
#include "../tools/ltfilesmanagerdialog.h"
#include "../tools/trackrecordsdialog.h"
#include "logindialog.h"
#include "preferencesdialog.h"
#include "../tools/sessionanalysiswidget.h"
#include "../tools/sessiontimeswidget.h"
#include "../core/sessiontimer.h"
#include "delaywidget.h"
#include "updatescheckerdialog.h"


namespace Ui {
class LTWindow;
}

class LTWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LTWindow(QWidget *parent = 0);
    ~LTWindow();

    void loadSettings();
    void saveSettings();
    void setFonts(const QFont &mainFont, const QFont &commentaryFont);

    void connectToServer();
    void startRecording(bool autoRecord = false);
    void stopRecording(bool autoStop = false);

    void setupDialogs();

public slots:
    bool close();

    void eventPlayerOpenFile(QString);
    void eventPlayerPlayClicked(int);
    void eventPlayerPauseClicked();
    void eventPlayerRewindToStartClicked();
    void eventPlayerForwardToEndClicked();
    void eventPlayerRewindClicked();
    void eventPlayerStopClicked(bool connect = true);

    void ltWidgetDriverSelected(int id);

private slots:
    void tryAuthorize();
    void authorized(QString);
    void authorizationError();
    void error(QAbstractSocket::SocketError);
    void error(QNetworkReply::NetworkError);
    void on_actionConnect_triggered();

    void eventDataChanged(const DataUpdates &);
    void driverDataChanged(int, const DataUpdates &);
    void dataChanged(const DataUpdates &);
    void sessionStarted();
    void showNoSessionBoard(bool, QString);
    void updateWeather();

    void onNewVersionAvailable();

    void timeout();
//    void on_tableWidget_cellDoubleClicked(int row, int column);
//    void on_tableWidget_cellClicked(int row, int);
    void on_tabWidget_currentChanged(int index);

    void on_actionHead_to_head_triggered();
    void on_actionLap_time_comparison_triggered();


    void on_actionRecord_triggered();
    void on_actionStop_recording_triggered();
    void autoStopRecording();

    void on_actionPreferences_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionOpen_triggered();


    void showSessionBoard(bool show);

    void on_actionSession_analysis_triggered();

    void on_actionLT_files_data_base_triggered();

    void on_actionFollow_a_driver_triggered();

    void on_actionSession_times_triggered();

    void on_actionDriver_tracker_triggered();

    void on_actionTrack_records_triggered();

    void on_actionCheck_for_updates_triggered();

private:
    Ui::LTWindow *ui;
    DataStreamReader *streamReader;    

    QSettings *settings;

    QList<HeadToHeadDialog*> h2hDialog;
    QList<FollowADriverDialog*> fadDialog;
    QList<LapTimeComparisonDialog*> ltcDialog;

    int currDriver;

    SessionTimer *sessionTimer;

    EventRecorder *eventRecorder;
    EventPlayer *eventPlayer;
    QAction *eventPlayerAction;

    bool recording;
    bool playing;

    PreferencesDialog *prefs;
    LoginDialog *loginDialog;

    EventData &eventData;
    SessionAnalysisWidget *saw;
    SessionTimesWidget *stw;

    QProgressDialog *connectionProgress;

    LTFilesManagerDialog *ltFilesManagerDialog;
    TrackRecordsDialog *trackRecordsDialog;

    AboutDialog *aboutDialog;
    DriverTrackerWidget *driverTrackerWidget;

    DelayWidget *delayWidget;
    QAction *delayWidgetAction;

    UpdatesCheckerDialog *updatesCheckerDialog;

};

#endif // LTWINDOW_H
