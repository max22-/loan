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

std::string QtMsgTypeToString(QtMsgType type) {
    std::string typeString = "[]";
    switch (type) {
        case QtDebugMsg:
            typeString = "[Debug]";
            break;
        case QtInfoMsg:
            typeString = "[Info]";
            break;
        case QtWarningMsg:
            typeString = "[Warning]";
            break;
        case QtCriticalMsg:
            typeString = "[Critical]";
            break;
        case QtFatalMsg:
            typeString = "[Fatal]";
            break;
    }
    return typeString;
}

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // If we are on Windows, we send all messages to cerr, because cout doesn't work with GUI apps.
    #ifdef Q_OS_WIN
        std::ostream& infoStream = std::cerr;
    #else
        std::ostream& infoStream = std::cout;
    #endif
    std::ostream& errorStream = std::cerr;

    std::ostream* strm;
    if(type == QtDebugMsg || type == QtInfoMsg)
        strm = &infoStream;
    else
        strm = &errorStream;
    // ***********************************************************************************************

    // We then send the message to the desired streamed, either cout or cerr. Always cerr under windows, and cout for infos and debug  and cerr only for errors under other OS.
    *strm << QtMsgTypeToString(type) << " " << msg.toStdString() << " (" << context.file << ":" << context.line << ", " << context.function << ")" << std::endl;

    // We then try to write the message to the log file, if we can open it.
    QString logFilePath = Config::getInstance().logDirectory().absoluteFilePath(startupDateTime.toString("yyyy-MM-dd hh:mm:ss.log"));
    QFile logFile(logFilePath);

    if(logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream tstrm(&logFile);
        QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
        tstrm << timeStamp << QString::fromStdString(QtMsgTypeToString(type)) << " " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
        logFile.close();
    }
    else
        std::cerr << "Impossible to open log file \"" << logFilePath.toStdString() << "\"." << std::endl;

    if(type == QtFatalMsg)
        abort();
}
