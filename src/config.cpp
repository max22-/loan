#include "config.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() = default;

Config::~Config() = default;

int Config::maxRecordingTimeS() {
    return 30;
}

QString Config::desiredAudioCodec() {
    return "audio/pcm";
}

int Config::desiredSampleRate() {
    return 44100;
}
int Config::desiredChannelCount() {
    return 1;
}
int Config::desiredSampleSize() {
    return 16;
}
QAudioFormat::Endian Config::desiredByteOrder() {
    return QAudioFormat::LittleEndian;
}

QAudioFormat::SampleType Config::desiredSampleType() {
    return QAudioFormat::UnSignedInt;
}

QDir Config::tempDirectory() {
    return QDir(QDir::current().absolutePath() + QDir::separator() + "tmp");
}

QDir Config::outboxDirectory() {
    return QDir(QDir::current().absolutePath() + QDir::separator() + "box");
}

QDir Config::inboxDirectory() {
    return QDir(QDir::current().absolutePath() + QDir::separator() + "box");
}

QDir Config::logDirectory() {
    return QDir::current();
}

QString Config::logFileName(QDateTime startupTime) {
    return QDir::current().absoluteFilePath(startupTime.toString(fileNameFormat()) + ".log");
}

QString Config::tempAudioFileName() {
    return tempDirectory().absoluteFilePath("message.raw");
}

QString Config::tempMP3FileName() {
    return tempDirectory().absoluteFilePath("message.mp3");
}

QString Config::checkMP3ConverterInstallationCommand() {
    return "ffmpeg -version";
}

QString Config::MP3ConversionCommand(const QAudioFormat& format) {
    QString command = "ffmpeg -f ";
    switch(format.sampleType()) {
        case QAudioFormat::Unknown:
            throw "Invalid audio sample type.";
        case QAudioFormat::SignedInt:
            command += "s";
            break;
        case QAudioFormat::UnSignedInt:
            command += "u";
            break;
        case QAudioFormat::Float:
            command += "f";
            break;
    }
    command += QString::number(format.sampleSize());
    if(format.sampleSize() != 8) {
        switch (format.byteOrder()) {
            case QAudioFormat::BigEndian:
                command += "be";
                break;
            case QAudioFormat::LittleEndian:
                command += "le";
                break;
        }
    }
    command += " -ar " + QString::number(format.sampleRate()) + " -ac " + QString::number(format.channelCount());
    command += " -i " + tempAudioFileName();
    command += " " + tempMP3FileName();

    return command;
}

QString Config::timeStampFormat() {         // must be compatible with the filesystem (no colon allowed under Windows for example)
    return "yyyy-MM-dd hh:mm:ss";
}

QString Config::fileNameFormat() {         // must be compatible with the filesystem (no colon allowed under Windows for example)
    return "yyyy-MM-dd_hh-mm-ss";
}
