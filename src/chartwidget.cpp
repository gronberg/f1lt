#include "chartwidget.h"
#include "ui_chartwidget.h"
#include <QClipboard>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include "ltdata.h"
#include <cmath>
#include <QDebug>

ChartWidget::ChartWidget(double n, double x, QColor col, QWidget *parent) :
    QWidget(parent), min(n), max(x), color(col)
{
    menu = new QMenu(this);
    copyAction = menu->addAction("Copy chart");
    saveAction = menu->addAction("Save as image");

    connect(copyAction, SIGNAL(triggered()), this, SLOT(onCopy()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton)
    {
        menu->exec(QWidget::mapToGlobal(ev->pos()));
    }
}

void ChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(27, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(27, height()-25, 27, 10);

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((height()-35.0)/4.0);
    double yFactor2 = (double)((max-min)/4.0);
    double j = min;
    for (int i = height()-25; i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(round(j)));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(27, i, width()-5, i);
        }
    }    

    if (driverData.posHistory.size()>1)
    {
        int sz = (driverData.posHistory.size() > driverData.lapData.size() ? driverData.posHistory.size()-1 : driverData.posHistory.size());
        double xFactor = (width()-32) / /*((lapData.size() < 5) ?*/ (sz) /*: 5)*/;
        double j = 1.0;
        double i = 27.0;
        int prevJ = 1;

        double jFactor = driverData.posHistory.size() < 5 ? 1.0 : (double)((driverData.posHistory.size()-1)/6.0);

        if (driverData.posHistory.size() > driverData.lapData.size())
        {
//            jFactor = driverData.posHistory.size() < 5 ? 1.0 : (double)((driverData.posHistory.size()-1)/5.0);
            j = 1.0;
        }
        else
        {
//            jFactor = driverData.posHistory.size() < 5 ? 1.0 : (double)((driverData.posHistory.size())/5.0);
            j = 0.0;
            prevJ = 0;
        }

        for (; i < width()-15.0 && round(j) < driverData.posHistory.size(); /*i += xFactor,*/ j += jFactor)
        {
//            std::cout<<"POSHISTORY="<<driverData.posHistory.size()<<", "<<round(j)<<std::endl;
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));

            if (driverData.posHistory.size() > driverData.lapData.size())
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));
            else
                p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.lapData[round(j)].numLap));

            if (i > 27)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
        }
    }

//    if (!data.isEmpty())
//    {
//        double xFactor = (width()-32) / 5;
//        double j = 1.0;
//        for (int i = 27; i < width()-15; i += xFactor, j += (double)(data.size()/5.0))
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 27)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }

}

void ChartWidget::drawChart(QPainter *p)
{
    if (driverData.posHistory.size()>1)
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        int sz = (driverData.posHistory.size() > driverData.lapData.size() ? driverData.posHistory.size()-1 : driverData.posHistory.size());
        double xFactor = (width()-32) / (sz);
        double yFactor = ((height()-35) / (max-min));

        double x = 27, j = x + xFactor;
        double y = (double)(height())-25.0 - (double)(driverData.posHistory[1]-min) * yFactor;

        int i = (driverData.posHistory.size() > driverData.lapData.size() ? 2 : 1);
        for (; i < driverData.posHistory.size(); ++i, j += xFactor)
        {
            double y2 = (double)(height())-25.0 - (double)(driverData.posHistory[i]-min) * yFactor;
            if (driverData.posHistory[i] <= 0)
            {
                y2 = y;
            }
            p->drawLine(x, y, j, y2);
            x = j;
            y = y2;

            if (i >= 0 && i < driverData.lapData.size() && driverData.lapData[i-1].lapTime.toString() == "IN PIT")
            {
                QPainterPath path;
                path.addEllipse(QPoint(j, y2), 6, 6);
                p->setBrush(QBrush(color));
                p->setPen(color);

                p->drawPath(path);
            }
            pen.setWidth(2);
            p->setPen(pen);
        }
    }
}

void ChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    p.end();
}

void ChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);
    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    //draw driver name:
    p.setPen(LTData::colors[LTData::WHITE]);
    p.drawText(40, 20, driverData.driver);

    p.end();

    qApp->clipboard()->setImage(img);
}

void ChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);
        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawAxes(&p);
        drawChart(&p);

        //draw driver name:
        p.setPen(LTData::colors[LTData::WHITE]);
        p.drawText(40, 20, driverData.driver);

        p.end();

        img.save(fName, "PNG");
    }
}

//========================================================================

void LapTimeChartWidget::setData(const DriverData &dd)
{
    driverData = dd;

    max = 0.0;
    for (int i = 0; i < dd.lapData.size(); ++i)
    {
        double secs = (double)(dd.lapData[i].lapTime.toMsecs() / 1000.0);

        if (secs > max)
            max = secs;
    }
    if (max == 0.0 || max > 180.0)
        max = 180.0;

    else
        max = max + 0.1*max;
}

void LapTimeChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(28, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(28, height()-25, 28, 30);

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

//    std::cout<<"max="<<max<<std::endl;
    for (int i = height()-25, j = min; i >= 50; i-= (height()-55)/6, j += (max-min)/6)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(28, i, width()-5, i);
        }
    }

    if (!driverData.lapData.isEmpty())
    {
        double xFactor = (width()-33) / /*((lapData.size() < 5) ?*/ driverData.lapData.size() /*: 5)*/;
        double j = 0.0;
        double i = 28.0;
        int prevJ = 0;

        double jFactor = driverData.lapData.size() < 5 ? 1.0 : (double)(driverData.lapData.size()/6.0);
        for (; i < width()-15.0 && round(j) < driverData.lapData.size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.lapData[round(j)].numLap));

            if (i > 28)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 30);
            }
        }
    }
}

void LapTimeChartWidget::drawChart(QPainter *p)
{
    if (!driverData.lapData.empty())
    {
        QPen pen(colors[0]);
        pen.setWidth(2);

        p->setRenderHint(QPainter::Antialiasing);

        double xFactor = (width()-33) / driverData.lapData.size();
        double yFactor = ((height()-60) / (max-min));

        double x, x2, j;
        double sec1y, sec2y, sec3y, lapy, sec1y2, sec2y2, sec3y2, lapy2;

        x = 28.0;
        j = x + xFactor;

        LapTime sector1 = driverData.lapData[0].sector1;
        LapTime sector2 = driverData.lapData[0].sector2;
        LapTime sector3 = driverData.lapData[0].sector3;
        LapTime lapTime = driverData.lapData[0].lapTime;

        if (driverData.lapData.size() > 1)
        {
            if (lapTime.toString() == "IN PIT")
            {
                lapTime = driverData.lapData[1].lapTime;
                LapTime pl = LapTime(driverData.getPitTime(driverData.lapData[0].numLap));

                lapTime = lapTime - pl + LapTime(5000);
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData.lapData[1].sector1;
                LapTime pl = LapTime(driverData.getPitTime(driverData.lapData[0].numLap));

                sector1 = sector1 - pl + LapTime(5000);
            }

            if (sector2.toString() == "")
                sector2 = driverData.lapData[1].sector2;

            if (sector3.toString() == "")
                sector3 = driverData.lapData[1].sector3;
        }

        double secs = sector1.toDouble();
        sec1y = (double)(height()-25 - secs * yFactor);


        secs = sector2.toDouble();
        sec2y = (double)(height()-25 - secs * yFactor);

        secs = sector3.toDouble();
        sec3y = (double)(height()-25 - secs * yFactor);

        secs = lapTime.toDouble();//-QTime::fromString(lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
        //secs = (double)(msecs/1000.0);
        lapy = (double)(height()-25 - secs * yFactor);

        if (driverData.lapData[0].lapTime.toString() == "IN PIT")
        {
            QPainterPath path;
            path.addEllipse(QPoint(round(x), lapy), 6, 6);
            p->setBrush(QBrush(colors[3]));
            p->setPen(colors[3]);

            if (driverData.lapData[0].scLap)
            {
                pen.setColor(colors[4]);
                p->setBrush(QBrush(colors[4]));
            }

            p->drawPath(path);
        }


        for (int i = 1; i < driverData.lapData.size(); ++i, j += xFactor)
        {
            x2 = j;

            sector1 = driverData.lapData[i].sector1;
            sector2 = driverData.lapData[i].sector2;
            sector3 = driverData.lapData[i].sector3;
            lapTime = driverData.lapData[i].lapTime;


            if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
            {
                lapTime = driverData.lapData[i-1].lapTime;
                if (lapTime.toString() == "IN PIT" || lapTime.toString() == "RETIRED")
                {
//                    if (i - 1 > 0)
//                        lapTime = lapData[i-2].lapTime;
                    if (i < driverData.lapData.size()-1)
                    {
                        lapTime = driverData.lapData[i+1].lapTime;

                        LapTime pl = driverData.getPitTime(driverData.lapData[i].numLap);
                        lapTime = lapTime - pl + LapTime(5000);
                    }
                }
            }

            if (sector1.toString() == "")
            {
                sector1 = driverData.lapData[i-1].sector1;

                if (sector1.toString() == "" && i < driverData.lapData.size()-1)
                    sector1 = driverData.lapData[i+1].sector1;
            }

            if (sector2.toString() == "")
            {
                sector2 = driverData.lapData[i-1].sector2;

                if (sector2.toString() == "" && i < driverData.lapData.size()-1)
                    sector2 = driverData.lapData[i+1].sector2;
            }

            if (sector3.toString() == "")
            {
                sector3 = driverData.lapData[i-1].sector3;
                if (sector3.toString() == "")
                {
//                    if (i - 1 > 0)
//                        sector3 = lapData[i-2].sector3;
                    if (i < driverData.lapData.size()-1)
                        sector3 = driverData.lapData[i+1].sector3;
                }
            }

            secs = sector1.toDouble();
            if (secs > max) secs = max;
            sec1y2 = (double)(height()-25 - secs * yFactor);

            secs = sector2.toDouble();
            if (secs > max) secs = max;
            sec2y2 = (double)(height()-25 - secs * yFactor);

            secs = sector3.toDouble();
            if (secs > max) secs = max;
            sec3y2 = (double)(height()-25 - secs * yFactor);

            secs = lapTime.toDouble();
            if (secs > max) secs = max;
            lapy2 = (double)(height()-25 - secs * yFactor);

            pen.setColor(colors[0]);
            p->setPen(pen);
            p->drawLine(round(x), sec1y, round(x2), sec1y2);

            pen.setColor(colors[1]);
            p->setPen(pen);
            p->drawLine(round(x), sec2y, round(x2), sec2y2);

            pen.setColor(colors[2]);
            p->setPen(pen);
            p->drawLine(round(x), sec3y, round(x2), sec3y2);

            pen.setColor(colors[3]);
            if (driverData.lapData[i].scLap)
                pen.setColor(colors[4]);

            p->setPen(pen);
            p->drawLine(round(x), lapy, round(x2), lapy2);

            if (driverData.lapData[i].lapTime.toString() == "IN PIT")
            {
                QPainterPath path;
                path.addEllipse(QPoint(round(x2), lapy2), 6, 6);
                p->setBrush(QBrush(colors[3]));
                p->setPen(colors[3]);

                if (driverData.lapData[i].scLap)
                {
                    pen.setColor(colors[4]);
                    p->setBrush(QBrush(colors[4]));
                }
                p->drawPath(path);
//                pen.setWidth(4);
//                p.setPen(pen);
//                p.drawPoint(x2, lapy2);
//                pen.setWidth(2);
            }

            x = x2;
            sec1y = sec1y2;
            sec2y = sec2y2;
            sec3y = sec3y2;
            lapy = lapy2;
        }
    }
    //------------------------------------draw chart legend--------------


    //--------------------------------------------------------
}

