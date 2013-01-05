#ifndef DRIVERRECORDSDIALOG_H
#define DRIVERRECORDSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "../core/trackrecords.h"

namespace Ui {
class DriverRecordsDialog;
}

class DriverRecordsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DriverRecordsDialog(QWidget *parent = 0);
    ~DriverRecordsDialog();

    void exec(const TrackWeekendRecords &records, QString trackName);

    void loadRecords();
    void loadRecords(const TrackWeekendRecords &records, QString trackName);

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);
    
private slots:
    void on_comboBox_currentIndexChanged(int);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::DriverRecordsDialog *ui;
    TrackWeekendRecords driverRecords;
    QString trackName;
};

#endif // DRIVERRECORDSDIALOG_H
