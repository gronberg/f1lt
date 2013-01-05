#ifndef TRACKRECORDSDIALOG_H
#define TRACKRECORDSDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QSettings>

#include "driverrecordsdialog.h"

namespace Ui {
class TrackRecordsDialog;
}

class TrackRecordsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TrackRecordsDialog(QWidget *parent = 0);
    ~TrackRecordsDialog();

    void loadTrackRecords();

    void exec();

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);

    void update();

    
private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

    void on_trackVersionBox_activated(const QString &arg1);

    void on_yearBox_currentIndexChanged(const QString &arg1);

private:
    Ui::TrackRecordsDialog *ui;

    int currentIndex;
    TrackVersion *currentTV;
    TrackWeekendRecords *currentTWR;

    DriverRecordsDialog *drDialog;
};

#endif // TRACKRECORDSDIALOG_H
