#include "File.h"

#include <QFile>
#include <QTextStream>

QString File::readAll(const QString &path) {
    QFile file(path);
    file.open(QFile::ReadOnly);
    QString text = file.readAll();
    file.close();
    return text;
}

QList<QString> File::readLines(const QString &path) {
    QFile file(path);
    file.open(QFile::ReadOnly);

    QTextStream stream(&file);
    QList<QString> list;
    
    while (!stream.atEnd()) {
        list.append(stream.readLine());
    }

    file.close();
    return list;
}

void File::write(const QString &path, const QString &text) {
    QFile file(path);
    file.open(QFile::Truncate | QFile::WriteOnly);
    file.write(text.toUtf8());
    file.close();
}
