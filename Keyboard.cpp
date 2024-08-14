#include "Keyboard.h"
#include "Game.h"
#include "Board.h"
#include "Attr.h"
#include "Color.h"

Keyboard::Keyboard(Game *game) : QFrame(game), game(game) {
    keyLayout = new QVBoxLayout(this);
    keyLayout->setSpacing(5);
    keyLayout->setContentsMargins(0, 0, 0, 0);

    newRow({'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'});
    newRow({'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'});

    QHBoxLayout *bottomRow = newRow({'Z', 'X', 'C', 'V', 'B', 'N', 'M'});

    auto backspaceKey = new QPushButton(tr("Back"), this);
    backspaceKey->setFixedWidth(70);
    connect(backspaceKey, &QPushButton::clicked, this, [game] {
        game->getBoard()->removeLast();
    });
    bottomRow->insertWidget(0, backspaceKey);

    auto enterKey = new QPushButton(tr("Enter"), this);
    enterKey->setFixedWidth(70);
    connect(enterKey, &QPushButton::clicked, this, [game] {
        game->getBoard()->checkGuess();
    });
    bottomRow->addWidget(enterKey);
}

Keyboard::~Keyboard() {}

void Keyboard::markKey(const QChar &letter, Color color) {
    if (Attr::get().keyColors.contains(letter) &&
        color <= Attr::get().keyColors[letter]) {
        return;
    }

    Attr::get().keyColors[letter] = color;
    keys[letter]->setStyleSheet(RGBA::getStyleSheet(color));
}

void Keyboard::clearColors() {
    for (const auto &key : keys.values()) {
        key->setStyleSheet("");
    }
}

void Keyboard::updateColors() {
    QHash<QChar, Color> colors = Attr::get().keyColors;
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        keys[it.key()]->setStyleSheet(RGBA::getStyleSheet(it.value()));
    }
}

QHBoxLayout *Keyboard::newRow(const QList<QChar> &letters) {
    auto rowFrame = new QFrame(this);
    keyLayout->addWidget(rowFrame);

    auto hboxLayout = new QHBoxLayout(rowFrame);
    hboxLayout->setSpacing(5);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    for (const auto &letter : letters) {
        hboxLayout->addWidget(newKey(letter));
    }

    return hboxLayout;
}

QPushButton *Keyboard::newKey(const QChar &letter) {
    auto key = new QPushButton(letter, this);
    connect(key, &QPushButton::clicked, this, [this, letter] {
        game->getBoard()->append(letter);
    });
    keys.insert(letter, key);

    return key;
}
