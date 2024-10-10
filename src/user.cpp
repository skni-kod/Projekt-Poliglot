#include "user.h"
#include<windows.h>
#include<QStandardPaths>

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

void user::loadUserData() {

    QFile file(filePath);

    if(!file.exists()){
        playerLevel = 1;
        playerPoints = 0;
        file.close();
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "The file cannot be opened to read " << filePath;
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
                    qDebug() << "File reading error " << filePath;
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
                    qDebug() << "File reading error " << filePath;
                    return;
                }
                playerPoints = temp;
            }
        }
    }

    file.close();
}

void user::saveUserData() {
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "The file cannot be opened to write " << filePath;
        return;
    }

    QTextStream out (&file);

    out << "Level=" << playerLevel << "\n";
    out << "Points=" << playerPoints << "\n";

    file.close();
}

void user::filePathInit()
{
    filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    //windows specific folder creating function
    if (CreateDirectory(filePath.toStdWString().c_str(), NULL)){
        qDebug() << "Created a folder";
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        qDebug() << "Folder already exists";
    }
    else{
        qDebug() << "Failed to create a folder for some reason";
    }
    filePath += "/user_stats.txt";

}
