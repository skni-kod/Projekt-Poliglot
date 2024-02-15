#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChar>
#include "word_gen.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    word_gen word;
    word.showResult();
    Board board(nullptr, word.getWords());
    int count = board.UsedWordsCount();

    while(count < 4){
        word_gen word;
        word.showResult();
        board = Board(nullptr, word.getWords());
        count = board.UsedWordsCount();
    }


    /// TYMCZASOWE WYSWIETLANIE PLANSZY
    Scheme s = board.Scheme();
    for (const auto &row : s){
        QString temp;
        for (QChar sign : row){
            if(sign != '#'){
                temp += sign;
            }
            else temp += " ";
            temp += " ";
        }
        ui->textBrowser->append(temp);
    }

    ///////////////

}

MainWindow::~MainWindow()
{
    delete ui;
}
