#include "GameBar.h"
#include "Game.h"
#include "Board.h"
#include "Dialog.h"
#include "Attr.h"

GameBar::GameBar(Game *game) : QFrame(game) {
    timer = nullptr;
    textLength = 0;

    hboxLayout = new QHBoxLayout(this);
    hboxLayout->setSpacing(4);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    iconButton = new QPushButton(this);
    iconButton->setIconSize(QSize(24, 24));
    hboxLayout->addWidget(iconButton);

    hintLabel = new QLabel(this);
    hboxLayout->addWidget(hintLabel);
    hboxLayout->addStretch();

    restartButton = newButton(getIcon("Restart.svg"), "Restart");
    connect(restartButton, &QPushButton::clicked, this, [game] {
        game->restart();
    });

    giveUpButton = newButton(getIcon("GiveUp.svg"), "Give Up");
    connect(giveUpButton, &QPushButton::clicked, this, [game] {
        game->getBoard()->makeLost();
    });

    auto settingsButton = newButton(getIcon("Settings.svg"), "Settings");
    connect(settingsButton, &QPushButton::clicked, this, [game] {
        auto settingsDialog = new SettingsDialog(game);
        settingsDialog->show();
    });
    
    auto statsButton = newButton(getIcon("Stats.svg"), "Statistics");
    connect(statsButton, &QPushButton::clicked, this, [game] {
        auto statsDialog = new StatsDialog(game);
        statsDialog->show();
    });

    auto helpButton = newButton(getIcon("Help.svg"), "Help");
    connect(helpButton, &QPushButton::clicked, this, [game] {
        auto helpDialog = new HelpDialog(game);
        helpDialog->show();
    });
}

void GameBar::setHintIcon(const QIcon &icon) {
    iconButton->setIcon(icon);
}

void GameBar::setHintText(const QString &text) {
    if (!Attr::animated) {
        hintLabel->setText(text);
        return;
    }

    textLength = 0;
    if (timer != nullptr) {
        timer->stop();    
    }    

    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this, [this, text] {
        if (textLength > text.size()) {
            timer->stop();
            return;
        }

        hintLabel->setText(text.left(++textLength));
    });
    timer->start();
}

void GameBar::setHintVisible(bool visible) {
    iconButton->setVisible(visible);
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

QPushButton *GameBar::newButton(const QIcon &icon, const QString &tip) {
    auto button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(QSize(24, 24));
    button->setToolTip(tip);
    button->setCursor(Qt::PointingHandCursor);
    hboxLayout->addWidget(button);
    return button;
}
