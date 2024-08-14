#pragma once

#include <QFrame>
#include <QBoxLayout>
#include <QPushButton>
#include <QHash>

class Game;

enum class Color;

class Keyboard : public QFrame {
    Q_OBJECT

public:
    Keyboard(Game *game);
    ~Keyboard();

    void markKey(const QChar &letter, Color color);
    void clearColors();
    void updateColors();

private:
    Game *game;
    QVBoxLayout *keyLayout;
    QHash<QChar, QPushButton *> keys;

    QHBoxLayout *newRow(const QList<QChar> &letters);
    QPushButton *newKey(const QChar &letter);
};
