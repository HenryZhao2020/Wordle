#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QKeyEvent>

class GameBar;
class Board;
class Keyboard;

class Game : public QMainWindow {
    Q_OBJECT

public:
    Game();
    ~Game();

    GameBar *getGameBar();
    Board *getBoard();
    Keyboard *getKeyboard();

    void restart();
    void loadSave();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QVBoxLayout *mainLayout;
    GameBar *gameBar;
    Board *board;
    Keyboard *keyboard;
};
