TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    boardstate.cpp \
    boardmodel.cpp \
    statemodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    boardstate.h \
    boardmodel.h \
    statemodel.h
