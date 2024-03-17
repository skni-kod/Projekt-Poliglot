#ifndef WORD_GEN_H
#define WORD_GEN_H
#include <QVector>
#include <QRandomGenerator>

class word_gen
{
public:
    word_gen(int difficulty);
    void showResult();
    int nr_letters = 0;
    QVector<QChar> get_Letters;
    std::vector<QString>& getWords();
    int counter = 0;

private:
    int difficulty; //1 - ez, 2 - mid, 3 - hard
    QVector<QChar> pol_letters;
    QRandomGenerator generator;
    void showLetters();
    void draw_letters();
    void get_Word_File(const QString& fileName);
    bool word_Letters(const QString& word);
    std::vector<QString> wordsDrawn;
};

#endif // WORD_GEN_H
