#include "mainwindow.h"
#include <QApplication>
#include "statechart.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Statechart stateMachine;

    stateMachine.start();
    stateMachine.submitEvent("toc");
    w.show();
    //w.showFullScreen();

    return a.exec();
}
