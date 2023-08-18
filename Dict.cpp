#include "Dict.h"

QList<QString> Dict::words = {};
QList<QString> Dict::answers = {};

void Dict::load() {
    words = File::readLines("Words.txt");
    answers = File::readLines("Answers.txt");
}

bool Dict::isExist(const QString &word) {
    return words.contains(word);
}

QString Dict::generateAnswer() {
    return answers[QRandomGenerator::global()->bounded(answers.size())];
}
