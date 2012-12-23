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

    void eventDataChanged();
    void driverDataChanged(int);
    void dataChanged();
    void sessionStarted();
    void showNoSessionBoard(bool, QString);

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

private:
    Ui::LTWindow *ui;
    DataStreamReader *streamReader;

    //very simple passwd encoding algorithm
    QString encodePasswd(QString passwd)
    {
        int sz = passwd.size();
        QString ret;
        for (int i = 0; i < sz; ++i)
        {
            char c = passwd[i].toAscii();
            c ^= (1 << (i%8));
            ret += c;
        }
        return ret;
    }


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

};

#endif // LTWINDOW_H
