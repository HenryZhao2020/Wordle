#pragma once

class Game;
enum class Color;

/**
 * @brief Displays a simplified "QWERTY" keyboard for input.
 */
class Keyboard : public QFrame {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'Keyboard' instance.
     * @param game The parent 'Game' instance.
     */
    Keyboard(Game *game);
    ~Keyboard();

    /**
     * @brief Marks the key with the specified letter in the specified color
     * to indicate the correctness of its corresponding letter.
     * @param letter The letter on the key.
     * @param color The new color of the key.
     */
    void markKey(const QChar &letter, const Color &color);

    /**
     * @brief Resets the colors of all keys to transparent.
     */
    void clearColors();

    /**
     * @brief Updates the RGB colors of all keys.
     * @note This function is called after changing the color blind setting.
     */
    void updateColors();

private:
    Game *game;
    QVBoxLayout *vboxLayout;
    // Store a letter and its corresponding key on the keyboard
    QHash<QChar, QPushButton *> keys;

    /**
     * @brief Creates a row of keys with the
     * specified letters on the keyboard.
     * @param letters The letters on the keys.
     * @return The row layout for addition of widgets.
     */
    QHBoxLayout *newRow(const QList<QChar> &letters);

    /**
     * @brief Creates a key displaying the specified letter.
     * @param letter The letter on the key.
     * @return The created key.
     */
    QPushButton *newKey(const QChar &letter);
};
