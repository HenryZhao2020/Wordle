#include "GameBar.h"
#include "Game.h"
#include "Board.h"
#include "Dialog.h"
#include "Attr.h"

GameBar::GameBar(Game *game) : QFrame(game) {
    // Set up the layout
    hboxLayout = new QHBoxLayout(this);
    hboxLayout->setSpacing(5);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    // Display the icon on the left
    iconButton = new QPushButton(this);
    iconButton->setToolTip("Hint");
    hboxLayout->addWidget(iconButton);

    // Display the hint text beside the icon
    hintLabel = new QLabel(this);
    hboxLayout->addWidget(hintLabel);
    hboxLayout->addStretch();

    // Display a set of control buttons on the right
    restartButton = newButton(Icon::load("Restart.svg"), "Restart", [game] {
        game->restart();
    });

    giveUpButton = newButton(Icon::load("GiveUp.svg"), "Give Up", [game] {
        game->getBoard()->lose();
    });

    newButton(Icon::load("Settings.svg"), "Settings", [game] {
        auto dialog = new SettingsDialog(game);
        dialog->show();
    });

    newButton(Icon::load("Stats.svg"), "Statistics", [game] {
        auto dialog = new StatsDialog(game);
        dialog->show();
    });

    newButton(Icon::load("Help.svg"), "Help", [game] {
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
    // If the text does not change, exit the function
    if (hintLabel->text() == text) {
        return;
    }

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

    setHintText((chance == 1) ? "Only 1 chance left!"
                              : QString("%0 chances left.").arg(chance));
}

template <typename Callable>
QPushButton *GameBar::newButton(const QIcon &icon, const QString &tip,
                                const Callable &callable) {
    auto button = new QPushButton(this);
    button->setIcon(icon);
    button->setToolTip(tip);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, callable);
    hboxLayout->addWidget(button);

    return button;
}
