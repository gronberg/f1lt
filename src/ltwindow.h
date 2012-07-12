#ifndef LTWINDOW_H
#define LTWINDOW_H

#include <QMainWindow>

#include <QByteArray>
#include <QIcon>
#include <QList>
#include <QSettings>
#include <QTimer>

#include "datastreamreader.h"
#include "eventplayer.h"
#include "eventrecorder.h"
#include "headtoheaddialog.h"
#include "laptimecomparisondialog.h"
#include "logindialog.h"
#include "lttablewidget.h"
#include "preferencesdialog.h"
#include "sessionanalysiswidget.h"


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

public slots:
    bool close();

    void eventPlayerPlayClicked(int);
    void eventPlayerPauseClicked();
    void eventPlayerRewindToStartClicked();
    void eventPlayerForwardToEndClicked();
    void eventPlayerRewindClicked();
    void eventPlayerStopClicked(bool connect = true);

private slots:
    void on_authorized(QString);
    void on_authorizationError();
    void error(QAbstractSocket::SocketError);
    void error(QNetworkReply::NetworkError);
    void on_actionConnect_triggered();

    void on_eventDataChanged();
    void on_driverDataChanged(int);
    void on_dataChanged();
    void on_sessionStarted();
    void on_showNoSessionBoard(bool, QString);

    void timeout();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_tableWidget_cellClicked(int row, int column);
    void on_tabWidget_currentChanged(int index);

    void on_actionHead_to_head_triggered();
    void on_actionLap_time_comparison_triggered();


    void on_actionRecord_triggered();
    void on_actionStop_recording_triggered();
    void on_autoStopRecording();

    void on_actionPreferences_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionOpen_triggered();


    void showSessionBoard(bool show);

    void on_actionSession_analysis_triggered();

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
    QList<LapTimeComparisonDialog*> ltcDialog;

    int currDriver;

    QTimer *eventTimer;

    EventRecorder *eventRecorder;
    EventPlayer *eventPlayer;
    QAction *eventPlayerAction;

    bool recording;
    bool playing;

    PreferencesDialog *prefs;
    LoginDialog *loginDialog;

    EventData &eventData;
    SessionAnalysisWidget *saw;
};

#endif // LTWINDOW_H
