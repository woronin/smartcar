QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = smartcar
DEPENDPATH += .

INCLUDEPATH += .

# Input
HEADERS += channel.h define.h vench.h \
    forms/bluetoothmode.h \
    forms/mainwindow.h
FORMS += \
    forms/bluetoothmode.ui \
    forms/mainwindow.ui
SOURCES += channel.cpp \
    forms/bluetoothmode.cpp \
    forms/mainwindow.cpp \
           main.cpp \
           vench.cpp

RESOURCES += \
    res.qrc


