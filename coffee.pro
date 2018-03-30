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

target.path = $$[QT_INSTALL_EXAMPLES]/sql/books
INSTALLS += target
