QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gecommon.cpp \
    gesegment.cpp \
    gesignalhandler.cpp \
    gestone.cpp \
    main.cpp \
    gamewidget.cpp \
    networkwidget.cpp \

HEADERS += \
    gamewidget.hpp \
    gecommon.hpp \
    geconfig.hpp \
    gesegment.hpp \
    gesignalhandler.hpp \
    gestone.hpp \
    netconfig.hpp \
    networkwidget.hpp \
    comconfig.hpp\

FORMS += \
    gamewidget.ui \
    networkwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
