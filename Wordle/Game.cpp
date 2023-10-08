#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"
#include "Dict.h"

Game::Game() : QMainWindow(nullptr, Qt::MSWindowsFixedSizeDialogHint) {
    auto widget = new QWidget(this);
    setCentralWidget(widget);

    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setSpacing(45);
    vboxLayout->setContentsMargins(30, 30, 30, 30);

    bar = new GameBar(this);
    bar->setHintVisible(Attr::hintVisible);
    bar->setHintIcon(getIcon("Bulb.svg"));
    bar->setHintText("Type a letter to begin...");
    bar->setRestartButtonVisible(false);
    bar->setGiveUpButtonVisible(false);
    vboxLayout->addWidget(bar);

    board = new Board(this);
    vboxLayout->addWidget(board, false, Qt::AlignCenter);

    keyboard = new Keyboard(board);
    vboxLayout->addWidget(keyboard);
}

GameBar *Game::getGameBar() {
    return bar;
}

Board *Game::getBoard() {
    return board;
}

Keyboard *Game::getKeyboard() {
    return keyboard;
}

void Game::restart() {
    Attr::row = 0;
    Attr::column = 0;
    Attr::answer = Dict::generate();
    Attr::guesses = QList<QString>(6);
    Attr::squareColors = QList<QList<Color>>(6);
    Attr::keyColors.clear();
    Attr::ended = false;

    bar->setHintIcon(getIcon("Bulb.svg"));
    bar->setHintText("Type a letter to begin...");
    bar->setRestartButtonVisible(false);

    vboxLayout->removeWidget(board);
    vboxLayout->removeWidget(keyboard);
    board->deleteLater();
    keyboard->deleteLater();

    board = new Board(this);
    vboxLayout->addWidget(board, false, Qt::AlignCenter);

    keyboard = new Keyboard(board);
    vboxLayout->addWidget(keyboard);

    qDebug() << Attr::answer;
}

void Game::restore() {
    if (!Attr::load()) {
        Attr::answer = Dict::generate();
        qDebug() << Attr::answer;
        return;
    }

    if (Attr::ended) {
        restart();
        return;
    }

    bar->refreshChancesLeft();
    bar->setGiveUpButtonVisible(Attr::row > 0);    
    bar->setHintVisible(Attr::hintVisible);

    for (int r = 0; r < Attr::row; r++) {
        for (int c = 0; c < 5; c++) {
            board->setTextAt(r, c, Attr::guesses[r][c]);
        }
    }

    for (int i = 0; i < Attr::column; i++) {
        board->setTextAt(Attr::row, i, Attr::guesses[Attr::row][i]);
    }

    board->remarkAll();
    keyboard->remarkAll();

    qDebug() << Attr::answer;
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (!board->isEnabled()) {
        return;
    }

    if (event->key() == Qt::Key_Return) {
        board->checkGuess();
    } else if (event->key() == Qt::Key_Backspace) {
        board->removeLast();
    } else if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
        board->append(event->text().toUpper());
    }
}
