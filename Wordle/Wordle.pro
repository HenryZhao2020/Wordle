QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Attr.cpp \
    Board.cpp \
    Color.cpp \
    Dialog.cpp \
    File.cpp \
    Game.cpp \
    GameBar.cpp \
    Icon.cpp \
    Keyboard.cpp \
    Main.cpp \
    Square.cpp \
    WinGui.cpp \
    Word.cpp

HEADERS += \
    Attr.h \
    Board.h \
    Color.h \
    Dialog.h \
    File.h \
    Game.h \
    GameBar.h \
    Icon.h \
    Keyboard.h \
    PCH.h \
    Square.h \
    WinGui.h \
    Word.h

PRECOMPILED_HEADER += PCH.h

include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

TARGET = "Wordle"

QMAKE_TARGET_DESCRIPTION = "Wordle"

RC_ICONS = "Logo.ico"

VERSION = 2.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
