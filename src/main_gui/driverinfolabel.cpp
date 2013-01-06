#include "driverinfolabel.h"
#include <QPainter>

#include <QDebug>

DriverInfoLabel::DriverInfoLabel(QWidget *parent) :
    QWidget(parent), driverData(0)
{
    backgroundPixmap = QPixmap(":/ui_icons/label-small.png");
}

QSize DriverInfoLabel::sizeHint()
{
    return backgroundPixmap.size();
}

QSize DriverInfoLabel::minimumSize()
{
    return backgroundPixmap.size();
}

void DriverInfoLabel::update()
{
    if (height() < sizeHint().height())
    {
        QSize size = backgroundPixmap.size();
        size.setHeight(size.height() + 10);
        setMinimumSize(size);
        updateGeometry();
    }
    repaint();
}

void DriverInfoLabel::paintEvent(QPaintEvent *)
{
    if (driverData == 0)
        return;

    QPainter p;
    p.begin(this);
    int x = (width() - backgroundPixmap.width())/2;

    QColor color = /*QColor(170, 170, 170);*/SeasonData::getInstance().getCarColor(driverData->getNumber());
    int numX = x + 105;
    int numY = 1;
    p.setBrush(color);
    p.drawRect(numX, numY, 30, 20);
    numX = x + 20;
    numY = 30;

    p.drawPixmap(x, 0, backgroundPixmap);

    QString txt = QString::number(driverData->getPosition());

    if (driverData->getPosition() < 10)
        txt = " " + txt;

    p.setFont(QFont("Arial", 20, 100));
    p.setPen(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND)));

    if (driverData->isInPits())
        p.setPen(QColor(SeasonData::getInstance().getColor(LTPackets::PIT)));

    p.drawText(numX, numY, txt);

    txt = driverData->getDriverName();
    p.setPen(QColor(SeasonData::getInstance().getColor(LTPackets::WHITE)));
    p.setFont(QFont("Arial", 10, 100));
    numX = x + 140;
    numY = 15;
    p.drawText(numX, numY, txt);

    numX = x + 120;
    numY = 35;
    txt = SeasonData::getInstance().getTeamName(driverData->getNumber());
    p.drawText(numX, numY, txt);



    p.setPen(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND)));
    txt = QString::number(driverData->getNumber());
    if (driverData->getNumber() < 10)
        txt = " " + txt;
    numX = x + 115;
    numY = 15;
    p.drawText(numX, numY, txt);


    numX = x + 63;
    numY = 4;
    p.drawPixmap(numX, numY, SeasonData::getInstance().getHelmetsFactory().getHelmet(driverData->getNumber(), 30));
}
