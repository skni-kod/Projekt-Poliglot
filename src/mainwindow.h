#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "board.h"
#include "qgridlayout.h"
#include "qpushbutton.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_revealLetterButton_clicked();

private:
    Ui::MainWindow *ui;
    int width = 1920, height = 1080;
    int screenWidth = 1920, screenHeight = 1080;
    QString projectPath;
    Board board;
    bool isFirstBoard;
    user player;
    double gridCellSize = 0;
    QGridLayout* gridLayout = nullptr, *lettersGridLayout = nullptr;
    std::vector<QPushButton*> letButtons;
    QString userDataPath;
    int difficulty;
    void prepareGrid(int diff);
    void prepareLetterButtons();
    void setupButtons();
    void updateGrid(QString &newWord);
    void startNewBoard(int diff);
    void setupCheckButton();
    QElapsedTimer elapsedTime;
    void updateCornerLabel();
};
#endif // MAINWINDOW_H
