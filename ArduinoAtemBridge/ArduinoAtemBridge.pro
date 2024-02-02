QT       += core gui charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    handleserialdata.cpp \
    main.cpp \
    mainwindow.cpp \
    qarduinoups.cpp \
    qconfig.cpp \
    qdigitalclock.cpp \
    qhomescreen.cpp \
    qserialview.cpp \
    qvideoboard.cpp \
    qvideoboardview.cpp

HEADERS += \
    consts.h \
    handleserialdata.h \
    mainwindow.h \
    qarduinoups.h \
    qconfig.h \
    qdigitalclock.h \
    qhomescreen.h \
    qserialview.h \
    qvideoboard.h \
    qvideoboardview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# my compiler flags
QMAKE_CXXFLAGS += -v