void LapTimeChartWidget::drawLegend(QPainter *p, int x, int y)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));
    p->setPen(LTData::colors[LTData::DEFAULT]);
    p->drawRect(x, y, 200, 20);

    p->setPen(colors[0]);
    p->drawText(x+5, y+15, "S1");

    p->setPen(colors[1]);
    p->drawText(x+35, y+15, "S2");

    p->setPen(colors[2]);
    p->drawText(x+65, y+15, "S3");

    p->setPen(colors[3]);
    p->drawText(x+95, y+15, "Lap");

    p->drawText(x+145, y+15, "Pit");

    p->setPen(colors[4]);
    p->drawText(x+175, y+15, "SC");


    p->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    p->setPen(colors[3]);
    path.addEllipse(QPoint(x+138, y+10), 4, 4);
    p->setBrush(colors[3]);
    p->drawPath(path);
    p->setBrush(QBrush());
}

void LapTimeChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);
    drawLegend(&p, 35, 5);

    p.end();
}

void LapTimeChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);
    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);
    drawLegend(&p, 35, 25);

    //draw driver name:
    p.setPen(LTData::colors[LTData::WHITE]);
    p.drawText(40, 20, driverData.driver);

    p.end();

    qApp->clipboard()->setImage(img);
}

void LapTimeChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);
        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawAxes(&p);
        drawChart(&p);
        drawLegend(&p, 35, 25);

        //draw driver name:
        p.setPen(LTData::colors[LTData::WHITE]);
        p.drawText(40, 20, driverData.driver);

        p.end();

        img.save(fName, "PNG");
    }
}

//========================================================================

void GapChartWidget::drawAxes(QPainter *p)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(28, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(28, height()-25, 28, 10);

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //double factor =

//    min = 0;
//    max = 0;

//    for (int i = 0; i < lapData.size(); ++i)
//    {
//        if (lapData[i].gap[lapData[i].gap.size()-1] != 'L' && lapData[i].gap != "")
//        {
//            double gap = lapData[i].gap.toDouble();
//            if (gap > max)
//                max = gap;
//        }
//    }
    for (int i = height()-25, j = min; i >= 50; i-= (height()-75)/4, j += (max-min)/4)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(28, i, width()-5, i);
        }
    }
    p->setPen(QColor(LTData::colors[LTData::WHITE]));
    p->drawText(5, 15, ">1L");
    QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
    pen.setStyle(Qt::DashLine);
    p->setPen(pen);
    p->drawLine(28, 10, width()-5, 10);

    p->setPen(QColor(LTData::colors[LTData::WHITE]));
    p->drawText(5, 35, QString(">%1").arg(max));
    pen.setColor(QColor(LTData::colors[LTData::DEFAULT]));
    pen.setStyle(Qt::DashLine);
    p->setPen(pen);
    p->drawLine(28, 30, width()-5, 30);

    if (!driverData.lapData.isEmpty())
    {
        double xFactor = (width()-33) / /*((lapData.size() < 5) ?*/ driverData.lapData.size() /*: 5)*/;
        double j = 0.0;
        double i = 28.0;
        int prevJ = 0;

        double jFactor = driverData.lapData.size() < 5 ? 1.0 : (double)(driverData.lapData.size()/6.0);
        for (; i < width()-15.0 && round(j) < driverData.lapData.size(); /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(driverData.lapData[round(j)].numLap));

            if (i > 28)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
        }
    }

