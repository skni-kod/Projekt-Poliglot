#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <vector>
#include <QString>

using Scheme = std::vector<std::vector<QChar>>;

class Board : public QObject
{
    Q_OBJECT
private:
    static int maxSize;
    static int maxTries;
    int minX = maxSize, maxX = 0, minY = maxSize, maxY = 0; //będą użyte do przycięcia schematu o nieużywane pola
    int usedWordsCount = 0;
    //std::vector<QString> words = {"pobić", "opić", "bić", "obić", "poić", "pić"}, constWords;
    //std::vector<QString> words = {"kwit", "kotwica", "kwiat", "taki", "wacik", "koci", "kita", "kwita", "ciotka", "kwota", "owak", "owca"}, constWords;
    std::vector<QString> words = {"port",
                                  "trop",
                                  "urok",
                                  "tor",
                                  "koc",
                                  "trup",
                                  "urlop",
                                  "pomruk",
                                  "por",
                                  "prom",
                                  "luk",
                                  "kult",
                                  "tok",
                                  "kur",
                                  "okup",
                                  "rum",
                                  "kocur"};
    std::unordered_map<QChar, std::vector<std::pair<int, int>>> signMap;
    QVector<QChar> letters;
    std::vector<std::pair<int, int>> allCoords, tips;
    Scheme scheme;
    QString longestWord();
    void addCoord(QChar letter, int x, int y);
    bool checkNeighbours(int x, int y, int safeX, int safeY);
    static bool compareLength(const QString &str1, const QString &str2);
public:
    explicit Board(QObject *parent = nullptr, std::vector<QString> wordsList = {}, QVector<QChar> letterSet = {});
    Scheme Scheme() const;
    int UsedWordsCount() const;
    int Width() const;
    int Height() const;
    QVector<QChar>& Letters();
    void operator=(const Board& obj);

signals:
};

#endif // BOARD_H
