#include "jsonfile.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <cmath>
#include <QDebug>

JsonFile::JsonFile(QString path)
{
    this->path=path;
}

JsonFile::~JsonFile() {
    delete timeStamp;
    delete nickname;
    delete age;
    delete city;
    delete evaluation;
    delete MP3FileName;
}

QJsonValue unwrapObject(QJsonObject o, const QString& key) {
    auto val = o.value(key);
    if(val == QJsonValue::Undefined)
        throw QString("Json file is invalid : " + key + " is missing.");
    return val;
}

int* extractIntValue(QJsonObject o, const QString& key) {
    auto val = unwrapObject(o, key);
    if(val.type() != QJsonValue::Double)
        throw QString("Json file : expected int, got " + QString(val.type()));
    double intpart;
    if(std::modf(val.toDouble(), &intpart) != 0.0)
        throw QString("Found double instead of int in Json file.");
    return new int(val.toInt());
}

QString* extractStringValue(QJsonObject o, const QString& key) {
    auto val = unwrapObject(o, key);
    if(val.type() != QJsonValue::String)
        throw QString("Json file : expected String, got " + QString(val.type()));
    return new QString(val.toString());
}

QDateTime* extractQDateTimeValue(QJsonObject o, const QString& key) {
    auto val = unwrapObject(o, key);
    if(val.type() != QJsonValue::String)
        throw QString("Json file : expected QDateTime, got " + QString(val.type()));
    auto datetimeString = val.toString();
    auto dateTime = QDateTime::fromString(datetimeString, "YYYY-MM-dd hh:mm:ss");
    return new QDateTime(dateTime);
}

JsonFile& JsonFile::load() {
    QFile file(path);
    file.open(QFile::ReadOnly);
    auto jsonData = QString::fromUtf8(file.readAll());
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject o = d.object();

    nickname = extractStringValue(o, "nickname");
    age = extractIntValue(o, "age");
    city = extractStringValue(o, "city");
    evaluation = extractIntValue(o, "evaluation");
    MP3FileName = extractStringValue(o, "filename");
    timeStamp = extractQDateTimeValue(o, "timestamp");
    return *this;
}

JsonFile& JsonFile::save() {

    return *this;
}

JsonFile& JsonFile::setTimeStamp(const QDateTime& timeStamp) {
    this->timeStamp = new QDateTime(timeStamp);
    return *this;
}

JsonFile& JsonFile::setNickName(const QString& nickname) {
    this->nickname = new QString(nickname);
    return *this;
}

JsonFile& JsonFile::setAge(const int& age) {
    this->age = new int(age);
    return *this;
}

JsonFile& JsonFile::setCity(const QString& city) {
    this->city = new QString(city);
    return *this;
}

JsonFile& JsonFile::setEvaluation(const int& evaluation) {
    this->evaluation = new int(evaluation);
    return *this;
}

JsonFile& JsonFile::setMP3FileName(const QString& MP3fileName) {
    this->MP3FileName = new QString(MP3fileName);
    return *this;
}





QDateTime JsonFile::getTimeStamp() {
    if(timeStamp == nullptr)
        throw QString("timeStamp is not set.");
    return *timeStamp;
}

QString JsonFile::getNickname() {
    if(nickname == nullptr)
        throw QString("nickname is not set.");
    return *nickname;
}

int JsonFile::getAge() {
    if(age == nullptr)
        throw QString("age is not set.");
    return *age;
}

QString JsonFile::getCity() {
    if(city == nullptr)
        throw QString("city is not set");
    return *city;
}

int JsonFile::getEvaluation() {
    if(evaluation == nullptr)
        throw QString("evaluation is not set.");
    return *evaluation;
}

QString JsonFile::getMP3FileName() {
    if(MP3FileName == nullptr)
        throw QString("MP3FileName is not set.");
    return *MP3FileName;
}
