#pragma once

#include <QFrame>

class Game;
class GameBar;
class Square;

enum class Color;

class Board : public QFrame {
    Q_OBJECT

public:
    Board(Game *game);
    ~Board();

    void append(const QChar &letter);
    void removeLast();
    void checkGuess();
    void placeLetter(int row, int col, const QChar &letter, bool animated);
    void markSquare(int row, int col, Color color, bool animated);
    void updateColors();
    void lose();
    void setFrozen(bool frozen);
    bool isFrozen();

private:
    Game *game;
    GameBar *gameBar;
    Square *squares[6][5];
    bool frozen;

    bool isGuessValid(const QString &guess);
    bool isHardValid(const QString &guess);
    void markGuess(const QString &guess);
    void endRound(const QIcon &icon, const QString &text);
    void win();
};
