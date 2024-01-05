#pragma once

class Game;
class GameBar;
class Square;
enum class Color;

/**
 * @brief Displays squares in a 6x5 grid.
 */
class Board : public QFrame {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'Board' instance.
     * @param game The parent 'Game' instance.
     */
    Board(Game *game);
    ~Board();

    /**
     * @brief Appends the specified letter to the current row.
     * @param letter The appended letter.
     */
    void append(const QChar &letter);

    /**
     * @brief Removes the last letter from the current row.
     */
    void removeLast();

    /**
     * @brief Validates and checks whether the guess
     * on the current row is correct.
     */
    void checkGuess();

    /**
     * @brief Places a letter on a square.
     * @param row The row that the letter will be.
     * @param column The column that the letter will be.
     * @param letter The placed letter.
     * @param animated Whether animation is enabled.
     */
    void placeLetter(int row, int column, const QChar &letter, bool animated);

    /**
     * @brief Marks a square in the specified row and column on the board.
     * @param row The row that the square is in.
     * @param column The column that the square is in.
     * @param color The color of the square.
     * @param animated Whether animation is enabled.
     */
    void markSquare(int row, int column, const Color &color, bool animated);

    /**
     * @brief Updates the RGB colors of all squares.
     * @note This function is called after changing the color blind setting.
     */
    void updateColors();

    /**
     * @brief Handles the end of game when the player gives up or
     * does not guess the word correctly in 6 tries.
     */
    void lose();

    /**
     * @brief Sets whether the board is interactive
     * with keyboard and mouse.
     * @param interactive Whether the board is interactive
     * with keyboard and mouse.
     */
    void setInteractive(bool interactive);

    /**
     * @brief Returns whether the board is interactive
     * with keyboard and mouse.
     * @return Whether the board is interactive
     * with keyboard and mouse.
     */
    bool isInteractive();

private:
    Game *game;
    GameBar *gameBar;
    // Store all squares
    Square *squares[6][5];
    // Whether the board is interactive with keyboard and mouse
    bool interactive = true;

    /**
     * @brief Checks whether a guess is valid.
     * @details
     * A guess is considered valid if it meets the following conditions:
     *   - Contains 5 letters.
     *   - An existing word.
     *   - If hard mode is enabled, the subsequent guess must use
     *     all yellow and green letters from the previous row.
     *
     * @param guess The current guess.
     * @return Whether the guess is valid.
     */
    bool isGuessValid(const QString &guess);

    /**
     * @brief Checks whether a guess meets the conditions of hard mode.
     * @details
     * If hard mode is enabled, the subsequent guess must use
     * all yellow and green letters from the previous row.
     *
     * @param guess The current guess.
     * @return Whether the guess is valid in hard mode.
     */
    bool isHardValid(const QString &guess);

    /**
     * @brief Marks each letter of the guess to indicate correctness.
     * @param guess The current guess.
     */
    void markGuess(const QString &guess);

    /**
     * @brief Ends the game.
     * @param icon The displayed icon on the game bar.
     * @param text The displayed text on the game bar.
     */
    void end(const QIcon &icon, const QString &text);

    /**
     * @brief Handles the end of game when the player
     * guesses the word correctly.
     */
    void win();
};
