QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Adam_Chlewicki_Mandelbrot_Set
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        drawingArea.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        drawingArea.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

QMAKE_LFLAGS += -fopenmp

QMAKE_CXXFLAGS += -fopenmp

LIBS += -fopenmp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
