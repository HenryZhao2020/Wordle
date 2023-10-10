QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(SingleApplication/SingleApplication.pri)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = "Wordle"

QMAKE_TARGET_DESCRIPTION = "Wordle"

RC_ICONS = "Logo.ico"

VERSION = 1.3.0

SOURCES += *.cpp

HEADERS += *.h

PRECOMPILED_HEADER = PCH.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
