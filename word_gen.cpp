#include "word_gen.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
word_gen::word_gen(){}

void word_gen::showResult()
{
    draw_letters();
    showLetters();
    QString filePath1 = QDir::currentPath();

    std::reverse(filePath1.begin(), filePath1.end());
    int indToRem = filePath1.indexOf("/");
    filePath1 = filePath1.sliced(indToRem);
    std::reverse(filePath1.begin(), filePath1.end());
    filePath1 += "Projekt-Poliglot/baza_slow/popularne.txt";
    qDebug() <<filePath1;
    get_Word_File(filePath1);
}

std::vector<QString>& word_gen::getWords()
{
    return wordsDrawn;
}

void word_gen::showLetters()
{
    qDebug() << "Letters drawn: ";
    for (const QChar& letter : get_Letters) {
        qDebug() << letter;
    }
}

void word_gen::draw_letters()
{
    get_Letters.clear();

    nr_letters = QRandomGenerator::global()->bounded(6) + 5;

    for (int i = 0; i < nr_letters; ++i) {
        int kodAscii = QRandomGenerator::global()->bounded('a', 'z' + 1);
        get_Letters.append(QChar(kodAscii));
    }
}

void word_gen::get_Word_File(const QString& fileName)
{
    QFile plik(fileName);

    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "The file cannot be opened" << fileName;
            return;
    }

    QTextStream in(&plik);

    while (!in.atEnd()) {
        QString word = in.readLine();
        if (word.length() <= nr_letters && word_Letters(word)){
            qDebug() << word;
            wordsDrawn.push_back(word);
        }

    }
    plik.close();
}

bool word_gen::word_Letters(const QString &word)
{
    QList<QChar> copy_getLetters = get_Letters;
    for (const QChar& litera : word) {
        if (!copy_getLetters.contains(litera)) {
            return false;
        }
        copy_getLetters.removeOne(litera);
    }
    return true;
}
