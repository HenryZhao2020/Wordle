#pragma once

class Board;
enum class Color;

/**
 * @brief Displays a capitalized letter.
 */
class Square : public QPushButton {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'Square' instance.
     * @param board The parent 'Board' instance.
     */
    Square(Board *board);
    ~Square();

    /**
     * @brief Gradually increases the font size to 24px.
     */
    void zoomIn();

    /**
     * @brief Gradually increases the background opacity to 1.
     */
    void fadeIn(const Color &color);

private:
    // Used for zoom-in effect
    QTimer *zoomTimer = nullptr;
    // Store the current font size (px)
    int fontSize;

    // Used for fade-in effect
    QTimer *fadeTimer = nullptr;
    // Store the current background opacity
    double alpha;
};
