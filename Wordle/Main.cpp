#include "Game.h"
#include "Attr.h"
#include "Dict.h"

Game *game;

void raiseWidget(QWidget *widget) {
    HWND hwnd = (HWND) widget->winId();
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
    }
    SetForegroundWindow(hwnd);
}

void receivedMessage(quint32, QByteArray message) {
    if (message == "Raise") {
        raiseWidget(game);
    }
}

int main(int argc, char *argv[]) {
    SingleApplication app(argc, argv, true);

    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        app.sendMessage("Raise");
        return 0;
    }

    QDir::setCurrent(QApplication::applicationDirPath());    
    QDir::setCurrent("Res");

    QFontDatabase::addApplicationFont("Ubuntu_Bold.ttf");
    Dict::load();

    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));
    QObject::connect(&app, &QApplication::aboutToQuit, &app, &Attr::save);
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     &app, &receivedMessage);

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, &app, [] {
        QApplication::setStyle("Fusion");
        game->adjustSize();
    });
    timer->start(1000);

    game = new Game();
    game->restore();
    game->show();

    return app.exec();
}
