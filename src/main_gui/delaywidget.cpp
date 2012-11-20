#include "delaywidget.h"
#include "ui_delaywidget.h"

DelayWidget::DelayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DelayWidget)
{
    ui->setupUi(this);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(delayChanged(int)));
}

DelayWidget::~DelayWidget()
{
    delete ui;
}
