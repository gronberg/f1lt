#include "driverradar.h"

DriverRadar::DriverRadar(QWidget *parent) :
    QWidget(parent), radarX(0), radarY(0), radarR(0.0), radarPitR(0.0), radarLappedR(0.0)
{
    loadDriversList();
    setMinimumSize(200, 200);
}

DriverRadar::~DriverRadar()
{
    for (int i = 0; i < drp.size(); ++i)
        delete drp[i];
}

void DriverRadar::loadDriversList()
{
    for (int i = 0; i < drp.size(); ++i)
        delete drp[i];

    drp.resize(EventData::getInstance().getDriversData().size());

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i] = new DriverRadarPositioner(&EventData::getInstance().getDriversData()[i]);
    }
}

void DriverRadar::setupDrivers(int speed)
{
    trackMap = EventData::getInstance().getEventInfo().trackImg.scaledToWidth(radarPitR*1.4, Qt::SmoothTransformation);
    if (trackMap.height() > trackMap.width())
        trackMap = EventData::getInstance().getEventInfo().trackImg.scaledToHeight(radarPitR*1.4, Qt::SmoothTransformation);

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i]->setStartupPosition();
        drp[i]->setSpeed(speed);
    }

    repaint();
}

void DriverRadar::update()
{
    for (int i = 0; i < drp.size(); ++i)
        drp[i]->update();

    repaint();
}

void DriverRadar::resizeEvent(QResizeEvent *)
{
    radarR = width() < height() ? (double)width() / 2.0 - 20.0 : (double)height() / 2.0 - 20.0;
    radarX = width()/2;
    radarY = height()/2;

    radarLappedR = radarR * 0.75;
    radarPitR = radarR * 0.5;

    for (int i = 0; i < drp.size(); ++i)
        drp[i]->setRadarCoords(radarX, radarY, radarR, radarPitR, radarLappedR);

    trackMap = EventData::getInstance().getEventInfo().trackImg.scaledToWidth(radarPitR*1.4, Qt::SmoothTransformation);
    if (trackMap.height() > trackMap.width())
        trackMap = EventData::getInstance().getEventInfo().trackImg.scaledToHeight(radarPitR*1.4, Qt::SmoothTransformation);
}

void DriverRadar::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QBrush(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND))));
    p.drawRect(0, 0, width(), height());

    QPainterPath path;

    QPen pen(QColor(255, 255, 255), 5);

    if (EventData::getInstance().getFlagStatus() == LTPackets::SAFETY_CAR_DEPLOYED)
        pen.setColor(SeasonData::getInstance().getColor(LTPackets::YELLOW));

//    path.addEllipse(QPoint(radarX, radarY), radarR, radarR);
    p.setBrush(QBrush());
    p.setPen(pen);
    p.drawEllipse(QPoint(radarX, radarY), (int)radarR, (int)radarR);
//    p.drawPath(path);

//    path.addEllipse(QPoint(radarX, radarY), radarLappedR, radarLappedR);
    pen.setWidth(3);
    p.setPen(pen);
    p.drawEllipse(QPoint(radarX, radarY), (int)radarLappedR, (int)radarLappedR);
//    p.drawPath(path);

    pen.setColor(QColor(255, 255, 255));
//    path.addEllipse(QPoint(radarX, radarY), radarPitR, radarPitR);
    pen.setWidth(5);
    p.setPen(pen);
//    p.drawEllipse(QPoint(radarX, radarY), radarPitR, radarPitR);
    int x = radarX - radarPitR;
    int y = radarY - radarPitR;
    int w = radarX + radarPitR - x;
    int h = radarY + radarPitR - y;
    p.drawArc(x, y, w, h, 270*16, 180*16);
//    p.drawPath(path);

    pen.setWidth(2);
    p.setPen(pen);
    p.drawLine(radarX, radarY - radarR - 10, radarX, radarY - radarLappedR + 10);

//    p.drawPixmap(radarX - trackMap.width()/2, radarY - trackMap.height()/2, trackMap);

    for (int i = drp.size() - 1; i >= 0; --i)
        drp[i]->paint(&p);

    p.end();
}
