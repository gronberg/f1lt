#include "delaywidget.h"
#include "ui_delaywidget.h"

#include <QDebug>

DelayWidget::DelayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DelayWidget), delay(0)
{
    ui->setupUi(this);
    ui->syncLabel->setVisible(false);
//    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(delayChanged(int)));
}

DelayWidget::~DelayWidget()
{
    delete ui;
}


void DelayWidget::on_spinBox_valueChanged(int arg1)
{
    qDebug() << "DELAYWIDGET" << delay << arg1;

    int prevDelay = delay;
    delay = arg1;

    emit delayChanged(prevDelay, delay);
}

void DelayWidget::synchronizingTimer(bool sync)
{
    ui->syncLabel->setVisible(sync);
}