//    if (!lapData.isEmpty())
//    {
//        double xFactor = (width()-32) / ((lapData.size() < 5) ? lapData.size() : 5);
//        double j = 0.0;

//        double jFactor = lapData.size() < 5 ? 1.0 : (double)(lapData.size()/5.0);
//        for (int i = 28; i < width()-15 && round(j) < lapData.size(); i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(lapData[round(j)].numLap));

//            if (i > 28)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }

}

void GapChartWidget::drawChart(QPainter *p)
{
    if (!driverData.lapData.empty())
    {
        p->setBrush(QBrush(color));
        QPen pen(color);
        pen.setWidth(2);
        p->setPen(pen);
        p->setRenderHint(QPainter::Antialiasing);

        double xFactor = (width()-30) / driverData.lapData.size();
        double yFactor = ((height()-75) / (max-min));

        double gap = driverData.lapData[0].gap.toDouble();

        if (driverData.lapData[0].gap.size() > 0 && driverData.lapData[0].gap[driverData.lapData[0].gap.size()-1] == 'L')
            gap = -1.0;

        double x = 28, y = height()-25 - gap * yFactor, y2;

        if (gap == -1.0)
            y = 10;

        else if (gap > max)
            y = 30;

        for (int i = 1, j = x + xFactor; i < driverData.lapData.size(); ++i, j += xFactor)
        {
            bool ok;
            gap = driverData.lapData[i].gap.toDouble(&ok);
            y2 = height()-25 - gap * yFactor;

            if (gap > max)
                y2 = 30;

            if (EventData::getInstance().eventType == LTData::RACE_EVENT)
            {
                if (driverData.lapData[i].pos == 1 && driverData.lapData[i].gap == "LAP")
                {
                    gap = 0;
                    y2 = height()-25 - gap * yFactor;
                }
                else if (driverData.lapData[i].gap.size() > 0 && driverData.lapData[i].gap[driverData.lapData[i].gap.size()-1] == 'L')
                {
                    y2 = 10;
                    gap = -1.0;
                }
                else if (gap <= 0 || !ok || gap > max)
                    y2 = 30;

//                else
//                    y2 = height()-25 - gap * yFactor;
            }

            p->drawLine(x, y, j, y2);
            x = j;
            y = y2;

            if (driverData.lapData[i].lapTime.toString() == "IN PIT")
            {
                QPainterPath path;
                path.addEllipse(QPoint(j, y2), 6, 6);
                p->setBrush(QBrush(color));
                p->setPen(color);

                p->drawPath(path);
            }
            pen.setWidth(2);
            p->setPen(pen);
        }
    }
}

void GapChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    p.end();
}


void GapChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawAxes(&p);
    drawChart(&p);

    //draw driver name:
    p.setPen(LTData::colors[LTData::WHITE]);
    p.drawText(40, 25, driverData.driver);

    p.end();

    qApp->clipboard()->setImage(img);
}

void GapChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        drawAxes(&p);
        drawChart(&p);

        //draw driver name:
        p.setPen(LTData::colors[LTData::WHITE]);
        p.drawText(40, 25, driverData.driver);

        p.end();

        img.save(fName, "PNG");
    }
}
//========================================================================

void LapCompChartWidget::setData(DriverData *dd)
{
    double lMin = 99999999.0, lMax = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        driverData[i] = dd[i];

        for (int j = 0; j < driverData[i].lapData.size(); ++j)
        {
            double secs = driverData[i].lapData[j].lapTime.toDouble();//-QTime::fromString(driverData[i].lapData[j].lapTime, "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));

            if (secs == 0 && j > 0)
                secs = driverData[i].lapData[j-1].lapTime.toDouble();//-QTime::fromString(driverData[i].lapData[j-1].lapTime, "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));

            if (secs == 0 && j < driverData[i].lapData.size()-1)
            {
                secs = driverData[i].lapData[j+1].lapTime.toDouble();//-QTime::fromString(driverData[i].lapData[j+1].lapTime, "m:ss.zzz").secsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
                if (driverData[i].lapData[j].lapTime.toString() == "IN PIT")
                {
                    LapTime pl(driverData[i].getPitTime(driverData[i].lapData[j].numLap));
                    secs = (driverData[i].lapData[j+1].lapTime - pl + LapTime(5000)).toDouble();
                }
            }

            if (secs < lMin)
                lMin = secs;

            if (secs > lMax)
                lMax = secs;
        }
    }
    if (lMax != 0)
    {
        max = (double)(lMax + lMax * 0.01);
        min = (double)(lMin - lMin * 0.01);

        if (max > 180)
            max = 180;

        if (min < 0)
           min = 0;
    }
}

void LapCompChartWidget::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(37, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(37, height()-25, 37, 10);

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    double yFactor = (double)((height()-35.0)/6.0);
    double yFactor2 = (double)((max-min)/6.0);
    double j = min;
    for (int i = height()-25; i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j, 0, 'f', 1));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(37, i, width()-5, i);
        }
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = (width()-42) / /*((lapData.size() < 5) ?*/ (lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = 37.0;
        int prevJ = firstLap;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
        for (; i < width()-15.0 && round(j) < lastLap; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

            if (i > 37)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
        }
    }

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}

