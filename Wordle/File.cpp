#include "File.h"

QString File::readAll(const QString &path) {
    QFile file(path);

    // If the file fails to open, return an empty string
    // Make sure to use 'QFile::Text' as it enforces '\n' line separator
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return "";
    }

    // Read file content
    QString content = file.readAll();
    file.close();

    return content;
}

QStringList File::readLines(const QString &path) {
    return readAll(path).split('\n');
}
