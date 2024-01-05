#pragma once

class GameBar;
class Board;
class Keyboard;

/**
 * @brief Displays primary game elements.
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
     * @brief Provides access to the private 'GameBar' instance.
     * @return The 'GameBar' instance.
     */
    GameBar *getGameBar();

    /**
     * @brief Provides access to the private 'Board' instance.
     * @return The 'Board' instance.
     */
    Board *getBoard();

    /**
     * @brief Provides access to the private 'Keyboard' instance.
     * @return The 'Keyboard' instance.
     */
    Keyboard *getKeyboard();

    /**
     * @brief Resets the progress, then starts a new round.
     */
    void restart();

    /**
     * @brief Restores the game state from the loaded attributes.
     */
    void load();

    /**
     * @brief Overrides the original QMainWindow::show() method.
     */
    void show();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QVBoxLayout *vboxLayout;
    GameBar *gameBar;
    Board *board;
    Keyboard *keyboard;
};