void LapCompChartWidget::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[4] = {37.0, 37.0, 37.0, 37.0};
    double y1[4];
    double y2[4];
    double yFactor = ((height()-35) / (max-min));
    int msecs;
    double secs;

    for (int i = 0; i < 4; ++i)
    {
        if (!driverData[i].lapData.isEmpty())
        {
            if (driverData[i].lapData[0].numLap < firstLap)
                firstLap = driverData[i].lapData[0].numLap;

            if (driverData[i].lapData.last().numLap > lastLap)
                lastLap = driverData[i].lapData.last().numLap;

            msecs = -QTime::fromString(driverData[i].lapData[0].lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
            secs = (double)(msecs / 1000.0);

            if (secs > max) secs = max;
            if (secs < min) secs = min;
            y1[i] = (double)(height()-25.0 - (double)(secs-min) * yFactor);
        }
    }
    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = (width()-42) / (lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);
    int index[4] = {0,0,0,0};
    int j[4] = {(int)(x[0]), (int)(x[0]), (int)(x[0]), (int)(x[0])};
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 4; ++k)
        {
        	LapData ld = driverData[k].getLapData(i);
//            if (!driverData[k].lapData.empty() && index[k] < driverData[k].lapData.size() && driverData[k].lapData[index[k]].numLap == i)
        	if (ld.carID == driverData[k].carID && ld.numLap == i)
            {
                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                LapTime lapTime = ld.lapTime;//driverData[k].lapData[index[k]].lapTime;

                if (lapTime.toString() == "IN PIT")
                {
                    if (index[k] > 0)
                    {
                    	lapTime = driverData[k].getLapData(i-1).lapTime;
//                        lapTime = driverData[k].lapData[index[k]-1].lapTime;
                    }

                    else if (index[k] < driverData[k].lapData.size()-1)
                    {
                    	lapTime = driverData[k].getLapData(i+1).lapTime;
//                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
                        LapTime pl(driverData[k].getPitTime(driverData[k].lapData[index[k]].numLap));

                        lapTime = lapTime - pl + LapTime(5000);
                    }

                    if  (lapTime.toString() == "IN PIT" && index[k] < driverData[k].lapData.size()-1)
                    {
                    	lapTime = driverData[k].getLapData(i+1).lapTime;
//                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
                        LapTime pl(driverData[k].getPitTime(driverData[k].lapData[index[k]].numLap));

                        lapTime = lapTime - pl + LapTime(5000);
                    }
                }
                msecs = -QTime::fromString(lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
                secs = (double)(msecs / 1000.0);

                if (secs > max) secs = max;
                if (secs < min) secs = min;
                y2[k] = (double)(height()-25.0 - (double)(secs-min) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                p->drawLine(x[k], y1[k], j[k], y2[k]);

                if (driverData[k].lapData[index[k]].lapTime.toString() == "IN PIT")
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(j[k], y2[k]), 6, 6);
                    p->setBrush(QBrush(colors[k]));
                    p->drawPath(path);
                }

                y1[k] = y2[k];
                x[k] = j[k];

                j[k] += xFactor;

                ++index[k];
            }
            else
            {
                x[k] += xFactor;
                j[k] = x[k];
            }
        }
    }

}

void LapCompChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);   

    drawChart(&p);
    //------------------------------------draw chart legend--------------
//    p.setRenderHint(QPainter::Antialiasing, false);
//    p.setBrush(QColor(20, 20, 20));

//    int yy = 0;
//    for (int i = 0; i < 4; ++i)
//    {
//        if (driverData[i].carID > 0)
//        {
//            p.setPen(QColor(20, 20, 20));
//            p.drawRect(40, yy, 115, 20);

//            p.setPen(colors[i]);
//            p.drawText(45, yy+20, driverData[i].driver);
//            yy += 20;
//        }
//    }
//    if (yy > 5)
//    {
//        p.setBrush(QBrush());
//        p.setPen(LTData::colors[LTData::DEFAULT]);
//        p.drawRect(40, 5, 115, yy);
//    }
    //--------------------------------------------------------

    p.end();
}

void LapCompChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    int yy = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (driverData[i].carID > 0)
        {
            p->setPen(QColor(20, 20, 20));
            p->drawRect(40, yy, 115, 20);

            p->setPen(colors[i]);
            p->drawText(45, yy+20, driverData[i].driver);
            yy += 20;
        }
    }
    if (yy > 5)
    {
        p->setBrush(QBrush());
        p->setPen(LTData::colors[LTData::DEFAULT]);
        p->drawRect(40, 5, 115, yy);
    }
}

void LapCompChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();

    qApp->clipboard()->setImage(img);
}

void LapCompChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawChart(&p);

        drawLegend(&p);

        p.end();

        img.save(fName, "PNG");
    }
}

//========================================================================

