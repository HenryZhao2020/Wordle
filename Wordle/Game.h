#pragma once

#include "PCH.h"

class GameBar;
class Board;
class Keyboard;

class Game : public QMainWindow {
    Q_OBJECT

public:
    Game();

    GameBar *getGameBar();
    Board *getBoard();
    Keyboard *getKeyboard();
    void restart();
    void restore();

private:
    QVBoxLayout *vboxLayout;
    GameBar *bar;
    Board *board;
    Keyboard *keyboard;

    void keyPressEvent(QKeyEvent *event);
};
