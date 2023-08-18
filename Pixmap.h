#ifndef PIXMAP_H
#define PIXMAP_H

#include <QPixmap>
#include <QHash>

class Pixmap {
public:
    static QPixmap &get(const QString &fileName);

private:
    static QHash<QString, QPixmap> pixmaps;
};

#endif
