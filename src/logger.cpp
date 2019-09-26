#include "logger.h"
#include <QtGlobal>
#include <QString>
#include <iostream>
#include <QDateTime>
#include <QFile>
#include "config.h"
#include <QTextStream>
#include "startupdatetime.h"
#include <QMessageBox>

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logFilePath = Config::getInstance().logDirectory().absoluteFilePath(startupDateTime.toString("yyyy-MM-dd hh:mm:ss.log"));
    QFile logFile(logFilePath);
    if(logFile.open(QIODevice::WriteOnly | QIODevice::Append) == false) {
        QMessageBox msgBox;
        msgBox.setText("Impossible d'ouvrir le fichier log.\n Message d'erreur : \"" + logFile.errorString() +"\".");
        msgBox.exec();
        throw QString("Impossible to open log file " + logFilePath);
    }

    QTextStream tstrm(&logFile);
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");

    switch (type) {
        case QtDebugMsg:
            std::cerr << "[Debug] " << msg.toStdString() << " (" << context.file << ":" << context.line << ", " << context.function << ")" << std::endl;
            tstrm << timeStamp << "[Debug] " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtInfoMsg:
            std::cerr << "[Info] " << msg.toStdString() << " (" << context.file << ":" << context.line << ", " << context.function << ")" << std::endl;
            tstrm << timeStamp << "[Info] " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtWarningMsg:
            std::cerr << "[Warning] " << msg.toStdString() << " (" << context.file << ":" << context.line << ", " << context.function << ")" << std::endl;
            tstrm << timeStamp<< "[Warning] " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtCriticalMsg:
            std::cerr << "[Critical] : " << msg.toStdString() << " (" << context.file << ":" << context.line << ", " << context.function << ")" << std::endl;
            tstrm << timeStamp << "[Critical] : " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtFatalMsg:
            std::cerr << "[Fatal] " << msg.toStdString() << " (" << context.file << ":" << context.line << ", " << context.function << ")" << std::endl;
            tstrm << timeStamp << "[Fatal] " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            abort();
    }

    logFile.close();
}
