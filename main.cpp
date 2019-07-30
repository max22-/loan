#include "mainwindow.h"
#include <QApplication>
#include <exception>
#include <QDebug>

#include <iostream>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        MainWindow w;

        w.show();
        //w.showFullScreen();
        return a.exec();
    } catch (QString qs) {
        qDebug() << "Caught exception of type QString : " << qs;
        return EXIT_FAILURE;
    } catch (std::exception e) {
        qDebug() << "Caught exception of type exception : " << e.what();
        return EXIT_FAILURE;
    }
}
