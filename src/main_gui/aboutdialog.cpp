#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "../core/f1ltcore.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    loadChangelog();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::loadChangelog()
{
    QFile file(":/files/CHANGELOG");
    if (file.open(QIODevice::ReadOnly))
    {
        ui->textEdit->setText("CHANGELOG:\n");
        ui->textEdit->append(file.readAll());
    }
}
