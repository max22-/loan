#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QAudioFormat>
#include <QDir>

class Config
{
public:
    static Config& getInstance();
    Config(Config&) = delete;
    Config& operator=(Config&) = delete;

    int maxRecordingTimeS();
    QString audioCodec();
    int sampleRate();
    int channelCount();
    int sampleSize();
    QAudioFormat::Endian byteOrder();
    QAudioFormat::SampleType sampleType();

    QDir tempDirectory();
    QDir outboxDirectory();
    QDir inboxDirectory();
    QDir logDirectory();
    QString tempAudioFileName();
    QString tempMP3AudioFileName();

    QString checkAudioConverterInstallationCommand();

private:
    Config();
    ~Config();
};

#endif // CONFIG_H
