#include "Dialog.h"
#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"

Dialog::Dialog(Game *game, const QPixmap &pixmap, const QString &title)
    : QDialog(game, Qt::MSWindowsFixedSizeDialogHint) {
    this->game = game;

    setWindowIcon(pixmap);
    setWindowTitle(title);
    setModal(true);

    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(10);
    vboxLayout->setContentsMargins(30, 30, 30, 30);

    buttonLayout = new QHBoxLayout(this);
    buttonLayout->setSpacing(5);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setAlignment(Qt::AlignCenter);
    vboxLayout->addSpacing(40);
    vboxLayout->addLayout(buttonLayout);

    okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(okButton);
}

void Dialog::close() {
    QDialog::close();
    deleteLater();
}

StatsDialog::StatsDialog(Game *game)
    : Dialog(game, Pixmap::get("Stats.png"), "Statistics") {
    formLayout = new QFormLayout();
    formLayout->setHorizontalSpacing(80);
    formLayout->setVerticalSpacing(10);
    formLayout->setContentsMargins(0, 0, 0, 0);
    vboxLayout->insertLayout(0, formLayout);

    addRow("# Played:", Attr::numPlayed);
    addRow("# Won:", Attr::numWon);
    addRow("% Won:", Attr::rateWon);
    addRow("Best Try:", Attr::bestTry);
    addRow("Current Streak:", Attr::streak);
    addRow("Max Streak:", Attr::maxStreak);

    auto resetButton = new QPushButton("Reset", this);
    connect(resetButton, &QPushButton::clicked, this, &StatsDialog::reset);
    buttonLayout->insertWidget(0, resetButton);
}

void StatsDialog::addRow(const QString &labelText, int value) {
    auto label = new QLabel(QString::number(value), this);
    formLayout->addRow(labelText, label);
}

void StatsDialog::reset() {
    Attr::numPlayed = 0;
    Attr::numWon = 0;
    Attr::rateWon = 0;
    Attr::bestTry = 0;
    Attr::streak = 0;
    Attr::maxStreak = 0;

    close();
}

SettingsDialog::SettingsDialog(Game *game)
    : Dialog(game, Pixmap::get("Settings.png"), "Settings") {
    newBox("Enable Animation", &Attr::animated);
    newBox("Show Hint", &Attr::hintVisible);
    newBox("Hard Mode", &Attr::hard);
    newBox("Color Blind Mode", &Attr::colorBlind);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::apply);

    auto cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(cancelButton);
}

QCheckBox *SettingsDialog::newBox(const QString &text, bool *var) {
    auto box = new QCheckBox(text, this);
    box->setChecked(*var);
    vboxLayout->insertWidget(boxes.size(), box);
    boxes[box] = var;
    return box;
}

void SettingsDialog::apply() {
    for (auto &box : boxes.keys()) {
        *boxes[box] = box->isChecked();
    }

    game->getGameBar()->setHintVisible(Attr::hintVisible);
    game->getBoard()->remarkAll();
    game->getKeyboard()->remarkAll();
}

HelpDialog::HelpDialog(Game *game)
    : Dialog(game, Pixmap::get("Help.png"), "Help") {
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(newTextEdit("Rules.html"), "Rules");
    tabWidget->addTab(newTextEdit("About.html"), "About");
    vboxLayout->insertWidget(0, tabWidget);
}

QTextEdit *HelpDialog::newTextEdit(const QString &htmlFile) {
    auto textEdit = new QTextEdit(tabWidget);
    textEdit->setReadOnly(true);
    textEdit->setHtml(File::readAll(htmlFile));
    return textEdit;
}
