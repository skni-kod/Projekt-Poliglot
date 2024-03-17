#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "qboxlayout.h"
#include "qlabel.h"
#include <QFrame>

class GridCell : public QFrame
{
public:
    GridCell(QChar letter = '#', bool isVisible = false, double size = 60);
    void setLetterVisibility();
private:
    QChar m_letter = '#';
    QLabel *label = nullptr;
    QVBoxLayout *layout = nullptr;
    ~GridCell();
};

#endif // GRIDCELL_H
