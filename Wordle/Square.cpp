#include "Square.h"
#include "Board.h"
#include "Color.h"

Square::Square(Board *board) : QPushButton(board) {

}

Square::~Square() {
    // Ensure all pointers are deleted
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }

    if (fadeTimer != nullptr) {
        fadeTimer->deleteLater();
    }
}

void Square::zoomIn() {
    // Stop the animation if it is currently running
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }

    // Reset the font size (px) to 0
    fontSize = 0;

    // Set up the animation
    zoomTimer = new QTimer(this);
    connect(zoomTimer, &QTimer::timeout, this, [this] {
        // Stop the timer after the font size reaches 24px
        if (fontSize >= 24) {
            zoomTimer->deleteLater();
            zoomTimer = nullptr;
            return;
        }

        // Increase the font size by 1px after each interval
        setStyleSheet(QString("font-size: %0px").arg(++fontSize));
    });

    // Start the animation
    zoomTimer->start(5);
}

void Square::fadeIn(const Color &color) {
    // Stop the animation if it is currently running
    if (fadeTimer != nullptr) {
        fadeTimer->deleteLater();
    }

    // Reset the alpha value to 0
    alpha = 0;

    // Set up the animation
    fadeTimer = new QTimer(this);
    connect(fadeTimer, &QTimer::timeout, this, [this, color] {
        // Stop the timer after the alpha value reaches 1
        if (alpha >= 1) {
            fadeTimer->deleteLater();
            fadeTimer = nullptr;
            return;
        }

        // Increase the alpha value by 0.05 after each interval
        alpha += 0.05;
        setStyleSheet(rgba(color, alpha));
    });

    // Start the animation
    fadeTimer->start(10);
}
