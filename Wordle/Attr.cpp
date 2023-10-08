#include "Attr.h"

int Attr::row;
int Attr::column;

QString Attr::answer;
QList<QString> Attr::guesses(6);
QList<QList<Color>> Attr::squareColors(6);
QHash<QString, Color> Attr::keyColors;
bool Attr::ended;

int Attr::numPlayed;
int Attr::numWon;
int Attr::rateWon;
int Attr::bestTry;
int Attr::streak;
int Attr::maxStreak;

bool Attr::animated = true;
bool Attr::hintVisible = true;
bool Attr::hard;
bool Attr::colorBlind;

void Attr::save() {
    QFile file("../Saved");
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
    QFile file("../Saved");
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
