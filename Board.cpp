#include "Board.h"
#include "Game.h"
#include "GameBar.h"
#include "Square.h"
#include "Attr.h"

QList<QList<int>> SEQUENCES = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6},
};

Board::Board(Game *game) : QFrame(game) {
    this->game = game;
    gameBar = game->getGameBar();
    notPlaced = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    xsTurn = true;

    auto gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < 9; i++) {
        squares[i] = new Square(this, i);
        gridLayout->addWidget(squares[i], i / 3, i % 3);
    }
}

void Board::place(int i, const QPixmap &pixmap, bool animated) {
    squares[i]->setIcon(pixmap);
    disconnect(squares[i], &QPushButton::clicked, nullptr, nullptr);

    if (animated) {
        squares[i]->zoom();
    } else {
        squares[i]->setIconSize(QSize(64, 64));
    }

    notPlaced.removeOne(i);
}

void Board::placeX(int i) {
    if (!xsTurn) {
        return;
    }

    place(i, Pixmap::get("X.png"), Attr::animated);
    Attr::xPlaced.append(i);
    xsTurn = false;
    gameBar->setHintVisible(false);

    if (isEnded()) {
        return;
    }

    QTimer::singleShot(500, this, [this] {
        gameBar->setHintVisible(Attr::hintVisible);
        gameBar->setHintText("Computer's Turn");
    });

    QTimer::singleShot(1000, this, &Board::placeO);
}

void Board::placeO() {
    int i = nextO();
    place(i, Pixmap::get("O.png"), Attr::animated);
    Attr::oPlaced.append(i);

    if (isEnded()) {
        return;
    }

    QTimer::singleShot(500, this, [this] {
        xsTurn = true;
        gameBar->setHintText("Your Turn");
    });
}

void Board::setXsTurn(bool turn) {
    xsTurn = turn;
}

int Board::lastMissingIndex(const QList<int> &placed) {
    auto isMissingOnly = [this, placed] (int a, int b, int c) {
        return placed.contains(a) && placed.contains(b) &&
               notPlaced.contains(c);
    };

    for (auto &seq : SEQUENCES) {
        if (isMissingOnly(seq[0], seq[1], seq[2])) {
            return seq[2];
        } else if (isMissingOnly(seq[0], seq[2], seq[1])) {
            return seq[1];
        } else if (isMissingOnly(seq[1], seq[2], seq[0])) {
            return seq[0];
        }
    }

    return -1;
}

int Board::nextO() {
    int index = lastMissingIndex(Attr::oPlaced);
    if (index == -1) {
        index = lastMissingIndex(Attr::xPlaced);
    }
    if (index == -1) {
        index = notPlaced[QRandomGenerator::global()->bounded(notPlaced.size())];
    }
    return index;
}

bool Board::isEnded() {
    for (auto &seq : SEQUENCES) {
        auto isThreeInRow = [seq] (const QList<int> &placed) {
            for (auto &i : seq) {
                if (!placed.contains(i)) {
                    return false;
                }
            }
            return true;
        };

        if (isThreeInRow(Attr::xPlaced)) {
            makeWinnerX(seq);
            return true;
        } else if (isThreeInRow(Attr::oPlaced)) {
            makeWinnerO(seq);
            return true;
        }
    }

    if (notPlaced.isEmpty()) {
        makeTie();
        return true;
    }

    return false;
}

void Board::makeEnded(const QPixmap &pixmap, const QString &text) {
    for (auto &i : Attr::xPlaced) {
        squares[i]->setIcon(Pixmap::get("X_Alpha.png"));
    }

    for (auto &i : Attr::oPlaced) {
        squares[i]->setIcon(Pixmap::get("O_Alpha.png"));
    }

    for (auto &square : squares) {
        connect(square, &Square::clicked, this, [this] {
            game->restart();
        });
    }

    gameBar->setHintVisible(Attr::hintVisible);
    gameBar->setHintPixmap(pixmap);
    gameBar->setHintText(text);
    gameBar->setRestartButtonVisible(true);

    QTimer::singleShot(2000, this, [this] {
        gameBar->setHintText("Click any square to restart...");
    });

    Attr::ended = true;
    Attr::numPlayed++;
}

void Board::makeWinnerX(const QList<int> &seq) {
    makeEnded(Pixmap::get("Confetti.png"), "You Won!");
    Attr::xScore++;

    for (auto &i : seq) {
        squares[i]->setIcon(Pixmap::get("X.png"));
        if (Attr::animated) {
            squares[i]->flash();
        }
    }
}

void Board::makeWinnerO(const QList<int> &seq) {
    makeEnded(Pixmap::get("Computer.png"), "Computer Won!");
    Attr::oScore++;

    for (auto &i : seq) {
        squares[i]->setIcon(Pixmap::get("O.png"));
        if (Attr::animated) {
            squares[i]->flash();
        }
    }
}

void Board::makeTie() {
    makeEnded(Pixmap::get("Handshake.png"), "Tie!");
    Attr::numTied++;
}
