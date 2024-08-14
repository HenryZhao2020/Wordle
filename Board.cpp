#include "Board.h"
#include "Game.h"
#include "GameBar.h"
#include "Square.h"
#include "Keyboard.h"
#include "Attr.h"
#include "Color.h"
#include "Dict.h"
#include "IconUtil.h"

Board::Board(Game *game)
    : QFrame(game), game(game), gameBar(game->getGameBar()) {
    setFrozen(false);

    auto gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 5; ++c) {
            squares[r][c] = new Square(this);
            gridLayout->addWidget(squares[r][c], r, c);
        }
    }
}

Board::~Board() {}

void Board::append(const QChar &letter) {
    int row = Attr::get().row;
    int &col = Attr::get().col;

    if (isFrozen() || col >= 5) {
        return;
    }

    placeLetter(row, col++, letter, Attr::get().animated);
    Attr::get().letters[row].append(letter);

    gameBar->setInfoVisible(false);
}

void Board::removeLast() {
    int row = Attr::get().row;
    int &col = Attr::get().col;

    if (isFrozen() || col <= 0) {
        return;
    }

    placeLetter(row, --col, ' ', false);
    Attr::get().letters[row].removeLast();

    gameBar->setInfoVisible(false);
}

void Board::checkGuess() {
    if (isFrozen()) {
        return;
    }

    const QString &guess = Attr::get().letters[Attr::get().row];
    if (!isGuessValid(guess)) {
        gameBar->setInfoIcon(IconUtil::load(":/icons/Warning.svg"));
        return;
    }
    gameBar->setInfoVisible(false);

    setFrozen(true);
    markGuess(guess);

    ++Attr::get().row;
    Attr::get().col = 0;

    if (guess == Attr::get().answer) {
        win();
    } else if (Attr::get().row == 6) {
        lose();
    } else {
        QTimer::singleShot(1000, this, [this] {
            gameBar->setInfoIcon(IconUtil::load(":/icons/Bell.svg"));
            gameBar->setGiveUpEnabled(true);
            gameBar->displayChances();
            setFrozen(false);
        });
    }
}

void Board::placeLetter(int row, int col, const QChar &letter, bool animated) {
    squares[row][col]->setText(letter);

    if (animated) {
        squares[row][col]->zoomIn();
    }
}

void Board::markSquare(int row, int col, Color color, bool animated) {
    if (animated) {
        squares[row][col]->fadeIn(color);
    } else {
        squares[row][col]->setStyleSheet(RGBA::getStyleSheet(color));
    }
}

void Board::updateColors() {
    for (int r = 0; r < Attr::get().row; ++r) {
        for (int c = 0; c < 5; ++c) {
            markSquare(r, c, Attr::get().colors[r][c], false);
        }
    }
}

void Board::setFrozen(bool frozen) {
    this->frozen = frozen;
}

bool Board::isFrozen() {
    return frozen;
}

bool Board::isGuessValid(const QString &guess) {
    if (guess.size() != 5) {
        gameBar->setInfoText(tr("Please enter 5 letters!"));
    } else if (!Dict::isValid(guess)) {
        gameBar->setInfoText(tr("'%0' does not exist!").arg(guess));
    } else if (Attr::get().hard && Attr::get().row > 0) {
        return isHardValid(guess);
    } else {
        return true;
    }
    return false;
}

bool Board::isHardValid(const QString &guess) {
    int prevRow = Attr::get().row - 1;
    const QList<Color> &prevColors = Attr::get().colors[prevRow];
    const QString &prevLetters = Attr::get().letters[prevRow];

    for (int i = 0; i < 5; ++i) {
        const QChar &letter = prevLetters[i];
        if (prevColors[i] != Color::GRAY && !guess.contains(letter)) {
            gameBar->setInfoText(tr("Missing letter '%0'!").arg(letter));
            return false;
        }
    }

    for (int i = 0; i < 5; ++i) {
        const QChar &letter = prevLetters[i];
        if (prevColors[i] == Color::GREEN && guess[i] != letter) {
            gameBar->setInfoText(tr("Letter #%0 must be '%1'!")
                                     .arg(i + 1).arg(letter));
            return false;
        }
    }

    return true;
}

void Board::markGuess(const QString &guess) {
    int row = Attr::get().row;
    QString guessCopy(guess);
    QList<Color> &colors = Attr::get().colors[row];
    QHash<QChar, int> counts;

    for (const auto &letter : Attr::get().answer) {
        ++counts[letter];
    }

    for (int i = 0; i < 5; ++i) {
        const QChar &letter = guessCopy[i];
        if (letter == Attr::get().answer[i]) {
            colors.append(Color::GREEN);
            counts[letter]--;
            guessCopy[i] = ' ';
        } else if (!Attr::get().answer.contains(letter)) {
            colors.append(Color::GRAY);
            guessCopy[i] = ' ';
        }
    }

    for (int i = 0; i < 5; ++i) {
        const QChar &letter = guessCopy[i];
        if (letter == ' ') {
            continue;
        } else if (counts[letter] > 0) {
            colors.insert(i, Color::YELLOW);
            counts[letter]--;
        } else {
            colors.insert(i, Color::GRAY);
        }
    }

    for (int i = 0; i < 5; ++i) {
        QTimer::singleShot(i * 150, this, [this, row, i, colors] {
            markSquare(row, i, colors[i], Attr::get().animated);
        });

        game->getKeyboard()->markKey(guess[i], colors[i]);
    }
}

void Board::endRound(const QIcon &icon, const QString &text) {
    gameBar->setInfoIcon(icon);
    gameBar->setInfoText(text);
    gameBar->setRestartEnabled(true);
    gameBar->setGiveUpEnabled(false);

    setEnabled(false);
    setFrozen(false);

    game->getKeyboard()->setEnabled(false);

    Attr::get().ended = true;
    ++Attr::get().totalPlays;
}

void Board::win() {
    endRound(IconUtil::load(":/icons/Win.svg"), tr("You guessed the answer correctly!"));

    ++Attr::get().totalWins;
    ++Attr::get().streak;
    Attr::get().updateMinTries();
    Attr::get().updateMaxStreak();
    Attr::get().updateTriesDist();
}

void Board::lose() {
    endRound(IconUtil::load(":/icons/Lose.svg"),
             tr("The answer is '%0'.").arg(Attr::get().answer));

    Attr::get().streak = 0;
}
