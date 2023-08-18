#include "Attr.h"

int Attr::row = 0;
int Attr::column = 0;

QString Attr::answer = "";
QList<QString> Attr::guesses = QList<QString>(6);
QList<QList<Color>> Attr::squareColors = QList<QList<Color>>(6);
QHash<QString, Color> Attr::keyColors = {};
bool Attr::ended = false;

int Attr::numPlayed = 0;
int Attr::numWon = 0;
int Attr::rateWon = 0;
int Attr::bestTry = 0;
int Attr::streak = 0;
int Attr::maxStreak = 0;

bool Attr::animated = true;
bool Attr::hintVisible = true;
bool Attr::hard = false;
bool Attr::colorBlind = false;

void Attr::save() {
    QFile file("Wordle_Data");
    file.open(QFile::WriteOnly);

    QDataStream stream(&file);
    stream << row;
    stream << column;

    stream << answer;
    stream << guesses;
    stream << squareColors;
    stream << keyColors;
    stream << ended;

    stream << numPlayed;
    stream << numWon;
    stream << rateWon;
    stream << bestTry;
    stream << streak;
    stream << maxStreak;

    stream << animated;
    stream << hintVisible;
    stream << hard;
    stream << colorBlind;

    file.close();
}

bool Attr::load() {
    QFile file("Wordle_Data");
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream >> row;
    stream >> column;

    stream >> answer;
    stream >> guesses;
    stream >> squareColors;
    stream >> keyColors;
    stream >> ended;

    stream >> numPlayed;
    stream >> numWon;
    stream >> rateWon;
    stream >> bestTry;
    stream >> streak;
    stream >> maxStreak;

    stream >> animated;
    stream >> hintVisible;
    stream >> hard;
    stream >> colorBlind;

    file.close();
    return true;
}
