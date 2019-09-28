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

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QHash<QtMsgType, QString> typeToString({
        {QtDebugMsg, "[Debug]"},
        {QtInfoMsg, "[Info]"},
        {QtWarningMsg, "[Warning]"},
        {QtCriticalMsg, "[Critical]"},
        {QtFatalMsg, "[Fatal]"}
    });

    QString fullMessage = typeToString.value(type, "[Unknown Type]") + " " + msg;
    #ifdef QT_DEBUG
        fullMessage = fullMessage + " (" + context.file + ":" + QString::number(context.line) + ", " + context.function + ")";
    #endif

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
