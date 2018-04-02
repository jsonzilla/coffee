TEMPLATE = app
INCLUDEPATH += .

HEADERS     = \
    coffeewindow.h \
    coffeedelegate.h \
    databasemamager.h \
    tablemodelutils.h
RESOURCES   = \
    coffees.qrc
SOURCES     = main.cpp \
    coffeewindow.cpp \
    coffeedelegate.cpp \
    databasemamager.cpp
FORMS       = \
    coffeewindow.ui

QT += sql widgets widgets

target.path = build
INSTALLS += target
