#include "Keyboard.h"
#include "Game.h"
#include "Board.h"
#include "Attr.h"
#include "Color.h"

Keyboard::Keyboard(Game *game) : QFrame(game), game(game) {
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(5);
    vboxLayout->setContentsMargins(0, 0, 0, 0);

    // First row of letters
    newRow({'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'});

    // Second row of letters
    newRow({'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'});

    // Bottom row of letters
    QHBoxLayout *bottomRow = newRow({'Z', 'X', 'C', 'V', 'B', 'N', 'M'});

    // Insert the <Backspace> key to the beginning of the bottom row
    auto backspaceKey = new QPushButton("Back", this);
    backspaceKey->setFixedWidth(75);
    // Remove the last letter from the board on click
    connect(backspaceKey, &QPushButton::clicked, this, [game] {
        game->getBoard()->removeLast();
    });
    bottomRow->insertWidget(0, backspaceKey);

    // Append the <Enter> key to the end of the bottom row
    auto enterKey = new QPushButton("Enter", this);
    enterKey->setFixedWidth(75);
    // Check the current guess on click
    connect(enterKey, &QPushButton::clicked, this, [game] {
        game->getBoard()->checkGuess();
    });
    bottomRow->addWidget(enterKey);
}

Keyboard::~Keyboard() {

}

void Keyboard::markKey(const QChar &letter, const Color &color) {
    /*
     * Rules for updating the color of a key:
     *   - Previously green (3): no need for update
     *   - Previously yellow (2): update to green (3)
     *   - Previously gray (1): update to yellow (2) or green (3)
     *
     * To determine whether to update the color of a key,
     * compare the enum value of its previous color and the new color.
     * If new color > previous color, then update the color.
     *
     * The above rules do not apply to transparent keys.
     */

    if (Attr::get().keyColors.contains(letter) &&
        color <= Attr::get().keyColors[letter]) {
        return;
    }

    Attr::get().keyColors[letter] = color;
    keys[letter]->setStyleSheet(rgba(color));
}

void Keyboard::clearColors() {
    for (const auto &key : keys.values()) {
        key->setStyleSheet("");
    }
}

void Keyboard::updateColors() {
    QHash<QChar, Color> colors = Attr::get().keyColors;
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        keys[it.key()]->setStyleSheet(rgba(it.value()));
    }
}

QHBoxLayout *Keyboard::newRow(const QList<QChar> &letters) {
    // Display a row of keys
    auto rowFrame = new QFrame(this);
    vboxLayout->addWidget(rowFrame);

    // Set up the layout
    auto hboxLayout = new QHBoxLayout(rowFrame);
    hboxLayout->setSpacing(5);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    // Create a key for each letter from the list
    for (const auto &letter : letters) {
        hboxLayout->addWidget(newKey(letter));
    }

    // Return the layout for any addition of keys
    return hboxLayout;
}

QPushButton *Keyboard::newKey(const QChar &letter) {
    auto key = new QPushButton(letter, this);
    // Append the corresponding letter to the board on click
    connect(key, &QPushButton::clicked, this, [this, letter] {
        game->getBoard()->append(letter);
    });
    keys.insert(letter, key);

    return key;
}
