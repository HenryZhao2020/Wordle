#ifndef DICT_H
#define DICT_H

#include "PCH.h"

class Dict {
public:
    static void load();
    static bool isExist(const QString &word);
    static QString generateAnswer();

private:
    static QList<QString> words, answers;
};

#endif
