#include "trackrecordsdialog.h"
#include "ui_trackrecordsdialog.h"

#include "../core/trackrecords.h"

TrackRecordsDialog::TrackRecordsDialog(QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::TrackRecordsDialog), currentIndex(-1)
{
    ui->setupUi(this);
    drDialog = new DriverRecordsDialog(this);
}

TrackRecordsDialog::~TrackRecordsDialog()
{
    delete ui;
}

void TrackRecordsDialog::exec()
{
    TrackRecords &tr = TrackRecords::getInstance();
    if (ui->listWidget->count() != tr.getTrackRecords().size())
    {
        for (int i = 0; i < tr.getTrackRecords().size(); ++i)
        {
            QListWidgetItem *item = ui->listWidget->item(i);
            if (!item)
            {
                item = new QListWidgetItem();
                ui->listWidget->insertItem(i, item);
            }
            item->setText(tr[i].trackName);
        }

        for (int i = ui->listWidget->count() - 1; i >= tr.getTrackRecords().size(); --i)
        {
            QListWidgetItem *item = ui->listWidget->item(i);
            ui->listWidget->removeItemWidget(item);
        }
        loadTrackRecords();
    }
    QDialog::show();
}

void TrackRecordsDialog::loadTrackRecords()
{
    if (currentIndex == -1)
        return;

    TrackRecordsAtom tr = TrackRecords::getInstance()[currentIndex];

    if (tr != TrackRecords::null())
    {
        ui->trackNameLabel->setText(tr.trackName);
        ui->trackMapLabel->setPixmap(tr.trackMap);

        ui->qRTLabel->setText(tr.trackRecords[QUALI_RECORD].time.toString());
        ui->qRDLabel->setText(tr.trackRecords[QUALI_RECORD].driver);
        ui->qRDTLabel->setText("("+tr.trackRecords[QUALI_RECORD].team+")");
        ui->qRYLabel->setText(tr.trackRecords[QUALI_RECORD].year);

        ui->rRTLabel->setText(tr.trackRecords[RACE_RECORD].time.toString());
        ui->rRDLabel->setText(tr.trackRecords[RACE_RECORD].driver);
        ui->rRDTLabel->setText("("+tr.trackRecords[RACE_RECORD].team+")");
        ui->rRYLabel->setText(tr.trackRecords[RACE_RECORD].year);

        ui->s1TLabel->setText(tr.sessionRecords[S1_RECORD].time.toString());
        ui->s1DLabel->setText(tr.sessionRecords[S1_RECORD].driver);
        ui->s1DTLabel->setText(tr.sessionRecords[S1_RECORD].team);
        ui->s1SLabel->setText(tr.sessionRecords[S1_RECORD].session);

        ui->s2TLabel->setText(tr.sessionRecords[S2_RECORD].time.toString());
        ui->s2DLabel->setText(tr.sessionRecords[S2_RECORD].driver);
        ui->s2DTLabel->setText(tr.sessionRecords[S2_RECORD].team);
        ui->s2SLabel->setText(tr.sessionRecords[S2_RECORD].session);

        ui->s3TLabel->setText(tr.sessionRecords[S3_RECORD].time.toString());
        ui->s3DLabel->setText(tr.sessionRecords[S3_RECORD].driver);
        ui->s3DTLabel->setText(tr.sessionRecords[S3_RECORD].team);
        ui->s3SLabel->setText(tr.sessionRecords[S3_RECORD].session);

        ui->tTLabel->setText(tr.sessionRecords[TIME_RECORD].time.toString());
        ui->tDLabel->setText(tr.sessionRecords[TIME_RECORD].driver);
        ui->tDTLabel->setText(tr.sessionRecords[TIME_RECORD].team);
        ui->tSLabel->setText(tr.sessionRecords[TIME_RECORD].session);
    }
}

void TrackRecordsDialog::on_listWidget_clicked(const QModelIndex &index)
{
    currentIndex = index.row();
    loadTrackRecords();
}

void TrackRecordsDialog::on_toolButton_clicked()
{
    if (currentIndex > 0)
    {
        --currentIndex;
        ui->listWidget->setCurrentRow(currentIndex);
        loadTrackRecords();
    }
}

void TrackRecordsDialog::on_toolButton_2_clicked()
{
    if (currentIndex < TrackRecords::getInstance().getTrackRecords().size() - 1)
    {
        ++currentIndex;
        ui->listWidget->setCurrentRow(currentIndex);
        loadTrackRecords();
    }
}

void TrackRecordsDialog::saveSettings(QSettings &settings)
{
    settings.setValue("ui/trackrecords_geometry", saveGeometry());
    settings.setValue("ui/trackrecords_splitter", ui->splitter->saveState());

    drDialog->saveSettings(settings);

}

void TrackRecordsDialog::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/trackrecords_geometry").toByteArray());
    ui->splitter->restoreState(settings.value("ui/trackrecords_splitter").toByteArray());

    drDialog->loadSettings(settings);
}

void TrackRecordsDialog::setFont(const QFont &font)
{
    for (int i = 0; i < ui->gridLayout->count(); ++i)
    {
        ui->gridLayout->itemAt(i)->widget()->setFont(font);
    }
    for (int i = 0; i < ui->gridLayout_2->count(); ++i)
    {
        ui->gridLayout_2->itemAt(i)->widget()->setFont(font);
    }

    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);

    drDialog->setFont(font);
}

void TrackRecordsDialog::on_pushButton_clicked()
{
    if (currentIndex == -1)
        return;

    TrackRecords::getInstance().gatherSessionRecords(true);
    drDialog->exec(TrackRecords::getInstance()[currentIndex]);
}
