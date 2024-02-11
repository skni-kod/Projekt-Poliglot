#include "board.h"
#include "qdebug.h"
#include <random>
#include <algorithm>

int Board::maxSize = 20;

// zwraca najdłuższe słowo i usuwa je
QString Board::longestWord()
{
    QString toReturn = words.back();
    words.pop_back();
    return toReturn;
}

bool Board::compareLength(const QString& str1, const QString& str2){
    return str1.length() < str2.length();
}

void Board::addCoord(QChar letter, int x, int y){
    signMap[letter].push_back({x,y});
    allCoords.push_back({x, y});
}

//sprawdzi czy nie skoliduje z innym slowem albo czy nie wyjdzie poza obszar
bool Board::checkNeighbours(int x, int y, int safeX, int safeY){
    if (scheme[y][x] == '#' &&
        (scheme[y+1][x] == '#' || (y+1 == safeY && x == safeX)) &&
        (scheme[y-1][x] == '#' || (y-1 == safeY && x == safeX)) &&
        (scheme[y][x+1] == '#' || (y == safeY && x+1 == safeX)) &&
        (scheme[y][x-1] == '#' || (y == safeY && x-1 == safeX)) &&
        x+1 < maxSize && x > 0 && y+1 < maxSize && y > 0 &&
        std::find(tips.begin(), tips.end(), (std::pair<int,int>){x,y}) == tips.end()) return true;
    else return false;
}

Board::Board(QObject *parent)
    : QObject{parent}
{
    std::sort(words.begin(), words.end(), compareLength);
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

    //wstaw go zaczynając w y,x
    int y = maxSize/2 ;
    int x = maxSize/2 - lWord.length()/2;

    tips.push_back({x-1, y}); //wstaw kratkę przed właściwym słowem
    for (int i = 0; i < lWord.length(); i++){
        scheme[y][x] = lWord.at(i);
        addCoord(lWord.at(i), x, y);
        x++;

        if(minX > x) minX = x;
        if(minY > y) minY = y;
        if(maxX < x) maxX = x;
        if(maxY < y) maxY = y;
    }
    tips.push_back({x, y}); //wstaw kratkę po właściwym słowie

    for(auto& word : words){
        qDebug() << word;
    }

    bool vertical = true;

    int unsuccessfulTries = 0;

    while(words.size() > 0){

        QString lWord = longestWord();
        qDebug() << "now trying word: " << lWord;

        int crossLetterIndex = rand() % (int)lWord.length();
        QChar crossLetter = lWord.at(crossLetterIndex);
        qDebug() << "Chosen crossLetter:  " << crossLetter;

        int index = rand() % signMap[crossLetter].size();
        int x = signMap[crossLetter].at(index).first;
        int y = signMap[crossLetter].at(index).second;

        //
        //first - to co przed skrzyzowaniem, second - to co po
        //

        QString first = lWord.sliced(0, crossLetterIndex);
        QString second = lWord.sliced(crossLetterIndex+1);

        if(vertical){
            bool ok = true;

            //sprawdź czy mozna dac pierwsza polowe slowa
            for(int i = 1; i <= first.length(); i++){

                int yy = y - i;

                if(!checkNeighbours(x, yy, x, y)){
                    ok = false;
                }
            }

            //sprawdź czy można dać drugą połowę słowa
            for(int i = 1; i <= second.length(); i++){

                int yy = y + i;

                if(!checkNeighbours(x, yy, x, y)){
                    ok = false;
                }
            }

            //jak nie można dać słowa bo koliduje to wróć słowo do words
            if(!ok) {

                if(unsuccessfulTries < 200){
                    words.push_back(lWord);
                    //jak się nie udało to spróbuj inną orientacją
                    unsuccessfulTries++;

                    if(vertical)
                        vertical = false;
                    else vertical = true;

                    continue;
                }
                else{
                    //poddaj się i wywal słowo
                    qDebug() << "throwing away word: " << lWord;
                    continue;
                }

            }

            unsuccessfulTries = 0;

            for(int i = 1; i <= first.length(); i++){

                int yy = y - i;
                qDebug() << "Now inserting letter: " << first.at(first.length() - 1 - (i-1)) << " from word: " << lWord;
                scheme[yy][x] = first.at(first.length() - 1 - (i-1));

                addCoord(first.at(first.length() - 1 - (i-1)), x, yy);
                //if(minX > x) minX = x;
                if(minY > yy) minY = yy;
                //if(maxX < x) maxX = x;
                //if(maxY < yy) maxY = yy;

                if(i == first.length()) //w ostatniej iteracji dodaj czubek
                    tips.push_back({x, yy-1});
            }
            for(int i = 1; i <= second.length(); i++){

                int yy = y + i;
                qDebug() << "Now inserting letter: " << second.at(i-1) << " from word: " << lWord;
                scheme[yy][x] = second.at(i-1);

                addCoord(second.at(i-1), x, yy);
                if(maxY < yy) maxY = yy;
                if(i == second.length())
                    tips.push_back({x, yy+1});
            }
        }

        //    GDY POZIOMO
        else{
            bool ok = true;

            for(int i = 1; i <= first.length(); i++){

                int xx = x - i;

                if(!checkNeighbours(xx, y, x, y)){
                    ok = false;
                }
            }

            for(int i = 1; i <= second.length(); i++){

                int xx = x + i;

                if(!checkNeighbours(xx, y, x, y)){
                    ok = false;
                }
            }

            if(!ok) {
                if(unsuccessfulTries < 200){
                    words.push_back(lWord);
                    //jak się nie udało to spróbuj inną orientacją
                    unsuccessfulTries++;

                    if(vertical)
                        vertical = false;
                    else vertical = true;

                    continue;
                }
                else{
                    unsuccessfulTries = 0;
                    //poddaj się i wywal słowo
                    qDebug() << "throwing away word: " << lWord;
                    continue;
                }
            }

            unsuccessfulTries = 0;
            for(int i = 1; i <= first.length(); i++){

                int xx = x - i;
                qDebug() << "Now inserting letter: " << first.at(first.length() - 1 - (i-1)) << " from word: " << lWord;
                scheme[y][xx] = first.at(first.length() - 1 - (i-1));
                addCoord(first.at(first.length() - 1 - (i-1)), xx, y);
                if(minX > xx) minX = xx;
                if(i == first.length())
                    tips.push_back({xx-1, y});
            }
            for(int i = 1; i <= second.length(); i++){

                int xx = x + i;
                qDebug() << "Now inserting letter: " << second.at(i-1) << " from word: " << lWord;
                scheme[y][xx] = second.at(i-1);
                addCoord(second.at(i-1), xx, y);
                if(maxX < x) maxX = x;
                if(i == second.length())
                    tips.push_back({xx+1, y});
            }
        }

        if(vertical)
            vertical = false;
        else vertical = true;
    }
}

Scheme Board::Scheme() const
{
    return scheme;
}
