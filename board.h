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
    int minX = 20, maxX = 0, minY = 20, maxY = 0; //będą użyte do przycięcia schematu o nieużywane pola
    //std::vector<QString> words = {"pobić", "opić", "bić", "obić", "poić", "pić"}, constWords;
    std::vector<QString> words = {"kwit", "kotwica", "kwiat", "taki", "wacik", "koci", "kita", "kwita"}, constWords;
    std::unordered_map<QChar, std::vector<std::pair<int, int>>> signMap;
    std::vector<QChar> letters;
    std::vector<std::pair<int, int>> allCoords;
    Scheme scheme;
    QString longestWord();
    void addCoord(QChar letter, int x, int y);
    bool checkNeighbours(int x, int y, int safeX, int safeY);
public:
    explicit Board(QObject *parent = nullptr);
    Scheme Scheme() const;

signals:
};

#endif // BOARD_H
