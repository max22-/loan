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
    QString desiredAudioCodec();
    int desiredSampleRate();
    int desiredChannelCount();
    int desiredSampleSize();
    QAudioFormat::Endian desiredByteOrder();
    QAudioFormat::SampleType desiredSampleType();

    QDir tempDirectory();
    QDir outboxDirectory();
    QDir inboxDirectory();
    QDir logDirectory();
    QString tempAudioFileName();
    QString tempMP3FileName();

    QString checkMP3ConverterInstallationCommand();
    QString MP3ConversionCommand(const QAudioFormat& format);

    QString timeStampFormat();
    QString fileNameFormat();

private:
    Config();
    ~Config();
};

#endif // CONFIG_H
