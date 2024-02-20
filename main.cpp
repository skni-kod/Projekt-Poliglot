#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication::setStyle("Fusion");
    QApplication a(argc, argv);
    MainWindow w;

    QScreen *screen = QApplication::primaryScreen();

    if(screen){
        QRect screenGeometry = screen->geometry();
        w.resize(screenGeometry.width(), screenGeometry.height());
    }
    // Ustawienie rozmiaru głównego okna na rozmiar ekranu


    w.showMaximized();
    return a.exec();
}
