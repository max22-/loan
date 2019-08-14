#include "mainwindow.h"
#include <QApplication>
#include <exception>
#include <QDebug>
#include "init.h"
#include "logger.h"
#include <QtGlobal>
#include <QDateTime>
#include "startupdatetime.h"
#include <iostream>

const QDateTime startupDateTime = QDateTime::currentDateTime();

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logger);
    qDebug() << "Starting...";
    try {
        init();
        QApplication a(argc, argv);
        MainWindow w;

        w.show();
        //w.showFullScreen();
        int retCode = a.exec();
        qDebug() << "Quitting with return code " << retCode;
        return retCode;
    } catch (QString const& qs) {
        qDebug() << "Caught exception of type QString : " << qs;
        return EXIT_FAILURE;
    } catch (std::exception const& e) {
        qDebug() << "Caught exception of type exception : " << e.what();
        return EXIT_FAILURE;
    }
}
