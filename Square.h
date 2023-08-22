#ifndef SQUARE_H
#define SQUARE_H

#include "PCH.h"
#include "Color.h"

class Square : public QPushButton {
    Q_OBJECT

public:
    using QPushButton::QPushButton;

    void mark(Color color, double alpha = 1);
    void fade(Color color, double alpha = 0);
    void zoom(int fontSize = 1);
};

#endif
