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
#include <QStandardPaths>
#include <QTimer>
#include <QShortcut>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "word_gen.h"
#include "gridcell.h"
#include "user.h"
#include "completiondialog.h"
#include "customdialog.h"

//<a target="_blank" href="https://icons8.com/icon/KLD9V6A735yg/done">Check</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    qDebug() << "Elapsed time: " << elapsedTime.elapsed() << "milliseconds";
    QScreen *screen = QApplication::primaryScreen();

    isFirstBoard = true;

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
    ui->continueButton->setDisabled(true);

    player.filePathInit();

    setupButtons();

    ui->verticalLayoutWidget->resize(width, height);

    //USER STATS MANIPULATIONS
    player.loadUserData();
    player.saveUserData();

    imgService = new ImageService(this);

    connect(imgService, &ImageService::imageDownloaded, this, [this](const QPixmap &pixmap) {
        backgroundImage = pixmap;
    });

    imgService->downloadImage(width, height);
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

            if (QWidget *widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
    }

    if (!gridLayout)
        gridLayout = new QGridLayout(ui->gridFrame);

    for (int i = 0; i < (int)s.size(); i++){
        for(int j = 0; j < (int)s[0].size() ; j++){
            if(s[i][j] != '#'){
                qDebug()<<"Adding new gridCell " << s[i][j];
                GridCell *cell = new GridCell(s[i][j].toUpper(), false, gridCellSize);
                gridLayout->addWidget(cell, i, j, Qt::AlignCenter);
            }
        }
    }

    ui->textBrowser->clear();

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
    //CZITY
    ui->textBrowser->hide();

    updateCornerLabel();

    elapsedTime.start();

    if(!backgroundImage.isNull())
        ui->imgLabel->setPixmap(backgroundImage);
    ui->imgLabel->setGeometry(0,0,width,height);
    imgService->downloadImage(width,height);

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
    if(isFirstBoard){
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
        setupCheckButton();

        QShortcut *enterShortcut = new QShortcut(QKeySequence(Qt::Key_Enter), ui->checkButton);
        connect(enterShortcut, &QShortcut::activated, ui->checkButton, &QPushButton::click);
    }

    //ui->textEdit->resize(ui->textEdit->width(), ui->lettersFrame->height() - 150);
    if(lettersGridLayout == nullptr){
        lettersGridLayout = new QGridLayout(ui->lettersFrame);
    }

    // deleting buttons from lettersGridLayout
    for (int i = 0; i < lettersGridLayout->count(); ++i) {
        QLayoutItem *item = lettersGridLayout->takeAt(i);
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    // deleting pointers from nonexisting buttons that have been just deleted
    for (QPushButton *button : letButtons) {
        delete button;
    }
    letButtons.clear();

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
                          "font: 45px;"
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

void MainWindow::setupCheckButton(){
    connect(ui->checkButton, &QPushButton::clicked, this, [this](){
        QString wordToCheck = ui->textEdit->toPlainText().toLower();
        int result = board.checkWord(wordToCheck);
        int level = player.getLevel();
        if(result == 1){

            //there is this word on a board
            qDebug() << "word is present: " << wordToCheck;
            updateGrid(wordToCheck);

            if(board.AlreadyGuessedWords().find(wordToCheck) == board.AlreadyGuessedWords().end()){
                player.updatePoints('I', wordToCheck.length()*2);
                board.addGuessedWordCount();
            }

            board.addGuessedWord(wordToCheck);

            //if board completed
            qDebug() << "guessed word count: " << board.getGuessedWordCount()
                     << "presentwords.size()" << (int)board.PresentWords().size();
            if(board.getGuessedWordCount() == (int)board.PresentWords().size()){

                int elapsedTimeInSeconds = elapsedTime.elapsed() / 1000;

                player.setLevel(player.getLevel()+1);
                updateCornerLabel();
                player.saveUserData();

                QEventLoop loop;
                QTimer::singleShot(1000, &loop, &QEventLoop::quit);
                loop.exec();

                CompletionDialog *completionDialog = new CompletionDialog(this);
                completionDialog->setElapsedTime(elapsedTimeInSeconds, level, board.getGuessedWordCount());
                completionDialog->show();

                connect(completionDialog, &CompletionDialog::nextClicked, this, [completionDialog, this](){
                    completionDialog->deleteLater();
                    isFirstBoard = false;
                    elapsedTime.restart();
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

            player.saveUserData();
        }
        else{
            //there is no word like that, ignore
            qDebug() << "word does not exist: " << wordToCheck;
        }
        updateCornerLabel();
        player.saveUserData();
        qDebug() << "Points: " << player.getPoints();
        ui->textEdit->clear();
        for(auto& button : letButtons){
            button->show();
        }
    });
}

void MainWindow::startNewBoard(int diff){
    setStyleSheet("background-color: rgba(46, 56, 43, 0.0)");
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
    QString buttonPreset = "QPushButton {border: 10px; "
                           "border-radius:49px; "
                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #253624, stop: 1 #679465);}";
    ui->playButton->setStyleSheet(buttonPreset);
    ui->optionsButton->setStyleSheet(buttonPreset);
    ui->closeButton->setStyleSheet(buttonPreset);
    ui->continueButton->setStyleSheet(buttonPreset);

    //set paths for used icons and images
    QString kodPath = "://sprites/skni_kod_logo_white.png";
    QString logoPath = ":/sprites/logo.png";
    QString closeIconPath =  ":/sprites/icons/icon-close-window.png";
    QString checkIconPath = ":/sprites/icons/icon-check.png";
    QString backspaceIconPath = ":/sprites/icons/icon-backspace.png";
    QString backIconPath = ":/sprites/icons/icon-back-white.png";
    QString poliglotIconPath = ":/sprites/icons/poliglot-icon.png";
    QString backSmallIconPath = ":/sprites/icons/icon-back-white-small.png";
    QString lightBulbIconPath = ":/sprites/icons/icon-light-bulb.png";

    QPixmap kodLogo(kodPath);
    QPixmap logo(logoPath);
    QPixmap closeIconMap(closeIconPath);
    QPixmap checkIconMap(checkIconPath);
    QPixmap backspaceIconMap(backspaceIconPath);
    QPixmap backIconMap(backIconPath);
    QPixmap poliglotIconMap(poliglotIconPath);
    QPixmap backSmallIconMap(backSmallIconPath);
    QPixmap lightBulbIconMap(lightBulbIconPath);

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

    QIcon backIcon(backIconMap);
    ui->back_Button->setIcon(backIcon);
    ui->back_Button->setIconSize(backIconMap.rect().size());
    ui->back_Button->setStyleSheet(buttonPreset1);

    QIcon backspaceIcon(backspaceIconMap);
    ui->clearButton->setIcon(backspaceIcon);
    ui->clearButton->setIconSize(backspaceIconMap.rect().size());
    ui->clearButton->setStyleSheet("QPushButton {"
                                   "border: 2px ;"
                                   "border-radius: 15px;"
                                   "background-color: #263023;"
                                   "min-width: 60px;"
                                   "min-height: 60px;"
                                   "padding: 5px;"
                                   "font: 20px;"
                                   "}"
                                   ""
                                   "QPushButton:pressed {"
                                   "background-color: #495c43;"
                                   "}");

    ui->difficultyBackButton->setIcon(backIcon);
    ui->difficultyBackButton->setIconSize(backIconMap.rect().size());
    ui->difficultyBackButton->setStyleSheet(buttonPreset1);

    QIcon backSmallIcon(backSmallIconMap);
    ui->backMenuButton->setIcon(backSmallIcon);
    ui->backMenuButton->setIconSize(backSmallIconMap.rect().size());
    ui->backMenuButton->setStyleSheet(buttonPreset1);

    QIcon lightBulbIcon(lightBulbIconMap);
    ui->revealLetterButton->setIcon(lightBulbIcon);
    ui->revealLetterButton->setIconSize(lightBulbIconMap.rect().size());
    ui->revealLetterButton->setStyleSheet(buttonPreset1);

    //define main menu play button behaviour
    connect(ui->playButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->difficultySelection->show();
        ui->difficultySelection->resize(width,height);
        ui->verticalWidget_2->resize(width,height);
    });

    // OPTIONS BUTTON
    connect(ui->optionsButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->menuOptions->show();
        ui->menuOptions->resize(width, height);
        ui->verticalWidget_3->resize(width, height);
    });
    // RETURN BUTON IN GAME
    connect(ui->backMenuButton, &QPushButton::clicked, this, [this](){
        ui->game->hide();
        ui->menu->show();
        ui->menu->resize(width, height);
        setStyleSheet("background-color: #2e382b");
        ui->verticalLayoutWidget->resize(width, height);
        ui->continueButton->setDisabled(false);
    });
    connect(ui->continueButton, &QPushButton::clicked, this, [this](){
        ui->menu->hide();
        ui->game->show();
        setStyleSheet("background-color: rgba(0, 0, 0, 0)");
        ui->menu->resize(width, height);
        ui->verticalLayoutWidget->resize(width, height);
    });
    // CLOSING THE GAME "X" BUTTON
    connect(ui->gameCloseButton, &QPushButton::clicked, this, [this](){
        CustomDialog *dialog = new CustomDialog(this, "Czy na pewno chcesz wyjść z gry?", false);
        dialog->show();
        connect(dialog, &CustomDialog::nextClicked, this, [dialog](){
            dialog->deleteLater();
            QApplication::exit(0);
        });
        return;
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
    ui->easyButton->setCursor(Qt::PointingHandCursor);
    ui->mediumButton->setStyleSheet(diffButtonPreset);
    ui->mediumButton->setCursor(Qt::PointingHandCursor);
    ui->hardButton->setStyleSheet(diffButtonPreset);
    ui->hardButton->setCursor(Qt::PointingHandCursor);

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
    //Points Button
    ui->checkBox_3->setChecked(false);
    connect(ui->okButton, &QPushButton::clicked,this,[this](){
        if(ui->checkBox_3->isChecked()){
            player.setPoints(0);
            qDebug()<<"Points = 0";
        }
        else {
            qDebug()<<"Sth went wrong";
        }
    });

    ui->gridFrame->setObjectName("gridFrame");
    ui->gridFrame->setStyleSheet("#gridFrame {background-color: rgba(46, 56, 43, 0.8); "
                                 "border: 10px solid #3a4736;"
                                 "border-radius: 30px;}");

    ui->lettersFrame->setObjectName("lettersFrame");
    ui->lettersFrame->setStyleSheet("#lettersFrame {background-color: rgba(46, 56, 43, 0.8); "
                                    "border: 10px solid #3a4736;"
                                    "border-radius: 30px;}");

    ui->textEdit->setStyleSheet("QTextEdit {background-color: rgba(46, 56, 43, 0.8); "
                                "border: 10px solid #3a4736;"
                                "border-radius: 30px;}");

    ui->pointsLabel->setStyleSheet(                            "border: 2px ;"
                                   "border-radius: 15px;"
                                   "background-color: rgba(46, 56, 43, 0.8);"
                                   "padding: 5px;");
    ui->label->setStyleSheet("border-radius: 10px;"
                             "background-color: rgba(46, 56, 43, 0.8);"
                             "padding: 8px;");

}

void MainWindow::updateCornerLabel(){
    ui->pointsLabel->setText((QString)"Punkty: " + QString::number(player.getPoints()) + "\n" +
                             "Level: " + QString::number(player.getLevel()));
}

// used to put guessed words onto the board
void MainWindow::updateGrid(QString& newWord)
{
    std::unordered_map<QString, std::vector<std::pair<int, int>>> map = board.WordMap();
    auto signMap = board.signMap;

    int i = 0;

    for(auto& pair : map[newWord]){

        GridCell *gridCell = dynamic_cast<GridCell*>(gridLayout->itemAtPosition(pair.second, pair.first)->widget());
        if(gridCell){
            gridCell->setLetterVisibility();
            board.revealedSignMap[newWord[i]].push_back(pair);
            board.removeFromSignMap(newWord[i], pair.first, pair.second);
        }
        else{
            qDebug()<<"Unable to reach gridcell!";
        }
        i++;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
    delete gridLayout;
}

void MainWindow::on_revealLetterButton_clicked()
{
    if (player.getPoints() < 200){
        CustomDialog *dialog = new CustomDialog(this, "Nie masz wystarczającej ilości punktów!", true);
        dialog->show();
        connect(dialog, &CustomDialog::nextClicked, this, [dialog](){
            dialog->deleteLater();
        });
        return;
    }

    bool ok = false;
    auto letters = board.Letters();
    auto revealedLetters = board.revealedSignMap;
    int i = 0;
    qDebug() << letters;

    while(!ok && i < 300){
        i++;
        QChar pickedLetter = letters[rand() % ((int)letters.length())];

        int index = 0;
        switch(board.signMap[pickedLetter].size()){
            case 1:
                qDebug() << "Nie ma litery";
                continue;
            case 2:
                break;
            default:
                index = rand() % ((int)board.signMap[pickedLetter].size()-2);
                break;
        }

        std::pair<int, int> coords = board.signMap[pickedLetter][1+index];
        qDebug()<<"Wybrano litere: " << pickedLetter << " i bedzie na kordach " << coords;

        if (coords != (std::pair<int, int>){-100, -100}){

            GridCell *gridCell = dynamic_cast<GridCell*>(gridLayout->itemAtPosition(coords.second, coords.first)->widget());
            if(gridCell){
                gridCell->setLetterVisibility();
                board.revealedSignMap[pickedLetter].push_back(coords);
                board.removeFromSignMap(pickedLetter, coords.first, coords.second);
                ok = true;
            }
            else{
                qDebug()<<"Unable to reach gridcell!";
            }
        }
        else{
            qDebug()<<"kurde jest juz";
        }
    }

    if(i>300){
        qDebug()<<"nie da sie sr";
        return;
    }

    player.updatePoints('D', 200);
    updateCornerLabel();
}

