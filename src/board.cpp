#include "board.h"
#include "qdebug.h"
#include <random>
#include <algorithm>

int Board::maxSize = 20;

int Board::maxTries = 150;

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

void Board::addCoord(QString word, QChar letter, int x, int y){
    signMap[letter].push_back({x,y});
    allCoords.push_back({x, y});
    wordMap[word].push_back({x,y});
}

//sprawdzi czy nie skoliduje z innym slowem albo czy nie wyjdzie poza obszar
bool Board::checkNeighbours(int x, int y, int safeX, int safeY){
    if (x+1 < maxSize-1 && x > 0 && y+1 < maxSize-1 && y > 0 &&
        scheme[y][x] == '#' &&
        (scheme[y+1][x] == '#' || (y+1 == safeY && x == safeX)) &&
        (scheme[y-1][x] == '#' || (y-1 == safeY && x == safeX)) &&
        (scheme[y][x+1] == '#' || (y == safeY && x+1 == safeX)) &&
        (scheme[y][x-1] == '#' || (y == safeY && x-1 == safeX)) &&
        std::find(tips.begin(), tips.end(), (std::pair<int,int>){x,y}) == tips.end()) return true;
    else return false;
}

Board::Board(QObject *parent, std::vector<QString> wordsList, QVector<QChar> letterSet)
    : QObject{parent}
{
    letters = letterSet;

    //placeholder inpossible values
    for(auto letter : letters){
        revealedSignMap[letter].push_back({-99, -99});
        signMap[letter].push_back({-100, -100});
    }

    usedWordsCount = 0;
    guessedWordsCount = 0;
    alreadyGuessedWords.clear();
    presentWords.clear();

    if(wordsList.size() < 3)
        return;
    words = wordsList;
    std::sort(words.begin(), words.end(), compareLength);

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
    qDebug() << "First word: " << lWord;

    //wstaw go zaczynając w y,x
    int y = maxSize/2 ;
    int x = maxSize/2 - lWord.length()/2;

    tips.push_back({x-1, y}); //wstaw kratkę przed właściwym słowem
    minX = x;
    for (int i = 0; i < lWord.length(); i++){
        scheme[y][x] = lWord.at(i);
        addCoord(lWord, lWord.at(i), x, y);
        maxX = x;
        x++;
    }
    minY = maxY = y;

    presentWords.push_back(lWord);

    tips.push_back({x, y}); //wstaw kratkę po właściwym słowie
    usedWordsCount++;

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

        if(unsuccessfulTries >= maxTries){
            removedWords.push_back(lWord);
            qDebug() << "throwing away word: " << lWord;
            unsuccessfulTries = 0;
            continue;
        }

        if(signMap[crossLetter].size() == 0){
            unsuccessfulTries++;
            words.push_back(lWord);
            continue;
        }


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

            //check if crossLetter has been already used
            if(std::find(tips.begin(), tips.end(), (std::pair<int, int>){x,y}) != tips.end()){
                ok = false;
            }

            //jak nie można dać słowa bo koliduje to wróć słowo do words
            if(!ok) {

                if(unsuccessfulTries < maxTries){
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
                    removedWords.push_back(lWord);
                    qDebug() << "throwing away word: " << lWord;
                    unsuccessfulTries = 0;
                    continue;
                }
            }

            presentWords.push_back(lWord);
            tips.push_back({x,y});
            usedWordsCount++;

            unsuccessfulTries = 0;

            for(int i = 1; i <= first.length(); i++){

                int yy = y - i;
                qDebug() << "Now inserting letter: " << first.at(first.length() - 1 - (i-1)) << " from word: " << lWord;
                scheme[yy][x] = first.at(first.length() - 1 - (i-1));

                addCoord(lWord, first.at(first.length() - 1 - (i-1)), x, yy);

                if(minY > yy) minY = yy;
                if(maxY < yy) maxY = yy;

                if(i == first.length()) //w ostatniej iteracji dodaj czubek
                    tips.push_back({x, yy-1});
            }

            addCoord(lWord, crossLetter, x, y);

            for(int i = 1; i <= second.length(); i++){

                int yy = y + i;
                qDebug() << "Now inserting letter: " << second.at(i-1) << " from word: " << lWord;
                scheme[yy][x] = second.at(i-1);

                addCoord(lWord, second.at(i-1), x, yy);
                if(maxY < yy) maxY = yy;
                if(minY > yy) minY = yy;
                if(i == second.length())
                    tips.push_back({x, yy+1});
            }
            signMap[crossLetter].erase(signMap[crossLetter].begin() + index);
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

            //check if crossLetter has been already used
            if(std::find(tips.begin(), tips.end(), (std::pair<int, int>){x,y}) != tips.end()){
                ok = false;
            }

            if(!ok) {
                if(unsuccessfulTries < maxTries){
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
                    removedWords.push_back(lWord);
                    //poddaj się i wywal słowo
                    qDebug() << "throwing away word: " << lWord;
                    continue;
                }
            }

            presentWords.push_back(lWord);
            usedWordsCount++;
            tips.push_back({x,y});
            unsuccessfulTries = 0;

            for(int i = 1; i <= first.length(); i++){

                int xx = x - i;
                qDebug() << "Now inserting letter: " << first.at(first.length() - 1 - (i-1)) << " from word: " << lWord;
                scheme[y][xx] = first.at(first.length() - 1 - (i-1));
                addCoord(lWord, first.at(first.length() - 1 - (i-1)), xx, y);
                if(minX > xx) minX = xx;
                if(maxX < xx) maxX = xx;
                if(i == first.length())
                    tips.push_back({xx-1, y});
            }

            addCoord(lWord, crossLetter, x, y);

            for(int i = 1; i <= second.length(); i++){

                int xx = x + i;
                qDebug() << "Now inserting letter: " << second.at(i-1) << " from word: " << lWord;
                scheme[y][xx] = second.at(i-1);
                addCoord(lWord, second.at(i-1), xx, y);
                if(maxX < xx) maxX = xx;
                if(minX > xx) minX = xx;
                if(i == second.length())
                    tips.push_back({xx+1, y});
            }
            signMap[crossLetter].erase(signMap[crossLetter].begin() + index);
        }

        if(vertical)
            vertical = false;
        else vertical = true;
    }


    qDebug() << "min X: " << minX
        << "max X: " << maxX
           << "min Y: " << minY
             << "max Y: " << maxY;
}

