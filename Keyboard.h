#pragma once

#include "PCH.h"
#include "Color.h"

class Board;

class Keyboard : public QFrame {
    Q_OBJECT

public:
    Keyboard(Board *board);

    bool verifyKey(const QString &letter, Color color);
    void mark(const QString &letter, Color color);
    void remarkAll();

private:
    Board *board;
    QVBoxLayout *vboxLayout;
    QHash<QString, QPushButton *> keys;

    QHBoxLayout *newRow();
    QPushButton *newKey(const QString &letter);
    void addKeys(const QList<QString> &letters, QHBoxLayout *layout);
};
