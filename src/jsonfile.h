#ifndef JSONFILE_H
#define JSONFILE_H

#include <QDateTime>
#include <QMetaType>

class JsonFile
{
public:
    JsonFile();
    JsonFile(const JsonFile& other);
    JsonFile& operator=(const JsonFile& other);
    ~JsonFile();

    JsonFile& load(QString path);
    JsonFile& save(QString path);

    JsonFile& setTimeStamp(const QDateTime& timeStamp);
    JsonFile& setNickName(const QString& nickname);
    JsonFile& setAge(const int& age);
    JsonFile& setCity(const QString& city);
    JsonFile& setEvaluation(const int& evaluation);

    QDateTime getTimeStamp() const;
    QString getNickname() const;
    int getAge() const;
    QString getCity() const;
    int getEvaluation() const;
    QString getMP3FileName() const;

private:

    QString *nickname = nullptr;
    int *age = nullptr;
    QString *city = nullptr;
    int *evaluation = nullptr;
    QDateTime *timeStamp = nullptr;

    void clear();
};

Q_DECLARE_METATYPE(JsonFile)

#endif // JSONFILE_H
