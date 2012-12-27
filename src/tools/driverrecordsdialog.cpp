#include "driverrecordsdialog.h"
#include "ui_driverrecordsdialog.h"

DriverRecordsDialog::DriverRecordsDialog(QWidget *parent) :
    QDialog(parent/*, Qt::Window*/),
    ui(new Ui::DriverRecordsDialog)
{
    ui->setupUi(this);
}

DriverRecordsDialog::~DriverRecordsDialog()
{
    delete ui;
}

void DriverRecordsDialog::exec(const TrackRecordsAtom &records)
{
    for (int i = ui->tableWidget->rowCount()-1; i >= 0; --i)
        ui->tableWidget->removeRow(i);

    setWindowTitle("Driver records: " + records.trackName);

    driverRecords = records;

    loadRecords();

    QDialog::show();
}

void DriverRecordsDialog::loadRecords()
{
    int currentIndex = ui->comboBox->currentIndex();
    for (int i = 0; i < driverRecords.driverRecords.size(); ++i)
    {
        ui->tableWidget->insertRow(i);

        QTableWidgetItem *item = new QTableWidgetItem(driverRecords.driverRecords[i].driver);
        item->setForeground(SeasonData::getInstance().getColor(LTPackets::WHITE));
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem(driverRecords.driverRecords[i].team);
        item->setForeground(SeasonData::getInstance().getColor(LTPackets::WHITE));
        ui->tableWidget->setItem(i, 1, item);

        QColor color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

        Record rec;

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(S1_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][S1_RECORD];


        if (rec.time == driverRecords.sessionRecords[S1_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[S1_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        QString text = rec.time;
        if (currentIndex == 5)
            text += " (" + rec.session + ")";

        item = new QTableWidgetItem(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 2, item);

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(S2_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][S2_RECORD];

        if (rec.time == driverRecords.sessionRecords[S2_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[S2_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        else
            color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

        text = rec.time;
        if (currentIndex == 5)
            text += " (" + rec.session + ")";

        item = new QTableWidgetItem(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, item);

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(S3_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][S3_RECORD];


        if (rec.time == driverRecords.sessionRecords[S3_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[S3_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        else
            color = SeasonData::getInstance().getColor(LTPackets::YELLOW);

        text = rec.time;
        if (currentIndex == 5)
            text += " (" + rec.session + ")";

        item = new QTableWidgetItem(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 4, item);

        if (currentIndex == 5)
            rec = driverRecords.driverRecords[i].getWeekendRecord(TIME_RECORD);
        else
            rec = driverRecords.driverRecords[i].sessionRecords[ui->comboBox->currentIndex()][TIME_RECORD];


        if (rec.time == driverRecords.sessionRecords[TIME_RECORD].time && driverRecords.driverRecords[i].driver == driverRecords.sessionRecords[TIME_RECORD].driver)
            color = SeasonData::getInstance().getColor(LTPackets::VIOLET);

        else
            color = SeasonData::getInstance().getColor(LTPackets::GREEN);

        text = rec.time;
        if (currentIndex == 5)
            text += " (" + rec.session + ")";

        item = new QTableWidgetItem(text);
        item->setForeground(color);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 5, item);
    }
}


void DriverRecordsDialog::setFont(const QFont &font)
{
    ui->tableWidget->setFont(font);
}

void DriverRecordsDialog::saveSettings(QSettings &settings)
{
    settings.setValue("ui/driver_records_geometry", saveGeometry());
    settings.setValue("ui/driver_records_table", ui->tableWidget->saveGeometry());
    settings.setValue("ui/driver_records_session", ui->comboBox->currentIndex());

    QList<QVariant> list;
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
        list.append(ui->tableWidget->columnWidth(i));

    settings.setValue("ui/driver_records_columns", list);
}

void DriverRecordsDialog::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/driver_records_geometry").toByteArray());
    ui->tableWidget->restoreGeometry(settings.value("ui/driver_records_table").toByteArray());
    ui->comboBox->setCurrentIndex(settings.value("ui/driver_records_session", 5).toInt());

    QList<QVariant> list;
    list = settings.value("ui/driver_records_columns").toList();

    if (list.size() > 0 && list[0].toInt() > 0)
    {
        for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
            ui->tableWidget->setColumnWidth(i, list[i].toInt());
    }
}

void DriverRecordsDialog::on_comboBox_currentIndexChanged(int index)
{
    for (int i = ui->tableWidget->rowCount()-1; i >= 0; --i)
        ui->tableWidget->removeRow(i);
    loadRecords();
}
