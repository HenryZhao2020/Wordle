#pragma once

#include <QPushButton>
#include <QTimer>

class Board;

enum class Color;

class Square : public QPushButton {
    Q_OBJECT

public:
    Square(Board *board);
    ~Square();

    void zoomIn();
    void fadeIn(Color color);

private:
    QTimer *zoomTimer = nullptr;
    int fontSize;

    QTimer *fadeTimer = nullptr;
    double alpha;
};
