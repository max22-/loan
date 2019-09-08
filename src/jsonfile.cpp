#include "jsonfile.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <cmath>
#include <QDebug>

JsonFile::JsonFile(QString path)
{
    this->path=path;
    nickname = nullptr;
    age = nullptr;
    city = nullptr;
    evaluation = nullptr;
    MP3FileName = nullptr;
    timeStamp = nullptr;
}

JsonFile::JsonFile(const JsonFile& other) {
    qDebug() << "Copy constructor";
    path = other.path;

    if(other.nickname != nullptr)
        nickname = new QString(*other.nickname);
    else
        nickname = nullptr;

    if(other.age != nullptr)
        age = new int(*other.age);
    else
        age = nullptr;

    if(other.city != nullptr)
        city = new QString(*other.city);
    else
        city = nullptr;

    if(other.evaluation != nullptr)
        evaluation = new int(*other.evaluation);
    else
        evaluation = nullptr;

    if(other.MP3FileName != nullptr)
        MP3FileName = new QString(*other.MP3FileName);
    else
        MP3FileName = nullptr;

    if(other.timeStamp != nullptr)
        timeStamp = new QDateTime(*other.timeStamp);
    else
        timeStamp = nullptr;
}

template <typename T>
T* assign(T* v1, const T* v2) {
    if(v2 == nullptr) {
        if(v1 != nullptr)
            delete v1;
        return nullptr;
    }
    if(v1 != nullptr)
        delete v1;
    v1 = new T(*v2);
    return v1;
}

JsonFile& JsonFile::operator=(const JsonFile &other) {
    qDebug() << "Assignment operator";

    if(this == &other) // auto assignment
        return *this;

    nickname = assign(nickname, other.nickname);
    age = assign(age, other.age);
    city = assign(city, other.city);
    evaluation = assign(evaluation, other.evaluation);
    MP3FileName = assign(MP3FileName, other.MP3FileName);
    timeStamp = assign(timeStamp, other.timeStamp);
    return *this;
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
    auto dateTime = QDateTime::fromString(datetimeString, "yyyy-MM-dd hh:mm:ss");
    return new QDateTime(dateTime);
}

JsonFile& JsonFile::load() {
    QFile file(path);
    if(!file.exists())
        throw QString("Cannot load " + path + " : file doesn't exist.");
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
    if(nickname == nullptr)
        throw QString("JsonFile::save : nickname is not set");
    if(age == nullptr)
        throw QString("JsonFile::save : age is not set");
    if(city == nullptr)
        throw QString("JsonFile::save : city is not set");
    if(evaluation == nullptr)
        throw QString("JsonFile::save : evaluation is not set");
    if(MP3FileName == nullptr)
        throw QString("JsonFile::save : MP3FileName is not set");
    if(timeStamp == nullptr)
        throw QString("JsonFile::save : timeStamp is not set");

    QJsonObject jsonObject;
    jsonObject.insert("nickname", *nickname);
    jsonObject.insert("age", *age);
    jsonObject.insert("city", *city);
    jsonObject.insert("evaluation", *evaluation);
    jsonObject.insert("filename", *MP3FileName);
    jsonObject.insert("timestamp", timeStamp->toString("yyyy-MM-dd hh:mm:ss"));

    QJsonDocument jsonDocument(jsonObject);

    QFile jsonFile(path);
    if(jsonFile.open(QFile::WriteOnly)) {
        jsonFile.write(jsonDocument.toJson());
        jsonFile.close();
    }
    else
        throw QString("Couldn't write json data to file " + path);

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
