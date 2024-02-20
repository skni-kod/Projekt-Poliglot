#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QFrame>

class GridCell : public QFrame
{
public:
    GridCell(QChar letter = '#', bool isVisible = false, double size = 60);
};

#endif // GRIDCELL_H
