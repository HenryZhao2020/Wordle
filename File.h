#ifndef FILE_H
#define FILE_H

#include <QList>

class File {
public:
    static QString readAll(const QString &path);
    static QList<QString> readLines(const QString &path);

    static void write(const QString &path, const QString &text);
};

#endif
