#include "ltfilesmanagerdialog.h"
#include "ui_ltfilesmanagerdialog.h"

#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QPushButton>

#include "../core/seasondata.h"
#include "../main_gui/ltitemdelegate.h"


LTFilesManagerDialog::LTFilesManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LTFilesManagerDialog)
{
    ui->setupUi(this);

    progress = new QProgressDialog(this);
    ui->treeWidget->header()->setMovable(false);

    ui->treeWidget->setItemDelegate(new LTItemDelegate(this));

    connect(&ltFilesManager, SIGNAL(ltListObtained(QStringList)), this, SLOT(ltListObtained(QStringList)));
    connect(&ltFilesManager, SIGNAL(ltFileObtained(QByteArray)), this, SLOT(ltFileObtained(QByteArray)));
    connect(&ltFilesManager, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(&ltFilesManager, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    connect(progress, SIGNAL(canceled()), &ltFilesManager, SLOT(cancel()));
}

LTFilesManagerDialog::~LTFilesManagerDialog()
{
    delete ui;
}

QString LTFilesManagerDialog::exec()
{
    currentFile = QString();
    downloadLTList();
    int ret = QDialog::exec();

    if (ret)
        return currentFile;

    return QString();
}

void LTFilesManagerDialog::downloadLTList()
{
    if (ltList.isEmpty())
    {
        getLTListFromDisk();
        updateTree(QStringList());

        progress->setMinimum(0);
        progress->setMaximum(0);
        progress->setWindowModality(Qt::WindowModal);
        progress->setLabelText("Updating database");
        progress->show();

        ltFilesManager.getLTList();
    }
}


void LTFilesManagerDialog::ltListObtained(QStringList list)
{
    if (progress->isVisible())
        progress->cancel();

    ltList.unite(list.toSet());

    updateTree(list);
}

void LTFilesManagerDialog::ltFileObtained(QByteArray buf)
{
    if (currentFile != "")
    {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(buf);

            QStringList arr = currentFile.split("/");
            QList<QTreeWidgetItem *> list = ui->treeWidget->findItems(arr.last(), Qt::MatchContains | Qt::MatchRecursive, 4);

            if (!list.isEmpty())
            {
                list.first()->setText(4, currentFile);
                list.first()->setText(5, "On disk / Online");
                list.first()->setTextColor(5, SeasonData::getInstance().getColor(LTPackets::GREEN));
            }

            QDialog::accept();
        }
        else
            QMessageBox::critical(this, "Error", "Could not save the file " + currentFile);
    }
}

void LTFilesManagerDialog::downloadProgress ( qint64 bytesReceived, qint64 bytesTotal )
{
    progress->setMaximum(bytesTotal);
    progress->setValue(bytesReceived);

    QStringList arr = progress->labelText().split("\n");
    if (!arr.isEmpty())
    {
        progress->setLabelText(arr[0] + "\n" + QString("%1 kB/%2 kB").arg((double)(bytesReceived/1024.0), 1, 'f', 2).arg((double)(bytesTotal/1024.0), 1, 'f', 2));
    }
}

void LTFilesManagerDialog::updateTree(const QStringList &onlineList)
{
    ui->treeWidget->clear();

    QList<QString> list = ltList.toList();
    qSort(list);

    QTreeWidgetItem *parent = 0;
    QListIterator<QString> iter(list);
    while (iter.hasNext())
    {
        QStringList array = parseEntry(iter.next(), onlineList);
        if (array.size() < 6)
            continue;

        QColor color = array[5].contains("On disk") ? SeasonData::getInstance().getColor(LTPackets::GREEN) : SeasonData::getInstance().getColor(LTPackets::CYAN);

//        QTreeWidgetItem *newParent = parent;
        if (!parent || parent->text(0) != array[0])
        {
            parent = new QTreeWidgetItem();
            parent->setText(0, array[0]);
            ui->treeWidget->addTopLevelItem(parent);
        }
        QTreeWidgetItem *child = new QTreeWidgetItem(parent);
        child->setText(0, array[0]);
        child->setText(1, array[1]);
        child->setText(2, array[2]);
        child->setText(3, array[3]);
        child->setText(4, array[4]);
        child->setText(5, array[5]);

        child->setTextAlignment(1, Qt::AlignCenter);
        child->setTextAlignment(3, Qt::AlignCenter);
        child->setTextAlignment(5, Qt::AlignCenter);

        child->setTextColor(5, color);
//        parent = parseEntry(parent, iter.next(), onlineList);
    }
}

QStringList LTFilesManagerDialog::parseEntry(QString entry, const QStringList &onlineList)
{
    QStringList array = entry.split("-");
    if (array.size() < 4)
        return QStringList();

    array[2] = SeasonData::getInstance().getEventNameFromShort(array[2]);
    array[3] = getSessionType(array[3]);

    QString file = entry;

    bool fExists = fileExists(file, false);
    array.append(file);

    QString onDisk = fExists ? "On disk" : "";

    if (fExists && onlineList.contains(entry))
        onDisk = "On disk / Online";

    else if (onlineList.contains(entry))
        onDisk = "Online";

    array.append(onDisk);


    return array;
}

QString LTFilesManagerDialog::getSessionType(QString session)
{
    QStringList array = session.split('.');
    if (array.size() < 2)
        return session.toUpper();

    return array[0].toUpper();
}

void LTFilesManagerDialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->childCount() == 0)
    {
        ui->playButton->setEnabled(true);
    }
    else
        ui->playButton->setEnabled(false);
}

void LTFilesManagerDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->childCount() == 0)
    {
        ui->playButton->setEnabled(true);
        on_playButton_clicked();
    }
    else
        ui->playButton->setEnabled(false);
}

void LTFilesManagerDialog::on_refreshButton_clicked()
{
    ltList.clear();
    downloadLTList();
}

void LTFilesManagerDialog::on_playButton_clicked()
{
    if (!ui->treeWidget->selectedItems().isEmpty())
    {
        QTreeWidgetItem *item = ui->treeWidget->selectedItems().first();

        QString fileName = item->text(4);
        if (!fileExists(fileName))
        {
            progress->setMinimum(0);
            progress->setMaximum(0);
            progress->setLabelText("Downloading file " + fileName + "\n 0/0");
            progress->setWindowModality(Qt::WindowModal);
            progress->show();

            ltFilesManager.getLTFile(fileName);
        }
        else
            QDialog::accept();
    }
}

bool LTFilesManagerDialog::fileExists(QString &file, bool saveCurrent)
{
    //check if file already contains absolute path
    if (QFile::exists(file))
    {
        if (saveCurrent)
            currentFile = file;

        return true;
    }


    //if not, check if ltdata dir exists
    QDir dir(F1LTCore::ltDataHomeDir());
    if (!dir.exists())
        return false;

    //try to find the file there
    if (QFile::exists(dir.absolutePath()+"/"+file))
    {
        if (saveCurrent)
            currentFile = dir.absolutePath()+"/"+file;

        file = dir.absolutePath()+"/"+file;
        return true;
    }

    //if it is still not found check in subdirs
    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        if (it.fileInfo().isDir())
        {
            if (QFile::exists(it.filePath() + "/" + file))
            {
                file = it.filePath()+"/"+file;
                if (saveCurrent)
                    currentFile = file;

                return true;
            }
        }
    }

    //if the file does not exist it needs to be downloaded and saved using this path
    if (saveCurrent)
        currentFile = dir.absolutePath()+"/"+file;

    return false;
}

void LTFilesManagerDialog::getLTListFromDisk()
{
    QDir dir(F1LTCore::ltDataHomeDir());
    if (!dir.exists())
        return;

    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        if (it.fileInfo().completeSuffix() == "lt")
            ltList.insert(it.fileName());
    }
}

void LTFilesManagerDialog::error(QNetworkReply::NetworkError code)
{
    currentFile = "";

    if (code != QNetworkReply::OperationCanceledError)
    {
        QMessageBox::critical(this, "Error", "Error downloading the file!\nCheck your network connection.");
    }
}

void LTFilesManagerDialog::loadSettings(QSettings *settings)
{
    restoreGeometry(settings->value("ui/ltfilesmanager_geometry").toByteArray());
    ui->treeWidget->header()->restoreState(settings->value("ui/ltfilesmanager_columns").toByteArray());
}

void LTFilesManagerDialog::saveSettings(QSettings *settings)
{
    settings->setValue("ui/ltfilesmanager_geometry", saveGeometry());
    settings->setValue("ui/ltfilesmanager_columns", ui->treeWidget->header()->saveState());
}
