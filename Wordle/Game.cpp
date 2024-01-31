#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"
#include "Word.h"

Game::Game() {
    // Create a central widget
    auto widget = new QWidget(this);
    setCentralWidget(widget);

    // Set up the layout
    mainLayout = new QVBoxLayout(widget);
    mainLayout->setSpacing(50);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Place the game bar on the top
    gameBar = new GameBar(this);
    gameBar->setHintIcon(Icon::load("Hint.svg"));
    gameBar->setHintText(tr("Enter a letter to begin..."));
    gameBar->setRestartVisible(false);
    gameBar->setGiveUpVisible(false);
    mainLayout->addWidget(gameBar);

    // Place the board in the center
    board = new Board(this);
    mainLayout->addWidget(board, 0, Qt::AlignCenter);

    // Place the keyboard on the bottom
    keyboard = new Keyboard(this);
    mainLayout->addWidget(keyboard);
}

Game::~Game() {

}

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
    // Reset the game progress
    Attr::get().resetProgress();
    qDebug() << Attr::get().answer;

    // Reset the game bar
    gameBar->setHintIcon(Icon::load("Hint.svg"));
    gameBar->setHintText(tr("Enter a letter to begin..."));
    gameBar->setRestartVisible(false);

    // Remove and delete the old board
    mainLayout->removeWidget(board);
    board->deleteLater();

    // Insert a new Board widget on top of the keyboard
    board = new Board(this);
    mainLayout->insertWidget(1, board, 0, Qt::AlignCenter);

    // Reset the keyboard
    keyboard->clearColors();
    keyboard->setEnabled(true);
}

void Game::load() {
    // If the round has ended previously, start a new round
    if (Attr::get().ended) {
        restart();
        return;
    }

    // Update the game bar
    gameBar->displayChances();
    gameBar->setGiveUpVisible(Attr::get().row > 0);

    // Place all previous letters on the board
    for (int r = 0; r < Attr::get().row; ++r) {
        for (int c = 0; c < 5; ++c) {
            board->placeLetter(r, c, Attr::get().letters[r][c], false);
        }
    }

    for (int c = 0; c < Attr::get().column; ++c) {
        board->placeLetter(Attr::get().row, c,
                           Attr::get().letters[Attr::get().row][c], false);
    }

    // Mark all previous squares on the board
    board->updateColors();
    // Mark all previous keys on the keyboard
    keyboard->updateColors();

    qDebug() << Attr::get().answer;
}

void Game::show() {
    // Make this window not resizable
    setFixedSize(sizeHint());
    QMainWindow::show();
}

void Game::keyPressEvent(QKeyEvent *event) {
    // If the board is not interactive, disable keyboard input
    if (!board->isInteractive()) {
        return;
    }

    // Press <Enter> (Windows) or <Return> (Mac) key to check guess
    if (event->key() == Qt::Key_Return) {
        board->checkGuess();
    // Press <Backspace> key to remove the last letter
    } else if (event->key() == Qt::Key_Backspace) {
        board->removeLast();
    // Press <A> to <Z> key to append a letter
    } else if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
        board->append(event->text().toUpper()[0]);
    }
}
