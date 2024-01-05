#pragma once

class Game;

/**
 * @brief Displays a hint on the left and a set of buttons on the right.
 */
class GameBar : public QFrame {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'GameBar' instance.
     * @param game The parent 'Game' instance.
     */
    GameBar(Game *game);
    ~GameBar();

    /**
     * @brief Sets the icon on the left.
     * @param icon The displayed icon.
     */
    void setHintIcon(const QIcon &icon);

    /**
     * @brief Sets the hint text beside the icon.
     * @param text The displayed hint text.
     */
    void setHintText(const QString &text);

    /**
     * @brief Shows or hides the hint on the left.
     * @param visible The visibility of the hint.
     */
    void setHintVisible(bool visible);

    /**
     * @brief Shows or hides the 'Restart' button.
     * @param visible The visibility of the 'Restart' button.
     */
    void setRestartVisible(bool visible);

    /**
     * @brief Shows or hides the 'Give Up' button.
     * @param visible The visibility of the 'Give Up' button.
     */
    void setGiveUpVisible(bool visible);

    /**
     * @brief Displays the number of chances left to guess the word.
     */
    void displayChances();

private:
    QHBoxLayout *hboxLayout;
    QPushButton *iconButton;
    QLabel *hintLabel;
    QPushButton *restartButton;
    QPushButton *giveUpButton;

    // Used for the typewriter effect
    QTimer *hintTimer = nullptr;
    // Store the displayed length of the hint text
    int hintLength;

    /**
     * @brief Displays the subsequent character after each interval,
     * simulating the interface of a typewriter.
     * @param text The displayed hint text.
     */
    void typewriteHint(const QString &text);

    /**
     * @brief Creates a new button on the right.
     * @param icon Icon of the button.
     * @param tip Tool tip of the button.
     * @param callable Button's action on click.
     * @return The created button.
     */
    template <typename Callable>
    QPushButton *newButton(const QIcon &icon, const QString &tip,
                           const Callable &callable);
};
