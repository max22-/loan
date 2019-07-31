#include "init.h"
#include <QDebug>
#include "config.h"
#include <QProcess>

void checkAudioConverter() {
    if(QProcess::execute(Config::getInstance().checkAudioConverterInstallationCommand()) != 0)
        throw QString("Audio converter is not installed.");
}

void createDirectory(QDir dir) {
    if(dir.exists() == false) {
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
    qDebug("Initialization...");
    initDirectories();
    checkAudioConverter();
}


