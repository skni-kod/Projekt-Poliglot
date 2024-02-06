#include "board.h"
#include "qdebug.h"

int Board::maxSize = 20;

// zwraca najdłuższe słowo i usuwa je
QString Board::longestWord()
{
    int longestLength = 0, longestIndex = 0, i = 0;
    for (auto& word : words){
        if(longestLength < word.length()){
            longestLength = word.length();
            longestIndex = i;
        }
        i++;
    }
    QString toReturn = words[longestIndex];
    auto it = std::next(words.begin(), longestIndex);
    words.erase(it);
    return toReturn;
}

Board::Board(QObject *parent)
    : QObject{parent}
{
    constWords = words;

    for (int i = 0; i < maxSize; i++){
        std::vector<QChar> row;
        for (int j = 0; j < maxSize; j++){
            row.push_back('#');

        }
        scheme.push_back(row);
        row.clear();
    }

    //wyznacz najdłuższy element
    QString lWord = longestWord();

    int y = maxSize/2 ;
    int x = maxSize/2 - lWord.length()/2;

    for (int i = 0; i < lWord.length(); i++){
        scheme[y][x] = lWord.at(i);
        x++;
    }

    for(auto& word : words){
        qDebug() << word;
    }
}

Scheme Board::Scheme() const
{
    return scheme;
}

