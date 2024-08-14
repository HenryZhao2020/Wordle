#include "Attr.h"
#include "Dict.h"

#include <QFile>
#include <QDataStream>

const QMap<Lang::Name, QLocale> Lang::LOCALES = {
    {Lang::ENGLISH, QLocale(QLocale::English, QLocale::LatinScript, QLocale::World)},
    {Lang::CHINESE_SIMP, QLocale(QLocale::Chinese, QLocale::SimplifiedChineseScript)},
    {Lang::CHINESE_TRAD, QLocale(QLocale::Chinese, QLocale::TraditionalChineseScript)},
};

QStringList Lang::getLangNames() {
    QStringList names;
    for (const auto &locale : LOCALES) {
        names.append(locale.nativeLanguageName());
    }
    return names;
}

QString Lang::getLangTerrCode(Name lang) {
    QString langCode = QLocale::languageToCode(LOCALES[lang].language());
    QString terrCode = QLocale::territoryToCode(LOCALES[lang].territory());
    return langCode + "_" + terrCode;
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
    ++triesDist[row];
}

void Attr::resetProgress() {
    row = 0;
    col = 0;
    answer = Dict::getAnswer();
    letters = QStringList(6);
    colors = QList<QList<Color>>(6);
    keyColors = {};
    ended = false;
}

void Attr::resetSettings() {
    lang = Lang::ENGLISH;
    hard = false;
    colorBlind = false;
    animated = true;
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
    QFile file("Wordle_Data");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QDataStream out(&file);
    out << row;
    out << col;
    out << answer;
    out << letters;
    out << colors;
    out << keyColors;
    out << ended;

    out << lang;
    out << hard;
    out << colorBlind;
    out << animated;    

    out << totalPlays;
    out << totalWins;
    out << minTries;
    out << streak;
    out << maxStreak;
    out << triesDist;

    file.close();
}

bool Attr::load() {
    QFile file("Wordle_Data");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QDataStream in(&file);
    in >> row;
    in >> col;
    in >> answer;
    in >> letters;
    in >> colors;
    in >> keyColors;
    in >> ended;

    in >> lang;
    in >> hard;
    in >> colorBlind;
    in >> animated;

    in >> totalPlays;
    in >> totalWins;
    in >> minTries;
    in >> streak;
    in >> maxStreak;
    in >> triesDist;

    file.close();
    return true;
}

Attr::Attr() {
    resetProgress();
    resetSettings();
    resetStats();
}

Attr::~Attr() {}

Attr &Attr::get() {
    static Attr attr;
    return attr;
}
