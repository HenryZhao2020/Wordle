#pragma once

#include "PCH.h"
#include "Color.h"

class Square : public QPushButton {
    Q_OBJECT

public:
    using QPushButton::QPushButton;

    void mark(Color color, double alpha = 1);
    void fade(Color color);
    void zoom();

private:
    double alpha;
    int fontSize;
};
