#include "GameBar.h"
#include "Game.h"
#include "Board.h"
#include "Dialog.h"
#include "Attr.h"

GameBar::GameBar(Game *game) : QFrame(game) {
    // Set up the layout
    barLayout = new QHBoxLayout(this);
    barLayout->setSpacing(5);
    barLayout->setContentsMargins(0, 0, 0, 0);

    // Display the icon on the left
    iconButton = new QPushButton(this);
    iconButton->setToolTip("Hint");
    barLayout->addWidget(iconButton);

    // Display the hint text beside the icon
    hintLabel = new QLabel(this);
    barLayout->addWidget(hintLabel);
    barLayout->addStretch();

    // Display a set of control buttons on the right
    restartButton = newButton(Icon::load("Restart.svg"), tr("Restart"), [game] {
        game->restart();
    });

    giveUpButton = newButton(Icon::load("GiveUp.svg"), tr("Give Up"), [game] {
        game->getBoard()->lose();
    });

    newButton(Icon::load("Settings.svg"), tr("Settings"), [game] {
        auto dialog = new SettingsDialog(game);
        dialog->show();
    });

    newButton(Icon::load("Stats.svg"), tr("Statistics"), [game] {
        auto dialog = new StatsDialog(game);
        dialog->show();
    });

    newButton(Icon::load("Help.svg"), tr("Help"), [game] {
        auto dialog = new HelpDialog(game);
        dialog->show();
    });
}

GameBar::~GameBar() {
    // Ensure all pointers are deleted
    if (hintTimer != nullptr) {
        hintTimer->deleteLater();
    }
}

void GameBar::setHintIcon(const QIcon &icon) {
    iconButton->setIcon(icon);

    // Restore the visibility of the hint
    setHintVisible(Attr::get().hinted);
}

void GameBar::setHintText(const QString &text) {
    // Apply animation if enabled
    if (Attr::get().animated) {
        typewriteHint(text);
    } else {
        hintLabel->setText(text);
    }

    // Restore the visibility of the hint
    setHintVisible(Attr::get().hinted);
}

void GameBar::setHintVisible(bool visible) {
    iconButton->setVisible(visible);
    hintLabel->setVisible(visible);
}

void GameBar::setRestartVisible(bool visible) {
    restartButton->setVisible(visible);
}

void GameBar::setGiveUpVisible(bool visible) {
    giveUpButton->setVisible(visible);
}

void GameBar::typewriteHint(const QString &text) {
    // Stop the animation if it is currently running
    if (hintTimer != nullptr) {
        hintTimer->deleteLater();
    }

    // Reset the displayed length of the hint text to zero
    hintLength = 0;
    hintLabel->setText("");

    // Set up the animation
    hintTimer = new QTimer(this);
    connect(hintTimer, &QTimer::timeout, this, [this, text] {
        // Stop the timer after revealing the entire hint text
        if (hintLength >= text.size()) {
            hintTimer->deleteLater();
            hintTimer = nullptr;
            return;
        }

        // Revealing a subsequent character after each interval
        hintLabel->setText(text.first(++hintLength));
    });

    // Start the animation
    hintTimer->start(15);
}

void GameBar::displayChances() {
    int chance = 6 - Attr::get().row;

    setHintIcon((chance == 1) ? Icon::load("Warning.svg")
                              : Icon::load("Hint.svg"));

    setHintText((chance == 1) ? tr("THIS IS YOUR LAST GUESS!")
                              : tr("%0 guesses remaining.").arg(chance));
}

template <typename Callable>
QPushButton *GameBar::newButton(const QIcon &icon, const QString &tip,
                                const Callable &callable) {
    auto button = new QPushButton(this);
    button->setIcon(icon);
    button->setToolTip(tip);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, callable);
    barLayout->addWidget(button);

    return button;
}
