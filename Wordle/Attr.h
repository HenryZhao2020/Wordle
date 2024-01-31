#pragma once

// Forward declarations
enum class Color;

/**
 * @brief Contains the names of available languages.
 */
namespace Lang {
    const QString ENGLISH = "English";
    const QString CHINESE_SIMPLIFIED = "简体中文";
}

    /**
 * @brief Contains shared attributes for saving and loading game state.
 */
class Attr {
public:
    // Game Progress
    /**
     * @brief The current row on the board.
     */
    int row;
    /**
     * @brief The current column on the board.
     */
    int column;
    /**
     * @brief Wordle answer for the current round.
     */
    QString answer;
    /**
     * @brief Stores the letters of the squares on the board.
     */
    QStringList letters;
    /**
     * @brief Stores the colors of the squares on the board.
     */
    QList<QList<Color>> colors;
    /**
     * @brief Stores the colors of the letters on the keyboard.
     */
    QHash<QChar, Color> keyColors;
    /**
     * @brief Whether the current round has ended.
     */
    bool ended;

    // Settings
    /**
     * @brief Whether hard mode is enabled.
     */
    bool hard;
    /**
     * @brief Whether color blind mode is enabled.
     */
    bool colorBlind;
    /**
     * @brief Whether animations are enabled.
     */
    bool animated;
    /**
     * @brief Whether the hint on the left of the game bar is visible.
     */
    bool hinted;
    /**
     * @brief Displayed language.
     */
    QString lang;

    // Statistics
    /**
     * @brief Total number of gameplays.
     */
    int totalPlays;
    /**
     * @brief Total number of wins.
     */
    int totalWins;
    /**
     * @brief Minimum number of tries to win a Wordle game.
     */
    int minTries;
    /**
     * @brief Current streak.
     */
    int streak;
    /**
     * @brief Maximum streak in gameplay history.
     */
    int maxStreak;
    /**
     * @brief Distribution of each number of tries.
     */
    QHash<int, int> triesDist;

    // Calculated properties based on the existing attributes
    /**
     * @brief Returns the percentage of winning to the nearest integer.
     * @return The percentage of winning to the nearest integer.
     */
    int percentWon();

    // Update the attributes
    /**
     * @brief Updates the minimum number of tries to win a Wordle game.
     */
    void updateMinTries();
    /**
     * @brief Updates the maximum streak.
     */
    void updateMaxStreak();
    /**
     * @brief Updates the tries distribution.
     */
    void updateTriesDist();

    // Reset attributes
    /**
     * @brief Resets the game progress.
     */
    void resetProgress();
    /**
     * @brief Resets all settings to their default values.
     */
    void resetSettings();
    /**
     * @brief Resets all statistics to zero.
     */
    void resetStats();

    // Save & Load
    /**
     * @brief Saves all attributes to the game folder.
     */
    void save();
    /**
     * @brief Loads all attributes from the game folder.
     * @return true if successful; false otherwise.
     */
    bool load();

    // Singleton access
    /**
     * @brief Provides access to the singleton instance.
     * @return The singleton instance.
     */
    static Attr &get();

private:
    // Private constructor to prevent external instantiation
    Attr();
    ~Attr();
};
