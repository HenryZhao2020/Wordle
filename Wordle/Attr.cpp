#include "Attr.h"
#include "Word.h"

Attr::Attr() {
    // Reset all attributes to their default values
    resetProgress();
    resetSettings();
    resetStats();
}

Attr::~Attr() {

}

int Attr::percentWon() {
    // Avoid division by 0
    return (totalPlays > 0) ? qRound(100.0 * totalWins / totalPlays) : 0;
}

void Attr::updateMinTries() {
    minTries = (minTries > 0) ? qMin(minTries, row) : row;
}

void Attr::updateMaxStreak() {
    maxStreak = qMax(maxStreak, streak);
}

void Attr::updateTriesDist() {
    triesDist[row]++;
}

void Attr::resetProgress() {
    row = 0;
    column = 0;
    answer = Word::getAnswer();
    letters = QStringList(6);
    colors = QList<QList<Color>>(6);
    keyColors = {};
    ended = false;
}

void Attr::resetSettings() {
    hard = false;
    colorBlind = false;
    animated = true;
    hinted = true;
    lang = Lang::ENGLISH;
}

void Attr::resetStats() {
    totalPlays = 0;
    totalWins = 0;
    minTries = 0;
    streak = 0;
    maxStreak = 0;
    triesDist = {};
}

void Attr::save() {
    QFile file("../Saved");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QDataStream out(&file);
    out << row;
    out << column;
    out << answer;
    out << letters;
    out << colors;
    out << keyColors;
    out << ended;

    out << hard;
    out << colorBlind;
    out << animated;
    out << hinted;
    out << lang;

    out << totalPlays;
    out << totalWins;
    out << minTries;
    out << streak;
    out << maxStreak;
    out << triesDist;

    file.close();
}

bool Attr::load() {
    QFile file("../Saved");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QDataStream in(&file);
    in >> row;
    in >> column;
    in >> answer;
    in >> letters;
    in >> colors;
    in >> keyColors;
    in >> ended;

    in >> hard;
    in >> colorBlind;
    in >> animated;
    in >> hinted;
    in >> lang;

    in >> totalPlays;
    in >> totalWins;
    in >> minTries;
    in >> streak;
    in >> maxStreak;
    in >> triesDist;

    file.close();
    return true;
}

Attr &Attr::get() {
    static Attr attr;
    return attr;
}
