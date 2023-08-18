#include "PCH.h"
#include "Game.h"
#include "Attr.h"
#include "Dict.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    QFontDatabase::addApplicationFont("Fonts/Ubuntu_Bold.ttf");
    QDir::setCurrent("Wordle_Files");
    Dict::load();

    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));
    QObject::connect(&app, &QApplication::aboutToQuit, &app, &Attr::save);

    Game game;
    game.restore();
    game.show();

    qDebug() << Attr::answer;

    return app.exec();
}
