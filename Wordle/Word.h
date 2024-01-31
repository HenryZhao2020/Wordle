#pragma once

/**
 * @brief Processes words and generates answers.
 */
namespace Word {
    /**
     * @brief Generates a random word from the Wordle answers.
     * @return A random word from the Wordle answers.
     */
    const QString &getAnswer();

    /**
     * @brief Checks whether a word is in the word bank.
     * @param word The word awaiting for validation.
     * @return 'true' if a word is valid; 'false' otherwise.
     */
    bool isValid(const QString &word);

    /**
     * @brief Loads all words from files.
     */
    void load();
}
