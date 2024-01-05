#include "Word.h"

// Word bank
QStringList words;
// Wordle answers
QStringList answers;

const QString &Word::getAnswer() {
    return answers[QRandomGenerator::global()->bounded(answers.size())];
}

bool Word::isValid(const QString &word) {
    return words.contains(word);
}

void Word::load() {
    words = File::readLines("Words.txt");
    answers = File::readLines("Answers.txt");
}
