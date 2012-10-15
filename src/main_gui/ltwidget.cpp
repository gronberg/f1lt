#include "ltwidget.h"
#include "ui_ltwidget.h"

#include <QDebug>
#include <QPainter>

#include "models/practicemodel.h"
#include "models/qualimodel.h"
#include "models/racemodel.h"

LTWidget::LTWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LTWidget),
    eventData(EventData::getInstance()),
    eventType((LTPackets::EventType)0),
    currDriverId(0),
    ltModel(0),
    thumbnailsSize(75)
{
    ui->setupUi(this);
    loadCarImages();
    itemDelegate = new LTMainItemDelegate(ui->tableView, drawCarThumbnails, thumbnailsSize);
    ui->tableView->setItemDelegate(itemDelegate);        
    //connect(ui->tableView, SIGNAL(headerClicked(int)), this, SLOT(on_tableView_headerClicked(int)));
}

LTWidget::~LTWidget()
{
    if (ltModel)
        delete ltModel;
    delete ui;
}

void LTWidget::clearData()
{
    if (ltModel)
    {
        delete ltModel;
        ltModel = 0;
    }
}

void LTWidget::loadCarImages()
{
    showDiff = 0;
    currDriverId = 0;

    SeasonData::getInstance().getCarThumbnailsFactory().loadCarThumbnails(thumbnailsSize);
}

void LTWidget::setFont(const QFont &font)
{
    ui->tableView->setFont(font);
    QWidget::setFont(font);
}

void LTWidget::updateLT()
{
    if (eventType != eventData.getEventType() || !ltModel)
    {
        if (ltModel)
            delete ltModel;

        if (eventData.getEventType() == LTPackets::PRACTICE_EVENT)
        {
            ltModel = new PracticeModel(this);
            ui->tableView->setModel(ltModel);
        }
        else if (eventData.getEventType() == LTPackets::QUALI_EVENT)
        {
            ltModel = new QualiModel(this);
            ui->tableView->setModel(ltModel);
        }
        else if (eventData.getEventType() == LTPackets::RACE_EVENT)
        {
            ltModel = new RaceModel(this);
            ui->tableView->setModel(ltModel);
        }
        resizeEvent(0);
        eventType = eventData.getEventType();
    }
//    else
//        ui->tableView->reset();

    if (ltModel)
    {
        ltModel->updateLT();
        if (currDriverId > 0)
            ui->tableView->setCurrentIndex(ltModel->indexOf(currDriverId));

    }
}