double GapCompChartWidget::calculateInterval(int lap)
{
    LapData ld1 = (driverIdx[0] >= 0) ? eventData.driversData[driverIdx[0]].getLapData(lap) : LapData();
    LapData ld2 = (driverIdx[1] >= 0) ? eventData.driversData[driverIdx[1]].getLapData(lap) : LapData();
    QString gap1 = ld1.gap;
    QString gap2 = ld2.gap;

    if ((ld1.lapTime.toString() == "" && ld1.gap == "") || (ld2.lapTime.toString() == "" && ld2.gap == ""))
        return 0.0;

    if ((gap1 != "" && gap2 != "" && gap1[gap1.size()-1] != 'L' && gap2[gap2.size()-1] != 'L') ||
        ((ld1.pos == 1 && gap1.isNull()) || (ld2.pos == 1 && gap2.isNull())))
    {
        double interval = gap1.toDouble() - gap2.toDouble();
        return interval;
    }
    else if ((gap1 != "" && gap1[gap1.size()-1] == 'L') || (gap2 != "" && gap2[gap2.size()-1] == 'L'))
    {
        int pos1 = ld1.pos;
        int pos2 = ld2.pos;

        bool neg = true;
        if (pos2 < pos1)
        {
            int tmp = pos1;
            pos1 = pos2;
            pos2 = tmp;
            neg = false;
        }

        QList<QString> intervals;
//        intervals.reserve(pos2 - pos1);
        for (int i = 0; i < eventData.driversData.size(); ++i)
        {
            LapData ld = eventData.driversData[i].getLapData(lap);
            int pos = ld.pos;
            if (pos > pos1 && pos <= pos2)
            {
                if (ld.interval != "" && ld.interval[ld.interval.size()-1] == 'L')
                    return neg ? -1000.0 : 1000.0;

                intervals.append(ld.interval);
            }
        }
        double interval = 0.0;
        for (int i = 0; i < intervals.size(); ++i)
            interval += intervals[i].toDouble();

        if (neg && ld1.lapTime.isValid() && interval > ld1.lapTime.toDouble())
            return -1000.0;
        if (!neg && ld2.lapTime.isValid() && interval > ld2.lapTime.toDouble())
            return 1000.0;


        return neg ? -interval : interval;
    }

    return 0.0;
}


void GapCompChartWidget::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(37, (height()-25), width()-5, (height()-25));

    //y axe
    p->drawLine(37, height()-25, 37, 10);

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    if (max >= 60)
        max = 60;

    double yFactor = (double)((height()-35.0)/6.0);
    double yFactor2 = (double)((max-min)/6.0);
    double j = min;
    for (int i = height()-25; i > 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(j, 0, 'f', 1));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(37, i, width()-5, i);
        }
    }

    if (max == 60)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, 15, ">60");
        QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(37, 10, width()-5, 10);
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = (width()-42) / /*((lapData.size() < 5) ?*/ (lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = 37.0;
        int prevJ = firstLap;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
        for (; i < width()-15.0 && round(j) < lastLap; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

            if (i > 37)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
        }
    }

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}

void GapCompChartWidget::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[2] = {37.0, 37.0};
    double y1[2];
    double y2[2];
//    int msecs;
//    double secs;

    for (int i = 0; i < 2; ++i)
    {
        DriverData dd = driverIdx[i] >= 0 ? eventData.driversData[driverIdx[i]] : DriverData();
        if (!dd.lapData.isEmpty())
        {
            if (dd.lapData[0].numLap < firstLap)
                firstLap = dd.lapData[0].numLap;

            if (dd.lapData.last().numLap > lastLap)
                lastLap = dd.lapData.last().numLap;

//            msecs = -QTime::fromString(eventData.driversData[driverIdx[i]].lapData[0].lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//            secs = (double)(msecs / 1000.0);

//            if (secs > max) secs = max;
//            if (secs < min) secs = min;
//            y1[i] = (double)(height()-25.0 - (double)(secs-min) * yFactor);
        }
    }
    int index[2] = {0,0};
    min = 0.0; max = 0.0;
    QList<double> intervals;
    for (int i = firstLap; i <= lastLap; ++i)
    {
        intervals.append(calculateInterval(i));
        double interval = fabs(intervals.last());

        if (interval > max)
            max = interval;
//        for (int k = 0; k < 2; ++k)
//        {
//            if (!eventData.driversData[driverIdx[k]].lapData.empty() && index[k] < eventData.driversData[driverIdx[k]].lapData.size() &&
//                    eventData.driversData[driverIdx[k]].lapData[index[k]].numLap == i)
//            {

//            }
//        }
    }
    if (max != 1000)
        max += max * 0.05;

    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = (width()-42) / (lastLap - firstLap);
    double yFactor = ((height()-35) / (max-min));

    p->setRenderHint(QPainter::Antialiasing);
    int j[2] = {(int)(x[0]), (int)(x[0])};
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
            DriverData dd = (driverIdx[k] >= 0) ? eventData.driversData[driverIdx[k]] : DriverData();
            LapData ld = dd.getLapData(i);
