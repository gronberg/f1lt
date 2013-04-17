#ifndef UPDATECHECKERDIALOG_H
#define UPDATECHECKERDIALOG_H

#include <QAbstractButton>
#include <QDialog>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QSettings>

namespace Ui {
class UpdatesCheckerDialog;
}

class UpdatesCheckerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdatesCheckerDialog(QWidget *parent = 0);
    ~UpdatesCheckerDialog();

    void checkForUpdates();
    bool newVersionIsAvailable(QString version);

    void loadSettings(QSettings &settings);
    void saveSettings(QSettings &settings);

public slots:
    void finished();
    void error(QNetworkReply::NetworkError);
    void show(bool check = true);

signals:
    void newVersionAvailable();
    
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::UpdatesCheckerDialog *ui;

    QNetworkAccessManager manager;
    QNetworkRequest req;
    QNetworkReply *reply;
};

#endif // UPDATECHECKERDIALOG_H
