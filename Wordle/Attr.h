#pragma once

#include "PCH.h"
#include "Color.h"

namespace Attr {
    extern int row;
    extern int column;

    extern QString answer;
    extern QList<QString> guesses;
    extern QList<QList<Color>> squareColors;
    extern QHash<QString, Color> keyColors;
    extern bool ended;

    extern int numPlayed;
    extern int numWon;
    extern int rateWon;
    extern int bestTry;
    extern int streak;
    extern int maxStreak;

    extern bool animated;
    extern bool hintVisible;
    extern bool hard;
    extern bool colorBlind;

    void save();
    bool load();
}
