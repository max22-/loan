#include "config.h"
#include <QDebug>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config()
{
    qDebug("Config();");
}

Config::~Config()
{
    qDebug("~Config();");
}

int Config::maxRecordingTimeS() {
    return 30;
}

QString Config::audioCodec() {
    return "audio/pcm";
}

QString Config::tempAudioFileName() {
    return "message.raw";
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
