#include "Game.h"
#include "Dialog.h"
#include "Attr.h"
#include "Dict.h"
#include "FileUtil.h"
#include "SingleApplication"

#include <QDir>
#include <QFontDatabase>
#include <QTranslator>
#include <QLibraryInfo>
#include <QTimer>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

void raiseWidget(QWidget* widget) {
#ifdef Q_OS_WINDOWS
    HWND hwnd = (HWND)widget->winId();

    // check if widget is minimized to Windows task bar
    if (::IsIconic(hwnd)) {
        ::ShowWindow(hwnd, SW_RESTORE);
    }

    ::SetForegroundWindow(hwnd);
#else
    widget->show();
    widget->raise();
    widget->activateWindow();
#endif
}

int main(int argc, char *argv[]) {
#ifdef Q_OS_WINDOWS
    SingleApplication app(argc, argv, true);
    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        app.sendMessage("RAISE_WIDGET");
        return 0;
    }
#else
    SingleApplication app(argc, argv, true);
#endif

    QDir::setCurrent(app.applicationDirPath());

    app.setStyle("Fusion");
    app.setStyleSheet(FileUtil::readAll(":/conf/Styles.qss"));
    QObject::connect(&app, &QApplication::aboutToQuit, [] {
        Attr::get().save();
    });

    QFontDatabase::addApplicationFont(":/fonts/MontserratAlternates-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ZCOOLKuaiLe-Regular.ttf");

    Dict::load();
    bool loaded = Attr::get().load();

    QTranslator baseTrans, appTrans;
    if (Attr::get().lang != Lang::ENGLISH) {
        QString basePath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
        QString code = Lang::getLangTerrCode(Attr::get().lang);

        if (baseTrans.load("qtbase_" + code, basePath)) {
            app.installTranslator(&baseTrans);
        }
        if (appTrans.load(":/i18n/Wordle_" + code)) {
            app.installTranslator(&appTrans);
        }
    }

    Game game;
    game.show();

#ifdef Q_OS_WINDOWS
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     &game, [&game] () { raiseWidget(&game); } );
#else
    QObject::connect(&app, &SingleApplication::instanceStarted,
                     &game, [&game] () { raiseWidget(&game); } );
#endif

    if (loaded) {
        game.loadSave();
    } else {
        auto dialog = new HelpDialog(&game);
        QTimer::singleShot(500, dialog, &Dialog::show);
    }

    return app.exec();
}
