QT       += core gui serialport opengl
LIBS     += -lopengl32 -lglu32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection/genericport.cpp \
    connection/serialporthandler.cpp \
    displaywidget.cpp \
    main.cpp \
    mainwindow.cpp \
    objloader.cpp \
    textrenderer.cpp

HEADERS += \
    connection/genericport.h \
    connection/serialporthandler.h \
    displaywidget.h \
    mainwindow.h \
    objloader.h \
    textrenderer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
