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


#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include <iostream>
#include <QColorDialog>
#include <QFontDialog>

#include "../core/colorsmanager.h"
#include "../net/networksettings.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
//    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "pieczar", "F1LT", this);
    settings = new QSettings("f1lt.ini", QSettings::IniFormat, this);

    dcDialog = new DriverColorsDialog(this);
}

PreferencesDialog::~PreferencesDialog()
{
//    std::cout<<settings->fileName().toStdString()<<std::endl;
    delete ui;
}

int PreferencesDialog::exec(QSettings *set)
{
    colors = ColorsManager::getInstance().getColors();
    setButtonColor(ui->colorGreenButton, colors[LTPackets::GREEN]);
    setButtonColor(ui->colorWhiteButton, colors[LTPackets::WHITE]);
    setButtonColor(ui->colorVioletButton, colors[LTPackets::VIOLET]);
    setButtonColor(ui->colorRedButton, colors[LTPackets::RED]);
    setButtonColor(ui->colorYellowButton, colors[LTPackets::YELLOW]);
    setButtonColor(ui->colorCyanButton, colors[LTPackets::CYAN]);

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
    setAutoRecord(settings->value("ui/auto_record").toBool());    
    setDrawCarThumbnails(settings->value("ui/car_thumbnails").toBool());
    setDrawTrackerClassification(settings->value("ui/draw_tracker_classification").toBool());

    setAutoStopRecord(settings->value("ui/auto_stop_record", -1).toInt());
    setAutoSaveRecord(settings->value("ui/auto_save_record", -1).toInt());


    QNetworkProxy proxy = NetworkSettings::getInstance().getProxy();

    ui->proxyHostEdit->setText(proxy.hostName());
    ui->proxyPortEdit->setValue(proxy.port());
    ui->proxyUserEdit->setText(proxy.user());
    ui->proxyPasswordEdit->setText(proxy.password());

    switch (proxy.type())
    {
        case QNetworkProxy::HttpCachingProxy: ui->proxyTypeBox->setCurrentIndex(0); break;
        case QNetworkProxy::Socks5Proxy: ui->proxyTypeBox->setCurrentIndex(1); break;
        default: break;
    }

    ui->proxyCheckBox->setChecked(NetworkSettings::getInstance().usingProxy());

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
    settings->setValue("ui/car_thumbnails", drawCarThumbnails());

    settings->setValue("ui/auto_record", isAutoRecord());    
    settings->setValue("ui/auto_stop_record", getAutoStopRecord());
    settings->setValue("ui/auto_save_record", getAutoSaveRecord());

    settings->setValue("ui/auto_connect", isAutoConnect());
    settings->setValue("ui/draw_tracker_classification", drawTrackerClassification());

    ColorsManager::getInstance().setColors(colors);
}

QNetworkProxy PreferencesDialog::getProxy()
{
    QNetworkProxy proxy;

//    if (ui->proxyCheckBox->isChecked())
    {
        proxy.setHostName(ui->proxyHostEdit->text());
        proxy.setUser(ui->proxyUserEdit->text());
        proxy.setPassword(ui->proxyPasswordEdit->text());
        proxy.setPort(ui->proxyPortEdit->value());

        switch (ui->proxyTypeBox->currentIndex())
        {
            case 0: proxy.setType(QNetworkProxy::HttpCachingProxy); break;
            case 1: proxy.setType(QNetworkProxy::Socks5Proxy); break;
        }
    }

    return proxy;
}

bool PreferencesDialog::proxyEnabled()
{
    return ui->proxyCheckBox->isChecked();
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


bool PreferencesDialog::drawCarThumbnails()
{
    return ui->thumbnailsCheckBox->isChecked();
}

void PreferencesDialog::setDrawCarThumbnails(bool val)
{
    ui->thumbnailsCheckBox->setChecked(val);
}


void PreferencesDialog::setAutoRecord(bool val)
{
    ui->autoRecordBox->setChecked(val);
}

bool PreferencesDialog::isAutoRecord()
{
    return ui->autoRecordBox->isChecked();
}

void PreferencesDialog::setAutoStopRecord(int val)
{
    if (val >= 0)
    {
        ui->autoStopRecordSpinBox->setEnabled(true);
        ui->autoStopRecordBox->setChecked(true);
        ui->autoStopRecordSpinBox->setValue(val);
    }
    else
    {
        ui->autoStopRecordSpinBox->setEnabled(false);
        ui->autoStopRecordBox->setChecked(false);
    }
}

int PreferencesDialog::getAutoStopRecord()
{
    if (!ui->autoStopRecordBox->isChecked())
        return -1;

    else
        return ui->autoStopRecordSpinBox->value();
}

void PreferencesDialog::setAutoSaveRecord(int val)
{
    if (val >= 0)
    {
        ui->autoSaveRecordSpinBox->setEnabled(true);
        ui->autoSaveRecordBox->setChecked(true);
        ui->autoSaveRecordSpinBox->setValue(val);
    }
    else
    {
        ui->autoSaveRecordSpinBox->setEnabled(false);
        ui->autoSaveRecordBox->setChecked(false);
    }
}

int PreferencesDialog::getAutoSaveRecord()
{
    if (!ui->autoSaveRecordBox->isChecked())
        return -1;

    else
        return ui->autoSaveRecordSpinBox->value();
}

void PreferencesDialog::on_autoStopRecordBox_toggled(bool checked)
{
    ui->autoStopRecordSpinBox->setEnabled(checked);
}

void PreferencesDialog::setAutoConnect(bool val)
{
	ui->autoConnectBox->setChecked(val);
}
bool PreferencesDialog::isAutoConnect()
{
	return ui->autoConnectBox->isChecked();
}

bool PreferencesDialog::drawTrackerClassification()
{
    return ui->trackerBox->isChecked();
}

void PreferencesDialog::setDrawTrackerClassification(bool val)
{
    ui->trackerBox->setChecked(val);
}

void PreferencesDialog::on_proxyCheckBox_toggled(bool checked)
{
    ui->proxyBox->setEnabled(checked);
}

void PreferencesDialog::on_pushButton_clicked()
{
    colors[LTPackets::WHITE] = ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE);
    setButtonColor(ui->colorWhiteButton, colors[LTPackets::WHITE]);
}

