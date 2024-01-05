#include "Board.h"
#include "Game.h"
#include "GameBar.h"
#include "Square.h"
#include "Keyboard.h"
#include "Attr.h"
#include "Color.h"
#include "Word.h"

Board::Board(Game *game)
    : QFrame(game), game(game), gameBar(game->getGameBar()) {
    // Set up the layout
    auto gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Place squares in the grid
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 5; ++c) {
            squares[r][c] = new Square(this);
            gridLayout->addWidget(squares[r][c], r, c);
        }
    }
}

Board::~Board() {

}

void Board::append(const QChar &letter) {
    // If the board is not interactive, or if the current row is full,
    // exit the function
    if (!isInteractive() || Attr::get().column >= 5) {
        return;
    }

    // Place the letter on the board
    placeLetter(Attr::get().row, Attr::get().column++,
                letter, Attr::get().animated);

    // Append the letter to the list
    Attr::get().letters[Attr::get().row].append(letter);

    // Hide hint from the game bar
    gameBar->setHintVisible(false);
}

void Board::removeLast() {
    // If the board is not interactive, or if the current row is empty,
    // exit the function
    if (!isInteractive() || Attr::get().column <= 0) {
        return;
    }

    // Remove the letter from the board
    placeLetter(Attr::get().row, --Attr::get().column, ' ', false);

    // Remove the last letter from the list
    Attr::get().letters[Attr::get().row].removeLast();

    // Hide hint from the game bar
    gameBar->setHintVisible(false);
}

void Board::checkGuess() {
    // If the board is not interactive, exit the function
    if (!isInteractive()) {
        return;
    }

    // Guess on the current row
    const QString &guess = Attr::get().letters[Attr::get().row];

    // If the guess is invalid, display warning and exit the function
    if (!isGuessValid(guess)) {
        gameBar->setHintIcon(Icon::load("Warning.svg"));
        return;
    }

    // Hide hint from the game bar
    gameBar->setHintVisible(false);
    // Disable mouse and keyboard interaction with the board
    setInteractive(false);
    // Mark squares on the current row and keys on the keyboard
    markGuess(guess);

    // Shift the current row down
    Attr::get().row++;
    Attr::get().column = 0;

    // If the guess matches the answer, player wins
    if (guess == Attr::get().answer) {
        win();
    // If there is no chance left, player loses
    } else if (Attr::get().row == 6) {
        lose();
    // Otherwise, the round continues
    } else {
        QTimer::singleShot(1000, this, [this] {
            gameBar->setHintIcon(Icon::load("Hint.svg"));
            gameBar->setGiveUpVisible(true);
            gameBar->displayChances();
            setInteractive(true);
        });
    }
}

void Board::placeLetter(int row, int column,
                        const QChar &letter, bool animated) {
    squares[row][column]->setText(letter);

    // Apply the animation if enabled
    if (animated) {
        squares[row][column]->zoomIn();
    }
}

void Board::markSquare(int row, int column,
                       const Color &color, bool animated) {
    if (animated) {
        squares[row][column]->fadeIn(color);
    } else {
        squares[row][column]->setStyleSheet(rgba(color));
    }
}

void Board::updateColors() {
    for (int r = 0; r < Attr::get().row; ++r) {
        for (int c = 0; c < 5; ++c) {
            markSquare(r, c, Attr::get().colors[r][c], false);
        }
    }
}

void Board::setInteractive(bool interactive) {
    this->interactive = interactive;
}

bool Board::isInteractive() {
    return interactive;
}

bool Board::isGuessValid(const QString &guess) {
    // Check whether the guess contains 5 letters
    if (guess.size() != 5) {
        gameBar->setHintText("Your guess must contain 5 letters!");
    // Check whether the guess is an existing word
    } else if (!Word::isValid(guess)) {
        gameBar->setHintText(QString("\"%1\" does not exist!").arg(guess));
    // Check whether the guess is valid in hard mode
    } else if (Attr::get().hard && Attr::get().row > 0) {
        return isHardValid(guess);
    // If all checks have passed, the guess is valid
    } else {
        return true;
    }
    // Otherwise, the guess is invalid
    return false;
}

