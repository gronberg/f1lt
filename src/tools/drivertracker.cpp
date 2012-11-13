#include "drivertracker.h"
#include "drivertrackerpositioner.h"


DriverTracker::DriverTracker(QWidget *parent) : DriverRadar(parent)
{
    loadDriversList();

    label = QPixmap(":/ui_icons/label.png");
}

void DriverTracker::setupDrivers()
{
    trackMap = EventData::getInstance().getEventInfo().trackImg;
    QSize size = trackMap.size();
    size.setWidth(size.width() + 120);
    size.setHeight(size.height() + 120);
    setMinimumSize(size);

    for (int i = 0; i < drp.size(); ++i)
        drp[i]->setStartupPosition();

    for (int i = 0; i < drp.size(); ++i)
    {
        DriverTrackerPositioner *dtp = static_cast<DriverTrackerPositioner*>(drp[i]);

        int px = 60+(width() - trackMap.width())/2;
        int py = (height() - trackMap.height()-50)/2;
        int pitX = 15;
        int pitY = height()-60;
        int pitW = width() - 30;
        int pitH = 50;
        dtp->setMapCoords(px, py, pitX, pitY, pitW, pitH);
    }

    repaint();
}

void DriverTracker::loadDriversList()
{
    for (int i = 0; i < drp.size(); ++i)
        delete drp[i];

    drp.resize(EventData::getInstance().getDriversData().size());

    for (int i = 0; i < drp.size(); ++i)
    {
        drp[i] = new DriverTrackerPositioner(&EventData::getInstance().getDriversData()[i]);
    }
}


void DriverTracker::resizeEvent(QResizeEvent *)
{
    for (int i = 0; i < drp.size(); ++i)
    {
        DriverTrackerPositioner *dtp = static_cast<DriverTrackerPositioner*>(drp[i]);

        int px = 60+(width() - trackMap.width())/2;
        int py = (height() - trackMap.height()-50)/2;
        int pitX = 15;
        int pitY = height()-60;
        int pitW = width() - 30;
        int pitH = 50;
        dtp->setMapCoords(px, py, pitX, pitY, pitW, pitH);
    }
}

void DriverTracker::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QBrush(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND))));
    p.drawRect(0, 0, width(), height());

    int px = 60+(width() - trackMap.width())/2;
    int py = (height() - trackMap.height()-50)/2;

    QPoint point(px, py);
    p.drawPixmap(point, trackMap);

    p.setPen(QColor(255, 0, 0));
    p.drawRect(15, height()-60, width()-30, 50);

    for (int i = drp.size() - 1; i >= 0; --i)
        drp[i]->paint(&p);

    //draw legend
    for (int i = 0; i < EventData::getInstance().getDriversData().size(); ++i)
    {
        DriverData *dd = EventData::getInstance().getDriverDataByPosPtr(i+1);
        if (dd)
        {
            QString txt = QString("%1 %2").arg(dd->getNumber()).arg(SeasonData::getInstance().getDriverShortName(dd->getDriverName()));

            p.setFont(QFont("Arial", 10, 100));
            QColor drvColor = SeasonData::getInstance().getCarColor(*dd);
            p.setBrush(drvColor);

            int x = 20;
            int y = 10 + i * 20;

            int numX = x + 14;
            int numY = y + p.fontMetrics().height()/2 + 8;

            p.setPen(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND)));
            p.drawRect(x, y, 70, 20);
            p.drawText(numX, numY, txt);
            p.drawPixmap(x, y, label);

            if (!dd->isRetired())
            {
                p.setPen(SeasonData::getInstance().getColor(LTPackets::WHITE));

                if (dd->isInPits())
                    p.setPen(SeasonData::getInstance().getColor(LTPackets::PIT));
                QString pos = QString::number(i+1);

                if (i+1 < 10)
                    pos = "  " + pos;
                p.drawText(x-15, numY, pos);

                QString gap = "";
                if (i > 0)
                {
                    if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
                    {
                        DriverData *first = EventData::getInstance().getDriverDataByPosPtr(1);
                        if (first)
                        {
                            int quali = EventData::getInstance().getQualiPeriod();
                            gap = DriverData::calculateGap(dd->getQualiTime(quali), first->getQualiTime(quali));

                            if (gap != "")
                                gap = "+" + gap;
                        }
                    }
                    else
                    {
                        gap = dd->getLastLap().getGap();
                        if (gap != "")
                            gap = "+" + gap;

                        if (!gap.contains("L") && gap.size() < 5)
                            gap = "  " + gap;
                    }
                }
                else
                {
                    if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
                        gap = QString("LAP %1").arg(EventData::getInstance().getCompletedLaps());

                    else if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
                        gap = dd->getLastLap().getTime().toString();

                    else
                        gap = dd->getQualiTime(EventData::getInstance().getQualiPeriod()).toString();
                }
                p.setPen(SeasonData::getInstance().getColor(LTPackets::YELLOW));
                p.drawText(x+70, numY, gap);
            }

        }
    }

    p.end();
}
