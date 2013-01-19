#include "colorsmanager.h"

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
