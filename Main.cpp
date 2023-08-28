#include "PCH.h"
#include "Game.h"
#include "Attr.h"
#include "Dict.h"

void restoreWindow() {
    HWND hwnd = (HWND) File::readAll("WinID").toInt();
    SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    SetForegroundWindow(hwnd); 
    SetActiveWindow(hwnd); 
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
    RedrawWindow(hwnd, NULL, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    QDir::setCurrent("Wordle_Files");

    RunGuard guard("Wordle");
    if (!guard.tryToRun()) {
        restoreWindow();
        return 0;
    }

    QFontDatabase::addApplicationFont("../Fonts/Ubuntu_Bold.ttf");
    Dict::load();

    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));
    QObject::connect(&app, &QApplication::aboutToQuit, &app, &Attr::save);

    Game game;
    game.restore();
    game.show();

    qDebug() << Attr::answer;

    File::write("WinID", QString::number(game.winId()));
    return app.exec();
}
