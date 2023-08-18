#ifndef GAMEBAR_H
#define GAMEBAR_H

#include "PCH.h"

class Game;
class Dialog;

class GameBar : public QFrame {
    Q_OBJECT

public:
    GameBar(Game *game);

    void setHintPixmap(const QPixmap &pixmap);
    void setHintText(const QString &text, int size = 0);
    void setHintVisible(bool visible);
    void setRestartButtonVisible(bool visible);
    void setGiveUpButtonVisible(bool visible);
    void refreshChancesLeft();

private:
    QHBoxLayout *hboxLayout;
    QLabel *pixmapLabel;
    QLabel *hintLabel;
    QPushButton *restartButton;
    QPushButton *giveUpButton;

    QPushButton *newButton(const QPixmap &pixmap, const QString &tip);
};

#endif
