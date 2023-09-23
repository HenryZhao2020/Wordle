#pragma once

#include "PCH.h"

class Game;
class Dialog;

class GameBar : public QFrame {
    Q_OBJECT

public:
    GameBar(Game *game);

    void setHintIcon(const QIcon &icon);
    void setHintText(const QString &text);
    void setHintVisible(bool visible);
    void setRestartButtonVisible(bool visible);
    void setGiveUpButtonVisible(bool visible);
    void refreshChancesLeft();

private:
    QTimer *timer;
    int textLength;

    QHBoxLayout *hboxLayout;
    QPushButton *iconButton;
    QLabel *hintLabel;
    QPushButton *restartButton;
    QPushButton *giveUpButton;

    QPushButton *newButton(const QIcon &icon, const QString &tip);
};
