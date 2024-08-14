#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"
#include "IconUtil.h"

Game::Game() {
    auto widget = new QWidget(this);
    setCentralWidget(widget);

    mainLayout = new QVBoxLayout(widget);
    mainLayout->setSpacing(50);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    gameBar = new GameBar(this);
    gameBar->setInfoIcon(IconUtil::load(":/icons/Bell.svg"));
    gameBar->setInfoText(tr("Enter a letter to begin..."));
    gameBar->setRestartEnabled(false);
    gameBar->setGiveUpEnabled(false);
    mainLayout->addWidget(gameBar);

    board = new Board(this);
    mainLayout->addWidget(board, 1, Qt::AlignCenter);

    keyboard = new Keyboard(this);
    mainLayout->addWidget(keyboard, 0, Qt::AlignCenter);

    setFixedSize(sizeHint());
}

Game::~Game() {}

GameBar *Game::getGameBar() {
    return gameBar;
}

Board *Game::getBoard() {
    return board;
}

Keyboard *Game::getKeyboard() {
    return keyboard;
}

void Game::restart() {
    Attr::get().resetProgress();

    gameBar->setInfoIcon(IconUtil::load(":/icons/Bell.svg"));
    gameBar->setInfoText(tr("Enter a letter to begin..."));
    gameBar->setRestartEnabled(false);

    mainLayout->removeWidget(board);
    board->deleteLater();

    board = new Board(this);
    mainLayout->insertWidget(1, board, 1, Qt::AlignCenter);

    keyboard->clearColors();
    keyboard->setEnabled(true);
}

void Game::loadSave() {
    if (Attr::get().ended) {
        restart();
        return;
    }

    gameBar->displayChances();
    gameBar->setGiveUpEnabled(Attr::get().row > 0);

    for (int r = 0; r < Attr::get().row; ++r) {
        for (int c = 0; c < 5; ++c) {
            board->placeLetter(r, c, Attr::get().letters[r][c], false);
        }
    }

    for (int c = 0; c < Attr::get().col; ++c) {
        const QChar &letter = Attr::get().letters[Attr::get().row][c];
        board->placeLetter(Attr::get().row, c, letter, false);
    }

    board->updateColors();
    keyboard->updateColors();
}

void Game::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);

    if (board->isFrozen()) {
        return;
    }

    if (event->key() == Qt::Key_Return) {
        board->checkGuess();
    } else if (event->key() == Qt::Key_Backspace) {
        board->removeLast();
    } else if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
        board->append(event->text().toUpper()[0]);
    }
}
