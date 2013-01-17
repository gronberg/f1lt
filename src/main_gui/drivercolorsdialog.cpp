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


#include "drivercolorsdialog.h"
#include "ui_drivercolorsdialog.h"

#include <QColorDialog>
#include <QDebug>
#include <QLabel>
#include <QToolButton>
#include "../core/eventdata.h"

DriverColorsDialog::DriverColorsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DriverColorsDialog)
{
    ui->setupUi(this);
}

DriverColorsDialog::~DriverColorsDialog()
{
    delete ui;
}

int DriverColorsDialog::exec()
{
    SeasonData &sd = SeasonData::getInstance();
    colors = sd.getDriverColors();
    int i = 0;
    for (; i < sd.getTeams().size(); ++i)
    {
        if ((ui->verticalLayout->count()-2) <= i)
        {
            QHBoxLayout *layout = new QHBoxLayout();
            QLabel *label = new QLabel(this);
            label->setText(QString("%1 %2").arg(sd.getTeams()[i].driver1No).arg(sd.getTeams()[i].driver1Name));
            label->updateGeometry();
            QToolButton *button = new QToolButton(this);
            button->setMaximumHeight(16);
            button->setMaximumWidth(16);
            setButtonColor(button, /*sd.getCarColor(sd.getTeams()[i].driver1No)*/colors[i*2]);
            layout->addWidget(button);
            connect(button, SIGNAL(clicked()), this, SLOT(onColorButtonClicked()));

            button = new QToolButton(this);
            button->setText("Reset");
            button->setMaximumHeight(20);
            layout->addWidget(button);
            connect(button, SIGNAL(clicked()), this, SLOT(onResetButtonClicked()));

            layout->addWidget(label);
            //colors.append(sd.getCarColor(sd.getTeams()[i].driver1No));

//            layout = new QHBoxLayout(this);
            label = new QLabel(this);
            label->setText(QString("%1 %2").arg(sd.getTeams()[i].driver2No).arg(sd.getTeams()[i].driver2Name));
            label->updateGeometry();
            button = new QToolButton(this);
            button->setMaximumHeight(16);
            button->setMaximumWidth(16);
            setButtonColor(button, /*sd.getCarColor(sd.getTeams()[i].driver2No)*/colors[i*2+1]);
            layout->addWidget(button);
            connect(button, SIGNAL(clicked()), this, SLOT(onColorButtonClicked()));

            button = new QToolButton(this);
            button->setText("Reset");
            button->setMaximumHeight(20);
            layout->addWidget(button);
            connect(button, SIGNAL(clicked()), this, SLOT(onResetButtonClicked()));

            layout->addWidget(label);
            //colors.append(sd.getCarColor(sd.getTeams()[i].driver2No));

            ui->verticalLayout->insertLayout(ui->verticalLayout->count() - 2, layout);
        }
        else
        {
            QHBoxLayout *layout = static_cast<QHBoxLayout*>(ui->verticalLayout->itemAt(i)->layout());
            QLabel *label = static_cast<QLabel*>(layout->itemAt(2)->widget());
            QToolButton *button = static_cast<QToolButton*>(layout->itemAt(0)->widget());

            label->setText(QString("%1 %2").arg(sd.getTeams()[i].driver1No).arg(sd.getTeams()[i].driver1Name));
            setButtonColor(button, sd.getCarColor(sd.getTeams()[i].driver1No));

            label->setVisible(true);
            button->setVisible(true);

            button = static_cast<QToolButton*>(layout->itemAt(1)->widget());
            button->setVisible(true);

            label = static_cast<QLabel*>(layout->itemAt(5)->widget());
            button = static_cast<QToolButton*>(layout->itemAt(3)->widget());

            label->setText(QString("%1 %2").arg(sd.getTeams()[i].driver2No).arg(sd.getTeams()[i].driver2Name));
            setButtonColor(button, sd.getCarColor(sd.getTeams()[i].driver2No));

            label->setVisible(true);
            button->setVisible(true);

            button = static_cast<QToolButton*>(layout->itemAt(4)->widget());
            button->setVisible(true);
        }
    }
    for (; i < ui->verticalLayout->count()-2; ++i)
    {
        QHBoxLayout *layout = static_cast<QHBoxLayout*>(ui->verticalLayout->itemAt(i)->layout());
        QLabel *label = static_cast<QLabel*>(layout->itemAt(2)->widget());
        QToolButton *button = static_cast<QToolButton*>(layout->itemAt(0)->widget());

        label->setVisible(false);
        button->setVisible(false);

        button = static_cast<QToolButton*>(layout->itemAt(1)->widget());
        button->setVisible(false);

        label = static_cast<QLabel*>(layout->itemAt(5)->widget());
        button = static_cast<QToolButton*>(layout->itemAt(3)->widget());

        label->setVisible(false);
        button->setVisible(false);

        button = static_cast<QToolButton*>(layout->itemAt(4)->widget());
        button->setVisible(false);
    }

    return QDialog::exec();
}

