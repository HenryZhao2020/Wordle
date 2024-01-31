#include "Game.h"
#include "Attr.h"
#include "Word.h"

#include "SingleApplication"

Game *game;
// Whether the OS is in dark mode
bool dark = WinGui::isDarkMode();

/**
 * @brief Processes communication between different program instances.
 * @param message Sent message to the primary instance.
 */
void processMessage(quint32, QByteArray message) {
    if (message == "Raise") {
        WinGui::raiseWidget(game);
    }
}

/**
 * @brief Detects and adapts to any OS theme change.
 */
void adaptTheme() {
    bool newDark = WinGui::isDarkMode();
    if (newDark != dark) {
        QApplication::setStyle("Fusion");
        dark = newDark;
    }
}

/**
 * @brief Sets up the program, then starts the game.
 * @param argc Number of arguments.
 * @param argv List of arguments.
 * @return Execution code.
 */
int main(int argc, char **argv) {
    SingleApplication app(argc, argv, true);
    AllowSetForegroundWindow(DWORD(app.primaryPid()));

    // No secondary instances allowed
    // Switch to the primary instance instead
    if (app.isSecondary()) {
        // Send a message to notify the primary instance
        app.sendMessage("Raise");
        // Exit the secondary instance
        return 0;
    }

    // Set current directory to the parent folder
    QDir::setCurrent(app.applicationDirPath());
    // Set current directory to 'Res'
    QDir::setCurrent("Res");

    // Load words from files
    Word::load();

    // Load saved attributes if possible
    bool loaded = Attr::get().load();

    // Update the display language
    QTranslator translator;
    if (Attr::get().lang == Lang::CHINESE_SIMPLIFIED &&
        translator.load("../Wordle_zh_CN")) {
        app.installTranslator(&translator);
    }

    // Load external fonts
    QFontDatabase::addApplicationFont("Montserrat-SemiBold.ttf");
    // For displaying chinese characters
    QFontDatabase::addApplicationFont("MicrosoftYaheiUI-Bold.ttc");

    // Set application name
    app.setApplicationName(QApplication::tr("Wordle"));

    // Style application
    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));

    // Save attributes on quit
    QObject::connect(&app, &QApplication::aboutToQuit, &app, [] {
        Attr::get().save();
    });

    // Process communication between different program instances
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     &app, &processMessage);

    // Detect theme change every second
    QTimer themeTimer;
    QObject::connect(&themeTimer, &QTimer::timeout, &app, &adaptTheme);
    themeTimer.start(1000);

    // Start the game
    game = new Game();
    if (loaded) {
        game->load();
    }
    game->show();

    return app.exec();
}
