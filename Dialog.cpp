#include "Dialog.h"
#include "Game.h"
#include "GameBar.h"
#include "Board.h"
#include "Keyboard.h"
#include "Attr.h"
#include "FileUtil.h"
#include "IconUtil.h"

#include <QFrame>
#include <QGroupBox>
#include <QProgressBar>
#include <QFile>
#include <QProcess>

Dialog::Dialog(Game *game, const QIcon &icon, const QString &title)
    : QDialog(game), game(game) {
    setWindowIcon(icon);
    setWindowTitle(title);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    auto buttonFrame = new QFrame(this);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);

    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    okButton = new QPushButton(tr("OK"), this);
    connect(okButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(okButton);
}

Dialog::~Dialog() {}

void Dialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        deleteLater();
    }

    QDialog::keyPressEvent(event);
}

SettingsDialog::SettingsDialog(Game *game)
    : Dialog(game, IconUtil::load(":/icons/Settings.svg"), tr("Settings")) {
    auto gameGroup = newGroup(tr("Game"));
    addCheckBox(gameGroup, tr("Hard Mode"), Attr::get().hard);
    addCheckBox(gameGroup, tr("Color Blind Mode"), Attr::get().colorBlind);
    addCheckBox(gameGroup, tr("Enable Visual Effects"), Attr::get().animated);

    langBox = new QComboBox(this);
    langBox->addItems(Lang::getLangNames());
    langBox->setCurrentIndex(Attr::get().lang);

    auto langGroup = newGroup(tr("Language"));
    langGroup->addWidget(langBox);

    resetBox = new QComboBox(this);
    resetBox->addItem(tr("Don't Reset"));
    resetBox->addItem(tr("Reset Everything"));
    resetBox->addItem(tr("Reset Settings"));
    resetBox->addItem(tr("Reset Statistics"));
    buttonLayout->insertWidget(0, resetBox);
    buttonLayout->insertStretch(1);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::applySettings);

    auto cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(cancelButton);

    setFixedSize(sizeHint());
}

SettingsDialog::~SettingsDialog() {}

QVBoxLayout *SettingsDialog::newGroup(const QString &title) {
    auto group = new QGroupBox(title, this);
    mainLayout->insertWidget(mainLayout->count() - 2, group);

    auto groupLayout = new QVBoxLayout(group);
    groupLayout->setSpacing(10);
    groupLayout->setContentsMargins(20, 20, 20, 20);
    return groupLayout;
}

void SettingsDialog::addCheckBox(QLayout *layout, const QString &text, bool &state) {
    auto box = new QCheckBox(text, this);
    box->setChecked(state);
    boxes.insert(box, &state);
    layout->addWidget(box);
}

void SettingsDialog::applySettings() {
    for (auto it = boxes.begin(); it != boxes.end(); ++it) {
        *it.value() = it.key()->isChecked();
    }

    int resetIndex = resetBox->currentIndex();
    if (resetIndex == 1) {
        qApp->quit();
        QFile::remove("Wordle_Data");
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    } else if (resetIndex == 2) {
        Attr::get().resetSettings();
    } else if (resetIndex == 3) {
        Attr::get().resetStats();
    }

    game->getBoard()->updateColors();
    game->getKeyboard()->updateColors();

    int oldLang = Attr::get().lang;
    int newLang = langBox->currentIndex();
    Attr::get().lang = Lang::Name(newLang);

    if (newLang != oldLang) {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

StatsDialog::StatsDialog(Game *game)
    : Dialog(game, IconUtil::load(":/icons/Stats.svg"), tr("Statistics")) {
    auto gridFrame = new QFrame(this);
    mainLayout->insertWidget(0, gridFrame);

    gridLayout = new QGridLayout(gridFrame);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    addRow(tr("Total Played"), Attr::get().totalPlays);
    addRow(tr("Total Won"), Attr::get().totalWins);
    addRow(tr("Percentage Won"), Attr::get().percentWon());
    addRow(tr("Best Try"), Attr::get().minTries);
    addRow(tr("Current Streak"), Attr::get().streak);
    addRow(tr("Max Streak"), Attr::get().maxStreak);

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    gridLayout->addWidget(separator, gridLayout->rowCount(), 0, 1, 2);

    auto titleLabel = new QLabel(tr("Tries Distribution Table"), this);
    gridLayout->addWidget(titleLabel, gridLayout->rowCount(), 0, 1, 2);

    for (int i = 1; i <= 6; ++i) {
        addDist(i);
    }

    setFixedSize(sizeHint());
}

StatsDialog::~StatsDialog() {}

void StatsDialog::addRow(const QString &name, int value) {
    int row = gridLayout->rowCount();

    auto nameLabel = new QLabel(name + ":", this);
    gridLayout->addWidget(nameLabel, row, 0, Qt::AlignLeft);

    auto valLabel = new QLabel(QString::number(value), this);
    gridLayout->addWidget(valLabel, row, 1);
}

void StatsDialog::addDist(int i) {
    int row = gridLayout->rowCount();
    int totalWins = Attr::get().totalWins;
    const QHash<int, int> &dist = Attr::get().triesDist;

    auto bar = new QProgressBar(this);
    bar->setFixedWidth(320);
    bar->setValue((totalWins > 0) ? qRound(100.0 * dist[i] / totalWins) : 0);
    gridLayout->addWidget(bar, row, 0);

    auto label = new QLabel(QString("#%0").arg(i), this);
    gridLayout->addWidget(label, row, 1);
}

HelpDialog::HelpDialog(Game *game)
    : Dialog(game, IconUtil::load(":/icons/Help.svg"), tr("Help")) {
    auto tabWidget = new QTabWidget(this);
    tabWidget->addTab(newBrowser(":/help/" + tr("Rules.html")), tr("Rules"));
    tabWidget->addTab(newBrowser(":/help/" + tr("About.html")), tr("About"));
    mainLayout->insertWidget(0, tabWidget);

    setFixedSize(sizeHint());
}

HelpDialog::~HelpDialog() {}

QTextBrowser *HelpDialog::newBrowser(const QString &path) {
    auto browser = new QTextBrowser(this);
    browser->setHtml(FileUtil::readAll(path));
    browser->setOpenExternalLinks(true);
    return browser;
}
