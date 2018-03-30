TEMPLATE = app
INCLUDEPATH += .

HEADERS     = initdb.h \
    coffeewindow.h \
    coffeedelegate.h
RESOURCES   = \
    coffees.qrc
SOURCES     = main.cpp \
    coffeewindow.cpp \
    coffeedelegate.cpp
FORMS       = \
    coffeewindow.ui

QT += sql widgets widgets

target.path = ./build
INSTALLS += target
