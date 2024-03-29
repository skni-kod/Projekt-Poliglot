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
#include <QTimer>
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
        screenWidth = width;
        screenHeight = height;
    }

    setStyleSheet("background-color: #2e382b");
    ui->setupUi(this);
    setWindowTitle("Poliglot");
    ui->menu->resize(width, height);
    ui->game->hide();
    ui->menuOptions->hide();
    ui->difficultySelection->hide();

    //set project path
    projectPath = QDir::currentPath();

    std::reverse(projectPath.begin(), projectPath.end());
    int indToRem = projectPath.indexOf("/");
    projectPath = projectPath.sliced(indToRem);
    std::reverse(projectPath.begin(), projectPath.end());
    projectPath += "Projekt-Poliglot/";

    //userDataPath = projectPath + "user_data/user_stats.txt";

    userDataPath = ":/user_data/user_stats.txt";
    setupButtons();

    ui->verticalLayoutWidget->resize(width, height);

    //USER STATS MANIPULATIONS
    player.loadUserData(userDataPath);
    player.saveUserData(userDataPath);
}

void MainWindow::prepareGrid(int diff){
    this->difficulty = diff;
    word_gen word(diff);
    word.showResult();
    board = Board(nullptr, word.getWords(), word.get_Letters);
    int count = board.UsedWordsCount();

    int minCount;

    switch(diff){
    case 1:
        minCount = 4;
        break;
    case 2:
        minCount = 6;
        break;
    case 3:
        minCount = 8;
        break;
    }

    while(count < minCount){
        word_gen word(diff);
        word.showResult();
        board = Board(nullptr, word.getWords(), word.get_Letters);
        count = board.UsedWordsCount();
    }

    Scheme s = board.Scheme();

    double frameSize = 0;

    ui->gridFrame->setFrameShape(QFrame::Box);
    ui->gridFrame->setFixedSize(height-50, height-50);

    if(board.Width() > board.Height()){
        gridCellSize = (double)ui->gridFrame->height() / (double)board.Width() * 0.8;
        frameSize = (double)ui->gridFrame->height() * (double)((double)board.Height()/(double)board.Width());
        ui->gridFrame->setFixedHeight(frameSize);
    }
    else{
        gridCellSize = (double)ui->gridFrame->height() / (double)board.Height() * 0.8;
        frameSize = (double)ui->gridFrame->width() * (double)((double)board.Width()/(double)board.Height());
        ui->gridFrame->setFixedWidth(frameSize);
    }

    if(gridLayout != nullptr){
        QLayoutItem *item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            // Sprawdzanie, czy element jest widżetem
            if (QWidget *widget = item->widget()) {
                // Jeśli jest widżetem, usuń go
                widget->deleteLater();
            }
            // Usuń element z układu
            delete item;
        }
    }

    if (!gridLayout)
        gridLayout = new QGridLayout(ui->gridFrame);
    //gridLayout = new QGridLayout(ui->gridFrame);

    for (int i = 0; i < (int)s.size(); i++){
        for(int j = 0; j < (int)s[0].size() ; j++){
            if(s[i][j] != '#'){
                qDebug()<<"Adding new gridCell " << s[i][j];
                GridCell *cell = new GridCell(s[i][j].toUpper(), false, gridCellSize);
                gridLayout->addWidget(cell, i, j, Qt::AlignCenter);
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
    ui->textBrowser->show();

    ui->pointsLabel->setText((QString)"Punkty: " + QString::number(player.getPoints()));
}

void MainWindow::prepareLetterButtons(){

    QVector<QChar> let = board.Letters();

    //set correct height of letter buttons qframe
    int frameMult = let.size() / 4 + 1;
    ui->lettersFrame->setFixedHeight(110);

    if(frameMult >= 1){
        ui->lettersFrame->setFixedHeight(ui->lettersFrame->height() * frameMult);
    }

    //set clear button behaviour
    connect(ui->clearButton, &QPushButton::clicked, this, [this](){
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }
    });

    //set shorcut for clearing textEdit field as a backspace key
    QShortcut *backspaceShortcut = new QShortcut(QKeySequence(Qt::Key_Backspace), ui->clearButton);
    QObject::connect(backspaceShortcut, &QShortcut::activated, ui->clearButton, &QPushButton::click);

    //set check word button behaviour, sends word to be checked by a board
    connect(ui->checkButton, &QPushButton::clicked, this, [this](){
        QString wordToCheck = ui->textEdit->toPlainText().toLower();
        int result = board.checkWord(wordToCheck);

        if(result == 1){

            //there is this word on a board
            qDebug() << "word is present: " << wordToCheck;
            updateGrid(wordToCheck);
            if(board.AlreadyGuessedWords().find(wordToCheck) == board.AlreadyGuessedWords().end()){
                player.updatePoints('I', wordToCheck.length()*2);
                board.addGuessedWordCount();
            }
            board.addGuessedWord(wordToCheck);
            ui->pointsLabel->setText((QString)"Punkty: " + QString::number(player.getPoints()));
            player.saveUserData(userDataPath);

            //if board completed
            if(board.getGuessedWordCount() == (int)board.PresentWords().size()){
                QTimer::singleShot(3000, [this](){
                    startNewBoard(difficulty);

                });
            }
        }
        else if(result == 2){

            //there is word like that but not on a board
            qDebug() << "word exists: " << wordToCheck;
            if(board.AlreadyGuessedWords().find(wordToCheck) == board.AlreadyGuessedWords().end()){
                player.updatePoints('I', wordToCheck.length()*3);
            }
            board.addGuessedWord(wordToCheck);
            ui->pointsLabel->setText((QString)"Punkty: " + QString::number(player.getPoints()));
            player.saveUserData(userDataPath);
        }
        else{
            //there is no word like that, ignore
            qDebug() << "word does not exist: " << wordToCheck;
        }

        qDebug() << "Points: " << player.getPoints();
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }
    });

    QShortcut *enterShortcut = new QShortcut(QKeySequence(Qt::Key_Enter), ui->checkButton);
    connect(enterShortcut, &QShortcut::activated, ui->checkButton, &QPushButton::click);

    //ui->textEdit->resize(ui->textEdit->width(), ui->lettersFrame->height() - 150);

    if(lettersGridLayout != nullptr){
        QLayoutItem *item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            // Sprawdzanie, czy element jest widżetem
            if (QWidget *widget = item->widget()) {
                // Jeśli jest widżetem, usuń go
                widget->deleteLater();
            }
            // Usuń element z układu
            delete item;
        }
        delete lettersGridLayout;
    }

    lettersGridLayout = new QGridLayout(ui->lettersFrame);

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
                          "font: 35px;"
                          "font-family: Book Antiqua;"
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

            lettersGridLayout->addWidget(emptyWidget, i, j, Qt::AlignCenter);
            lettersGridLayout->addWidget(button, i, j, Qt::AlignCenter);

            if(letterIndex + 1 == let.size()){
                break;
            }
            else{
                letterIndex++;
            }
        }
    }
}

