#include "drivertrackerwidget.h"
#include "ui_drivertrackerwidget.h"

#include <QKeyEvent>

DriverTrackerWidget::DriverTrackerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriverTrackerWidget), speed(1)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui->driverTracker, SIGNAL(driverExcluded(int,bool)), ui->driverRadar, SLOT(excludeDriver(int,bool)));
    connect(ui->driverTracker, SIGNAL(driverSelected(int)), ui->driverRadar, SLOT(selectDriver(int)));
}

DriverTrackerWidget::~DriverTrackerWidget()
{
    delete ui;
}

void DriverTrackerWidget::update()
{
    ui->driverRadar->update();
    ui->driverTracker->update();
}

void DriverTrackerWidget::setup()
{    
    ui->driverRadar->setupDrivers(speed);
    ui->driverTracker->setupDrivers(speed);
}

void DriverTrackerWidget::loadSettings(QSettings *settings)
{
    ui->splitter->restoreState(settings->value("ui/tracker_splitter_pos").toByteArray());
    restoreGeometry(settings->value("ui/driver_tracker_geometry").toByteArray());
    ui->driverTracker->setDrawDriverClassification(settings->value("ui/draw_tracker_classification").toBool());
}

void DriverTrackerWidget::saveSettings(QSettings *settings)
{
    settings->setValue("ui/tracker_splitter_pos", ui->splitter->saveState());
    settings->setValue("ui/driver_tracker_geometry", saveGeometry());
}

void DriverTrackerWidget::on_pushButton_clicked()
{
    close();
}

void DriverTrackerWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        close();
}

void DriverTrackerWidget::exec()
{
    setWindowTitle("Driver tracker: " + EventData::getInstance().getEventInfo().eventName);

    ui->driverTracker->checkSetupCorrect(speed);
    ui->driverRadar->checkSetupCorrect(speed);

    show();
}

void DriverTrackerWidget::drawTrackerClassification(bool val)
{
    ui->driverTracker->setDrawDriverClassification(val);
}
