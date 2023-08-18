#ifndef DIALOG_H
#define DIALOG_H

#include "PCH.h"

class Game;

class Dialog : public QDialog {
    Q_OBJECT

public:
    Dialog(Game *game, const QPixmap &pixmap, const QString &title);

    void close();

protected:
    Game *game;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *okButton;
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

#endif
