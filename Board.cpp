#include "Board.h"
#include "Game.h"
#include "GameBar.h"
#include "Square.h"
#include "Keyboard.h"
#include "Attr.h"
#include "Dict.h"

Board::Board(Game *game) : QFrame(game) {
    this->game = game;
    gameBar = game->getGameBar();

    auto gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 5; c++) {
            squares[r][c] = new Square(this);
            gridLayout->addWidget(squares[r][c], r, c);
        }
    }
}

void Board::append(const QString &letter) {
    if (Attr::column == 5) {
        return;
    }

    squares[Attr::row][Attr::column]->setText(letter);
    if (Attr::animated) {
        squares[Attr::row][Attr::column]->zoom();
    }

    Attr::column++;
    Attr::guesses[Attr::row].append(letter);
    gameBar->setHintVisible(false);
}

void Board::removeLast() {
    if (Attr::column == 0) {
        return;
    }

    Attr::column--;
    Attr::guesses[Attr::row].removeLast();
    gameBar->setHintVisible(false);

    squares[Attr::row][Attr::column]->setText("");
}

void Board::checkGuess() {
    gameBar->setHintVisible(Attr::hintVisible);

    QString guess = Attr::guesses[Attr::row];
    if (!isGuessValid(guess)) {   
        gameBar->setHintIcon(getIcon("Exclamation.svg"));
        return;
    }

    markGuess(Attr::row, guess);    

    if (guess == Attr::answer) {
        makeWon();
    } else if (Attr::row == 5) {
        makeLost();
    } else {
        moveRowDown();
    }
}

void Board::setTextAt(int row, int column, const QString &text) {
    squares[row][column]->setText(text);
}

void Board::mark(int row, int column, Color color, bool animated) {
    if (animated) {
        squares[row][column]->fade(color);
    } else {
        squares[row][column]->mark(color);
    }
}

void Board::remarkAll() {
    for (int r = 0; r < Attr::row; r++) {
        for (int c = 0; c < 5; c++) {
            mark(r, c, Attr::squareColors[r][c], false);
        }
    }
}

bool Board::isGuessValid(const QString &guess) {
    if (guess.size() < 5) {
        gameBar->setHintText("Guesses must have 5 letters!");
        return false;
    }

    if (!Dict::isExist(guess)) {
        gameBar->setHintText("\"" + guess + "\" does not exist!");
        return false;
    }

    if (Attr::hard && Attr::row > 0) {
        return isHardConditionsMet(guess);
    }

    return true;
}

bool Board::isHardConditionsMet(const QString &guess) {
    QList<Color> prevColors = Attr::squareColors[Attr::row - 1];
    QString prevGuess = Attr::guesses[Attr::row - 1];

    for (int i = 0; i < 5; i++) {
        if (prevColors[i] == Color::GRAY) {
            continue;
        }

        int count = std::min(prevGuess.count(prevGuess[i]),
                             Attr::answer.count(prevGuess[i]));

        if (guess.count(prevGuess[i]) < count) {
            gameBar->setHintText("Missing \"" + QString(prevGuess[i]) + "\"!");
            return false;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (prevColors[i] == Color::GREEN && guess[i] != prevGuess[i]) {
            QString text = "Letter #%0 must be %1!";
            gameBar->setHintText(text.arg(i + 1).arg(prevGuess[i]));
            return false;
        }
    }

    return true;
}

void Board::markGuess(int row, const QString &guess) {
    QString guessCopy(guess);
    QString answerCopy(Attr::answer);

    for (int i = 0; i < 5; i++) {
        if (guessCopy[i] == Attr::answer[i]) {
            Attr::squareColors[row].append(Color::GREEN);
            answerCopy[i] = ' ';
            guessCopy[i] = ' ';
        } else if (!Attr::answer.contains(guessCopy[i])) {
            Attr::squareColors[row].append(Color::GRAY);
            guessCopy[i] = ' ';
        }
    }

    for (int i = 0; i < 5; i++) {
        if (guessCopy[i] == ' ') {
            continue;
        } else if (answerCopy.contains(guessCopy[i])) {
            Attr::squareColors[row].insert(i, Color::YELLOW);
            answerCopy[answerCopy.indexOf(guessCopy[i])] = ' ';
        } else {
            Attr::squareColors[row].insert(i, Color::GRAY);
        }
    }

    for (int i = 0; i < 5; i++) {
        Color color = Attr::squareColors[row][i];
        if (game->getKeyboard()->verifyKey(guess[i], color)) {
            Attr::keyColors[guess[i]] = color;
            game->getKeyboard()->mark(guess[i], color);
        }

        QTimer::singleShot(i * 150, this, [this, row, i, color] {
            mark(row, i, color, Attr::animated);
        });
    }
}

void Board::moveRowDown() {
    Attr::row++;
    Attr::column = 0;

    gameBar->setHintIcon(getIcon("Bulb.svg"));
    gameBar->refreshChancesLeft();
    gameBar->setGiveUpButtonVisible(true);
}

void Board::makeEnded(const QIcon &icon, const QString &text) {
    gameBar->setHintIcon(icon);
    gameBar->setHintText(text);
    gameBar->setRestartButtonVisible(true);
    gameBar->setGiveUpButtonVisible(false);
    gameBar->setHintVisible(Attr::hintVisible);

    setEnabled(false);
    game->getKeyboard()->setEnabled(false);

    Attr::ended = true;
    Attr::numPlayed++;
}

void Board::makeWon() {
    makeEnded(getIcon("Confetti.svg"), "You guessed the word correctly!");

    if (Attr::bestTry == 0 || Attr::row + 1 < Attr::bestTry) {
        Attr::bestTry = Attr::row + 1;
    }
    Attr::numWon++;
    if (Attr::numPlayed != 0) {
        Attr::rateWon = round((double) Attr::numWon / Attr::numPlayed * 100.0);
    }
    Attr::streak++;
    Attr::maxStreak = std::max(Attr::streak, Attr::maxStreak);
}

void Board::makeLost() {
    makeEnded(getIcon("Sad.svg"), "The word is \"" + Attr::answer + "\".");
    
    Attr::streak = 0;
    if (Attr::numPlayed != 0) {
        Attr::rateWon = round((double) Attr::numWon / Attr::numPlayed * 100.0);
    }
}
