#include "completiondialog.h"
#include <QPushButton>
CompletionDialog::CompletionDialog(QWidget *parent) : QFrame(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    elapsedTimeLabel = new QLabel(this);
    layout->addWidget(elapsedTimeLabel);
    setLayout(layout);
    setFixedSize(900, 600);
    setStyleSheet("background-color: #2e382b; border: 2px solid white;");
    move(parentWidget()->width() / 2 - width() / 2, parentWidget()->height() / 2 - height() / 2);

    //elapsedTimeLabel = new QLabel(this);
    elapsedTimeLabel->setMinimumWidth(350);
    elapsedTimeLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    QFont font("Book Antiqua;",28);
    elapsedTimeLabel->setFont(font);

    QString backIconPath = ":/sprites/icons/icon-back-white.png";
    QString checkIconPath = ":/sprites/icons/icon-check.png";

    QPushButton *buttonBack = new QPushButton(this);
    QPushButton *buttonNext = new QPushButton(this);
    buttonBack->setIcon(QIcon(backIconPath));
    buttonBack->setIconSize(QSize(32,32));
    buttonNext->setIcon(QIcon(checkIconPath));
    buttonNext->setIconSize(QSize(32,32));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(buttonBack,0,Qt::AlignTop | Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonNext,0,Qt::AlignBottom | Qt::AlignRight);
    layout->addLayout(buttonLayout);
}

void CompletionDialog::setElapsedTime(int seconds) {
    elapsedTimeLabel->setText(QString("Czas pokoniania levelu: %1 sekund").arg(seconds));
}
