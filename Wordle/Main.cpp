#include "Game.h"
#include "Attr.h"
#include "Word.h"

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

    // No secondary instances allowed
    // Switch to the primary instance instead
    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        // Send a message to notify the primary instance
        app.sendMessage("Raise");
        // Exit the secondary instance
        return 0;
    }

    // Set current directory to "Res"
    QDir::setCurrent(app.applicationDirPath());
    QDir::setCurrent("Res");

    // Load external fonts
    QFontDatabase::addApplicationFont("Montserrat-SemiBold.ttf");

    // Load words from files
    Word::load();

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
    game->load();
    game->show();

    return app.exec();
}
