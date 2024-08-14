#pragma once

#include <QStringList>

class FileUtil {
public:
    static QString readAll(const QString &path);
    static QStringList readLines(const QString &path);
};
