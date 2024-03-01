#ifndef USER_H
#define USER_H

#include <QString>
#include <QFile>
#include <QDebug>

class user
{
private:
    int playerLevel;
    int playerPoints;
public:

    user();

    int getLevel();
    int getPoints();

    void setLevel(int value);
    void setPoints(int value);

    void updatePoints(char sig,int value);
    void levelComplete(int score, int remainingTime);

    void loadUserData(const QString& fileName);
    void saveUserData(const QString& fileName);
};

#endif // USER_H
