#include "Dict.h"
#include "FileUtil.h"

#include <QRandomGenerator>

QStringList Dict::words;
QStringList Dict::answers;

const QString &Dict::getAnswer() {
    return answers[QRandomGenerator::global()->bounded(answers.size())];
}

bool Dict::isValid(const QString &word) {
    return words.contains(word);
}

void Dict::load() {
    words = FileUtil::readLines(":/conf/Words.txt");
    answers = FileUtil::readLines(":/conf/Answers.txt");
}
