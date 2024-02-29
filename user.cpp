#include "user.h"

user::user() {}

int user::getLevel() {
    return playerLevel;
}

int user::getPoints() {
    return playerPoints;
}

void user::setLevel(int value) {
    playerLevel = value;
}

void user::setPoints(int value) {
    playerPoints = value;
}

//We'll be mostly moving points up or down if we plan on making points the currency?
void user::updatePoints(char sig, int value) {
    if(sig == 'I') {
        playerPoints += value;
    } else if(sig == 'D') {
        playerPoints -= value;
    } else {
        qDebug() << "Invalid args in updatePoints";
    }
}

void user::levelComplete(int score, int remainingTime) {

    //Set in game settings possibly
    const double timeMultiplier = 1.5;

    //remainingTime in ms?
    playerPoints = playerPoints + score + (remainingTime * timeMultiplier);
    playerLevel++;
}

void user::loadUserData(const QString& fileName) {

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "The file cannot be opened to read " << fileName;
        return;
    }

    QTextStream in (&file);

    while (!in.atEnd()) {

        QString line = in.readLine();
        if(line.contains("Level=")) {
            QStringList list = line.split("=");
            if(list.size() > 1) {
                bool works;
                int temp = list[1].toInt(&works);
                if(!works) {
                    qDebug() << "File reading error " << fileName;
                    return;
                }
                playerLevel = temp;
            }
        } else if(line.contains("Points=")) {
            QStringList list = line.split("=");
            if(list.size() > 1) {
                bool notInt;
                int temp= list[1].toInt(&notInt);
                if(!notInt) {
                    qDebug() << "File reading error " << fileName;
                    return;
                }
                playerPoints = temp;
            }
        }
    }

    file.close();
}

void user::saveUserData(const QString& fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "The file cannot be opened to write " << fileName;
        return;
    }

    QTextStream out (&file);

    out << "Level=" << playerLevel << "\n";
    out << "Points=" << playerPoints << "\n";

    file.close();
}
