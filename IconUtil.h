#pragma once

#include <QHash>
#include <QIcon>

class IconUtil {
public:
    static const QIcon &load(const QString &path);
    static QIcon gray(const QString &path);

private:
    static QHash<QString, QIcon> cache;
};
