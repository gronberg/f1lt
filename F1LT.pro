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
SOURCES += src/sessionanalysiswidget.cpp \
    src/weatherchartswidget.cpp \
    src/nosessionboardwidget.cpp \
    src/main.cpp \
    src/ltwindow.cpp \
    src/datastreamreader.cpp \
    src/ltdata.cpp \
    src/httpdatareader.cpp \
    src/socketdatareader.cpp \
    src/lttablewidget.cpp \
    src/driverdatawidget.cpp \
    src/chartwidget.cpp \
    src/trackstatuswidget.cpp \
    src/sessiondatawidget.cpp \
    src/headtoheaddialog.cpp \
    src/driverdata.cpp \
    src/eventdata.cpp \
    src/eventrecorder.cpp \
    src/preferencesdialog.cpp \
    src/logindialog.cpp \
    src/eventplayer.cpp \
    src/laptimecomparisondialog.cpp \
    src/f1ltcore.cpp \
    src/eventstatuswidget.cpp \
    src/sessionlaptimeschart.cpp
HEADERS += src/sessionanalysiswidget.h \
    src/weatherchartswidget.h \
    src/nosessionboardwidget.h \
    src/ltwindow.h \
    src/datastreamreader.h \
    src/ltdata.h \
    src/httpdatareader.h \
    src/socketdatareader.h \
    src/lttablewidget.h \
    src/driverdatawidget.h \
    src/chartwidget.h \
    src/trackstatuswidget.h \
    src/sessiondatawidget.h \
    src/headtoheaddialog.h \
    src/driverdata.h \
    src/eventdata.h \
    src/eventrecorder.h \
    src/preferencesdialog.h \
    src/logindialog.h \
    src/eventplayer.h \
    src/laptimecomparisondialog.h \
    src/f1ltcore.h \
    src/eventstatuswidget.h \
    src/sessionlaptimeschart.h
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