bool Board::isHardValid(const QString &guess) {
    // Previous row
    int prevRow = Attr::get().row - 1;
    // Colors of the previous row
    const QList<Color> &prevColors = Attr::get().colors[prevRow];
    // Letters of the previous row
    const QString &prevLetters = Attr::get().letters[prevRow];

    // Check whether letters in yellow or green are in the current guess
    for (int i = 0; i < 5; ++i) {
        const QChar &letter = prevLetters[i];
        if (prevColors[i] != Color::GRAY && !guess.contains(letter)) {
            gameBar->setHintText(QString("Missing '%0'!").arg(letter));
            return false;
        }
    }

    // Check whether letters in green are in the correct positions
    for (int i = 0; i < 5; ++i) {
        const QChar &letter = prevLetters[i];
        if (prevColors[i] == Color::GREEN && guess[i] != letter) {
            gameBar->setHintText(QString("Letter #%0 must be '%1'!")
                                     .arg(i + 1).arg(letter));
            return false;
        }
    }

    // If all tests have passed, the guess is valid in hard mode
    return true;
}

void Board::markGuess(const QString &guess) {
    // Current row
    int row = Attr::get().row;
    // Temporary copy allowing modification of the original value
    QString guessCopy(guess);
    // Store the colors of the letters on the current row
    QList<Color> &colors = Attr::get().colors[row];
    // Store the number of occurrences of each letter in the answer
    QHash<QChar, int> counts;

    // Count the number of occurrences of each letter in the answer
    for (const auto &letter : Attr::get().answer) {
        counts[letter]++;
    }

    // Mark all letters that are in the correct positions in green
    // Mark all letters that are not part of the answer in gray
    for (int i = 0; i < 5; ++i) {
        const QChar &letter = guessCopy[i];
        if (letter == Attr::get().answer[i]) {
            colors.append(Color::GREEN);
            counts[letter]--;
            // Replace with an empty space to skip additional checking
            guessCopy[i] = ' ';
        } else if (!Attr::get().answer.contains(letter)) {
            colors.append(Color::GRAY);
            // Replace with an empty space to skip additional checking
            guessCopy[i] = ' ';
        }
    }

    // Mark all letters that are in the wrong positions in yellow
    // If the guess contains a certain letter more times than
    // it appears in the answer, mark the excess occurrences in gray
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
        // Apply colors to the squares on the board
        QTimer::singleShot(i * 150, this, [this, row, i, colors] {
            markSquare(row, i, colors[i], Attr::get().animated);
        });

        // Apply colors to the keyboard keys
        game->getKeyboard()->markKey(guess[i], colors[i]);
    }
}

void Board::end(const QIcon &icon, const QString &text) {
    // Update the game bar
    gameBar->setHintIcon(icon);
    gameBar->setHintText(text);
    gameBar->setRestartVisible(true);
    gameBar->setGiveUpVisible(false);

    // Disable the board and the keyboard
    setEnabled(false);
    setInteractive(false);
    game->getKeyboard()->setEnabled(false);

    // Update the statistics
    Attr::get().ended = true;
    Attr::get().totalPlays++;
}

void Board::win() {
    end(Icon::load("Win.svg"), "You guessed the word correctly!");

    // Update the statistics
    Attr::get().totalWins++;
    Attr::get().streak++;
    Attr::get().updateMinTries();
    Attr::get().updateMaxStreak();
    Attr::get().updateTriesDist();
}

void Board::lose() {
    end(Icon::load("Lose.svg"),
        QString("The word is \"%0\".").arg(Attr::get().answer));

    // Update the statistics
    Attr::get().streak = 0;
}
