#include "Dict.h"

QList<QString> words, answers;

void Dict::load() {
    words = File::readLines("Words.txt");
    answers = File::readLines("Answers.txt");
}

bool Dict::isExist(const QString &word) {
    return words.contains(word);
}

QString Dict::generate() {
    return answers[QRandomGenerator::global()->bounded(answers.size())];
}
