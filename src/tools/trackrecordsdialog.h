#ifndef TRACKRECORDSDIALOG_H
#define TRACKRECORDSDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QSettings>

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

    void saveSettings(QSettings *settings);
    void loadSettings(QSettings *settings);

    void setFont(const QFont &font);

    
private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::TrackRecordsDialog *ui;
    int currentIndex;
};

#endif // TRACKRECORDSDIALOG_H
