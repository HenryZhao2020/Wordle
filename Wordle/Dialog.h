#pragma once

#include "PCH.h"

class Game;

class Dialog : public QDialog {
    Q_OBJECT

public:
    Dialog(Game *game, const QIcon &icon, const QString &title);

protected:
    Game *game;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *okButton;

    void keyPressEvent(QKeyEvent *event);
};

class StatsDialog : public Dialog {
    Q_OBJECT

public:
    StatsDialog(Game *game);

private:
    QFormLayout *formLayout;

    void addRow(const QString &labelText, int value);
    void reset();
};

class SettingsDialog : public Dialog {
    Q_OBJECT

public:
    SettingsDialog(Game *game);

private:
    QHash<QCheckBox *, bool *> boxes;

    QCheckBox *newBox(const QString &text, bool *var);
    void apply();
};

class HelpDialog : public Dialog {
    Q_OBJECT

public:
    HelpDialog(Game *game);

private:
    QTabWidget *tabWidget;

    QTextEdit *newTextEdit(const QString &fileName);
};
