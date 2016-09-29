TEMPLATE = app

QT += qml quick quickcontrols2 svg network sql

CONFIG += c++11

SOURCES += main.cpp \
    appcontroller.cpp \
    databasemanager.cpp \
    user.cpp \
    syncengine.cpp \
    jsonparser.cpp \
    usermanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    qtquickcontrols2.conf

HEADERS += \
    appcontroller.h \
    databasemanager.h \
    user.h \
    syncengine.h \
    jsonparser.h \
    usermanager.h
