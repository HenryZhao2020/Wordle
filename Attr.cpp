#include "Attr.h"

void Attr::save() {
    QFile file("Wordle_Data");
    file.open(QFile::WriteOnly);

    QDataStream out(&file);
    out << row;
    out << column;

    out << answer;
    out << guesses;
    out << squareColors;
    out << keyColors;
    out << ended;

    out << numPlayed;
    out << numWon;
    out << rateWon;
    out << bestTry;
    out << streak;
    out << maxStreak;

    out << animated;
    out << hintVisible;
    out << hard;
    out << colorBlind;

    file.close();
}

bool Attr::load() {
    QFile file("Wordle_Data");
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in >> row;
    in >> column;

    in >> answer;
    in >> guesses;
    in >> squareColors;
    in >> keyColors;
    in >> ended;

    in >> numPlayed;
    in >> numWon;
    in >> rateWon;
    in >> bestTry;
    in >> streak;
    in >> maxStreak;

    in >> animated;
    in >> hintVisible;
    in >> hard;
    in >> colorBlind;

    file.close();
    return true;
}
