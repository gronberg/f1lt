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
