#pragma once

// Forward declarations
class GameBar;
class Board;
class Keyboard;

/**
 * @brief Displays primary game elements, including a game bar on the top,
 * a board in the center, and a simple QWERTY keyboard on the bottom.
 */
class Game : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'Game' instance.
     */
    Game();
    ~Game();

    /**
     * @brief Provides access to the 'GameBar' instance.
     * @return The 'GameBar' instance.
     */
    GameBar *getGameBar();

    /**
     * @brief Provides access to the 'Board' instance.
     * @return The 'Board' instance.
     */
    Board *getBoard();

    /**
     * @brief Provides access to the 'Keyboard' instance.
     * @return The 'Keyboard' instance.
     */
    Keyboard *getKeyboard();

    /**
     * @brief Resets the progress, then starts a new round.
     */
    void restart();

    /**
     * @brief Restores the game state.
     * @note Call this function AFTER loading the attributes.
     */
    void load();

    /**
     * @brief Make this window non-resizable upon opening.
     */
    void show();

protected:
    /**
     * @brief Allows keyboard input for the Wordle game.
     * @param event The key event.
     */
    void keyPressEvent(QKeyEvent *event) override;

private:
    QVBoxLayout *mainLayout;
    GameBar *gameBar;
    Board *board;
    Keyboard *keyboard;
};
