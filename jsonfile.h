#ifndef JSONFILE_H
#define JSONFILE_H

#include <QDateTime>

class JsonFile
{
public:
    JsonFile(QString path);
    ~JsonFile();
    JsonFile& load();
    void save();

    void setTimeStamp(QDateTime timeStamp);
    void setNickName(QString nickname);
    void setAge(int age);
    void setCity(QString city);
    void setEvaluation(int evaluation);
    void setMP3FileName(QString MP3fileName);

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
