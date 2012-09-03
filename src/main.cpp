#include <QtGui/QApplication>
#include <QMessageBox>
#include "main_gui/ltwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok = SeasonData::getInstance().loadSeasonFile();
    if (!ok)
    {
        QMessageBox::critical(0, "Error!", "Could not load input data file (season.dat). Click OK to exit.");
        exit(-1);
    }

    LTWindow w;
    w.show();
    
    return a.exec();
}
