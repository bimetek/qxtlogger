#-------------------------------------------------
#
# Project created by QtCreator 2014-10-06T17:58:59
#
#-------------------------------------------------

QT       -= gui

TARGET = qxtlogger
TEMPLATE = lib

DEFINES += QXTLOGGER_LIBRARY

SOURCES += qxtlogger.cpp \
    qxtlogstream.cpp \
    qxtloggerengine.cpp \
    qxtbasicstdloggerengine.cpp \
    qxtabstractiologgerengine.cpp \
    qxtabstractfileloggerengine.cpp \
    qxtbasicfileloggerengine.cpp

HEADERS += qxtlogger.h\
        qxtlogger_global.h \
    qxtlogstream.h \
    qxtloggerengine.h \
    qxtbasicstdloggerengine.h \
    qxtlogger_p.h \
    qxtabstractiologgerengine.h \
    qxtabstractfileloggerengine.h \
    qxtbasicfileloggerengine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
