#include "mainwindow.h"
#include <QApplication>


#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    qDebug("Hello, world");


    w.show();
    //w.showFullScreen();

    return a.exec();
}
