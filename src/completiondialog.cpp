#include "completiondialog.h"
#include <QPushButton>
CompletionDialog::CompletionDialog(QWidget *parent) : QFrame(parent) {
    layout = new QVBoxLayout(this);
    elapsedTimeLabel = new QLabel(this);
    layout->addWidget(elapsedTimeLabel);
    setLayout(layout);
    setFixedSize(900, 600);
    setObjectName("dialog");
    setStyleSheet("QFrame {background-color: #2e382b; "
                  "border: 10px solid #263023;"
                  "border-radius: 30px;}");
    move(parentWidget()->width() / 2 - width() / 2, parentWidget()->height() / 2 - height() / 2);

    //elapsedTimeLabel = new QLabel(this);
    elapsedTimeLabel->setMinimumWidth(350);
    elapsedTimeLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    elapsedTimeLabel->setAlignment(Qt::AlignCenter);
    QFont font("Book Antiqua",28);
    elapsedTimeLabel->setFont(font);

    QString backIconPath = ":/sprites/icons/icon-back-white.png";
    QString checkIconPath = ":/sprites/icons/icon-check.png";

    QString buttonPreset = "QPushButton {"
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

    QPushButton *buttonBack = new QPushButton(this);
    QPushButton *buttonNext = new QPushButton(this);
    buttonBack->setIcon(QIcon(backIconPath));
    buttonBack->setIconSize(QSize(64,64));
    buttonNext->setIcon(QIcon(checkIconPath));
    buttonNext->setIconSize(QSize(64,64));

    buttonBack->setStyleSheet(buttonPreset);
    buttonNext->setStyleSheet(buttonPreset);


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(buttonBack,0,Qt::AlignTop | Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonNext,0,Qt::AlignBottom | Qt::AlignRight);
    layout->addLayout(buttonLayout);

    connect(buttonBack, &QPushButton::clicked, this, [this](){
        this->hide();
        //mainWindow->showMenu();
    });

    connect(buttonNext, &QPushButton::clicked, this, &CompletionDialog::nextClicked);
}

void CompletionDialog::setElapsedTime(int seconds, int lvl, int wordsAmount) {
    int lvlpone = lvl+1;
    elapsedTimeLabel->setText(QString("Odnaleziono %1 słów w %2 sekund\n"
                                      "Level %3 → Level %4").arg(wordsAmount).arg(seconds).arg(lvl).arg(lvlpone));
}

CompletionDialog::~CompletionDialog()
{
    delete layout;
    delete elapsedTimeLabel;
}
