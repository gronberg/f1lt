# -------------------------------------------------
# Project created by QtCreator 2012-03-18T12:26:59
# -------------------------------------------------
QT += core \
    gui \
    network

TARGET = F1LT
PREFIX = /opt/$$TARGET
DEFINES += INSTALL_PREFIX=$$PREFIX
target.path = $$PREFIX/bin/
TEMPLATE = app

OBJECTS_DIR = obj/
MOC_DIR = moc/
UI_DIR = ui/

INSTALLS += target \
    DATA_FILES

DATA_FILES.files = season.dat trackdata.dat
DATA_FILES.path = $$PREFIX/share/

RC_FILE = f1lt.rc

#DEFINES += QT_NO_DEBUG_OUTPUT
SOURCES += src/tools/sessionanalysiswidget.cpp \
    src/main_gui/weatherchartswidget.cpp \
    src/main_gui/nosessionboardwidget.cpp \
    src/main.cpp \
    src/main_gui/ltwindow.cpp \
    src/net/datastreamreader.cpp \
    src/net/httpdatareader.cpp \
    src/net/socketdatareader.cpp \
    src/main_gui/driverdatawidget.cpp \
    src/charts/chartwidget.cpp \
    src/main_gui/sessiondatawidget.cpp \
    src/tools/headtoheaddialog.cpp \
    src/core/driverdata.cpp \
    src/core/eventdata.cpp \
    src/player/eventrecorder.cpp \
    src/main_gui/preferencesdialog.cpp \
    src/main_gui/logindialog.cpp \
    src/player/eventplayer.cpp \
    src/tools/laptimecomparisondialog.cpp \
    src/core/f1ltcore.cpp \
    src/main_gui/eventstatuswidget.cpp \
    src/charts/sessionlaptimeschart.cpp \
    src/charts/fplaptimeschart.cpp \
    src/charts/lapcompchart.cpp \
    src/charts/weatherchart.cpp \
    src/core/lapdata.cpp \
    src/net/packetparser.cpp \
    src/main_gui/ltwidget.cpp \
    src/main_gui/models/ltmodel.cpp \
    src/main_gui/models/practicemodel.cpp \
    src/main_gui/lttableview.cpp \
    src/main_gui/models/qualimodel.cpp \
    src/main_gui/models/racemodel.cpp \
    src/main_gui/models/driverlaphistorymodel.cpp \
    src/main_gui/ltitemdelegate.cpp \
    src/main_gui/models/speedrecordsmodel.cpp \
    src/main_gui/models/fastestlapsmodel.cpp \
    src/main_gui/models/pitstopsmodel.cpp \
    src/core/seasondata.cpp \
    src/core/sessiontimer.cpp \
    src/net/ltfilesmanager.cpp \
    src/tools/ltfilesmanagerdialog.cpp \
    src/main_gui/aboutdialog.cpp \
    src/charts/driverdatachart.cpp \
    src/tools/followadriverdialog.cpp \
    src/tools/sessiontimeswidget.cpp \
    src/tools/driverradarpositioner.cpp \
    src/tools/driverradar.cpp \
    src/tools/drivertrackerpositioner.cpp \
    src/tools/drivertracker.cpp \
    src/tools/drivertrackerwidget.cpp \
    src/tools/drivertrackerinfo.cpp \
    src/net/packetbuffer.cpp \
    src/main_gui/delaywidget.cpp \
    src/core/trackrecords.cpp \
    src/tools/trackrecordsdialog.cpp
HEADERS += src/tools/sessionanalysiswidget.h \
    src/main_gui/weatherchartswidget.h \
    src/main_gui/nosessionboardwidget.h \
    src/main_gui/ltwindow.h \
    src/net/datastreamreader.h \
    src/net/httpdatareader.h \
    src/net/socketdatareader.h \
    src/main_gui/driverdatawidget.h \
    src/charts/chartwidget.h \
    src/main_gui/sessiondatawidget.h \
    src/tools/headtoheaddialog.h \
    src/core/driverdata.h \
    src/core/eventdata.h \
    src/player/eventrecorder.h \
    src/main_gui/preferencesdialog.h \
    src/main_gui/logindialog.h \
    src/player/eventplayer.h \
    src/tools/laptimecomparisondialog.h \
    src/core/f1ltcore.h \
    src/main_gui/eventstatuswidget.h \
    src/charts/sessionlaptimeschart.h \
    src/charts/fplaptimeschart.h \
    src/charts/lapcompchart.h \
    src/charts/weatherchart.h \
    src/core/lapdata.h \
    src/net/packetparser.h \
    src/main_gui/ltwidget.h \
    src/main_gui/models/ltmodel.h \
    src/main_gui/models/practicemodel.h \
    src/main_gui/lttableview.h \
    src/main_gui/models/qualimodel.h \
    src/main_gui/models/racemodel.h \
    src/main_gui/models/driverlaphistorymodel.h \
    src/main_gui/ltitemdelegate.h \
    src/main_gui/models/speedrecordsmodel.h \
    src/main_gui/models/fastestlapsmodel.h \
    src/main_gui/models/pitstopsmodel.h \
    src/core/seasondata.h \
    src/core/ltpackets.h \
    src/core/sessiontimer.h \
    src/net/ltfilesmanager.h \
    src/tools/ltfilesmanagerdialog.h \
    src/main_gui/aboutdialog.h \
    src/charts/driverdatachart.h \
    src/tools/followadriverdialog.h \
    src/tools/sessiontimeswidget.h \
    src/tools/driverradarpositioner.h \
    src/tools/driverradar.h \
    src/tools/drivertrackerpositioner.h \
    src/tools/drivertracker.h \
    src/tools/drivertrackerwidget.h \
    src/tools/drivertrackerinfo.h \
    src/net/packetbuffer.h \
    src/main_gui/delaywidget.h \
    src/core/trackrecords.h \
    src/tools/trackrecordsdialog.h
FORMS += ui/sessionanalysiswidget.ui \
    ui/weatherchartswidget.ui \
    ui/nosessionboardwidget.ui \
    ui/ltwindow.ui \
    ui/driverdatawidget.ui \
    ui/chartwidget.ui \
    ui/sessiondatawidget.ui \
    ui/headtoheaddialog.ui \
    ui/preferencesdialog.ui \
    ui/connectiondialog.ui \
    ui/logindialog.ui \
    ui/eventplayer.ui \
    ui/laptimecomparisondialog.ui \
    ui/eventstatuswidget.ui \
    ui/ltwidget.ui \
    ui/ltfilesmanagerdialog.ui \
    ui/aboutdialog.ui \
    ui/followadriverdialog.ui \
    ui/sessiontimeswidget.ui \
    ui/drivertrackerwidget.ui \
    ui/delaywidget.ui \
    ui/trackrecordsdialog.ui
INCLUDEPATH += src/
RESOURCES += icons/icons.qrc \
    other_files.qrc

OTHER_FILES += \
    CHANGELOG
