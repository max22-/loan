#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QAudioFormat>

class Config
{
public:
    static Config& getInstance();
    Config(Config&) = delete;
    Config& operator=(Config&) = delete;

    int maxRecordingTimeS();
    QString audioCodec();
    QString tempAudioFileName();
    int sampleRate();
    int channelCount();
    int sampleSize();
    QAudioFormat::Endian byteOrder();
    QAudioFormat::SampleType sampleType();

private:
    Config();
    ~Config();
};

#endif // CONFIG_H
