#include "IconUtil.h"

QHash<QString, QIcon> IconUtil::cache;

const QIcon &IconUtil::load(const QString &path) {
    // Find the icon in the cache
    auto it = cache.find(path);
    if (it != cache.end()) {
        return it.value();
    }

    // Load the icon and insert it into the cache
    QIcon icon(path);
    auto result = cache.insert(path, icon);
    return result.value();
}

QIcon IconUtil::gray(const QString &path) {
    QImage image(path);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            int gray = qGray(color.rgb());
            color.setRgb(gray, gray, gray, color.alpha());
            image.setPixelColor(x, y, color);
        }
    }
    return QPixmap::fromImage(image);
}