void LTWidget::resizeEvent(QResizeEvent *e)
{
    int w = ui->tableView->viewport()->width();/*-viewport()->width()*0.0115*/;//event->size().width()-event->size().width()*0.0115;

    switch(eventData.getEventType())
    {
        case LTPackets::RACE_EVENT:
            if (drawCarThumbnails)
            {
                ui->tableView->setColumnWidth(0, 0.04 * w);
                ui->tableView->setColumnWidth(1, 0.06 * w);
                ui->tableView->setColumnWidth(2, 0.115 * w);
                ui->tableView->setColumnWidth(3, 0.18 * w);
                ui->tableView->setColumnWidth(4, 0.06 * w);
                ui->tableView->setColumnWidth(5, 0.1 * w);
                ui->tableView->setColumnWidth(6, 0.12 * w);
                ui->tableView->setColumnWidth(7, 0.09 * w);
                ui->tableView->setColumnWidth(8, 0.09 * w);
                ui->tableView->setColumnWidth(9, 0.09 * w);
                ui->tableView->setColumnWidth(10, 0.06 * w);
            }
            else
            {
                ui->tableView->setColumnWidth(0, 0.05 * w);
                ui->tableView->setColumnWidth(1, 0.07 * w);
                ui->tableView->setColumnWidth(2, 0);
                ui->tableView->setColumnWidth(3, 0.22 * w);
                ui->tableView->setColumnWidth(4, 0.1 * w);
                ui->tableView->setColumnWidth(5, 0.11 * w);
                ui->tableView->setColumnWidth(6, 0.11 * w);
                ui->tableView->setColumnWidth(7, 0.1 * w);
                ui->tableView->setColumnWidth(8, 0.1 * w);
                ui->tableView->setColumnWidth(9, 0.1 * w);
                ui->tableView->setColumnWidth(10, 0.04 * w);
            }
            break;

        case LTPackets::PRACTICE_EVENT:
            if (drawCarThumbnails)
            {
                ui->tableView->setColumnWidth(0, 0.04 * w);
                ui->tableView->setColumnWidth(1, 0.05 * w);
                ui->tableView->setColumnWidth(2, 0.12 * w);
                ui->tableView->setColumnWidth(3, 0.2 * w);
                ui->tableView->setColumnWidth(4, 0.14 * w);
                ui->tableView->setColumnWidth(5, 0.11 * w);
                ui->tableView->setColumnWidth(6, 0.09 * w);
                ui->tableView->setColumnWidth(7, 0.09 * w);
                ui->tableView->setColumnWidth(8, 0.09 * w);
                ui->tableView->setColumnWidth(9, 0.09 * w);
            }
            else
            {
                ui->tableView->setColumnWidth(0, 0.05 * w);
                ui->tableView->setColumnWidth(1, 0.07 * w);
                ui->tableView->setColumnWidth(2, 0);
                ui->tableView->setColumnWidth(3, 0.2 * w);
                ui->tableView->setColumnWidth(4, 0.14 * w);
                ui->tableView->setColumnWidth(5, 0.18 * w);
                ui->tableView->setColumnWidth(6, 0.09 * w);
                ui->tableView->setColumnWidth(7, 0.09 * w);
                ui->tableView->setColumnWidth(8, 0.09 * w);
                ui->tableView->setColumnWidth(9, 0.09 * w);
            }
            break;

        case LTPackets::QUALI_EVENT:
            if (drawCarThumbnails)
            {
                ui->tableView->setColumnWidth(0, 0.04 * w);
                ui->tableView->setColumnWidth(1, 0.044 * w);
                ui->tableView->setColumnWidth(2, 0.12 * w);
                ui->tableView->setColumnWidth(3, 0.18 * w);
                ui->tableView->setColumnWidth(4, 0.1 * w);
                ui->tableView->setColumnWidth(5, 0.1 * w);
                ui->tableView->setColumnWidth(6, 0.1 * w);
                ui->tableView->setColumnWidth(7, 0.09 * w);
                ui->tableView->setColumnWidth(8, 0.09 * w);
                ui->tableView->setColumnWidth(9, 0.09 * w);
                ui->tableView->setColumnWidth(10, 0.06 * w);
            }
            else
            {
                ui->tableView->setColumnWidth(0, 0.05 * w);
                ui->tableView->setColumnWidth(1, 0.05 * w);
                ui->tableView->setColumnWidth(2, 0);
                ui->tableView->setColumnWidth(3, 0.21 * w);
                ui->tableView->setColumnWidth(4, 0.11 * w);
                ui->tableView->setColumnWidth(5, 0.11 * w);
                ui->tableView->setColumnWidth(6, 0.11 * w);
                ui->tableView->setColumnWidth(7, 0.1 * w);
                ui->tableView->setColumnWidth(8, 0.1 * w);
                ui->tableView->setColumnWidth(9, 0.1 * w);
                ui->tableView->setColumnWidth(10, 0.06 * w);
            }
            break;
    }
    //tu sie sypie przy przejsciu do nast. sesji
    if (ltModel)
    {
        for (int i = 0; i < ltModel->rowCount(); ++i)
            ui->tableView->setRowHeight(i, 22);
    }
    QWidget::resizeEvent(e);
}

void LTWidget::on_tableView_clicked(const QModelIndex &index)
{    
    if (ltModel)
    {
        if (ltModel->indexInDriverRowsData(index))
        {
            DriverData dd = ltModel->getDriverData(index);
            currDriverId = dd.getCarID();

            ltModel->selectDriver(currDriverId, index.column());

            emit driverSelected(currDriverId);
        }
//        ui->tableView->setCurrentIndex(index);
    }
}

void LTWidget::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (ltModel && ltModel->indexInDriverRowsData(index))
    {
        DriverData dd = ltModel->getDriverData(index);
        currDriverId = dd.getCarID();
        emit driverDoubleClicked(currDriverId);
    }
}

void LTWidget::on_tableView_headerClicked(int column)
{
    if (ltModel)
    {
        ltModel->headerClicked(column);

        if (currDriverId > 0)
        {
            QModelIndex index = ltModel->indexOf(currDriverId);
            ui->tableView->setCurrentIndex(index);
        }
    }
}
