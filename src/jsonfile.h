#ifndef JSONFILE_H
#define JSONFILE_H

#include <QDateTime>

class JsonFile
{
public:
    JsonFile(QString path);
    JsonFile(const JsonFile& other);
    JsonFile& operator=(const JsonFile& other);
    ~JsonFile();

    JsonFile& load();
    JsonFile& save();

    JsonFile& setTimeStamp(const QDateTime& timeStamp);
    JsonFile& setNickName(const QString& nickname);
    JsonFile& setAge(const int& age);
    JsonFile& setCity(const QString& city);
    JsonFile& setEvaluation(const int& evaluation);
    JsonFile& setMP3FileName(const QString& MP3fileName);

    QDateTime getTimeStamp();
    QString getNickname();
    int getAge();
    QString getCity();
    int getEvaluation();
    QString getMP3FileName();

private:
    QString path;

    QDateTime *timeStamp = nullptr;
    QString *nickname = nullptr;
    int *age = nullptr;
    QString *city = nullptr;
    int *evaluation = nullptr;
    QString *MP3FileName = nullptr;
};

#endif // JSONFILE_H
