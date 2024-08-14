#include "Square.h"
#include "Board.h"
#include "Color.h"

Square::Square(Board *board) : QPushButton(board) {}

Square::~Square() {
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }

    if (fadeTimer != nullptr) {
        fadeTimer->deleteLater();
    }
}

void Square::zoomIn() {
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }
    fontSize = 1;
    setStyleSheet(QString("font-size: %0px").arg(fontSize));

    zoomTimer = new QTimer(this);
    connect(zoomTimer, &QTimer::timeout, this, [this] {
        if (fontSize >= 24) {
            zoomTimer->deleteLater();
            zoomTimer = nullptr;
            return;
        }

        setStyleSheet(QString("font-size: %0px").arg(++fontSize));
    });
    zoomTimer->start(4);
}

void Square::fadeIn(Color color) {
    if (fadeTimer != nullptr) {
        fadeTimer->deleteLater();
    }
    alpha = 0;

    fadeTimer = new QTimer(this);
    connect(fadeTimer, &QTimer::timeout, this, [this, color] {
        if (alpha >= 1) {
            fadeTimer->deleteLater();
            fadeTimer = nullptr;
            return;
        }

        alpha += 0.05;
        setStyleSheet(RGBA::getStyleSheet(color, alpha));
    });

    fadeTimer->start(10);
}
