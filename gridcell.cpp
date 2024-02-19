#include "gridcell.h"
#include "qboxlayout.h"
#include "qlabel.h"

GridCell::GridCell(QChar letter, bool isVisible) {
    setFixedSize(60,60);
    setStyleSheet("QFrame {background-color: rgb(107, 150, 122) ; border-radius: 8px;}");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = nullptr;

    if(isVisible){
        label = new QLabel((QString)letter, this);
    }
    else label = new QLabel("", this);

    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font: 30px");
    layout->addWidget(label);
    this->setLayout(layout);
}
