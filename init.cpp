#include "init.h"
#include <QDebug>
#include "config.h"
#include <QProcess>
#include <QAudioDeviceInfo>

void showAudioDeviceInfo() {
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    qDebug() << "******** Audio device info ********";
    qDebug() << "- Device name : " << info.deviceName();
    qDebug() << "Available audio formats :";
    qDebug() << "- Preferred format : " << info.preferredFormat();
    qDebug() << "- Supported byte orders : " << info.supportedByteOrders();
    qDebug() << "- Supported channel counts : " << info.supportedChannelCounts();
    qDebug() << "- Supported codecs : " << info.supportedCodecs();
    qDebug() << "- Supported sample rates : " << info.supportedSampleRates();
    qDebug() << "- Supported sample sizes : " << info.supportedSampleSizes();
    qDebug() << "- Supported sample types : " << info.supportedSampleTypes();
    qDebug() << "***********************************";
}

void checkAudioConverter() {
    if(QProcess::execute(Config::getInstance().checkMP3ConverterInstallationCommand()) != 0)
        throw QString("Audio converter is not installed.");
}

void createDirectory(const QDir& dir) {
    if(!dir.exists()) {
        qDebug() << "Creating " << dir.absolutePath() << "directory";
        if(dir.mkpath(".") == false) {
            throw QString("Could'nt create") + dir.absolutePath() + QString(" directory.");
        }
    }
}

void initDirectories() {
    Config& config = Config::getInstance();
    createDirectory(config.tempDirectory());
    createDirectory(config.outboxDirectory());
    createDirectory(config.inboxDirectory());
}

void init()
{
    qDebug() << "Initialization...";
    initDirectories();
    checkAudioConverter();
    showAudioDeviceInfo();
}


