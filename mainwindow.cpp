#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChar>
#include "word_gen.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);


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
    word_gen Word;
    Word.showResult();
}

MainWindow::~MainWindow()
{
    delete ui;
}