Scheme Board::Scheme() const
{
    return scheme;
}

int Board::UsedWordsCount() const
{
    return usedWordsCount;
}

int Board::Width() const
{
    return maxX - minX + 1;
}

int Board::Height() const
{
    return maxY - minY + 1;
}

std::vector<QString> &Board::PresentWords()
{
    return presentWords;
}

std::vector<QString> &Board::RemovedWords()
{
    return removedWords;
}

std::unordered_set<QString> &Board::AlreadyGuessedWords()
{
    return alreadyGuessedWords;
}

void Board::addGuessedWord(QString &guessedWord)
{
    alreadyGuessedWords.insert(guessedWord);
}

QVector<QChar>& Board::Letters()
{
    return letters;
}

std::unordered_map<QString, std::vector<std::pair<int, int>>>& Board::WordMap()
{
    return wordMap;
}

//return 0 if words is not present
//return 1 if word is present on a board
//return 2 if word like that exists but it is not on the board
int Board::checkWord(QString &wordToCheck)
{
    if(std::find(presentWords.begin(), presentWords.end(), wordToCheck) != presentWords.end()){
        return 1;
    }
    if(std::find(removedWords.begin(), removedWords.end(), wordToCheck) != removedWords.end()){
        return 2;
    }
    return 0;
}

void Board::operator=(const Board &obj)
{
    if(this != &obj){
        minX = obj.minX;
        maxX = obj.maxX;
        minY = obj.minY;
        maxY = obj.maxY;
        usedWordsCount = obj.usedWordsCount;
        words = obj.words;
        presentWords = obj.presentWords;
        removedWords = obj.removedWords;
        signMap = obj.signMap;
        wordMap = obj.wordMap;
        letters = obj.letters;
        allCoords = obj.allCoords;
        tips = obj.tips;
        scheme = obj.scheme;
        guessedWordsCount = obj.guessedWordsCount;
        alreadyGuessedWords = obj.alreadyGuessedWords;
        revealedSignMap = obj.revealedSignMap;
    }
}

void Board::addGuessedWordCount()
{
    guessedWordsCount++;
}

int Board::getGuessedWordCount() const
{
    return guessedWordsCount;
}

void Board::removeFromSignMap(QChar letter, int x, int y)
{
    // Znajdź znak w mapie
    auto it = signMap.find(letter);
    if (it != signMap.end()) {
        // Znajdź parę koordynatów w wektorze
        auto& coords = it->second;
        auto coordIt = std::find_if(coords.begin(), coords.end(), [x, y](const std::pair<int, int>& p) {
            return p.first == x && p.second == y;
        });

        // Jeśli znaleziono parę koordynatów, usuń ją
        if (coordIt != coords.end()) {
            coords.erase(coordIt);
        }
    }
}

