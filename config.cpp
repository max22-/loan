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
