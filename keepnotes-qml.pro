TEMPLATE = app

QT += qml quick quickcontrols2 svg network sql

CONFIG += c++11

SOURCES += src/main.cpp \
    src/control/appcontroller.cpp \
    src/database/databasemanager.cpp \
    src/model/user.cpp \
    src/sync/syncengine.cpp \
    src/utils/jsonparser.cpp \
    src/control/usermanager.cpp \
    src/model/notesmodel.cpp

RESOURCES += res/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    qtquickcontrols2.conf

HEADERS += \
    src/control/appcontroller.h \
    src/database/databasemanager.h \
    src/model/user.h \
    src/sync/syncengine.h \
    src/utils/jsonparser.h \
    src/control/usermanager.h \
    src/model/notesmodel.h

INCLUDEPATH += src/
