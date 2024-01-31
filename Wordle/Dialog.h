#pragma once

// Forward declarations
class Game;

/**
 * @brief The base class for dialog boxes in-game.
 */
class Dialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Make this window non-resizable upon opening.
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

    // The main layout of the dialog
    QVBoxLayout *mainLayout;
    // Display buttons horizontally
    QHBoxLayout *buttonLayout;
    // Create a universal 'OK' button that closes a dialog on click.
    QPushButton *okButton;
};

/**
 * @brief Displays and modifies game settings.
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
    // Store each check box and a pointer to its corresponding value
    QHash<QCheckBox *, bool *> boxes;
    // Display a list of available languages
    QComboBox *langBox;
    // Whether language is changed
    bool langChanged;

    /**
     * @brief Creates a group box that displays widgets vertically.
     * @param title The title of the group box.
     * @return The created group box.
     */
    QVBoxLayout *newGroupLayout(const QString &title);

    /**
     * @brief Creates a check box that toggles a setting.
     * @param name The name of the setting.
     * @param state The initial state of the check box.
     * @return The created check box.
     */
    QCheckBox *newCheckBox(const QString &name, bool &state);

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
 * @brief Displays rows of statistical data.
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
    QGridLayout *gridLayout;

    /**
     * @brief Adds a row of statistical entry.
     * @param name The name to be displayed on the left.
     * @param value The statistical value displayed on the right.
     */
    void addRow(const QString &name, int value);

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
