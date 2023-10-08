#include "Square.h"
#include "Attr.h"

void Square::mark(Color color, double alpha) {
    QString style = "color: white; background: rgba(%0, %1)";
    setStyleSheet(style.arg(rgb(color)).arg(alpha));
}

void Square::fade(Color color) {
    alpha = 0;

    auto timer = new QTimer(this);
    timer->setInterval(8);
    connect(timer, &QTimer::timeout, this, [this, timer, color] {
        if (alpha > 1) {
            timer->deleteLater();
            return;
        }

        alpha += 0.05;
        mark(color, alpha);
    });
    timer->start();
}

void Square::zoom() {
    fontSize = 0;

    auto timer = new QTimer(this);
    timer->setInterval(4);
    connect(timer, &QTimer::timeout, this, [this, timer] {
        if (fontSize > 24) {
            timer->deleteLater();
            return;
        }

        setStyleSheet("font-size: " + QString::number(++fontSize) + "px");
    });
    timer->start();
}
