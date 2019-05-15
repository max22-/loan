#include "mainwindow.h"
#include <QApplication>
#include "statechart.h"
#include "statehandler.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Statechart stateMachine;
    StateHandler stateHandler;
    qDebug("Hello, world");

    stateMachine.connectToState("TocState", &stateHandler, SLOT(tocState(bool)));
    stateMachine.start();
    stateMachine.submitEvent("toc");
    w.show();
    //w.showFullScreen();

    return a.exec();
}
