#ifndef LTFILESMANAGERDIALOG_H
#define LTFILESMANAGERDIALOG_H

#include <QDialog>
#include <QProgressDialog>
#include <QSettings>
#include <QTreeWidgetItem>

#include "../net/ltfilesmanager.h"

namespace Ui {
class LTFilesManagerDialog;
}

class LTFilesManagerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LTFilesManagerDialog(QWidget *parent = 0);
    ~LTFilesManagerDialog();

    QString exec();

//    QTreeWidgetItem *parseEntry(QTreeWidgetItem *parent, QString entry, const QStringList &onlineList);
    QStringList parseEntry(QString entry, const QStringList &onlineList);
    QString getSessionType(QString session);

    void downloadLTList();
    void getLTListFromDisk();

    void updateTree(const QStringList &onlineList);

    bool fileExists(QString &file, bool saveCurrent = true);

    void loadSettings(QSettings *);
    void saveSettings(QSettings *);


public slots:
    void ltListObtained(QStringList);
    void ltFileObtained(QByteArray);
    void downloadProgress ( qint64 bytesReceived, qint64 bytesTotal );
    void error (QNetworkReply::NetworkError code);
    
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_refreshButton_clicked();

    void on_playButton_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_cancelButton_clicked();

private:
    Ui::LTFilesManagerDialog *ui;

    QSet<QString> ltList;
    LTFilesManager ltFilesManager;

    QProgressDialog *progress;
    QString currentFile;
};

#endif // LTFILESMANAGERDIALOG_H
