QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network positioning

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fenpreferences.cpp \
    main.cpp \
    mainwindow.cpp \
    navdata.cpp \
    udpdata.cpp

HEADERS += \
    fenpreferences.h \
    mainwindow.h \
    navdata.h \
    udpdata.h

FORMS += \
    fenpreferences.ui \
    mainwindow.ui \
    navdata.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
