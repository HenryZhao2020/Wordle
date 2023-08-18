#include "Pixmap.h"

QHash<QString, QPixmap> Pixmap::pixmaps = {};

QPixmap &Pixmap::get(const QString &fileName) {
    if (pixmaps.contains(fileName)) {
        return pixmaps[fileName];
    }

    pixmaps[fileName] = QPixmap(fileName);
    return pixmaps[fileName];
}
