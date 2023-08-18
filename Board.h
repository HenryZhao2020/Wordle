#ifndef BOARD_H
#define BOARD_H

#include "PCH.h"
#include "Color.h"

class Game;
class GameBar;
class Square;

class Board : public QFrame {
    Q_OBJECT

public:
    Board(Game *game);

    void append(const QString &letter);
    void removeLast();
    void checkGuess();
    void setTextAt(int row, int column, const QString &text);
    void mark(int row, int column, Color color, bool animated);
    void remarkAll();
    void makeLost();

private:
    Game *game;
    GameBar *gameBar;
    Square *squares[6][5];

    bool isGuessValid(const QString &guess);
    bool isHardConditionsMet(const QString &guess);
    void markGuess(int row, const QString &guess);
    void moveRowDown();
    void makeEnded(const QPixmap &pixmap, const QString &text);
    void makeWon();
};

#endif
