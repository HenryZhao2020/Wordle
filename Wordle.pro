QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += *.cpp \
    SingleApplication/singleapplication_p.cpp \
    SingleApplication/singleapplication.cpp

HEADERS += *.h \
    SingleApplication/singleapplication_p.h \
    SingleApplication/singleapplication.h

TARGET = "Wordle"

QMAKE_TARGET_DESCRIPTION = "Wordle"

RC_ICONS = "Logo.ico"

PRECOMPILED_HEADER = "PCH.h"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
