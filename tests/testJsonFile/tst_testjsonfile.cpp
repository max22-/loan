#include <QtTest>

#include <QDir>
#include "jsonfile.h"

class testJsonFile : public QObject
{
    Q_OBJECT

public:
    testJsonFile();
    ~testJsonFile();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testEmptyJsonFile();
    void testCopyConstructor_data();
    void testCopyConstructor();
    void testAssignmentOperator_data();
    void testAssignmentOperator();
    void testAssignmentOperator2();
    void testSimpleLoad_data();
    void testSimpleLoad();
    void testSimpleSave_data();
    void testSimpleSave();
    void testChaining_data();
    void testChaining();
    void testFileDoesntExist();
    void testGettersSetters_data();
    void testGettersSetters();
    void testIncompleteSave_data();
    void testIncompleteSave();
    void testInvalidFile_data();
    void testInvalidFile();
    void testTestGetterAfterFailedLoad();

private:
    QDir databaseDirectory;
    void commonData();

};

testJsonFile::testJsonFile()
{
    databaseDirectory = QDir(QDir::current().absolutePath() + QDir::separator() + "database");
}

testJsonFile::~testJsonFile()
{

}

void testJsonFile::initTestCase()
{

}

void testJsonFile::cleanupTestCase()
{

}

void testJsonFile::init() {
    if(databaseDirectory.mkpath(".") == false)
        QFAIL("Could'nt create database directory.");
}

void testJsonFile::cleanup() {
    if(!databaseDirectory.removeRecursively())
        QFAIL("Couldn't remove database directory.");
}