//            if (!dd.lapData.empty() && index[k] < dd.lapData.size() && dd.lapData[index[k]].numLap == i)
            if (ld.carID == dd.carID && ld.numLap == i)
            {
                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                double driverY = fabs(intervals[i-firstLap]);

                int idx = i - firstLap;
                if (driverY == 0 && (i+1-firstLap) < intervals.size())
                {
                    idx = i + 1 - firstLap;
                    driverY = fabs(intervals[idx]);
                }


                if ((k == 0 && intervals[idx] < 0) || (k == 1 && intervals[idx] > 0))
                    driverY = 0;


//                if (dd.lapTime.toString() == "IN PIT")
//                {
//                    if (index[k] > 0)
//                        lapTime = driverData[k].lapData[index[k]-1].lapTime;

//                    else if (index[k] < driverData[k].lapData.size()-1)
//                    {
//                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
//                        LapTime pl(driverData[k].getPitTime(driverData[k].lapData[index[k]].numLap));

//                        lapTime = lapTime - pl + LapTime(5000);
//                    }

//                    if  (lapTime.toString() == "IN PIT" && index[k] < driverData[k].lapData.size()-1)
//                    {
//                        lapTime = driverData[k].lapData[index[k]+1].lapTime;
//                        LapTime pl(driverData[k].getPitTime(driverData[k].lapData[index[k]].numLap));

//                        lapTime = lapTime - pl + LapTime(5000);
//                    }
//                }
//                msecs = -QTime::fromString(lapTime, "m:ss.zzz").msecsTo(QTime::fromString("0:00.000", "m:ss.zzz"));
//                secs = (double)(msecs / 1000.0);

                if (driverY > max) driverY = max;

                y2[k] = (double)(height()-25.0 - driverY * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                p->drawLine(x[k], y1[k], j[k], y2[k]);

                if (ld.lapTime.toString() == "IN PIT")
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(j[k], y2[k]), 6, 6);
                    p->setBrush(QBrush(colors[k]));
                    p->drawPath(path);
                }

                y1[k] = y2[k];
                x[k] = j[k];

                j[k] += xFactor;

                ++index[k];
            }
            else
            {
                x[k] += xFactor;
                j[k] = x[k];
            }
        }
    }

}

void GapCompChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    drawChart(&p);

    p.end();
}

void GapCompChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(LTData::colors[LTData::DEFAULT]);
    p->drawRect(40, 5, 115, 45);

    double yy = 0.0;
    for (int i = 0; i < 2; ++i)
    {
        DriverData dd = driverIdx[i] >= 0 ? eventData.driversData[driverIdx[i]] : DriverData();
        if (dd.carID > 0)
        {
            p->setPen(colors[i]);
            p->drawText(45, yy+20, dd.driver);
            yy += 20;
        }
    }
}

void GapCompChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();

    qApp->clipboard()->setImage(img);
}

void GapCompChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawChart(&p);

        drawLegend(&p);

        p.end();

        img.save(fName, "PNG");
    }
}

//=========================================================

void PosCompChartWidget::drawAxes(QPainter *p, int firstLap, int lastLap)
{
    p->setPen(QColor(LTData::colors[LTData::WHITE]));

    //x axe
    p->drawLine(37, height()-25, width()-5, height()-25);

    //y axe
    p->drawLine(37, height()-25, 37, 10);

    p->setFont(QFont("Arial", 10));
    p->setPen(QColor(LTData::colors[LTData::WHITE]));


    double yFactor = (double)((height()-35.0)/6.0);
    double yFactor2 = (double)((max-min)/6.0);
    double j = min;

    for (int i = height()-25; i >= 10; i-= yFactor, j += yFactor2)
    {
        p->setPen(QColor(LTData::colors[LTData::WHITE]));
        p->drawText(5, i+5, QString("%1").arg(round(j)));

        if (i != height()-25)
        {
            QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
            pen.setStyle(Qt::DashLine);
            p->setPen(pen);
            p->drawLine(37, i, width()-5, i);
        }
    }

    if (lastLap - firstLap > 0)
    {
        double xFactor = (width()-42) / /*((lapData.size() < 5) ?*/ (lastLap - firstLap) /*: 5)*/;
        double j = firstLap;
        double i = 37.0;
        int prevJ = firstLap;

        double jFactor = (lastLap - firstLap) < 6 ? 1.0 : (double)((lastLap - firstLap)/6.0);
        for (; i < width()-15.0 && round(j) < lastLap; /*i += xFactor,*/ j += jFactor)
        {
            i += (double)(round(j) - prevJ) * xFactor;
            prevJ = round(j);
            p->setPen(QColor(LTData::colors[LTData::WHITE]));
            p->drawText(round(i)-5, height()-10, QString("L%1").arg(round(j)));

            if (i > 37)
            {
                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
                pen.setStyle(Qt::DashLine);
                p->setPen(pen);
                p->drawLine(round(i), height()-25, round(i), 10);
            }
        }
    }

//    if (lastLap - firstLap > 0)
//    {
//        double xFactor = (width()-42) / ((lastLap - firstLap < 5) ? lastLap - firstLap : 5);
//        double j = firstLap;

//        double jFactor = lastLap - firstLap < 5 ? 1.0 : (double)((lastLap - firstLap)/5.0);
//        for (int i = 37; i < width()-15 && round(j) < lastLap; i += xFactor, j += jFactor)
//        {
//            p->setPen(QColor(LTData::colors[LTData::WHITE]));
//            p->drawText(i-5, height()-10, QString("L%1").arg(round(j)));

//            if (i > 37)
//            {
//                QPen pen(QColor(LTData::colors[LTData::DEFAULT]));
//                pen.setStyle(Qt::DashLine);
//                p->setPen(pen);
//                p->drawLine(i, height()-25, i, 10);
//            }
//        }
//    }
}

