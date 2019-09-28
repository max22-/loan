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
#ifdef Q_OS_WIN
    #include <windows.h>
#endif

QString QtMsgTypeToString(QtMsgType type) {
    QString typeString = "[]";
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
    QString fullMessage = QtMsgTypeToString(type) + " " + msg + " (" + context.file + ":" + QString::number(context.line) + ", " + context.function + ")";

    // If we are on Windows, we use OutputDebugString for all messages.
    // If we are on another OS, we send qDebug and qInfo to std::cout, and other messages to std::cerr;
    #ifdef Q_OS_WIN
        OutputDebugString(reinterpret_cast<const wchar_t *>((fullMessage + "\n").utf16()));
    #else
        std::ostream* strm;
        if(type == QtDebugMsg || type == QtInfoMsg)
            strm = &std::cout;
        else
            strm = &std::cerr;
        *strm << fullMessage.toStdString() << std::endl;
    #endif

    // We then try to write the message to the log file, if we can open it.
    QString logFilePath = Config::getInstance().logDirectory().absoluteFilePath(startupDateTime.toString("yyyy-MM-dd hh:mm:ss.log"));
    QFile logFile(logFilePath);

    if(logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream tstrm(&logFile);
        QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
        tstrm << timeStamp << fullMessage << endl;
        logFile.close();
    }
    else {
        QString logFileErrorMsg = "Impossible to open log file \"" + logFilePath + "\".";
        #ifdef Q_OS_WIN
            OutputDebugString(reinterpret_cast<const wchar_t *>((logFileErrorMsg + "\n").utf16()));
        #else
            std::cerr << logFileErrorMsg.toStdString() << std::endl;
        #endif
    }

    if(type == QtFatalMsg)
        abort();
}
