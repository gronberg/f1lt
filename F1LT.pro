# -------------------------------------------------
# Project created by QtCreator 2012-03-18T12:26:59
# -------------------------------------------------
QT += core \
    gui \
    network
PREFIX = ~/opt/
DEFINES += INSTALL_PREFIX=$$PREFIX
TARGET = F1LT
target.path = $$PREFIX/bin/
TEMPLATE = app
OBJECTS_DIR = obj/
MOC_DIR = moc/
UI_DIR = ui/
INSTALLS += target \
    DATA_FILES \
    LT_FILES
LT_FILES.files = ltdata/*
LT_FILES.path = ~/.f1lt/ltdata/
DATA_FILES.files = season.dat
DATA_FILES.path = $$PREFIX/share/$$TARGET/

# DEFINES += QT_NO_DEBUG_OUTPUT
SOURCES += src/tools/sessionanalysiswidget.cpp \
    src/main_gui/weatherchartswidget.cpp \
    src/main_gui/nosessionboardwidget.cpp \
    src/main.cpp \
    src/main_gui/ltwindow.cpp \
    src/core/datastreamreader.cpp \
    src/core/ltdata.cpp \
    src/net/httpdatareader.cpp \
    src/net/socketdatareader.cpp \
    src/main_gui/lttablewidget.cpp \
    src/main_gui/driverdatawidget.cpp \
    src/charts/chartwidget.cpp \
    src/main_gui/trackstatuswidget.cpp \
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
    src/charts/weatherchart.cpp
HEADERS += src/tools/sessionanalysiswidget.h \
    src/main_gui/weatherchartswidget.h \
    src/main_gui/nosessionboardwidget.h \
    src/main_gui/ltwindow.h \
    src/core/datastreamreader.h \
    src/core/ltdata.h \
    src/net/httpdatareader.h \
    src/net/socketdatareader.h \
    src/main_gui/lttablewidget.h \
    src/main_gui/driverdatawidget.h \
    src/charts/chartwidget.h \
    src/main_gui/trackstatuswidget.h \
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
    src/charts/weatherchart.h
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
    ui/eventstatuswidget.ui
INCLUDEPATH += src/
RESOURCES += icons/icons.qrc
