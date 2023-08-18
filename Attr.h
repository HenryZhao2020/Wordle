#ifndef ATTR_H
#define ATTR_H

#include "PCH.h"
#include "Color.h"

class Attr {
public:
    static int row;
    static int column;

    static QString answer;
    static QList<QString> guesses;
    static QList<QList<Color>> squareColors;
    static QHash<QString, Color> keyColors;
    static bool ended;

    static int numPlayed;
    static int numWon;
    static int rateWon;
    static int bestTry;
    static int streak;
    static int maxStreak;

    static bool animated;
    static bool hintVisible;
    static bool hard;
    static bool colorBlind;

    static void save();
    static bool load();
};

#endif