void DriverColorsDialog::onColorButtonClicked()
{
    QToolButton *button = static_cast<QToolButton*>(QObject::sender());

    for (int i = 0; i < ui->verticalLayout->count()-2; ++i)
    {
        QHBoxLayout *layout = static_cast<QHBoxLayout*>(ui->verticalLayout->itemAt(i)->layout());
        QToolButton *colorButton = static_cast<QToolButton*>(layout->itemAt(0)->widget());

        if (button == colorButton)
        {
            QColor color = QColorDialog::getColor(colors[i * 2], this);

            if (color.isValid())
            {
                setButtonColor(colorButton, color);
                colors[i * 2] = color;
            }

            return;
        }

        colorButton = static_cast<QToolButton*>(layout->itemAt(3)->widget());
        if (button == colorButton)
        {
            QColor color = QColorDialog::getColor(colors[i * 2 + 1], this);

            if (color.isValid())
            {
                setButtonColor(colorButton, color);
                colors[i * 2 + 1] = color;
            }

            return;
        }
    }
}

void DriverColorsDialog::onResetButtonClicked()
{
    QToolButton *button = static_cast<QToolButton*>(QObject::sender());

    for (int i = 0; i < ui->verticalLayout->count()-2; ++i)
    {
        QHBoxLayout *layout = static_cast<QHBoxLayout*>(ui->verticalLayout->itemAt(i)->layout());
        QToolButton *resetButton = static_cast<QToolButton*>(layout->itemAt(1)->widget());
        QToolButton *colorButton = static_cast<QToolButton*>(layout->itemAt(0)->widget());

        if (button == resetButton)
        {
            setButtonColor(colorButton, SeasonData::getInstance().getDefaultDriverColors()[i * 2]);
            colors[i * 2] = SeasonData::getInstance().getDefaultDriverColors()[i * 2];

            return;
        }

        resetButton = static_cast<QToolButton*>(layout->itemAt(4)->widget());
        colorButton = static_cast<QToolButton*>(layout->itemAt(3)->widget());
        if (button == resetButton)
        {
            setButtonColor(colorButton, SeasonData::getInstance().getDefaultDriverColors()[i * 2 + 1]);
            colors[i * 2 + 1] = SeasonData::getInstance().getDefaultDriverColors()[i * 2 + 1];

            return;
        }
    }
}

void DriverColorsDialog::setButtonColor(QToolButton *button, QColor color)
{
    QString styleSheet = "background-color: rgb(" + QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()) + ");\n     "\
            "border-style: solid;\n     "\
            "border-width: 1px;\n     "\
            "border-radius: 3px;\n     "\
            "border-color: rgb(153, 153, 153);\n     "\
            "padding: 3px;\n";

    button->setStyleSheet(styleSheet);
}

void DriverColorsDialog::on_buttonBox_accepted()
{
    SeasonData::getInstance().setDriverColors(colors);
    SeasonData::getInstance().getHelmetsFactory().reloadHelmets();
}

void DriverColorsDialog::on_pushButton_clicked()
{
    for (int i = 0; i < ui->verticalLayout->count()-2; ++i)
    {
        QHBoxLayout *layout = static_cast<QHBoxLayout*>(ui->verticalLayout->itemAt(i)->layout());
        QToolButton *colorButton = static_cast<QToolButton*>(layout->itemAt(0)->widget());

        setButtonColor(colorButton, SeasonData::getInstance().getDefaultDriverColors()[i * 2]);
        colors[i * 2] = SeasonData::getInstance().getDefaultDriverColors()[i * 2];

        colorButton = static_cast<QToolButton*>(layout->itemAt(3)->widget());

        setButtonColor(colorButton, SeasonData::getInstance().getDefaultDriverColors()[i * 2 + 1]);
        colors[i * 2 + 1] = SeasonData::getInstance().getDefaultDriverColors()[i * 2 + 1];
    }
}
