#pragma once

/**
 * @brief Enumerates different colors used when marking letters of
 * the square on the board and the keys on the keyboard.
 */
enum class Color {
    /**
     * @brief The letter is not in the word.
     */
    GRAY,
    /**
     * @brief The letter is in the word but in the wrong position.
     */
    YELLOW,
    /**
     * @brief The letter is in the correct position of the word.
     */
    GREEN,
};

/**
 * @brief Returns the style sheet with the RGBA color code
 * in either normal or color blind mode.
 * @param color The specified 'Color' enum.
 * @return The style sheet with the RGBA color code.
 */
QString rgba(const Color &color, double alpha = 1);
