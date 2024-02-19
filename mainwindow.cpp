#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChar>
#include <QFile>
#include <QXmlStreamReader>
#include <QDir>
#include <QGraphicsPixmapItem>
#include "word_gen.h"
#include "gridcell.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setStyleSheet("background-color: #1f241e");
    ui->setupUi(this);
    setWindowTitle("Poliglot");
    ui->menu->resize(1920, 1080);
    ui->game->hide();

    QString buttonPreset = "QPushButton {border: 10px; border-radius: 60px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #253624, stop: 1 #679465);}";
    ui->playButton->setStyleSheet(buttonPreset);
    ui->optionsButton->setStyleSheet(buttonPreset);
    ui->closeButton->setStyleSheet(buttonPreset);

    QString filePath = QDir::currentPath();

    std::reverse(filePath.begin(), filePath.end());
    int indToRem = filePath.indexOf("/");
    filePath = filePath.sliced(indToRem);
    std::reverse(filePath.begin(), filePath.end());
    QString kodPath = filePath + "Projekt-Poliglot/sprites/skni_kod_logo_white.png";
    filePath += "Projekt-Poliglot/sprites/logo.png";

    QPixmap kodLogo(kodPath);
    QPixmap logo(filePath);
    logo = logo.scaled(ui->logo->width(), ui->logo->height(), Qt::KeepAspectRatio);
    kodLogo = kodLogo.scaled(ui->kod->width(), ui->kod->height(), Qt::KeepAspectRatio);
    ui->kod->setPixmap(kodLogo);
    ui->logo->setPixmap(logo);

    connect(ui->playButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->game->show();
        ui->game->resize(1920,1080);
        prepareGrid();
    });

    connect(ui->closeButton, &QPushButton::clicked, this, [](){
        QApplication::exit(0);
    });

    // word_gen word;
    // word.showResult();
    // Board board(nullptr, word.getWords());
    // int count = board.UsedWordsCount();


    // while(count < 4){
    //     word_gen word;
    //     word.showResult();
    //     board = Board(nullptr, word.getWords());
    //     count = board.UsedWordsCount();
    // }


    // /// TYMCZASOWE WYSWIETLANIE PLANSZY
    // Scheme s = board.Scheme();
    // for (const auto &row : s){
    //     QString temp;
    //     for (QChar sign : row){
    //         if(sign != '#'){
    //             temp += sign;
    //         }
    //         else temp += " ";
    //         temp += " ";
    //     }
    //     ui->textBrowser->append(temp);
    // }

    ///////////////

}

void MainWindow::prepareGrid(){
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

    Scheme s = board.Scheme();

    for (int i = 0; i < (int)s.size(); i++){
        for(int j = 0; j < (int)s[0].size() ; j++){
            if(s[i][j] != '#'){
                GridCell *cell = new GridCell(s[i][j], false);
                ui->gridLayout->addWidget(cell, i, j, Qt::AlignCenter);
            }
        }
    }

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
