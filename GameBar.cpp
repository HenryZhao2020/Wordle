#include "GameBar.h"
#include "Game.h"
#include "Board.h"
#include "Dialog.h"
#include "Attr.h"

GameBar::GameBar(Game *game) : QFrame(game) {
    hboxLayout = new QHBoxLayout(this);
    hboxLayout->setSpacing(5);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    pixmapLabel = new QLabel(this);
    hboxLayout->addWidget(pixmapLabel);

    hintLabel = new QLabel(this);
    hboxLayout->addWidget(hintLabel);
    hboxLayout->addStretch();

    restartButton = newButton(Pixmap::get("Restart.png"), "Restart");
    connect(restartButton, &QPushButton::clicked, this, [game] {
        game->restart();
    });

    giveUpButton = newButton(Pixmap::get("GiveUp.png"), "Give Up");
    connect(giveUpButton, &QPushButton::clicked, this, [game] {
        game->getBoard()->makeLost();
    });

    auto statsButton = newButton(Pixmap::get("Stats.png"), "Statistics");
    connect(statsButton, &QPushButton::clicked, this, [game] {
        auto statsDialog = new StatsDialog(game);
        statsDialog->show();
    });

    auto settingsButton = newButton(Pixmap::get("Settings.png"), "Settings");
    connect(settingsButton, &QPushButton::clicked, this, [game] {
        auto settingsDialog = new SettingsDialog(game);
        settingsDialog->show();
    });

    auto helpButton = newButton(Pixmap::get("Help.png"), "Help");
    connect(helpButton, &QPushButton::clicked, this, [game] {
        auto helpDialog = new HelpDialog(game);
        helpDialog->show();
    });
}

void GameBar::setHintPixmap(const QPixmap &pixmap) {
    pixmapLabel->setPixmap(pixmap);
}

void GameBar::setHintText(const QString &text, int size) {
    if (!Attr::animated || size == text.size()) {
        hintLabel->setText(text);
        return;
    }

    hintLabel->setText(text.left(size));
    QTimer::singleShot(10, this, [this, text, size] {
        setHintText(text, size + 1);
    });
}

void GameBar::setHintVisible(bool visible) {
    pixmapLabel->setVisible(visible);
    hintLabel->setVisible(visible);
}

void GameBar::setRestartButtonVisible(bool visible) {
    restartButton->setVisible(visible);
}

void GameBar::setGiveUpButtonVisible(bool visible) {
    giveUpButton->setVisible(visible);
}

void GameBar::refreshChancesLeft() {
    int chance = 6 - Attr::row;
    if (chance == 1) {
        setHintText("ONLY 1 CHANCE LEFT!!!");
    } else {
        setHintText("You have " + QString::number(chance) + " chances left.");
    }
}

QPushButton *GameBar::newButton(const QPixmap &pixmap, const QString &tip) {
    auto button = new QPushButton(this);
    button->setIcon(pixmap);
    button->setIconSize(pixmap.size());
    button->setToolTip(tip);
    button->setCursor(Qt::PointingHandCursor);
    hboxLayout->addWidget(button);
    return button;
}
