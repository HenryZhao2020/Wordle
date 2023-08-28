#ifndef PIXMAP_H
#define PIXMAP_H

class Pixmap {
public:
    static QPixmap &get(const QString &fileName);

private:
    static QHash<QString, QPixmap> pixmaps;
};

#endif
