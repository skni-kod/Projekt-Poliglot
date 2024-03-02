#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    int width = 1920, height = 1080;
    QString projectPath;
    Board board;
    user player;
    double gridCellSize = 0;
    QGridLayout* gridLayout = nullptr;
    std::vector<QPushButton*> letButtons;
    QString userDataPath;
    void prepareGrid();
    void prepareLetterButtons();
    void setupButtons();
    void updateGrid(QString &newWord);
};
#endif // MAINWINDOW_H
