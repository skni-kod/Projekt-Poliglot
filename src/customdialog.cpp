#include "customdialog.h"

#include <QLabel>
#include <QPushButton>

CustomDialog::CustomDialog(QWidget *parent, QString message, bool okOnly) : QFrame(parent)
{
    layout = new QVBoxLayout(this);
    messageLabel = new QLabel(this);
    layout->addWidget(messageLabel);
    setLayout(layout);
    setFixedSize(600, 400);
    setObjectName("dialog");
    setStyleSheet("QFrame {background-color: #2e382b; "
                  "border: 10px solid #263023;"
                  "border-radius: 30px;}");
   move(parentWidget()->width() / 2 - width() / 2, parentWidget()->height() / 2 - height() / 2);

    messageLabel->setMinimumWidth(350);
    messageLabel->setStyleSheet("QFrame {border: 0px;}");
    messageLabel->setWordWrap(true);
    messageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    messageLabel->setAlignment(Qt::AlignCenter);
    QFont font("Book Antiqua",28);
    messageLabel->setFont(font);
    messageLabel->setText(message);

    QString closeIconPath = ":/sprites/icons/icon-close-window.png";
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

    QPushButton *buttonNext = new QPushButton(this);

    buttonNext->setIcon(QIcon(checkIconPath));
    buttonNext->setIconSize(QSize(64,64));
    buttonNext->setCursor(Qt::PointingHandCursor);
    buttonNext->setStyleSheet(buttonPreset);


    QHBoxLayout *buttonLayout = new QHBoxLayout();


    if(!okOnly){
        QPushButton *buttonClose = new QPushButton(this);
        buttonClose->setIcon(QIcon(closeIconPath));
        buttonClose->setIconSize(QSize(64,64));
        buttonClose->setCursor(Qt::PointingHandCursor);
        buttonClose->setStyleSheet(buttonPreset);
        connect(buttonClose, &QPushButton::clicked, this, [this](){
            this->hide();
        });
        buttonLayout->addWidget(buttonClose,0,Qt::AlignBottom | Qt::AlignLeft);
    }
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonNext,0,Qt::AlignBottom | Qt::AlignRight);


    layout->addLayout(buttonLayout);

    connect(buttonNext, &QPushButton::clicked, this, &CustomDialog::nextClicked);
    //TODO add pointing cursor
}

CustomDialog::~CustomDialog()
{
    delete layout;
    delete messageLabel;
}