void PreferencesDialog::on_pushButton_2_clicked()
{
    colors[LTPackets::CYAN] = ColorsManager::getInstance().getDefaultColor(LTPackets::CYAN);
    setButtonColor(ui->colorCyanButton, colors[LTPackets::CYAN]);
}

void PreferencesDialog::on_pushButton_3_clicked()
{
    colors[LTPackets::YELLOW] = ColorsManager::getInstance().getDefaultColor(LTPackets::YELLOW);
    setButtonColor(ui->colorYellowButton, colors[LTPackets::YELLOW]);
}

void PreferencesDialog::on_pushButton_4_clicked()
{
    colors[LTPackets::PIT] = ColorsManager::getInstance().getDefaultColor(LTPackets::PIT);
    colors[LTPackets::RED] = ColorsManager::getInstance().getDefaultColor(LTPackets::RED);
    setButtonColor(ui->colorRedButton, colors[LTPackets::PIT]);
}

void PreferencesDialog::on_pushButton_5_clicked()
{
    colors[LTPackets::GREEN] = ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN);
    setButtonColor(ui->colorGreenButton, colors[LTPackets::GREEN]);
}

void PreferencesDialog::on_pushButton_6_clicked()
{
    colors[LTPackets::VIOLET] = ColorsManager::getInstance().getDefaultColor(LTPackets::VIOLET);
    setButtonColor(ui->colorVioletButton, colors[LTPackets::VIOLET]);
}

void PreferencesDialog::on_colorWhiteButton_clicked()
{
    QColor color = QColorDialog::getColor(colors[LTPackets::WHITE], this);
    if (color.isValid())
    {
        colors[LTPackets::WHITE] = color;
        setButtonColor(ui->colorWhiteButton, color);
    }
}

void PreferencesDialog::on_colorCyanButton_clicked()
{
    QColor color = QColorDialog::getColor(colors[LTPackets::CYAN], this);
    if (color.isValid())
    {
        colors[LTPackets::CYAN] = color;
        setButtonColor(ui->colorCyanButton, color);
    }
}

void PreferencesDialog::on_colorYellowButton_clicked()
{
    QColor color = QColorDialog::getColor(colors[LTPackets::YELLOW], this);
    if (color.isValid())
    {
        colors[LTPackets::YELLOW] = color;
        setButtonColor(ui->colorYellowButton, color);
    }
}

void PreferencesDialog::on_colorRedButton_clicked()
{
    QColor color = QColorDialog::getColor(colors[LTPackets::PIT], this);
    if (color.isValid())
    {
        colors[LTPackets::PIT] = color;
        colors[LTPackets::RED] = color;
        setButtonColor(ui->colorRedButton, color);
    }
}

void PreferencesDialog::on_colorGreenButton_clicked()
{
    QColor color = QColorDialog::getColor(colors[LTPackets::GREEN], this);
    if (color.isValid())
    {
        colors[LTPackets::GREEN] = color;
        setButtonColor(ui->colorGreenButton, color);
    }
}

void PreferencesDialog::on_colorVioletButton_clicked()
{
    QColor color = QColorDialog::getColor(colors[LTPackets::VIOLET], this);
    if (color.isValid())
    {
        colors[LTPackets::VIOLET] = color;
        setButtonColor(ui->colorVioletButton, color);
    }
}

void PreferencesDialog::setButtonColor(QToolButton *button, QColor color)
{
    QString styleSheet = "background-color: rgb(" + QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()) + ");\n     "\
            "border-style: solid;\n     "\
            "border-width: 1px;\n     "\
            "border-radius: 3px;\n     "\
            "border-color: rgb(153, 153, 153);\n     "\
            "padding: 3px;\n";

    button->setStyleSheet(styleSheet);
}


void PreferencesDialog::on_pushButton_7_clicked()
{
    colors = ColorsManager::getInstance().getDefaultColors();
    setButtonColor(ui->colorWhiteButton, colors[LTPackets::WHITE]);
    setButtonColor(ui->colorCyanButton, colors[LTPackets::CYAN]);
    setButtonColor(ui->colorYellowButton, colors[LTPackets::YELLOW]);
    setButtonColor(ui->colorRedButton, colors[LTPackets::RED]);
    setButtonColor(ui->colorGreenButton, colors[LTPackets::GREEN]);
    setButtonColor(ui->colorVioletButton, colors[LTPackets::VIOLET]);
}

void PreferencesDialog::on_pushButton_8_clicked()
{
    if (dcDialog->exec())
        emit driversColorsChanged();
}

void PreferencesDialog::on_autoSaveRecordBox_toggled(bool checked)
{
    ui->autoSaveRecordSpinBox->setEnabled(checked);
}
