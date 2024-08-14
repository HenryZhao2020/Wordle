#pragma once

#include <QStringList>

class Dict {
public:
    static const QString &getAnswer();
    static bool isValid(const QString &word);
    static void load();

private:
    static QStringList words;
    static QStringList answers;
};
