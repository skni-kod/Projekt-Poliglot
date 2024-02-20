#include "gridcell.h"
#include "qboxlayout.h"
#include "qlabel.h"

GridCell::GridCell(QChar letter, bool isVisible, double size) {
    setFixedSize(size, size);

    QString frameSheet = "QFrame {background-color: rgb(107, 150, 122) ; border-radius: ";
    frameSheet += QString::number(size/4);
    frameSheet += "px;}";
    setStyleSheet(frameSheet);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = nullptr;

    if(isVisible){
        label = new QLabel((QString)letter, this);
    }
    else label = new QLabel("", this);

    label->setAlignment(Qt::AlignCenter);
    int fontSize = size/2;
    QString sheet = "font: ";
    sheet += QString::number(fontSize);
    sheet += "px";
    label->setStyleSheet(sheet);
    layout->addWidget(label);
    this->setLayout(layout);
}
