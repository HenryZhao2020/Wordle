#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class Game;

class GameBar : public QFrame {
    Q_OBJECT

public:
    GameBar(Game *game);
    ~GameBar();

    void setInfoIcon(const QIcon &icon);
    void setInfoText(const QString &text);
    void setInfoVisible(bool visible);
    void setRestartEnabled(bool visible);
    void setGiveUpEnabled(bool visible);
    void displayChances();

private:
    QHBoxLayout *barLayout;
    QPushButton *iconButton;
    QLabel *infoLabel;
    QPushButton *restartButton;
    QPushButton *giveUpButton;

    QTimer *infoTimer = nullptr;
    int infoLength;
    void typewriteInfo(const QString &text);

    template <typename Callable>
    QPushButton *newMenuButton(const QIcon &icon, const QString &tip, const Callable &call);
};
