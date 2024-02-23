#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board.h"
#include "qpushbutton.h"

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
    std::vector<QPushButton*> letButtons;
    void prepareGrid();
    void prepareLetterButtons();
};
#endif // MAINWINDOW_H
