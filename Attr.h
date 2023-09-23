#pragma once

#include "PCH.h"
#include "Color.h"

namespace Attr {
    inline int row;
    inline int column;

    inline QString answer;
    inline QList<QString> guesses(6);
    inline QList<QList<Color>> squareColors(6);
    inline QHash<QString, Color> keyColors;
    inline bool ended;

    inline int numPlayed;
    inline int numWon;
    inline int rateWon;
    inline int bestTry;
    inline int streak;
    inline int maxStreak;

    inline bool animated = true;
    inline bool hintVisible = true;
    inline bool hard;
    inline bool colorBlind;

    void save();
    bool load();
}