void testJsonFile::testEmptyJsonFile() {
    JsonFile jsonFile(databaseDirectory.absoluteFilePath("inexistentFile.json"));
    QVERIFY_EXCEPTION_THROWN(jsonFile.getNickname(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getAge(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getCity(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getEvaluation(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getMP3FileName(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getTimeStamp(), QString);

}

void VERIFY_JSONFILE_PROPERTIES(JsonFile& jsonFile,
                                QString nickname,
                                int age,
                                QString city,
                                int evaluation,
                                QString MP3FileName,
                                int year,
                                int month,
                                int day,
                                int hour,
                                int minute,
                                int second) {
    QVERIFY2(jsonFile.getNickname() == nickname, QString("Nicknames don't match : expected \"" + nickname + "\", got \"" + jsonFile.getNickname() + "\".").toStdString().c_str());
    QVERIFY2(jsonFile.getAge() == age, QString("Ages don't match : expected " + QString::number(age) + ", got " + QString::number(jsonFile.getAge()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getCity() == city, QString("Cities don't match : expected \"" + city + "\", got \"" + jsonFile.getCity() + "\".").toStdString().c_str());
    QVERIFY2(jsonFile.getEvaluation() == evaluation, QString("Evaluations don't match : expected " + QString::number(evaluation) + ", got " + QString::number(jsonFile.getEvaluation()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getMP3FileName() == MP3FileName, QString("MP3 file names don't match : expected \"" + MP3FileName + "\", got \"" + jsonFile.getMP3FileName() + "\".").toStdString().c_str());
    QVERIFY2(jsonFile.getTimeStamp().date().year() == year, QString("Years don't match : expected " + QString::number(year) + ", got " + QString::number(jsonFile.getTimeStamp().date().year()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getTimeStamp().date().month() == month, QString("Months don't match : expected " + QString::number(month) + ", got " + QString::number(jsonFile.getTimeStamp().date().month()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getTimeStamp().date().day() == day, QString("Days don't match : expected " + QString::number(day) + ", got " + QString::number(jsonFile.getTimeStamp().date().day()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getTimeStamp().time().hour() == hour, QString("Hours don't match : expected " + QString::number(hour) + ", got " + QString::number(jsonFile.getTimeStamp().time().hour()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getTimeStamp().time().minute() == minute, QString("Minutes don't match : expected " + QString::number(minute) + ", got " + QString::number(jsonFile.getTimeStamp().time().minute()) + ".").toStdString().c_str());
    QVERIFY2(jsonFile.getTimeStamp().time().second()== second, QString("Seconds don't match : expected " + QString::number(second) + ", got " + QString::number(jsonFile.getTimeStamp().time().second()) + ".").toStdString().c_str());
}

void testJsonFile::commonData() {
    QTest::addColumn<QString>("jsonFileName");
    QTest::addColumn<QString>("jsonData");
    QTest::addColumn<QString>("nickname");
    QTest::addColumn<int>("age");
    QTest::addColumn<QString>("city");
    QTest::addColumn<int>("evaluation");
    QTest::addColumn<QString>("MP3FileName");
    QTest::addColumn<int>("year");
    QTest::addColumn<int>("month");
    QTest::addColumn<int>("day");
    QTest::addColumn<int>("hour");
    QTest::addColumn<int>("minute");
    QTest::addColumn<int>("second");

    auto timeStamp1 = "2019-08-17 16:33:00";
    auto jsonData1 = QString(
        "{\"nickname\": \"Maxime\","
        "\"age\": 33,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"filename\": \"2019-08-17 16:33:00.mp3\","
        "\"timestamp\": \"2019-08-17 16:33:00\""
        "}"
    );

    QTest::newRow("Maxime") << timeStamp1 + QString(".json") << jsonData1 << "Maxime" << 33 << "Plérin" << 3 << timeStamp1 + QString(".mp3") << 2019 << 8 << 17 << 16 << 33 << 0;

    auto timeStamp2 = "2020-01-15 14:45:04";
    auto jsonData2 = QString(
        "{\"nickname\": \"J.J. Brun's\","
        "\"age\": 20,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 4,"
        "\"filename\": \"2020-01-15 14:45:04.mp3\","
        "\"timestamp\": \"2020-01-15 14:45:04\""
        "}"
    );

    QTest::newRow("J.J. Brun's") << timeStamp2 + QString(".json") << jsonData2 << "J.J. Brun's" << 20 << "Plérin" << 4 << timeStamp2 + QString(".mp3") << 2020 << 1 << 15 << 14 << 45 << 4;

    auto timeStamp3 = "2021-03-27 09:32:27";
    auto jsonData3 = QString(
        "{\"nickname\": \"Stéphane\","
        "\"age\": 21,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 5,"
        "\"filename\": \"2021-03-27 09:32:27.mp3\","
        "\"timestamp\": \"2021-03-27 09:32:27\""
        "}"
    );

    QTest::newRow("Stéphane") << timeStamp3 + QString(".json") << jsonData3 << "Stéphane" << 21 << "Plérin" << 5 << timeStamp3 + QString(".mp3") << 2021 << 3 << 27 << 9 << 32 << 27;
}

void testJsonFile::testCopyConstructor_data() {
    commonData();
}

void testJsonFile::testCopyConstructor() {
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    try {
        JsonFile* o1 = new JsonFile(databaseDirectory.absoluteFilePath(jsonFileName));
        o1->setNickName(nickname)
                .setAge(age)
                .setCity(city)
                .setEvaluation(evaluation)
                .setMP3FileName(MP3FileName)
                .setTimeStamp(QDateTime(QDate(year, month, day), QTime(hour, minute, second)));
        JsonFile o2 = *o1;
        delete o1;

        VERIFY_JSONFILE_PROPERTIES(o2, nickname, age, city, evaluation, MP3FileName, year, month, day, hour, minute, second);
    }
    catch (const QString s) {
        QFAIL(QString("Caught exception : " + s).toStdString().c_str());
    }
}

void testJsonFile::testAssignmentOperator_data() {
    commonData();
}

void testJsonFile::testAssignmentOperator() {
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    try {
        JsonFile* o1 = new JsonFile(databaseDirectory.absoluteFilePath(jsonFileName));
        o1->setNickName(nickname)
                .setAge(age)
                .setCity(city)
                .setEvaluation(evaluation)
                .setMP3FileName(MP3FileName)
                .setTimeStamp(QDateTime(QDate(year, month, day), QTime(hour, minute, second)));
        JsonFile o2(databaseDirectory.absoluteFilePath("inexistentFile.json"));
        o2 = *o1;
        delete o1;

        VERIFY_JSONFILE_PROPERTIES(o2, nickname, age, city, evaluation, MP3FileName, year, month, day, hour, minute, second);
    }
    catch (const QString s) {
        QFAIL(QString("Caught exception : " + s).toStdString().c_str());
    }

}

void testJsonFile::testAssignmentOperator2() {

    // We test if the destination object if cleared if we assign it a blank JsonFile.

    JsonFile o1(databaseDirectory.absoluteFilePath("inexistentFile.json"));
    o1.setNickName("Maxime")
            .setAge(33)
            .setCity("Plérin")
            .setEvaluation(5)
            .setMP3FileName("2019-08-31 12:13:00.mp3")
            .setTimeStamp(QDateTime(QDate(2019, 8, 31), QTime(12, 13, 0)));
    JsonFile o2(databaseDirectory.absoluteFilePath("inexistentFile2.json"));
    o1 = o2;

    QVERIFY_EXCEPTION_THROWN(o2.getNickname(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getAge(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getCity(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getEvaluation(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getMP3FileName(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getTimeStamp(), QString);
}

void testJsonFile::testSimpleLoad_data() {
    commonData();
}

void testJsonFile::testSimpleLoad()
{
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    try {
        QFile file(databaseDirectory.absoluteFilePath(jsonFileName));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            QFAIL("Couldn't write to json file.");
        QTextStream out(&file);
        out << jsonData;
        file.close();

        JsonFile jsonFile = JsonFile(databaseDirectory.absoluteFilePath(jsonFileName)).load();
        qDebug() << &jsonFile;
        VERIFY_JSONFILE_PROPERTIES(jsonFile, nickname, age, city, evaluation, MP3FileName, year, month, day, hour, minute, second);
    } catch (QString s) {
        QFAIL(s.toStdString().c_str());
    }
}

void testJsonFile::testSimpleSave_data() {
    commonData();
}

void testJsonFile::testSimpleSave()
{
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime timeStamp(date, time);

    JsonFile jsonFile(databaseDirectory.absoluteFilePath(jsonFileName));
    jsonFile.setNickName(nickname);
    jsonFile.setAge(age);
    jsonFile.setCity(city);
    jsonFile.setEvaluation(evaluation);
    jsonFile.setMP3FileName(MP3FileName);
    jsonFile.setTimeStamp(timeStamp);

    jsonFile.save();

    QFile file(databaseDirectory.absoluteFilePath(jsonFileName));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Couldn't open json file.");
    QTextStream in(&file);
    auto text = in.readAll();
    file.close();

    QVERIFY2(jsonData == text, "Content of json document doesn't match the expected result.");
}

void testJsonFile::testChaining_data() {
    commonData();
}

void testJsonFile::testChaining() {
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    JsonFile o1 = JsonFile(databaseDirectory.absoluteFilePath(jsonFileName))
            .setNickName(nickname)
            .setAge(age)
            .setCity(city)
            .setEvaluation(evaluation)
            .setMP3FileName(MP3FileName)
            .setTimeStamp(QDateTime(QDate(year, month, day), QTime(hour, minute, second)));

    VERIFY_JSONFILE_PROPERTIES(o1, nickname, age, city, evaluation, MP3FileName, year, month, day, hour, minute, second);
}

void testJsonFile::testFileDoesntExist() {
    JsonFile jsonFile(databaseDirectory.absoluteFilePath("inexistentFile.json"));
    QVERIFY_EXCEPTION_THROWN(jsonFile.load(), QString);
}


void testJsonFile::testGettersSetters_data() {
    commonData();
}

template <typename T>
void testGettersSettersHelperFunction(JsonFile& jsonFile, const QString methodName, T (JsonFile::*getter)(void), JsonFile& (JsonFile::*setter)(const T&), const T value) {
    QVERIFY_EXCEPTION_THROWN((jsonFile.*getter)(), QString);
    (jsonFile.*setter)(value);
    try {
        T returnValue = (jsonFile.*getter)();
        QVERIFY2(returnValue == value, (methodName + "getter didn't return the correct nickname.").toStdString().c_str());
    } catch (QString s) {
        QFAIL(("Getting " + methodName + " after setting threw an exception.").toStdString().c_str());
    }
}

void testJsonFile::testGettersSetters() {
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime timeStamp(date, time);

    JsonFile jsonFile(databaseDirectory.absoluteFilePath(jsonFileName));

    testGettersSettersHelperFunction<QString>(jsonFile, "nickname", &JsonFile::getNickname, &JsonFile::setNickName, nickname);
    testGettersSettersHelperFunction<int>(jsonFile, "age", &JsonFile::getAge, &JsonFile::setAge, age);
    testGettersSettersHelperFunction<QString>(jsonFile, "city", &JsonFile::getCity, &JsonFile::setCity, city);
    testGettersSettersHelperFunction<int>(jsonFile, "evaluation", &JsonFile::getEvaluation, &JsonFile::setEvaluation, evaluation);
    testGettersSettersHelperFunction<QString>(jsonFile, "MP3FileName", &JsonFile::getMP3FileName, &JsonFile::setMP3FileName, MP3FileName);
    testGettersSettersHelperFunction<QString>(jsonFile, "city", &JsonFile::getCity, &JsonFile::setCity, city);
    testGettersSettersHelperFunction<QDateTime>(jsonFile, "timeStamp", &JsonFile::getTimeStamp, &JsonFile::setTimeStamp, timeStamp);

}

void testJsonFile::testIncompleteSave_data() {
    commonData();
}

void testJsonFile::testIncompleteSave() {
    QFETCH(QString, jsonFileName);
    QFETCH(QString, jsonData);
    QFETCH(QString, nickname);
    QFETCH(int, age);
    QFETCH(QString, city);
    QFETCH(int, evaluation);
    QFETCH(QString, MP3FileName);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);

    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime timeStamp(date, time);

    JsonFile jsonFile(databaseDirectory.absoluteFilePath(jsonFileName));
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(), QString);
    jsonFile.setNickName(nickname);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(), QString);
    jsonFile.setAge(age);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(), QString);
    jsonFile.setCity(city);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(), QString);
    jsonFile.setEvaluation(evaluation);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(), QString);
    jsonFile.setMP3FileName(MP3FileName);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(), QString);
    jsonFile.setTimeStamp(timeStamp);

    try {
        jsonFile.save();
    } catch (QString s) {
        QFAIL("When every parameter is set, no exception should be thrown when saving.");
    }

}

void testJsonFile::testInvalidFile_data() {
    QTest::addColumn<QString>("jsonData");

    auto jsonData1 = QString(
        "{\"nickname\": \"Maxime\","
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"filename\": \"2019-08-17 16:33:00.mp3\","
        "\"timestamp\": \"2019-08-17 16:33:00\""
        "}"
    );

    QTest::newRow("Maxime") << jsonData1;  // age is missing

    auto jsonData2 = QString(
        "{\"nickname\": \"J.J. Brun's\","
        "\"age\": 20,"
        "\"filename\": \"2020-01-15 14:45:04.mp3\","
        "\"timestamp\": \"2020-01-15 14:45:04\""
        "}"
    );

    QTest::newRow("J.J. Brun's") << jsonData2;  // city and evaluation are missing

    auto jsonData3 = QString(
        "{\"nickname\": \"Stéphane\","
        "\"age\": 21,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 5,"
        "\"timestamp\": \"2021-03-27 09:32:27\""
        "}"
    );

    QTest::newRow("Stéphane") << jsonData3;  // filename missing



    // ******* ****************************************Invalid types

    auto jsonData4 = QString(
        "{\"nickname\": \"Maxime\","
        "\"age\": \"33\","
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"filename\": \"2019-08-17 16:33:00.mp3\","
        "\"timestamp\": \"2019-08-17 16:33:00\""
        "}"
    );
    QTest::newRow("Maxime 2") << jsonData4; // age is a string instead of an int

    auto jsonData5 = QString(
        "{\"nickname\": \"J.J. Brun's\","
        "\"age\": 20,"
        "\"city\": 22190,"
        "\"evaluation\": 4,"
        "\"filename\": \"2020-01-15 14:45:04.mp3\","
        "\"timestamp\": \"2020-01-15 14:45:04\""
        "}"
    );
    QTest::newRow("J.J. Brun's 2") << jsonData5; // city is an int instead of a string

    auto jsonData6 = QString(
        "{\"nickname\": \"Stéphane\","
        "\"age\": 21,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 4.5,"
        "\"filename\": \"2021-03-27 09:32:27.mp3\","
        "\"timestamp\": \"2021-03-27 09:32:27\""
        "}"
    );
    QTest::newRow("Stéphane 2") << jsonData6; // evaluation is a float instead of an int

    auto jsonData7 = QString(
        "{\"nickname\": \"Maxime\","
        "\"age\": \"33\","
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"filename\": \"2019-08-17 16:33:00.mp3\","
        "\"timestamp\": \"1234\""
        "}"
    );
    QTest::newRow("Maxime 3") << jsonData7; // invalid timestamp

}

void testJsonFile::testInvalidFile() {
    QFETCH(QString, jsonData);

    auto jsonFileName = "test.json";

    QFile file(databaseDirectory.absoluteFilePath(jsonFileName));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        QFAIL("Couldn't write to json file.");
    QTextStream out(&file);
    out << jsonData;
    file.close();

    QVERIFY_EXCEPTION_THROWN(JsonFile jsonFile = JsonFile(databaseDirectory.absoluteFilePath(jsonFileName)).load(), QString);

}

void testJsonFile::testTestGetterAfterFailedLoad() {
    auto jsonFile = JsonFile(databaseDirectory.absoluteFilePath("inexistantFile.json"));
    jsonFile.setNickName("Maxime");
    jsonFile.setAge(33);
    jsonFile.setCity("Plérin");
    jsonFile.setEvaluation(3);
    jsonFile.setMP3FileName("2019-08-17 16:33:00.mp3");
    jsonFile.setTimeStamp(QDateTime(QDate(2019, 8, 17), QTime(16, 33, 0)));
    try {
        jsonFile.load();
    } catch (QString s) {
        // do nothing, an exception should be thrown and it's normal
    }
    QVERIFY_EXCEPTION_THROWN(jsonFile.getNickname(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getAge(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getCity(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getEvaluation(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getMP3FileName(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getTimeStamp(), QString);
}

QTEST_APPLESS_MAIN(testJsonFile)

#include "tst_testjsonfile.moc"
