#include "Dialog.h"
#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"

Dialog::Dialog(Game *game, const QIcon &icon, const QString &title)
    : QDialog(game, Qt::MSWindowsFixedSizeDialogHint) {
    this->game = game;

    setWindowIcon(icon);
    setWindowTitle(title);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(10);
    vboxLayout->setContentsMargins(30, 30, 30, 30);

    auto buttonFrame = new QFrame(this);
    vboxLayout->addSpacing(40);
    vboxLayout->addWidget(buttonFrame);

    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setSpacing(5);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setAlignment(Qt::AlignCenter);

    okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(okButton);
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

StatsDialog::StatsDialog(Game *game)
    : Dialog(game, getIcon("Stats.svg"), "Statistics") {
    auto formFrame = new QFrame(this);
    vboxLayout->insertWidget(0, formFrame);

    formLayout = new QFormLayout(formFrame);
    formLayout->setHorizontalSpacing(80);
    formLayout->setVerticalSpacing(10);
    formLayout->setContentsMargins(0, 0, 0, 0);

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
    : Dialog(game, getIcon("Settings.svg"), "Settings") {
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
    for (auto it = boxes.begin(); it != boxes.end(); it++) {
        *it.value() = it.key()->isChecked();
    }

    game->getGameBar()->setHintVisible(Attr::hintVisible);
    game->getBoard()->remarkAll();
    game->getKeyboard()->remarkAll();
}

HelpDialog::HelpDialog(Game *game)
    : Dialog(game, getIcon("Help.svg"), "Help") {
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(newTextEdit("Rules.html"), "Rules");
    vboxLayout->insertWidget(0, tabWidget);

    auto aboutFrame = new QFrame(this);
    tabWidget->addTab(aboutFrame, "About");

    auto aboutLayout = new QGridLayout(aboutFrame);
    aboutLayout->setSpacing(0);
    aboutLayout->setContentsMargins(0, 0, 0, 0);
    aboutLayout->addWidget(newTextEdit("About.html"), 0, 0, 1, 0);

    auto webButton = new QPushButton("Website", this);
    webButton->setCursor(Qt::PointingHandCursor);
    connect(webButton, &QPushButton::clicked, this, [] {
        static QUrl url("https://github.com/HenryZhao2020/TicTacToe");
        QDesktopServices::openUrl(url);
    });
    aboutLayout->addWidget(webButton, 1, 0);

    auto qtButton = new QPushButton("About Qt", this);
    qtButton->setCursor(Qt::PointingHandCursor);
    connect(qtButton, &QPushButton::clicked, this, &QApplication::aboutQt);
    aboutLayout->addWidget(qtButton, 1, 1);
}

QTextEdit *HelpDialog::newTextEdit(const QString &htmlFile) {
    auto textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setHtml(File::readAll(htmlFile));
    return textEdit;
}
