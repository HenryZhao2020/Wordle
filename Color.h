#pragma once

#include <QString>

enum class Color {
    GRAY,
    YELLOW,
    GREEN,
};

class RGBA {
public:
    static QString getStyleSheet(Color color, double alpha = 1);
};
