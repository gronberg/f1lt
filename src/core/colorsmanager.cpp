#include "colorsmanager.h"
#include "eventdata.h"

ColorsManager::ColorsManager()
{
    for (int i = 0; i < 10; ++i)
        colors.append(QColor());

    colors[LTPackets::DEFAULT] = QColor(150, 150, 150);
    colors[LTPackets::CYAN] = QColor(0, 255, 255);
    colors[LTPackets::GREEN] = QColor(0, 255, 0);
    colors[LTPackets::VIOLET] = QColor(255, 0, 255);
    colors[LTPackets::WHITE] = QColor(220, 220, 220);
    colors[LTPackets::YELLOW] = QColor(255, 255, 0);
    colors[LTPackets::RED] = QColor(231, 31, 31);
    colors[LTPackets::PIT] = QColor(231, 31, 31);
    colors[LTPackets::BACKGROUND] = QColor(20, 20, 20);
    colors[LTPackets::BACKGROUND2] = QColor(30, 30, 30);

    defaultColors = colors;

    driverColors.append(QColor(0, 255, 255));
    driverColors.append(QColor(0, 0, 255));
    driverColors.append(QColor(128, 0, 128));
    driverColors.append(QColor(255, 0, 255));
    driverColors.append(QColor(229, 30, 27));
    driverColors.append(QColor(165, 25, 28));
    driverColors.append(QColor(255, 255, 255));
    driverColors.append(QColor(148, 148, 176));
    driverColors.append(QColor(255, 209, 38));
    driverColors.append(QColor(255, 104, 57));
    driverColors.append(QColor(124, 196, 236));
    driverColors.append(QColor(12, 163, 218));
    driverColors.append(QColor(19, 139, 65));
    driverColors.append(QColor(114, 183, 136));
    driverColors.append(QColor(94, 93, 91));
    driverColors.append(QColor(157, 155, 156));
    driverColors.append(QColor(94, 109, 157));
    driverColors.append(QColor(39, 72, 125));
    driverColors.append(QColor(0, 255, 0));
    driverColors.append(QColor(26, 98, 21));
    driverColors.append(QColor(128, 64, 0));
    driverColors.append(QColor(206, 103, 0));
    driverColors.append(QColor(234, 78, 115));
    driverColors.append(QColor(245, 146, 166));

    defaultDriverColors = driverColors;
}

QColor ColorsManager::getCarColor(int no)
{
    QColor color = getColor(LTPackets::BACKGROUND);

    if (no > 0 && no < driverColors.size()+2)
        color = driverColors[no <= 12 ? no-1 : no -2];

    return color;
}

void ColorsManager::calculateDefaultDriverColors()
{
    int k = 0;
    for (int i = 0; i < 30 && k < driverColors.size(); ++i)
    {
        QPixmap car = SeasonData::getInstance().getCarImg(i);
        if (!car.isNull())
        {
            QColor average;
            if ((k % 2) == 0)
            {
                average = calculateAverageColor(car.toImage(), k);
            }
            else
            {
                average = driverColors[k-1];

                int red = average.red() + 50 > 255 ? 255 : average.red() + 50;
                int green = average.green() + 50 > 255 ? 255 : average.green() + 50;
                int blue = average.blue() + 50 > 255 ? 255 : average.blue() + 50;

                average = QColor(red, green, blue);
            }

            qDebug() << "NUMER=" << i << average;
            driverColors[k] = average;
            ++k;
        }
    }
}

class MyColor : public QColor
{
public:
    MyColor(QColor c) : QColor(c) { }
    bool operator<(const QColor &color) const
    {
        return ((red() < color.red()));// && (blue() < color.blue()) && (green() < color.green()));
    }
};

void addColor(QMap<MyColor, int> &colors, MyColor color)
{
    QList<MyColor> keys = colors.keys();
    for (int i = 0; i < keys.size(); ++i)
    {
        if ((abs(keys[i].red() - color.red()) <= 40) &&
            (abs(keys[i].green() - color.green()) <= 40) &&
            (abs(keys[i].blue() - color.blue()) <= 40))
        {
            colors[keys[i]]++;
            return;
        }
    }
    colors.insert(color, 1);
//    qDebug() << "COL="<<colors.key(color).red() << colors.key(color).green() << colors.key(color).blue();
}

QColor ColorsManager::calculateAverageColor(const QImage &car, int idx)
{
    QMap<MyColor, int> colors;

    for (int i = 0; i < car.width(); ++i)
    {
        for (int j = 0; j < car.height(); ++j)
        {
//            if (qRed(car.pixel(i, j)) != 0)
//            {
//                averageR += qRed(car.pixel(i, j));
//                ++rCnt;
//            }

//            if (qGreen(car.pixel(i, j)) != 0)
//            {
//                averageG += qGreen(car.pixel(i, j));
//                ++gCnt;
//            }

//            if (qBlue(car.pixel(i, j)) != 0)
//            {
//                averageB += qBlue(car.pixel(i, j));
//                ++bCnt;
//            }
            if ((qAlpha(car.pixel(i, j)) > 0) && ((qRed(car.pixel(i, j)) > 70) || (qGreen(car.pixel(i, j)) > 70) || (qBlue(car.pixel(i, j)) > 70)))
                addColor(colors, static_cast<MyColor>(QColor(car.pixel(i, j))));
        }
    }

    int max = 0;
    QColor color;
    QList<MyColor> keys = colors.keys();
    for (int i = 0; i < keys.size(); ++i)
    {
        qDebug() << keys[i].red() << keys[i].green() << keys[i].blue() << colors[keys[i]];
        if (colors[keys[i]] > max && !isColorInTheList(keys[i], idx))
        {
            max = colors[keys[i]];
            color = static_cast<QColor>(keys[i]);
        }
    }

//    if (secondDriver)
//    {
//        int red = color.red() + 50 > 255 ? 255 : color.red() + 50;
//        int green = color.green() + 50 > 255 ? 255 : color.green() + 50;
//        int blue = color.blue() + 50 > 255 ? 255 : color.blue() + 50;

//        color = QColor(red, green, blue);
//    }

    return color;

//    if (secondDriver)
    //        average += 5;
}

bool ColorsManager::isColorInTheList(QColor color, int idx)
{
    for (int i = 0; i < idx; ++i)
    {

        if ((abs(driverColors[i].red() - color.red()) <= 40) &&
            (abs(driverColors[i].green() - color.green()) <= 40) &&
            (abs(driverColors[i].blue() - color.blue()) <= 40))
            return true;
    }
    return false;
}
