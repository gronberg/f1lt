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




#ifndef QUALIMODEL_H
#define QUALIMODEL_H

#include "ltmodel.h"
#include "../../core/eventdata.h"

#include <QDebug>

class QualiLessThan
{
public:
    QualiLessThan(int period, const LapTime &time107) : qPeriod(period), time107p(time107) { }
    bool operator()(DriverData *d1, DriverData *d2)
    {
        if (qPeriod > 0)
        {            
            if (d1->getQualiTime(qPeriod).isValid() && d2->getQualiTime(qPeriod).isValid())
            {
                if (d1->getQualiTime(qPeriod) == d2->getQualiTime(qPeriod))
                    return (d1->getPosition() < d2->getPosition());

                return (d1->getQualiTime(qPeriod) < d2->getQualiTime(qPeriod));
            }
        }

//        else
        {
            //this is only because LT server likes to mess things up when drivers are out of 107%
            //during Q1 and doesn't sort them properly
            if (((!d1->getQualiTime(1).isValid() || d1->getQualiTime(1) > time107p) ||
                (!d2->getQualiTime(1).isValid() || d2->getQualiTime(1) > time107p)) &&
                (d1->getQualiTime(1).isValid() != d2->getQualiTime(1).isValid()))
            {
                return d1->getQualiTime(1) < d2->getQualiTime(1);
            }
        }

        if (d1->getColorData().driverColor() == LTPackets::PIT || d1->getColorData().driverColor() == LTPackets::RED ||
            d2->getColorData().driverColor() == LTPackets::PIT || d2->getColorData().driverColor() == LTPackets::RED)
        {
            int q = EventData::getInstance().getQualiPeriod();

            while (q > 0)
            {
                if (d1->getQualiTime(q).isValid() && d2->getQualiTime(q).isValid())
                {
                    if (d1->getQualiTime(q) == d2->getQualiTime(q))
                        return (d1->getPosition() < d2->getPosition());

                    return (d1->getQualiTime(q) < d2->getQualiTime(q));
                }
                --q;
            }
        }

//        if (qPeriod == 0 && EventData::getInstance().isSessionStarted() && d1->getLastLap().getTime().isValid() && d2->getLastLap().getTime().isValid())
//            return d1->getLastLap().getTime() < d2->getLastLap().getTime();

        return (d1->getPosition() < d2->getPosition());
    }

private:
    int qPeriod;
    LapTime time107p;
};

class QualiModel : public LTModel
{
public:
    QualiModel(QObject *parent = 0);

    virtual void updateLT();

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    virtual QVariant driverRowData(const DriverData &dd, const QModelIndex &index, int role) const;
    virtual QVariant headerRowData(const QModelIndex &index, int role) const;
    virtual QVariant extraRowData(const QModelIndex &index, int role) const;

    QVariant gapToSelected(const DriverData &dd) const;

    virtual bool selectDriver(int id, int column)
    {
        LTModel::selectDriver(id, column);
        if (column == 0 || column == 4 || column == 5 || column == 6)
        {
            updateLT();
            return true;
        }

        return false;
    }
    LapTime findBestQ1Time() const;

    virtual void headerClicked(int column);

private:
    EventData &eventData;
    int qualiPeriodSelected;
};

#endif // QUALIMODEL_H
