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
    //QString filePath2 = filePath1.mid(32,40);
    QString filePath2 = filePath1.mid(0,32);
    filePath2 = filePath2 + "Projekt-Poliglot/baza_slow/popularne.txt";
    qDebug() <<filePath2;
    get_Word_File(filePath2);
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
        if (word.length() <= nr_letters && word_Letters(word))
            qDebug() << word;
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
