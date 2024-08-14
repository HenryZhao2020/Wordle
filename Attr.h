#pragma once

#include <QHash>
#include <QList>
#include <QLocale>
#include <QMap>

enum class Color;

class Lang {
public:
    enum Name {
        ENGLISH,
        CHINESE_SIMP,
        CHINESE_TRAD,
    };

    static QStringList getLangNames();
    static QString getLangTerrCode(Name lang);

private:
    static const QMap<Name, QLocale> LOCALES;
};

class Attr {
public:
    // Progress
    int row;
    int col;
    QString answer;
    QStringList letters;
    QList<QList<Color>> colors;
    QHash<QChar, Color> keyColors;
    bool ended;

    // Settings
    Lang::Name lang;
    bool hard;
    bool colorBlind;
    bool animated;

    // Statistics
    int totalPlays;
    int totalWins;
    int minTries;
    int streak;
    int maxStreak;
    QHash<int, int> triesDist;

    int percentWon();
    void updateMinTries();
    void updateMaxStreak();
    void updateTriesDist();

    void resetProgress();
    void resetSettings();
    void resetStats();

    void save();
    bool load();

    static Attr &get();

private:
    Attr();
    ~Attr();
};
