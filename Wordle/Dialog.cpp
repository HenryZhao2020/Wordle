#include "Dialog.h"
#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"

Dialog::Dialog(Game *game, const QIcon &icon, const QString &title)
    : QDialog(game), game(game) {
    setWindowIcon(icon);
    setWindowTitle(title);
    // Disable user interaction with the main window
    setModal(true);
    // Free memory on close
    setAttribute(Qt::WA_DeleteOnClose);

    // Set up the layout
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Container for any additional widgets
    mainFrame = new QFrame(this);
    mainLayout->addWidget(mainFrame);
    mainLayout->addStretch();

    // Container for buttons
    auto buttonFrame = new QFrame(this);
    mainLayout->addSpacing(45);
    mainLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);

    // Display buttons horizontally
    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setSpacing(5);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    // Create a universal 'OK' button that closes a dialog on click.
    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &close);
    buttonLayout->addWidget(okButton);
}

Dialog::~Dialog() {

}

void Dialog::show() {
    // Make this window not resizable
    setFixedSize(sizeHint());
    QDialog::show();
}

SettingsDialog::SettingsDialog(Game *game)
    : Dialog(game, Icon::load("Settings.svg"), "Settings") {
    // Set up the layout for the main frame
    vboxLayout = new QVBoxLayout(mainFrame);
    vboxLayout->setSpacing(10);
    vboxLayout->setContentsMargins(0, 0, 0, 0);

    // Add check boxes
    addBox("Hard Mode", Attr::get().hard);
    addBox("Color Blind Mode", Attr::get().colorBlind);
    addBox("Enable Animation", Attr::get().animated);
    addBox("Show Hint", Attr::get().hinted);

    // Update all settings on click
    connect(okButton, &QPushButton::clicked, this, &updateSettings);

    // Reset all settings on click
    auto resetButton = new QPushButton("Reset", this);
    connect(resetButton, &QPushButton::clicked, this, &resetSettings);
    buttonLayout->addWidget(resetButton);

    // Discard all changes on click
    auto cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &close);
    buttonLayout->addWidget(cancelButton);
}

SettingsDialog::~SettingsDialog() {

}

void SettingsDialog::addBox(const QString &name, bool &state) {
    auto box = new QCheckBox(name, this);
    box->setChecked(state);
    boxes.insert(box, &state);
    vboxLayout->addWidget(box);
}

void SettingsDialog::updateSettings() {
    for (auto it = boxes.begin(); it != boxes.end(); ++it) {
        *it.value() = it.key()->isChecked();
    }

    applySettings();
}

void SettingsDialog::resetSettings() {
    Attr::get().resetSettings();

    applySettings();
}

void SettingsDialog::applySettings() {
    game->getGameBar()->setHintVisible(Attr::get().hinted);
    game->getBoard()->updateColors();
    game->getKeyboard()->updateColors();

    close();
}

StatsDialog::StatsDialog(Game *game)
    : Dialog(game, Icon::load("Stats.svg"), "Statistics") {
    // Set up the layout for the main frame
    gridLayout = new QGridLayout(mainFrame);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Add a list of statistical data
    addEntry("Total Played", Attr::get().totalPlays);
    addEntry("Total Won", Attr::get().totalWins);
    addEntry("Percentage Won", Attr::get().percentWon());
    addEntry("Best Try", Attr::get().minTries);
    addEntry("Current Streak", Attr::get().streak);
    addEntry("Max Streak", Attr::get().maxStreak);

    // Add a separator above the distribution table
    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    gridLayout->addWidget(separator, gridLayout->rowCount(), 0, 1, 2);

    // Display table title
    auto titleLabel = new QLabel("Best Tries Distribution", this);
    gridLayout->addWidget(titleLabel, gridLayout->rowCount(), 0, 1, 2);

    // Add each distribution to the table
    for (int i = 1; i <= 6; ++i) {
        addDist(i);
    }

    // Reset all statistics on click
    auto resetButton = new QPushButton("Reset", this);
    connect(resetButton, &QPushButton::clicked, this, &resetStats);
    buttonLayout->addWidget(resetButton);
}

StatsDialog::~StatsDialog() {

}

void StatsDialog::addEntry(const QString &name, int val) {
    int row = gridLayout->rowCount();

    auto nameLabel = new QLabel(name + ":", this);
    gridLayout->addWidget(nameLabel, row, 0, Qt::AlignLeft);

    auto valLabel = new QLabel(QString::number(val), this);
    gridLayout->addWidget(valLabel, row, 1);
}

void StatsDialog::addDist(int i) {
    int row = gridLayout->rowCount();
    int totalWins = Attr::get().totalWins;
    const QHash<int, int> &dist = Attr::get().triesDist;

    // Display a progress bar on the left to visualize percentage
    auto bar = new QProgressBar(this);
    bar->setFixedWidth(320);
    bar->setValue((totalWins > 0) ? qRound(100.0 * dist[i] / totalWins) : 0);
    gridLayout->addWidget(bar, row, 0);

    // Display the number of tries on the right
    auto label = new QLabel(QString("#%0").arg(i), this);
    gridLayout->addWidget(label, row, 1);
}

void StatsDialog::resetStats() {
    Attr::get().resetStats();
    close();
}

HelpDialog::HelpDialog(Game *game)
    : Dialog(game, Icon::load("Help.svg"), "Help") {
    // Set up the layout for the main frame
    auto vboxLayout = new QVBoxLayout(mainFrame);
    vboxLayout->setSpacing(0);
    vboxLayout->setContentsMargins(0, 0, 0, 0);

    // Create tabs for navigation
    auto tabWidget = new QTabWidget(this);
    tabWidget->addTab(newBrowser("Rules.html"), "Rules");
    tabWidget->addTab(newBrowser("About.html"), "About");
    vboxLayout->addWidget(tabWidget);
}

HelpDialog::~HelpDialog() {

}

QTextBrowser *HelpDialog::newBrowser(const QString &path) {
    auto browser = new QTextBrowser(this);
    // Display HTML content
    browser->setHtml(File::readAll(path));
    // Open hyperlinks in the HTML content on click
    browser->setOpenExternalLinks(true);
    connect(browser, &QTextBrowser::anchorClicked, this, [] {});

    return browser;
}
