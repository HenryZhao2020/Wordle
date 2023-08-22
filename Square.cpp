#include "Square.h"
#include "Attr.h"

void Square::mark(Color color, double alpha) {
    QString style = "color: white; background: rgba(%0, %1)";
    setStyleSheet(style.arg(rgb(color)).arg(alpha));
}

void Square::fade(Color color, double alpha) {
    if (alpha > 1) {
        return;
    }

    mark(color, alpha);
    QTimer::singleShot(8, this, [this, color, alpha] {
        fade(color, alpha + 0.05);
    });
}

void Square::zoom(int fontSize) {
    if (fontSize > 24) {
        return;
    }

    setStyleSheet("font-size: " + QString::number(fontSize) + "px");
    QTimer::singleShot(4, this, [this, fontSize] {
        zoom(fontSize + 1);
    });
}
