#include "mainwindow.h"
#include "qgridlayout.h"
#include "ui_mainwindow.h"
#include <QChar>
#include <QFile>
#include <QXmlStreamReader>
#include <QDir>
#include <QGraphicsPixmapItem>
#include "word_gen.h"
#include "gridcell.h"
#include "user.h"

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
    QString userFilePath = filePath + "Projekt-Poliglot/user_data/user_stats.txt"; //USER FILE PATH
    filePath += "Projekt-Poliglot/sprites/logo.png";

    QPixmap kodLogo(kodPath);
    QPixmap logo(filePath);
    logo = logo.scaled(ui->logo->width(), ui->logo->height(), Qt::KeepAspectRatio);
    kodLogo = kodLogo.scaled(ui->kod->width(), ui->kod->height(), Qt::KeepAspectRatio);
    ui->kod->setPixmap(kodLogo);
    ui->logo->setPixmap(logo);

    //USER STATS MANIPULATIONS
    user player;
    player.loadUserData(userFilePath);
    //player.updatePoints('I',9);
    //player.setLevel(2);
    player.saveUserData(userFilePath);

    //QString lev = QString::number(player.getLevel());
    //QString poi = QString::number(player.getPoints());

    connect(ui->playButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->game->show();
        ui->game->resize(1920,1080);
        prepareGrid();
        prepareLetterButtons();
    });

    connect(ui->closeButton, &QPushButton::clicked, this, [](){
        QApplication::exit(0);
    });

}

void MainWindow::prepareGrid(){
    word_gen word;
    word.showResult();
    board = Board(nullptr, word.getWords(), word.get_Letters);
    int count = board.UsedWordsCount();


    while(count < 4){
        word_gen word;
        word.showResult();
        board = Board(nullptr, word.getWords(), word.get_Letters);
        count = board.UsedWordsCount();
    }

    Scheme s = board.Scheme();

    double size = 0;
    double frameSize = 0;

    ui->gridFrame->setFrameShape(QFrame::Box);

    if(board.Width() > board.Height()){
        size = (double)ui->gridFrame->height() / (double)board.Width() * 0.8;
        frameSize = (double)ui->gridFrame->height() * (double)((double)board.Height()/(double)board.Width());
        ui->gridFrame->setFixedHeight(frameSize);
    }
    else{
        size = (double)ui->gridFrame->height() / (double)board.Height() * 0.8;
        frameSize = (double)ui->gridFrame->width() * (double)((double)board.Width()/(double)board.Height());
        ui->gridFrame->setFixedWidth(frameSize);
    }

    QGridLayout *layout = new QGridLayout(ui->gridFrame);

    for (int i = 0; i < (int)s.size(); i++){
        for(int j = 0; j < (int)s[0].size() ; j++){
            if(s[i][j] != '#'){
                GridCell *cell = new GridCell(s[i][j].toUpper(), false, size);
                layout->addWidget(cell, i, j, Qt::AlignCenter);
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

void MainWindow::prepareLetterButtons(){
    QVector<QChar> let = board.Letters();
    qDebug() << let;
    int frameMult = let.size() / 4 + 1;
    qDebug() << frameMult;
    if(frameMult >= 1){
        ui->lettersFrame->setFixedHeight(ui->lettersFrame->height() * frameMult);
    }



    connect(ui->clearButton, &QPushButton::clicked, this, [this](){
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }
    });

    connect(ui->checkButton, &QPushButton::clicked, this, [this](){
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }
    });

    QGridLayout *layout = new QGridLayout(ui->lettersFrame);

    int letterIndex = 0;
    for (int i = 0; i < frameMult; i++){
        for(int j = 0; j < 4; j++){
            QChar character = let[letterIndex].toUpper();
            QPushButton *button = new QPushButton(character);
            letButtons.push_back(button);


            button->setStyleSheet(
                      "QPushButton {"
                          "border: 2px solid #8f8f91;"
                          "border-radius: 15px;"
                          "background-color: #263023;"
                          "min-width: 60px;"
                          "min-height: 60px;"
                          "font: 20px;"
                      "}"
                      ""
                      "QPushButton:pressed {"
                          "background-color: #495c43;"
                      "}"
                      );

            button->setCursor(Qt::PointingHandCursor);

            connect(button, &QPushButton::clicked, this, [this, character, button](){
                ui->textEdit->setText(ui->textEdit->toPlainText() + character);
                button->hide();
            });

            QLabel *emptyWidget = new QLabel();
            emptyWidget->setFixedSize(60,60);

            layout->addWidget(emptyWidget, i, j, Qt::AlignCenter);
            layout->addWidget(button, i, j, Qt::AlignCenter);

            if(letterIndex + 1 == let.size()){
                break;
            }
            else{
                letterIndex++;
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
