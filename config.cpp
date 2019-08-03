#include "config.h"
#include <QDebug>
#include <QDir>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config()
{

}

Config::~Config()
{

}

int Config::maxRecordingTimeS() {
    return 30;
}

QString Config::audioCodec() {
    return "audio/pcm";
}

int Config::sampleRate() {
    return 8000;
}
int Config::channelCount() {
    return 1;
}
int Config::sampleSize() {
    return 8;
}
QAudioFormat::Endian Config::byteOrder() {
    return QAudioFormat::LittleEndian;
}

QAudioFormat::SampleType Config::sampleType() {
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

QString Config::tempAudioFileName() {
    return tempDirectory().absoluteFilePath("message.raw");
}

QString Config::tempMP3AudioFileName() {
    return tempDirectory().absoluteFilePath("message.mp3");
}

QString Config::checkMP3ConverterInstallationCommand() {
    return "ffmpeg -version";
}
