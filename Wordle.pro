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
    Dict.cpp \
    FileUtil.cpp \
    Game.cpp \
    GameBar.cpp \
    IconUtil.cpp \
    Keyboard.cpp \
    Main.cpp \
    Square.cpp

HEADERS += \
    Attr.h \
    Board.h \
    Color.h \
    Dialog.h \
    Dict.h \
    FileUtil.h \
    Game.h \
    GameBar.h \
    IconUtil.h \
    Keyboard.h \
    Square.h

PRECOMPILED_HEADER +=

include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

TARGET = "Wordle"
VERSION = "3.0.0"

win32 {
    RC_ICONS = "Wordle.ico"
    QMAKE_TARGET_DESCRIPTION = "Wordle"
} mac {
    ICON = "Wordle.icns"
}

TRANSLATIONS += \
    Wordle_zh_CN.ts \
    Wordle_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Wordle.qrc
