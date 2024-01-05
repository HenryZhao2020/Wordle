#pragma once

class Game;

/**
 * @brief The base class for dialog boxes in-game.
 */
class Dialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Overrides the original QDialog::show() method.
     */
    void show();

protected:
    /**
     * @brief Initializes a new 'Dialog' instance.
     * @param game The parent 'Game' instance.
     * @param icon The icon on the title bar.
     * @param title The text on the title bar.
     */
    Dialog(Game *game, const QIcon &icon, const QString &title);
    ~Dialog();

    Game *game;
    // Container for any additional widgets
    QFrame *mainFrame;
    // Display buttons horizontally
    QHBoxLayout *buttonLayout;
    // Create a universal 'OK' button that closes a dialog on click.
    QPushButton *okButton;
};

/**
 * @brief Displays a list of check boxes that toggle game settings.
 */
class SettingsDialog : public Dialog {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'SettingsDialog' instance.
     * @param game The parent 'Game' instance.
     */
    SettingsDialog(Game *game);
    ~SettingsDialog();

private:
    // Layout of the main frame
    QVBoxLayout *vboxLayout;
    // Store each check box and a pointer to its corresponding value
    QHash<QCheckBox *, bool *> boxes;

    /**
     * @brief Adds a check box that toggles a setting.
     * @param name The name of the setting.
     * @param state The initial state of the check box.
     */
    void addBox(const QString &name, bool &state);

    /**
     * @brief Updates all settings based on the states of the check boxes.
     */
    void updateSettings();

    /**
     * @brief Resets all settings to their default values.
     */
    void resetSettings();

    /**
     * @brief Applies changes after updating settings.
     */
    void applySettings();
};

/**
 * @brief Displays statistical data.
 */
class StatsDialog : public Dialog {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'StatsDialog' instance.
     * @param game The parent 'Game' instance.
     */
    StatsDialog(Game *game);
    ~StatsDialog();

private:
    // Layout of the main frame
    QGridLayout *gridLayout;

    /**
     * @brief Adds a statistical entry.
     * @param name The displayed text.
     * @param val The statistical value.
     */
    void addEntry(const QString &name, int val);

    /**
     * @brief Creates a progress bar displaying the distribution of
     * the specified number of tries.
     * @param i The number of tries.
     */
    void addDist(int i);

    /**
     * @brief Resets all statistics to zero.
     */
    void resetStats();
};

/**
 * @brief Displays rules and information about this game.
 */
class HelpDialog : public Dialog {
    Q_OBJECT

public:
    /**
     * @brief Initializes a new 'HelpDialog' instance.
     * @param game The parent 'Game' instance.
     */
    HelpDialog(Game *game);
    ~HelpDialog();

private:
    /**
     * @brief Creates a text browser displaying the content of a HTML file.
     * @param path The HTML file path.
     * @return The created text browser.
     */
    QTextBrowser *newBrowser(const QString &path);
};
