#include "mainwindow.h"
#include "qgridlayout.h"
#include "qscreen.h"
#include "ui_mainwindow.h"
#include <QChar>
#include <QFile>
#include <QXmlStreamReader>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QShortcut>
#include "word_gen.h"
#include "gridcell.h"
#include "user.h"

//<a target="_blank" href="https://icons8.com/icon/KLD9V6A735yg/done">Check</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QScreen *screen = QApplication::primaryScreen();

    //gets screen resolution
    if(screen){
        QRect screenGeometry = screen->geometry();
        width = screenGeometry.width();
        height = screenGeometry.height();
    }

    setStyleSheet("background-color: #1f241e");
    ui->setupUi(this);
    setWindowTitle("Poliglot");
    ui->menu->resize(width, height);
    ui->game->hide();

    QString buttonPreset = "QPushButton {border: 10px; border-radius: 60px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #253624, stop: 1 #679465);}";
    ui->playButton->setStyleSheet(buttonPreset);
    ui->optionsButton->setStyleSheet(buttonPreset);
    ui->closeButton->setStyleSheet(buttonPreset);

    ui->verticalLayoutWidget->setFixedSize(width, height);

    //set project path
    projectPath = QDir::currentPath();

    std::reverse(projectPath.begin(), projectPath.end());
    int indToRem = projectPath.indexOf("/");
    projectPath = projectPath.sliced(indToRem);
    std::reverse(projectPath.begin(), projectPath.end());
    projectPath += "Projekt-Poliglot/";

    QString userDataPath = projectPath + "user_data/user_stats.txt";

    QString kodPath = projectPath + "sprites/skni_kod_logo_white.png";
    QString logoPath = projectPath + "sprites/logo.png";
    QString closeIconPath = projectPath + "sprites/icons/icon-close-window.png";
    QString checkIconPath = projectPath + "sprites/icons/icon-check.png";
    QString backspaceIconPath = projectPath + "sprites/icons/icon-backspace.png";

    QPixmap kodLogo(kodPath);
    QPixmap logo(logoPath);
    QPixmap closeIconMap(closeIconPath);
    QPixmap checkIconMap(checkIconPath);
    QPixmap backspaceIconMap(backspaceIconPath);

    logo = logo.scaled(ui->logo->width(), ui->logo->height(), Qt::KeepAspectRatio);
    kodLogo = kodLogo.scaled(ui->kod->width(), ui->kod->height(), Qt::KeepAspectRatio);
    ui->kod->setPixmap(kodLogo);
    ui->logo->setPixmap(logo);

    //USER STATS MANIPULATIONS
    user player;
    player.loadUserData(userDataPath);
    player.setPoints(7);
    player.setLevel(3);
    player.saveUserData(userDataPath);


    QString buttonPreset1 =                       "QPushButton {"
                            "border: 2px ;"
                            "border-radius: 15px;"
                            "background-color: #263023;"
                            "min-width: 60px;"
                            "min-height: 60px;"
                            "font: 20px;"
                            "}"
                            ""
                            "QPushButton:pressed {"
                            "background-color: #495c43;"
                            "}";

    QIcon closeIcon(closeIconMap);
    ui->gameCloseButton->setIcon(closeIcon);
    ui->gameCloseButton->setIconSize(closeIconMap.rect().size());
    ui->gameCloseButton->setStyleSheet(buttonPreset1);

    QIcon checkIcon(checkIconMap);
    ui->checkButton->setIcon(checkIcon);
    ui->checkButton->setIconSize(checkIconMap.rect().size());
    ui->checkButton->setStyleSheet(buttonPreset1);

    QIcon backspaceIcon(backspaceIconMap);
    ui->clearButton->setIcon(backspaceIcon);
    ui->clearButton->setIconSize(backspaceIconMap.rect().size());
    ui->clearButton->setStyleSheet(buttonPreset1);

    connect(ui->playButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->game->show();
        ui->game->resize(width, height);
        ui->horizontalLayoutWidget_7->setFixedSize(width-50, height-50);
        prepareGrid();
        prepareLetterButtons();
    });

    connect(ui->gameCloseButton, &QPushButton::clicked, this, [](){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("Zamykanie gry");
        msgBox.setInformativeText("Czy na pewno chcesz opuścić grę?");
        QPushButton *yesButton = msgBox.addButton("Tak", QMessageBox::ActionRole);
        QPushButton *noButton = msgBox.addButton("Nie", QMessageBox::RejectRole);

        msgBox.exec();

        if(msgBox.clickedButton() == yesButton){
            QApplication::exit(0);
        }
        else if (msgBox.clickedButton() == noButton){}
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
    ui->gridFrame->setFixedSize(height-50, height-50);

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

                GridCell *cell = new GridCell(s[i][j].toUpper(), true, size);
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
    ui->lettersFrame->setFixedHeight(110);
    if(frameMult >= 1){
        ui->lettersFrame->setFixedHeight(ui->lettersFrame->height() * frameMult);
    }



    connect(ui->clearButton, &QPushButton::clicked, this, [this](){
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }
    });

    QShortcut *backspaceShortcut = new QShortcut(QKeySequence(Qt::Key_Backspace), ui->clearButton);
    QObject::connect(backspaceShortcut, &QShortcut::activated, ui->clearButton, &QPushButton::click);

    connect(ui->checkButton, &QPushButton::clicked, this, [this](){
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }

    });

    QShortcut *enterShortcut = new QShortcut(QKeySequence(Qt::Key_Enter), ui->checkButton);
    QObject::connect(enterShortcut, &QShortcut::activated, ui->checkButton, &QPushButton::click);

    //ui->textEdit->resize(ui->textEdit->width(), ui->lettersFrame->height() - 150);

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
