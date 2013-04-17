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


#include "ltwindow.h"
#include "ui_ltwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTableWidgetItem>

#include "logindialog.h"
#include "../net/networksettings.h"

LTWindow::LTWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LTWindow), eventData(EventData::getInstance())
{    
    ui->setupUi(this);    

    currDriver = -1;

    streamReader = new DataStreamReader(this);
    prefs = new PreferencesDialog(this);
    settings = new QSettings(F1LTCore::iniFile(), QSettings::IniFormat, this);
    loginDialog = new LoginDialog(this);
    ltFilesManagerDialog = new LTFilesManagerDialog(this);
    trackRecordsDialog = new TrackRecordsDialog(this);
    saw = new SessionAnalysisWidget();
    stw = new SessionTimesWidget();
    driverTrackerWidget = new DriverTrackerWidget();
    aboutDialog = new AboutDialog(this);
    updatesCheckerDialog = new UpdatesCheckerDialog(this);

//    ui->trackStatusWidget->setupItems();

    connect(streamReader, SIGNAL(tryAuthorize()), this, SLOT(tryAuthorize()));
    connect(streamReader, SIGNAL(authorized(QString)), this, SLOT(authorized(QString)));
    connect(streamReader, SIGNAL(eventDataChanged(const DataUpdates&)), this, SLOT(eventDataChanged(const DataUpdates&)));
    connect(streamReader, SIGNAL(driverDataChanged(int, const DataUpdates&)), this, SLOT(driverDataChanged(int, const DataUpdates&)));
    connect(streamReader, SIGNAL(dataChanged(const DataUpdates&)), this, SLOT(dataChanged(const DataUpdates&)));
    connect(streamReader, SIGNAL(sessionStarted()), this, SLOT(sessionStarted()));
    connect(streamReader, SIGNAL(authorizationError()), this, SLOT(authorizationError()));
    connect(streamReader, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(streamReader, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(streamReader, SIGNAL(noLiveSession(bool, QString)), this, SLOT(showNoSessionBoard(bool, QString)));

    connect(updatesCheckerDialog, SIGNAL(newVersionAvailable()), this, SLOT(onNewVersionAvailable()));

    sessionTimer = new SessionTimer(this);
    connect(sessionTimer, SIGNAL(updateWeather()), this, SLOT(updateWeather()));

    connect(&SeasonData::getInstance(), SIGNAL(seasonDataChanged()), &ImagesFactory::getInstance(), SLOT(reloadGraphics()));
    connect(&SeasonData::getInstance(), SIGNAL(seasonDataChanged()), &ColorsManager::getInstance(), SLOT(calculateDefaultDriverColors()));
    connect(&SeasonData::getInstance(), SIGNAL(seasonDataChanged()), saw, SLOT(setupColors()));


    connect(prefs, SIGNAL(driversColorsChanged()), saw, SLOT(setupColors()));

    eventRecorder = new EventRecorder(sessionTimer, this);
    eventPlayer = new EventPlayer(this);

    delayWidget = new DelayWidget(this);
    connect(delayWidget, SIGNAL(delayChanged(int, int)), streamReader, SLOT(setDelay(int, int)));
    connect(delayWidget, SIGNAL(delayChanged(int, int)), sessionTimer, SLOT(setDelay(int, int)));
    connect(sessionTimer, SIGNAL(synchronizingTimer(bool)), delayWidget, SLOT(synchronizingTimer(bool)));
    connect(sessionTimer, SIGNAL(synchronizingTimer(bool)), driverTrackerWidget, SLOT(pauseTimer(bool)));

    connect(ui->messageBoardWidget, SIGNAL(connectClicked()), this, SLOT(on_actionConnect_triggered()));
    connect(ui->messageBoardWidget, SIGNAL(playClicked()), this, SLOT(on_actionOpen_triggered()));
    connect(ui->messageBoardWidget, SIGNAL(loadClicked()), this, SLOT(on_actionLT_files_data_base_triggered()));

    loadSettings();
    ColorsManager::getInstance().calculateDefaultDriverColors();
    saw->setupColors();

    delayWidgetAction = ui->mainToolBar->addWidget(delayWidget);
    delayWidgetAction->setVisible(true);

    eventPlayerAction = ui->mainToolBar->addWidget(eventPlayer);
    eventPlayerAction->setVisible(false);
    recording = false;
    playing = false;

    connectionProgress = new QProgressDialog(this);

    connect(sessionTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(eventRecorder, SIGNAL(recordingStopped()), this, SLOT(autoStopRecording()));
    connect(eventPlayer, SIGNAL(playClicked(int)), this, SLOT(eventPlayerPlayClicked(int)));
    connect(eventPlayer, SIGNAL(pauseClicked()), this, SLOT(eventPlayerPauseClicked()));
    connect(eventPlayer, SIGNAL(rewindToStartClicked()), this, SLOT(eventPlayerRewindToStartClicked()));
    connect(eventPlayer, SIGNAL(forwardToEndClicked()), this, SLOT(eventPlayerForwardToEndClicked()));
    connect(eventPlayer, SIGNAL(rewindClicked()), this, SLOT(eventPlayerRewindClicked()));
    connect(eventPlayer, SIGNAL(stopClicked()), this, SLOT(eventPlayerStopClicked()));
    connect(eventPlayer, SIGNAL(nextPackets(QVector<Packet>)), streamReader, SLOT(parsePackets(QVector<Packet>)));

    connect(ui->ltWidget, SIGNAL(driverSelected(int)), ui->driverDataWidget, SLOT(printDriverData(int)));
    connect(ui->ltWidget, SIGNAL(driverDoubleClicked(int)), this, SLOT(ltWidgetDriverSelected(int)));


    ui->messageBoardWidget->setVisible(false);

    QStringList args = qApp->arguments();
    if (args.size() > 1)
    {
        if (eventPlayer->loadFromFile(args.at(1)) == false)
        {
            QMessageBox::critical(this, "Error opening file!", "Could not open specified file, or the file is corrupted.");
            connectToServer();
            return;
        }
        setWindowTitle("FILT - " + args.at(1));
        ui->actionRecord->setVisible(false);
        ui->actionStop_recording->setVisible(false);
        eventPlayerAction->setVisible(true);
        delayWidgetAction->setVisible(false);

        playing = true;

        eventPlayer->startPlaying();
    }
    else
    {
    	if (settings->value("ui/auto_connect").toBool())
    		connectToServer();
    	else
    	{
    		ui->messageBoardWidget->showStartupBoard();
    		showSessionBoard(true);
    	}
    }    

}

LTWindow::~LTWindow()
{
    if (recording)
        eventRecorder->stopRecording();

    saveSettings();

    delete ui;
}

//------------------------- updating the data ----------------------

void LTWindow::eventDataChanged(const DataUpdates &dataUpdates)
{
    if (!playing)
        setWindowTitle("F1LT - " + eventData.getEventInfo().eventName);
    if (!playing && !recording && !ui->actionRecord->isEnabled())
        ui->actionRecord->setEnabled(true);

    if (dataUpdates.commentaryUpdate)
    {        
        ui->commentaryWidget->update();
    }
    if (eventData.getCommentary().size() == 0 && ui->commentaryWidget->getCommentary().size() > 0)
        ui->commentaryWidget->clear();

//    ui->trackStatusWidget->updateTrackStatus(eventData);
    ui->eventStatusWidget->updateEventStatus();
    ui->sessionDataWidget->updateData(dataUpdates);

    if ((ui->tabWidget->currentIndex() == 2) && dataUpdates.weatherUpdate)
    	ui->weatherChartsWidget->updateCharts();

//    if (recording)
//        eventRecorder->updateEventData(eventData);

    ui->ltWidget->updateLT();

    if (trackRecordsDialog->isVisible())
        trackRecordsDialog->update();
}

void LTWindow::driverDataChanged(int carID, const DataUpdates &)
{
    if (!playing && !recording && !ui->actionRecord->isEnabled())
        ui->actionRecord->setEnabled(true);

    if (carID > -1)
    {
        ui->ltWidget->updateLT();

        ui->sessionDataWidget->updateFastestLaps();

        if (ui->tabWidget->currentIndex() == 0)
        {
            ui->driverDataWidget->updateDriverData(carID);
        }

        for (int i = 0; i < ltcDialog.size(); ++i)
            ltcDialog[i]->driverUpdated(*eventData.getDriverDataByIdPtr(carID));

        for (int i = 0; i < h2hDialog.size(); ++i)
            h2hDialog[i]->driverUpdated(*eventData.getDriverDataByIdPtr(carID));

        for (int i = 0; i < fadDialog.size(); ++i)
            fadDialog[i]->driverUpdated(*eventData.getDriverDataByIdPtr(carID));

        if (saw->isVisible())
            saw->update();

        if (stw->isVisible())
            stw->update();

        if (trackRecordsDialog->isVisible())
            trackRecordsDialog->update();
    }
}

void LTWindow::dataChanged(const DataUpdates &dataUpdates)
{
    if (playing)
        setWindowTitle("F1LT - " + eventData.getEventInfo().eventName + " (" + eventPlayer->playedFile() + ")");

    if (dataUpdates.commentaryUpdate)
    {        
        ui->commentaryWidget->update();
    }

//    ui->trackStatusWidget->updateTrackStatus(eventData);
    ui->eventStatusWidget->updateEventStatus();

    if (!eventPlayer->isPlaying())
        ui->sessionDataWidget->updateEventInfo();

//    ui->sessionDataWidget->updateFastestLaps();
//    ui->sessionDataWidget->updateSpeedRecords();
//    ui->sessionDataWidget->updatePitStops(true);
    ui->sessionDataWidget->updateData(dataUpdates);

//    if (currDriver >= 0)
    ui->driverDataWidget->updateDriverData(dataUpdates);//printDriverData(currDriver);

    for (int i = 0; i < ltcDialog.size(); ++i)
    {
        ltcDialog[i]->updateData(dataUpdates);
    }

    for (int i = 0; i < h2hDialog.size(); ++i)
    {
        h2hDialog[i]->updateData(dataUpdates);
    }

    for (int i = 0; i < fadDialog.size(); ++i)
    {
        fadDialog[i]->updateData(dataUpdates);
    }

    ui->ltWidget->updateLT();

    if ((ui->tabWidget->currentIndex() == 2) && dataUpdates.weatherUpdate)
		ui->weatherChartsWidget->updateCharts();

    if (saw->isVisible())
        saw->update();

    if (stw->isVisible())
        stw->update();

    if (trackRecordsDialog->isVisible())
        trackRecordsDialog->update();
}

void LTWindow::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
        case 0:
            ui->driverDataWidget->updateView();
            if (currDriver >= 0)
                ui->driverDataWidget->updateDriverData(currDriver);

            break;

        case 1:
            ui->sessionDataWidget->updateData(DataUpdates(true));
            break;
    }
}

void LTWindow::on_actionLap_time_comparison_triggered()
{
    for (int i = 0; i < ltcDialog.size(); ++i)
    {
        if (!ltcDialog[i]->isVisible())
        {
            ltcDialog[i]->show(ui->ltWidget->getCurrentDriverId());
            return;
        }
    }
    if (ltcDialog.size() < 30)
    {
        LapTimeComparisonDialog *ltc = new LapTimeComparisonDialog();
        ltc->show(ui->ltWidget->getCurrentDriverId());
        ltc->setFont(prefs->getMainFont());
        ltcDialog.append(ltc);
    }
}

void LTWindow::on_actionHead_to_head_triggered()
{    
    for (int i = 0; i < h2hDialog.size(); ++i)
    {
        if (!h2hDialog[i]->isVisible())
        {
            h2hDialog[i]->show(ui->ltWidget->getCurrentDriverId());
            return;
        }

    }
    if (h2hDialog.size() < 30)
    {
        HeadToHeadDialog *h2h = new HeadToHeadDialog();
        h2h->show(ui->ltWidget->getCurrentDriverId());
        h2h->setFont(prefs->getMainFont());
        h2hDialog.append(h2h);
    }
}

void LTWindow::on_actionFollow_a_driver_triggered()
{
    for (int i = 0; i < fadDialog.size(); ++i)
    {
        if (!fadDialog[i]->isVisible())
        {
            fadDialog[i]->show(ui->ltWidget->getCurrentDriverId());
            return;
        }

    }
    if (fadDialog.size() < eventData.getDriversData().size())
    {
        FollowADriverDialog *fad = new FollowADriverDialog();
        fad->show(ui->ltWidget->getCurrentDriverId());
        fad->setFont(prefs->getMainFont());
        fadDialog.append(fad);
    }
}

void LTWindow::sessionStarted()
{
    if (!sessionTimer->isActive() &&
        ((!playing && !eventData.isQualiBreak()) || (playing && eventPlayer->isPlaying() && !eventPlayer->isPaused())))
    {
        sessionTimer->start(1000);
        driverTrackerWidget->startTimer(1000);

        if (eventData.isSessionStarted() && !playing)
        {
            delayWidget->setEnabled(true);

            if (!recording && settings->value("ui/auto_record").toBool() && !eventRecorder->isSessionRecorded())
                startRecording(true);
        }
    }
}

void LTWindow::showNoSessionBoard(bool show, QString msg)
{
	if (show)
	{
        //ui->tableWidget->clear();
        ui->ltWidget->clearData();
        ui->commentaryWidget->clear();
		ui->driverDataWidget->clearData();
		ui->sessionDataWidget->clearData();
        driverTrackerWidget->setup();

		ui->messageBoardWidget->showSessionBoard(msg);
		showSessionBoard(true);
	}
	else
	{
        //here the new session is started
		showSessionBoard(false);
        eventRecorder->setSessionRecorded(false);
        ui->ltWidget->updateLT();
    }
}

void LTWindow::updateWeather()
{
    if (ui->tabWidget->currentIndex() == 2)
        ui->weatherChartsWidget->updateCharts();
}

void LTWindow::onNewVersionAvailable()
{
    updatesCheckerDialog->show(false);
}

void LTWindow::ltWidgetDriverSelected(int id)
{
    ui->driverDataWidget->printDriverData(id);
    ui->tabWidget->setCurrentIndex(0);
}

void LTWindow::timeout()
{
    if (recording)
        eventRecorder->timeout();

    if (eventPlayer->isPlaying())
        eventPlayer->timeout();

    ui->eventStatusWidget->updateEventStatus();

//    if (driverTrackerWidget->isVisible())
//        driverTrackerWidget->update();

    //during quali timer is stopped when we have red flag    

    if (eventData.isSessionStarted())
    {
        if (!playing && !recording && settings->value("ui/auto_record").toBool() && !eventRecorder->isSessionRecorded())
            startRecording(true);
    }

    if (!recording && !playing && !eventData.isSessionStarted())
    {
        sessionTimer->stop();
        driverTrackerWidget->stopTimer();
    }
}

//-------------------- settings ----------------------------

void LTWindow::loadSettings()
{
    NetworkSettings::getInstance().loadSettings(*settings);

    QFont mainFont, commentaryFont;
    QString sbuf = settings->value("fonts/main_family").toString();
    mainFont.setFamily(sbuf);

    int ibuf = settings->value("fonts/main_size").toInt();
    mainFont.setPointSize(ibuf);

    ibuf = settings->value("fonts/main_weight").toInt();
    mainFont.setWeight(ibuf);

    bool bbuf = settings->value("fonts/main_italic").toBool();
    mainFont.setItalic(bbuf);

//    prefs->setMainFont(mainFont);

    sbuf = settings->value("fonts/commentary_family").toString();
    commentaryFont.setFamily(sbuf);

    ibuf = settings->value("fonts/commentary_size").toInt();
    commentaryFont.setPointSize(ibuf);

    ibuf = settings->value("fonts/commentary_weight").toInt();
    commentaryFont.setWeight(ibuf);

    bbuf = settings->value("fonts/commentary_italic").toBool();
    commentaryFont.setItalic(bbuf);

//    prefs->setCommentaryFont(commentaryFont);
//    prefs->setSplitterOpaqueResize(settings->value("ui/ltresize").toBool());
//    prefs->setAlternatingRowColors(settings->value("ui/alt_colors").toBool());
//    prefs->setAutoRecord(settings->value("ui/auto_record").toBool());

    ui->splitter->setOpaqueResize(settings->value("ui/ltresize").toBool());    
    //ui->tableWidget->setAlternatingRowColors(settings->value("ui/alt_colors").toBool());

    setFonts(mainFont, commentaryFont);

    restoreGeometry(settings->value("ui/window_geometry").toByteArray());


    ui->splitter->restoreState(settings->value("ui/splitter_pos").toByteArray());
    ui->tabWidget->setCurrentIndex(settings->value("ui/current_tab1").toInt());

    if (ui->tabWidget->currentIndex() == 0)
        ui->driverDataWidget->setCurrentIndex(settings->value("ui/current_tab2").toInt());
    else if (ui->tabWidget->currentIndex() == 1)
        ui->sessionDataWidget->setCurrentIndex(settings->value("ui/current_tab2").toInt());

//    prefs->setReverseOrderLapHistory(settings->value("ui/reversed_lap_history").toBool());
//    prefs->setReverseOrderHeadToHead(settings->value("ui/reversed_head_to_head").toBool());
//    prefs->setReverseOrderLapTimeComparison(settings->value("ui/reversed_lap_time_comparison").toBool());

    ui->ltWidget->setDrawCarThumbnails(settings->value("ui/car_thumbnails").toBool());

    eventRecorder->setAutoSaveRecord(settings->value("ui/auto_save_record", -1).toInt());
    eventRecorder->setAutoStopRecord(settings->value("ui/auto_stop_record", -1).toInt());

    stw->loadSettings(*settings);
    driverTrackerWidget->loadSettings(settings);

    ltFilesManagerDialog->loadSettings(settings);
    trackRecordsDialog->loadSettings(*settings);

    ui->commentaryWidget->loadSettings(*settings);

    QList<QColor> colors = ColorsManager::getInstance().getColors();
    for (int i = 0; i < ColorsManager::getInstance().getColors().size(); ++i)
    {
        QVariant color = settings->value(QString("ui/color_%1").arg(i), colors[i]);

        colors[i] = color.value<QColor>();
    }
    ColorsManager::getInstance().setColors(colors);

    colors = ColorsManager::getInstance().getDriverColors();
    for (int i = 0; i < ColorsManager::getInstance().getDriverColors().size(); ++i)
    {
        QVariant color = settings->value(QString("ui/driver_color_%1").arg(i), colors[i]);

        colors[i] = color.value<QColor>();
    }
    ColorsManager::getInstance().setDriverColors(colors);

    saw->loadSettings(*settings);
    updatesCheckerDialog->loadSettings(*settings);

    if (settings->value("ui/check_for_updates", true).toBool())
        updatesCheckerDialog->checkForUpdates();
}

void LTWindow::saveSettings()
{
//    QString passwd = encodePasswd(settings->value("login/passwd").toString());
//    encodePasswd(passwd);

    NetworkSettings::getInstance().saveSettings(*settings);

    settings->setValue("ui/window_geometry", saveGeometry());
    settings->setValue("ui/splitter_pos", ui->splitter->saveState());
    settings->setValue("ui/current_tab1", ui->tabWidget->currentIndex());
    if (ui->tabWidget->currentIndex() == 0)
        settings->setValue("ui/current_tab2", ui->driverDataWidget->currentIndex());
    else if (ui->tabWidget->currentIndex() == 1)
        settings->setValue("ui/current_tab2", ui->sessionDataWidget->currentIndex());

    saw->saveSettings(*settings);
    stw->saveSettings(*settings);
    driverTrackerWidget->saveSettings(settings);
    ltFilesManagerDialog->saveSettings(settings);
    trackRecordsDialog->saveSettings(*settings);
    ui->commentaryWidget->saveSettings(*settings);

    for (int i = 0; i < ColorsManager::getInstance().getColors().size(); ++i)
    {
        QVariant color = ColorsManager::getInstance().getColors()[i];
        settings->setValue(QString("ui/color_%1").arg(i), color);
    }

    for (int i = 0; i < ColorsManager::getInstance().getDriverColors().size(); ++i)
    {
        QVariant color = ColorsManager::getInstance().getDriverColors()[i];
        settings->setValue(QString("ui/driver_color_%1").arg(i), color);
    }
    updatesCheckerDialog->saveSettings(*settings);

//    settings->setValue("ui/ltresize", prefs->isSplitterOpaqueResize());
//    settings->setValue("ui/alt_colors", prefs->isAlternatingRowColors());

//    settings->setValue("ui/reversed_lap_history", prefs->isReverseOrderLapHistory());
//    settings->setValue("ui/reversed_head_to_head", prefs->isReverseOrderHeadToHead());
//    settings->setValue("ui/reversed_lap_time_comparison", prefs->isReverseOrderLapTimeComparison());
//    settings->setValue("ui/auto_record", prefs->isAutoRecord());
}

void LTWindow::on_actionPreferences_triggered()
{
    if (prefs->exec(settings) == QDialog::Accepted)
    {
        setFonts(prefs->getMainFont(), prefs->getCommentaryFont());

        ui->splitter->setOpaqueResize(prefs->isSplitterOpaqueResize());
        //ui->tableWidget->setAlternatingRowColors(prefs->isAlternatingRowColors());

        eventRecorder->setAutoStopRecord(settings->value("ui/auto_stop_record").toInt());
        eventRecorder->setAutoSaveRecord(settings->value("ui/auto_save_record").toInt());

        NetworkSettings::getInstance().setProxy(prefs->getProxy(), prefs->proxyEnabled());

        for (int i = 0; i < h2hDialog.size(); ++i)
        {
            if (h2hDialog[i]->isVisible())
                h2hDialog[i]->updateData();
        }

        for (int i = 0; i < ltcDialog.size(); ++i)
        {
            if (ltcDialog[i]->isVisible())
                ltcDialog[i]->updateData();
        }

        ui->driverDataWidget->updateDriverData(currDriver);

        ui->ltWidget->setDrawCarThumbnails(settings->value("ui/car_thumbnails").toBool());
        driverTrackerWidget->drawTrackerClassification(settings->value("ui/draw_tracker_classification").toBool());        
    }
}

void LTWindow::setFonts(const QFont &mainFont, const QFont &commentaryFont)
{
    ui->ltWidget->setFont(mainFont);
    ui->driverDataWidget->setFont(mainFont, commentaryFont);
    ui->sessionDataWidget->setFont(mainFont);

    for (int i = 0; i < h2hDialog.size(); ++i)
        h2hDialog[i]->setFont(mainFont);

    for (int i = 0; i < ltcDialog.size(); ++i)
        ltcDialog[i]->setFont(mainFont);

    for (int i = 0; i < fadDialog.size(); ++i)
        fadDialog[i]->setFont(mainFont);

    ui->commentaryWidget->setFont(commentaryFont);
//    ui->trackStatusWidget->setFont(mainFont);
    ui->eventStatusWidget->setFont(mainFont);
    prefs->setFonts(mainFont, commentaryFont);
    trackRecordsDialog->setFont(mainFont);

    stw->setFont(mainFont);
}

bool LTWindow::close()
{
    for (int i = 0; i < h2hDialog.size(); ++i)
        h2hDialog[i]->close();

    for (int i = 0; i < ltcDialog.size(); ++i)
        ltcDialog[i]->close();

    for (int i = 0; i < fadDialog.size(); ++i)
        fadDialog[i]->close();

    if (saw->isVisible())
        saw->close();

    if (stw->isVisible())
        stw->close();

    if (driverTrackerWidget->isVisible())
        driverTrackerWidget->close();

    if (trackRecordsDialog->isVisible())
        trackRecordsDialog->close();

    TrackRecords::getInstance().saveTrackRecords(F1LTCore::trackRercordsFile(false));

    return QMainWindow::close();
}

void LTWindow::setupDialogs()
{
    for (int i = 0; i < h2hDialog.size(); ++i)
        h2hDialog[i]->loadDriversList();

    for (int i = 0; i < ltcDialog.size(); ++i)
        ltcDialog[i]->loadDriversList();

    for (int i = 0; i < fadDialog.size(); ++i)
        fadDialog[i]->loadDriversList();

    if (stw->isVisible())
        stw->loadDriversList();

    driverTrackerWidget->setup();
    ui->sessionDataWidget->resizeTrackMap();
}

//-------------------- connection with server ----------------------

void LTWindow::tryAuthorize()
{
    connectionProgress->setWindowModality(Qt::WindowModal);
    connectionProgress->setLabelText("Connecting...");
    connectionProgress->setCancelButton(0);
    connectionProgress->setRange(0,0);
    connectionProgress->setMinimumDuration(0);
    connectionProgress->show();
}

void LTWindow::authorized(QString)
{
    connectionProgress->cancel();
}

void LTWindow::authorizationError()
{
    if (!playing)
    {
        if (connectionProgress->isVisible())
            connectionProgress->cancel();

        QMessageBox::critical(this, tr("Login error!"), tr("Could not login to the server!\nCheck your email and password"));

        on_actionConnect_triggered();
        ui->actionRecord->setEnabled(false);
    }
}

void LTWindow::on_actionConnect_triggered()
{    
    if (loginDialog->exec(NetworkSettings::getInstance().getUser(), NetworkSettings::getInstance().getPassword(),
          NetworkSettings::getInstance().getProxy(), NetworkSettings::getInstance().usingProxy()) == QDialog::Accepted)
    {
        if (playing)
        {
            eventPlayer->stopPlaying();
            eventPlayerStopClicked(false);                        
        }

        streamReader->disconnectFromLTServer();
        QString email = loginDialog->getEmail();
        QString passwd = loginDialog->getPasswd();
        NetworkSettings::getInstance().setUserPassword(email, passwd);
        NetworkSettings::getInstance().setProxy(loginDialog->getProxy(), loginDialog->proxyEnabled());

        SeasonData::getInstance().checkSeasonData();
        streamReader->connectToLTServer();

//        settings->setValue("login/email", email);

//        QString encPasswd = encodePasswd(passwd);
//        settings->setValue("login/passwd", encPasswd);

        showSessionBoard(false);

        setupDialogs();
    }
}

void LTWindow::connectToServer()
{
    QString email = NetworkSettings::getInstance().getUser();
    QString passwd = NetworkSettings::getInstance().getPassword();

    if (email == "" || passwd == "")
    {
        if (loginDialog->exec() == QDialog::Accepted)
        {
            email = loginDialog->getEmail();            
            passwd = loginDialog->getPasswd();

            NetworkSettings::getInstance().setUserPassword(email, passwd);
            NetworkSettings::getInstance().setProxy(loginDialog->getProxy(), loginDialog->proxyEnabled());

            SeasonData::getInstance().checkSeasonData();
            streamReader->connectToLTServer();

            driverTrackerWidget->setup();

//            settings->setValue("login/email", email);
//            passwd = encodePasswd(loginDialog->getPasswd());
//            settings->setValue("login/passwd", passwd);
        }
    }
    else
    {
        streamReader->connectToLTServer();
        driverTrackerWidget->setup();
    }
}

void LTWindow::error(QAbstractSocket::SocketError er)
{
    if (!playing)
    {
        ui->actionRecord->setEnabled(false);
        if (QMessageBox::critical(this, tr("Connection error!"), tr("There was an error with connection to LT server\n Error code: ") + QString("%1").arg(er), QMessageBox::Retry, QMessageBox::Close)
                == QMessageBox::Retry)
        {
            //try to connect again
            streamReader->disconnectFromLTServer();
            connectToServer();
        }
//        else
//        {
//        	ui->messageBoardWidget->showStartupBoard();
//			showSessionBoard(true);
//        }
    }
}

void LTWindow::error(QNetworkReply::NetworkError er)
{
    if (!playing)
    {
        ui->actionRecord->setEnabled(false);
        if (QMessageBox::critical(this, tr("Connection error!"), tr("There was an error with connection to LT server\n Error code: ") + QString("%1").arg(er), QMessageBox::Retry, QMessageBox::Close)
                == QMessageBox::Retry)
        {
            //try to connect again
            streamReader->disconnectFromLTServer();
            connectToServer();
        }
//        else
//		{
//			ui->messageBoardWidget->showStartupBoard();
//			showSessionBoard(true);
//		}
    }
}

//-------------------- about ------------------------

void LTWindow::on_actionAbout_triggered()
{
    aboutDialog->exec();
    //QMessageBox::about(this, "About F1LT", "<b>F1LT</b> v" + F1LTCore::programVersion()+"<br/>by Mariusz Pilarek, 2012<br/><a href=\"http://f1lt.pl/\">Home site</a>");
}

void LTWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

//-------------------- event recorder ----------------------------

void LTWindow::startRecording(bool autoRecord)
{
    //if the current session is recorded while auto record has turned on we don't do anything
    if (autoRecord == true &&
        eventRecorder->isSessionRecorded() == true)
        return;

    recording = true;
    ui->actionStop_recording->setEnabled(true);
    ui->actionRecord->setEnabled(false);
    ui->actionLT_files_data_base->setEnabled(false);
//        ui->actionRecord->setIcon(QIcon(":/ui_icons/stop.png"));
    ui->actionOpen->setEnabled(false);
    eventRecorder->startRecording(delayWidget->getDelay());
//    connect(streamReader, SIGNAL(packetParsed(Packet)), eventRecorder, SLOT(appendPacket(Packet)));
    connect(streamReader, SIGNAL(packetParsed(QPair<Packet, qint64>)), eventRecorder, SLOT(appendPacket(QPair<Packet, qint64>)));

    if (!sessionTimer->isActive())
    {
        sessionTimer->start();
        driverTrackerWidget->startTimer();
    }
}

void LTWindow::stopRecording(bool autoStop)
{
    recording = false;
    ui->actionOpen->setEnabled(true);
    ui->actionRecord->setEnabled(true);
    ui->actionLT_files_data_base->setEnabled(true);
    ui->actionStop_recording->setEnabled(false);

    if (!autoStop)
        eventRecorder->stopRecording();

//    disconnect(streamReader, SIGNAL(packetParsed(Packet)), eventRecorder, SLOT(appendPacket(Packet)));
    disconnect(streamReader, SIGNAL(packetParsed(QPair<Packet, qint64>)), eventRecorder, SLOT(appendPacket(QPair<Packet, qint64>)));
}

void LTWindow::on_actionRecord_triggered()
{
    startRecording(false);
}

void LTWindow::on_actionStop_recording_triggered()
{
    stopRecording(false);
}

void LTWindow::autoStopRecording()
{
    stopRecording(true);
}

//-------------------- event player ----------------------------

void LTWindow::on_actionOpen_triggered()
{
    if (playing)
        eventPlayer->pausePlaying();

    QString ltDir = settings->value("ui/ltdir").toString();
    if (ltDir == "")
        ltDir = F1LTCore::ltDataHomeDir();

    QString fName = QFileDialog::getOpenFileName(this, "Select archive LT event file", ltDir, "*.lt");    

    if (!fName.isNull())
    {
        sessionTimer->stop();
        driverTrackerWidget->stopTimer();
        QFileInfo fi(fName);
        ltDir = fi.absolutePath();

        settings->setValue("ui/ltdir", ltDir);

        if (recording)
            stopRecording(false);

        eventPlayerOpenFile(fName);
    }
}

void LTWindow::on_actionLT_files_data_base_triggered()
{
    if (playing)
        eventPlayer->pausePlaying();

    QString fName = ltFilesManagerDialog->exec();    

    if (!fName.isNull())
    {
        sessionTimer->stop();
        driverTrackerWidget->stopTimer();
        if (recording)
            stopRecording(false);

        eventPlayerOpenFile(fName);
    }
}

void LTWindow::eventPlayerOpenFile(QString fName)
{
    if (eventPlayer->loadFromFile(fName) == false)
    {
        QMessageBox::critical(this, "Error opening file!", "Could not open specified file, or the file is corrupted.");
        return;
    }

    QFileInfo fInfo(fName);
    QString name = fInfo.fileName();

    showSessionBoard(false);

    //ui->tableWidget->clear();
    ui->commentaryWidget->clear();
    ui->driverDataWidget->clearData();
    ui->sessionDataWidget->clearData();

    ui->ltWidget->loadCarImages();

    ui->actionRecord->setVisible(false);
    ui->actionStop_recording->setVisible(false);
    eventPlayerAction->setVisible(true);
    delayWidgetAction->setVisible(false);

    streamReader->disconnectFromLTServer();
//    streamReader->clearData();

    playing = true;

    eventPlayer->startPlaying();
    setupDialogs();
    saw->resetView();
}

void LTWindow::eventPlayerPlayClicked(int interval)
{
    sessionTimer->start(interval);
    driverTrackerWidget->startTimer(interval);
}

void LTWindow::eventPlayerPauseClicked()
{
    sessionTimer->stop();
    driverTrackerWidget->stopTimer();
}

void LTWindow::eventPlayerRewindToStartClicked()
{
    sessionTimer->stop();
    driverTrackerWidget->stopTimer();    
    eventData.reset();
    ui->sessionDataWidget->clearData();

    eventPlayer->startPlaying();
    driverTrackerWidget->setup();
}

void LTWindow::eventPlayerForwardToEndClicked()
{
    sessionTimer->stop();
    driverTrackerWidget->stopTimer();
}

void LTWindow::eventPlayerRewindClicked()
{
    //clear all data
    ui->sessionDataWidget->clearFastestLaps();
    ui->ltWidget->clearModelsData();
    eventData.reset();

    //it should be safe here - drivers vector will remain the same in eventdata
//    driverTrackerWidget->setup();
}

void LTWindow::eventPlayerStopClicked(bool connect)
{
    streamReader->clearData();
    eventPlayerAction->setVisible(false);
    delayWidgetAction->setVisible(true);
    ui->actionRecord->setVisible(true);
    ui->actionStop_recording->setVisible(true);
    //ui->tableWidget->clear();
    ui->ltWidget->clearData();
    ui->driverDataWidget->clearData();
    ui->sessionDataWidget->clearData();
    ui->commentaryWidget->clear();
    saw->resetView();
    eventRecorder->setSessionRecorded(false);
    sessionTimer->stop();
    driverTrackerWidget->stopTimer();

    playing = false;
    SeasonData::getInstance().loadSeasonFile();

    ui->ltWidget->loadCarImages();

    setupDialogs();

    setWindowTitle("F1LT");
    if (connect)
    {
        if (settings->value("ui/auto_connect").toBool())
            connectToServer();
        else
        {
            ui->messageBoardWidget->showStartupBoard();
            showSessionBoard(true);
        }
    }            
}

void LTWindow::showSessionBoard(bool show)
{
    if (show)
    {
        ui->splitter->setVisible(!show);
        ui->eventStatusWidget->setVisible(!show);
        ui->messageBoardWidget->setVisible(show);
        delayWidget->setEnabled(false);
    }
    else
    {
        ui->messageBoardWidget->setVisible(show);
        ui->eventStatusWidget->setVisible(!show);
        ui->splitter->setVisible(!show);
//        delayWidget->setEnabled(true);
    }
	ui->actionRecord->setEnabled(!show);
    ui->actionHead_to_head->setEnabled(!show);
    ui->actionLap_time_comparison->setEnabled(!show);
    ui->actionFollow_a_driver->setEnabled(!show);
    ui->actionSession_analysis->setEnabled(!show);
    ui->actionSession_times->setEnabled(!show);
    ui->actionDriver_tracker->setEnabled(!show);
//    ui->actionTrack_records->setEnabled(!show);
}

void LTWindow::on_actionSession_analysis_triggered()
{
    saw->exec();
}



void LTWindow::on_actionSession_times_triggered()
{
    stw->exec();
}

void LTWindow::on_actionDriver_tracker_triggered()
{
    driverTrackerWidget->exec();
}

void LTWindow::on_actionTrack_records_triggered()
{
    trackRecordsDialog->exec();
}

void LTWindow::on_actionCheck_for_updates_triggered()
{
    updatesCheckerDialog->show();
}