void PosCompChartWidget::drawChart(QPainter *p)
{
    int firstLap = 99, lastLap = 0;

    double x[2] = {37.0, 37.0};
    double y1[2];
    double y2[2];
    double yFactor = ((height()-35) / (max-min));

    for (int i = 0; i < 2; ++i)
    {
        if (!driverData[i].lapData.isEmpty())
        {
            if (driverData[i].lapData[0].numLap < firstLap)
                firstLap = driverData[i].lapData[0].numLap;

            if (driverData[i].lapData.last().numLap > lastLap)
                lastLap = driverData[i].lapData.last().numLap;
        }
    }
    drawAxes(p, firstLap, lastLap);
    if (lastLap - firstLap == 0)
        return;


    double xFactor = (width()-42) / (lastLap - firstLap);

    p->setRenderHint(QPainter::Antialiasing);
    int index[2] = {0,0};
    int j[2] = {(int)(x[0]), (int)(x[0])};
    for (int i = firstLap; i <= lastLap; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
        	LapData ld = driverData[k].getLapData(i);
//            if (!driverData[k].lapData.empty() && index[k] < driverData[k].lapData.size() && driverData[k].lapData[index[k]].numLap == i)
        	if (ld.carID == driverData[k].carID && ld.numLap == i)
            {
                QPen pen;
                pen.setWidth(2);
                pen.setColor(colors[k]);
                p->setPen(pen);

                int pos = ld.pos;//driverData[k].lapData[index[k]].pos;

                y2[k] = (double)(height()-25.0 - (double)(pos-min) * yFactor);

                if (index[k] == 0)
                    y1[k] = y2[k];

                p->drawLine(x[k], y1[k], j[k], y2[k]);

                if (driverData[k].lapData[index[k]].lapTime.toString() == "IN PIT")
                {
                    QPainterPath path;
                    path.addEllipse(QPoint(j[k], y2[k]), 6, 6);
                    p->setBrush(QBrush(colors[k]));
                    p->drawPath(path);
                }

                y1[k] = y2[k];
                x[k] = j[k];

                j[k] += xFactor;

                ++index[k];
            }
            else
            {
                x[k] += xFactor;
                j[k] = x[k];
            }
        }
    }
}

void PosCompChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

    drawChart(&p);
    //------------------------------------draw chart legend--------------
//    p.setRenderHint(QPainter::Antialiasing, false);
//    p.setBrush(QColor(20, 20, 20));

//    int yy = 0;
//    for (int i = 0; i < 4; ++i)
//    {
//        if (driverData[i].carID > 0)
//        {
//            p.setPen(QColor(20, 20, 20));
//            p.drawRect(40, yy, 115, 20);

//            p.setPen(colors[i]);
//            p.drawText(45, yy+20, driverData[i].driver);
//            yy += 20;
//        }
//    }
//    if (yy > 5)
//    {
//        p.setBrush(QBrush());
//        p.setPen(LTData::colors[LTData::DEFAULT]);
//        p.drawRect(40, 5, 115, yy);
//    }
    //--------------------------------------------------------

    p.end();
}

void PosCompChartWidget::drawLegend(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setBrush(QColor(20, 20, 20));

    p->setPen(LTData::colors[LTData::DEFAULT]);
    p->drawRect(40, 5, 115, 45);

    double yy = 0.0;
    for (int i = 0; i < 2; ++i)
    {
        if (driverData[i].carID > 0)
        {
            p->setPen(colors[i]);
            p->drawText(45, yy+20, driverData[i].driver);
            yy += 20;
        }
    }
}

void PosCompChartWidget::onCopy()
{
    QImage img = QImage(width(), height(), QImage::Format_ARGB32);
    QPainter p;
    p.begin(&img);

    p.setBrush(QColor(20,20,20));
    p.setPen(QColor(20,20,20));
    p.drawRect(0, 0, width(), height());
    drawChart(&p);

    drawLegend(&p);

    p.end();

    qApp->clipboard()->setImage(img);
}

void PosCompChartWidget::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this, "Select file", "", "*.png");
    if (!fName.isNull())
    {
        QImage img = QImage(width(), height(), QImage::Format_ARGB32);
        QPainter p;
        p.begin(&img);

        p.setBrush(QColor(20,20,20));
        p.setPen(QColor(20,20,20));
        p.drawRect(0, 0, width(), height());
        drawChart(&p);

        drawLegend(&p);

        p.end();

        img.save(fName, "PNG");
    }
}
