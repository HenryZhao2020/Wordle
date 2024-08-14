#include "FileUtil.h"

#include <QFile>

QString FileUtil::readAll(const QString &path) {
    QFile file(path);
    QString content;

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        content = file.readAll();
        file.close();
    }

    return content;
}

QStringList FileUtil::readLines(const QString &path) {
    return readAll(path).split('\n');
}
