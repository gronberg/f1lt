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

    void exec(const TrackRecordsAtom &records);

    void loadRecords();

    void saveSettings(QSettings &settings);
    void loadSettings(QSettings &settings);

    void setFont(const QFont &font);
    
private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DriverRecordsDialog *ui;
    TrackRecordsAtom driverRecords;
};

#endif // DRIVERRECORDSDIALOG_H
