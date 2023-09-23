#include "Keyboard.h"
#include "Board.h"
#include "Attr.h"

Keyboard::Keyboard(Board *board) : QFrame(board) {
    this->board = board;

    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(5);
    vboxLayout->setContentsMargins(0, 0, 0, 0);

    addKeys({"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"}, newRow());
    addKeys({"A", "S", "D", "F", "G", "H", "J", "K", "L"}, newRow());

    QHBoxLayout *bottomRow = newRow();

    auto backspaceKey = new QPushButton("Back", this);
    backspaceKey->setObjectName("keyWide");
    connect(backspaceKey, &QPushButton::clicked, this, [board] {
        board->removeLast();
    });
    bottomRow->addWidget(backspaceKey);

    addKeys({"Z", "X", "C", "V", "B", "N", "M"}, bottomRow);

    auto enterKey = new QPushButton("Enter", this);
    enterKey->setObjectName("keyWide");
    connect(enterKey, &QPushButton::clicked, this, [board] {
        board->checkGuess();
    });
    bottomRow->addWidget(enterKey);
}

bool Keyboard::verifyKey(const QString &letter, Color color) {
    if (!Attr::keyColors.contains(letter)) {
        return true;
    }

    Color oldColor = Attr::keyColors[letter];
    return !(oldColor == Color::GREEN ||
            (oldColor == Color::YELLOW && color != Color::GREEN) ||
            (oldColor == Color::GRAY && color == Color::GRAY));
}

void Keyboard::mark(const QString &letter, Color color) {
    QString style = "color: white; background: rgb(%0)";
    keys[letter]->setStyleSheet(style.arg(rgb(color)));
}

void Keyboard::remarkAll() {
    for (auto it = Attr::keyColors.begin(); it != Attr::keyColors.end(); it++) {
        mark(it.key(), it.value());
    }
}

QHBoxLayout *Keyboard::newRow() {
    auto rowFrame = new QFrame(this);
    vboxLayout->addWidget(rowFrame);

    auto hboxLayout = new QHBoxLayout(rowFrame);
    hboxLayout->setSpacing(5);
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    return hboxLayout;
}

QPushButton *Keyboard::newKey(const QString &letter) {
    auto key = new QPushButton(letter, this);
    connect(key, &QPushButton::clicked, this, [this, letter] {
        board->append(letter);
    });
    keys[letter] = key;
    return key;
}

void Keyboard::addKeys(const QList<QString> &letters, QHBoxLayout *layout) {
    for (auto &letter : letters) {
        layout->addWidget(newKey(letter));
    }
}