void MainWindow::startNewBoard(int diff){
    difficulty = diff;
    ui->difficultySelection->hide();
    ui->game->show();
    ui->game->resize(width, height);
    ui->horizontalLayoutWidget_7->setFixedSize(width-50, height-50);
    prepareGrid(difficulty);
    prepareLetterButtons();
}

void MainWindow::setupButtons()
{
    //set style for main menu buttons
    QString buttonPreset = "QPushButton {border: 10px; border-radius: 60px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #253624, stop: 1 #679465);}";
    ui->playButton->setStyleSheet(buttonPreset);
    ui->optionsButton->setStyleSheet(buttonPreset);
    ui->closeButton->setStyleSheet(buttonPreset);

    //set paths for used icons and images
    QString kodPath = "://sprites/skni_kod_logo_white.png";
    QString logoPath = ":/sprites/logo.png";
    QString closeIconPath =  ":/sprites/icons/icon-close-window.png";
    QString checkIconPath = ":/sprites/icons/icon-check.png";
    QString backspaceIconPath = ":/sprites/icons/icon-backspace.png";
    QString backIconPath = ":/sprites/icons/icon-back-white.png";
    QString poliglotIconPath = ":/sprites/icons/poliglot-icon.png";

    QPixmap kodLogo(kodPath);
    QPixmap logo(logoPath);
    QPixmap closeIconMap(closeIconPath);
    QPixmap checkIconMap(checkIconPath);
    QPixmap backspaceIconMap(backspaceIconPath);
    QPixmap backIconMap(backIconPath);
    QPixmap poliglotIconMap(poliglotIconPath);

    QIcon poliglotIcon(poliglotIconMap);
    this->setWindowIcon(poliglotIcon);

    //set images
    logo = logo.scaled(ui->logo->width(), ui->logo->height(), Qt::KeepAspectRatio);
    kodLogo = kodLogo.scaled(ui->kod->width(), ui->kod->height(), Qt::KeepAspectRatio);
    ui->kod->setPixmap(kodLogo);
    ui->logo->setPixmap(logo);

    QString buttonPreset1 = "QPushButton {"
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

    //set icons and styles for in-game buttons
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

    QIcon backIcon(backIconMap);
    ui->back_Button->setIcon(backIcon);
    ui->back_Button->setIconSize(backIconMap.rect().size());
    ui->back_Button->setStyleSheet(buttonPreset1);

    ui->difficultyBackButton->setIcon(backIcon);
    ui->difficultyBackButton->setIconSize(backIconMap.rect().size());
    ui->difficultyBackButton->setStyleSheet(buttonPreset1);

    //define main menu play button behaviour
    connect(ui->playButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->difficultySelection->show();
        ui->difficultySelection->resize(width,height);
        ui->verticalWidget_2->resize(width,height);
    });

    connect(ui->optionsButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->menuOptions->show();
        ui->menuOptions->resize(width, height);
        ui->verticalWidget_3->resize(width, height);
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
    //Options
    //Back Button
    connect(ui->back_Button, &QPushButton::clicked,this,[this](){
        ui->menuOptions->hide();
        ui->menu->show();
        ui->menu->resize(width, height);
        ui->menuOptions->resize(width, height);
        ui->verticalLayoutWidget->resize(width, height);
    });
    //Text options

    ui->okButton->setIcon(checkIcon);

    ui->okButton->setStyleSheet(buttonPreset1);
    ui->okButton->setIconSize(checkIconMap.rect().size());

    //difficulty Buttons
    connect(ui->difficultyBackButton, &QPushButton::clicked, this, [this](){
        ui->difficultySelection->hide();
        ui->menu->show();
    });

    QString diffButtonPreset = "QPushButton {border: 10px; border-radius: 40px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #253624, stop: 1 #679465);}";

    ui->easyButton->setStyleSheet(diffButtonPreset);
    ui->mediumButton->setStyleSheet(diffButtonPreset);
    ui->hardButton->setStyleSheet(diffButtonPreset);

    //EASY INIT
    connect(ui->easyButton, &QPushButton::clicked, this, [this](){
        startNewBoard(1);
    });

    //MEDIUM INIT
    connect(ui->mediumButton, &QPushButton::clicked, this, [this](){
        startNewBoard(2);
    });

    //HARD INIT
    connect(ui->hardButton, &QPushButton::clicked, this, [this](){
        startNewBoard(3);
    });


    QString styleSheet = "QCheckBox::indicator {width: 50px; height: 50px;   }"
                         "QCheckBox { font-family: Book Antiqua; font-size: 50px; }";
    ui->checkBox->setStyleSheet(styleSheet);
    ui->checkBox_2->setStyleSheet(styleSheet);
    ui->checkBox_3->setStyleSheet(styleSheet);

    //Fullscreen Button
    ui->checkBox->setChecked(true);
    connect(ui->okButton, &QPushButton::clicked,this,[this](){
        if(ui->checkBox->isChecked()){
            showFullScreen();
            width = screenWidth;
            height = screenHeight;
            ui->menuOptions->resize(width, height);
            ui->verticalWidget_3->resize(width, height);
        }
        else {
            this->showNormal();
            width = 0.9*screenWidth;
            height = 0.9*screenHeight;
            resize(width, height);
            ui->menuOptions->resize(width, height);
            ui->verticalWidget_3->resize(width, height);
        }
    });
}

void MainWindow::updateGrid(QString& newWord)
{
    std::unordered_map<QString, std::vector<std::pair<int, int>>> map = board.WordMap();

    for(auto& pair : map[newWord]){

        GridCell *gridCell = dynamic_cast<GridCell*>(gridLayout->itemAtPosition(pair.second, pair.first)->widget());
        if(gridCell){
            gridCell->setLetterVisibility();
        }
        else{
            qDebug()<<"Unable to reach gridcell!";
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gridLayout;
}
