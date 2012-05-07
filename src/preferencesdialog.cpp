#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include <iostream>
#include <QFontDialog>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
//    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "pieczar", "F1LT", this);
    settings = new QSettings("f1lt.ini", QSettings::IniFormat, this);
}

PreferencesDialog::~PreferencesDialog()
{
//    std::cout<<settings->fileName().toStdString()<<std::endl;
    delete ui;
}

int PreferencesDialog::exec(QSettings *set)
{
    settings = set;

    QString sbuf = settings->value("fonts/main_family").toString();
    mainFont.setFamily(sbuf);

    int ibuf = settings->value("fonts/main_size").toInt();
    mainFont.setPointSize(ibuf);

    ibuf = settings->value("fonts/main_weight").toInt();
    mainFont.setWeight(ibuf);

    bool bbuf = settings->value("fonts/main_italic").toBool();
    mainFont.setItalic(bbuf);

    sbuf = settings->value("fonts/commentary_family").toString();
    commentaryFont.setFamily(sbuf);

    ibuf = settings->value("fonts/commentary_size").toInt();
    commentaryFont.setPointSize(ibuf);

    ibuf = settings->value("fonts/commentary_weight").toInt();
    commentaryFont.setWeight(ibuf);

    bbuf = settings->value("fonts/commentary_italic").toBool();
    commentaryFont.setItalic(bbuf);

    setSplitterOpaqueResize(settings->value("ui/ltresize").toBool());
    setAlternatingRowColors(settings->value("ui/alt_colors").toBool());
    setAutoRecord(settings->value("ui/auto_record").toBool());

    setReverseOrderLapHistory(settings->value("ui/reversed_lap_history").toBool());
    setReverseOrderHeadToHead(settings->value("ui/reversed_head_to_head").toBool());
    setReverseOrderLapTimeComparison(settings->value("ui/reversed_lap_time_comparison").toBool());

    return QDialog::exec();
}

void PreferencesDialog::on_buttonBox_accepted()
{
    settings->setValue("fonts/main_family", mainFont.family());
    settings->setValue("fonts/main_size", QString::number(mainFont.pointSize()));
    settings->setValue("fonts/main_weight", QString::number(mainFont.weight()));
    settings->setValue("fonts/main_italic", QString::number(mainFont.italic()));

    settings->setValue("fonts/commentary_family", commentaryFont.family());
    settings->setValue("fonts/commentary_size", QString::number(commentaryFont.pointSize()));
    settings->setValue("fonts/commentary_weight", QString::number(commentaryFont.weight()));
    settings->setValue("fonts/commentary_italic", QString::number(commentaryFont.italic()));

    settings->setValue("ui/ltresize", isSplitterOpaqueResize());
    settings->setValue("ui/alt_colors", isAlternatingRowColors());

    settings->setValue("ui/reversed_lap_history", isReverseOrderLapHistory());
    settings->setValue("ui/reversed_head_to_head", isReverseOrderHeadToHead());
    settings->setValue("ui/reversed_lap_time_comparison", isReverseOrderLapTimeComparison());
    settings->setValue("ui/auto_record", isAutoRecord());
}

void PreferencesDialog::setFonts(const QFont &f1, const QFont &f2)
{
    mainFont = f1;
    ui->mainFontButton->setText(QString("%1, %2").arg(mainFont.family()).arg(mainFont.pointSize()));
    ui->mainFontButton->setFont(mainFont);

    commentaryFont = f2;
    ui->commentaryFontButton->setText(QString("%1, %2").arg(commentaryFont.family()).arg(commentaryFont.pointSize()));
    ui->commentaryFontButton->setFont(commentaryFont);
}

void PreferencesDialog::on_mainFontButton_clicked()
{
    bool ok = false;
    mainFont = QFontDialog::getFont(&ok, mainFont, this);

    ui->mainFontButton->setText(QString("%1, %2").arg(mainFont.family()).arg(mainFont.pointSize()));
}

void PreferencesDialog::on_commentaryFontButton_clicked()
{
    bool ok = false;
    commentaryFont = QFontDialog::getFont(&ok, commentaryFont, this);

    ui->commentaryFontButton->setText(QString("%1, %2").arg(commentaryFont.family()).arg(commentaryFont.pointSize()));
}

void PreferencesDialog::setMainFont(const QFont &f)
{
    ui->mainFontButton->setFont(f);
    mainFont = f;
}
void PreferencesDialog::setCommentaryFont(const QFont &f)
{
    ui->commentaryFontButton->setFont(f);
    commentaryFont = f;
}

bool PreferencesDialog::isSplitterOpaqueResize()
{
    return ui->ltCheckBox->isChecked();
}

void PreferencesDialog::setSplitterOpaqueResize(bool val)
{
    ui->ltCheckBox->setChecked(val);
}

bool PreferencesDialog::isAlternatingRowColors()
{
    return ui->altCheckBox->isChecked();
}

void PreferencesDialog::setAlternatingRowColors(bool val)
{
    ui->altCheckBox->setChecked(val);
}


void PreferencesDialog::setReverseOrderLapHistory(bool val)
{
    ui->revLHBox->setChecked(val);
}

bool PreferencesDialog::isReverseOrderLapHistory()
{
    return ui->revLHBox->isChecked();
}

void PreferencesDialog::setReverseOrderHeadToHead(bool val)
{
    ui->revH2HBox->setChecked(val);
}

bool PreferencesDialog::isReverseOrderHeadToHead()
{
    return ui->revH2HBox->isChecked();
}

void PreferencesDialog::setReverseOrderLapTimeComparison(bool val)
{
    ui->revLTCBox->setChecked(val);
}

bool PreferencesDialog::isReverseOrderLapTimeComparison()
{
    return ui->revLTCBox->isChecked();
}

void PreferencesDialog::setAutoRecord(bool val)
{
    ui->autoRecordBox->setChecked(val);
}

bool PreferencesDialog::isAutoRecord()
{
    return ui->autoRecordBox->isChecked();
}
