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
    int realX, realY;
    std::vector<QString> words = {"pobić", "opić", "bić", "obić", "poić", "pić"}, constWords;
    Scheme scheme;
    QString longestWord();
public:
    explicit Board(QObject *parent = nullptr);
    Scheme Scheme() const;

signals:
};

#endif // BOARD_H
