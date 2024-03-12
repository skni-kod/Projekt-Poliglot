#include "word_gen.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

word_gen::word_gen(int difficulty)
{
    this->difficulty = difficulty;
}

void word_gen::showResult()
{
    draw_letters();
    showLetters();
    // QString filePath1 = QDir::currentPath();
    // qDebug() << filePath1;

    // std::reverse(filePath1.begin(), filePath1.end());
    // int indToRem = filePath1.indexOf("/");
    // filePath1 = filePath1.sliced(indToRem);
    // std::reverse(filePath1.begin(), filePath1.end());
    // filePath1 += "Projekt-Poliglot/baza_slow/popularne.txt";
    QString filePath1 = "://baza_slow/popularne.txt";
    qDebug() <<filePath1;
    counter =0;
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

    //easy - 5 to 6 letters
    //medium - 7 to 8 letters
    //hard - 9 to 10 letters
    switch(difficulty){
    case 1:
        nr_letters = QRandomGenerator::global()->bounded(5,7);
        break;
    case 2:
        nr_letters = QRandomGenerator::global()->bounded(7,9);
        break;
    case 3:
        nr_letters = QRandomGenerator::global()->bounded(9,11);
        break;
    }

    //nr_letters = QRandomGenerator::global()->bounded(6) + 5;

    for (int i = 0; i < nr_letters; ++i) {
        int randomNr = QRandomGenerator::global()->bounded(5);
        if(randomNr==0){
            int indexspecialLetter = QRandomGenerator::global()->bounded(9);
            QChar specialLetters[] = { QChar(261), QChar(263), QChar(281), QChar(322), QChar(324),
                                    QChar(243), QChar(347), QChar(378), QChar(280) };
            get_Letters.append(specialLetters[indexspecialLetter]);
        }
        else{
            QChar letter;
        //int kodAscii = QRandomGenerator::global()->bounded('a', 'z' + 1);
        //get_Letters.append(QChar(kodAscii));
            do{
            letter = QChar('a'+QRandomGenerator::global()->bounded(26));
            }
            while (letter == 'q' || letter == 'v' || letter == 'x');
            get_Letters.append(letter);
        }
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
            counter++;
        }

    }
    plik.close();

    int minWordCount = 0;
    //minimum required words amount:
    // easy - 5 words
    // medium - 6 words
    // hard - 8 words
    switch(difficulty){
    case 1:
        minWordCount = 5;
        break;
    case 2:
        minWordCount = 6;
        break;
    case 3:
        minWordCount = 10;
        break;
    default:
        qDebug() << "Wrong difficulty!";
        minWordCount = 6;
    }

    if(counter<minWordCount){
        qDebug() << "Not enough words. Draws again...";
        wordsDrawn.clear();
        draw_letters();
        showLetters();
        get_Word_File(fileName);
    }

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
