#include <QtTest>

#include <QDir>
#include "jsonfile.h"
#include "config.h"

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
    void testAutoAssignment_data();
    void testAutoAssignment();
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
    void testGetterAfterFailedLoad();       // inexistent file
    void testGetterAfterFailedLoad2_data();
    void testGetterAfterFailedLoad2();      // invalid file

private:
    QDir databaseDirectory;
    void commonData();
    void invalidCommonData();

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
    JsonFile jsonFile;
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

    auto MP3FileName1 = "2019-08-17_16-33-00.mp3";
    auto jsonFileName1 = "2019-08-17_16-33-00.json";
    auto jsonData1 = QString(
        "{\n"
        "    \"age\": 33,\n"
        "    \"city\": \"Plérin\",\n"
        "    \"evaluation\": 3,\n"
        "    \"nickname\": \"Maxime\",\n"
        "    \"timestamp\": \"2019-08-17 16:33:00\"\n"
        "}\n"
    );
    QTest::newRow("Maxime") << jsonFileName1 << jsonData1 << "Maxime" << 33 << "Plérin" << 3 << MP3FileName1 << 2019 << 8 << 17 << 16 << 33 << 0;


    auto MP3FileName2 = "2020-01-15_14-45-04.mp3";
    auto jsonFileName2 = "2020-01-15_14-45-04.json";
    auto jsonData2 = QString(
        "{\n"
        "    \"age\": 20,\n"
        "    \"city\": \"Plérin\",\n"
        "    \"evaluation\": 4,\n"
        "    \"nickname\": \"J.J. Brun's\",\n"
        "    \"timestamp\": \"2020-01-15 14:45:04\"\n"
        "}\n"
    );
    QTest::newRow("J.J. Brun's") << jsonFileName2 << jsonData2 << "J.J. Brun's" << 20 << "Plérin" << 4 << MP3FileName2 << 2020 << 1 << 15 << 14 << 45 << 4;


    auto MP3FileName3 = "2021-03-27_09-32-27.mp3";
    auto jsonFileName3 = "2021-03-27_09-32-27.mp3";
    auto jsonData3 = QString(
        "{\n"
        "    \"age\": 21,\n"
        "    \"city\": \"Plérin\",\n"
        "    \"evaluation\": 5,\n"
        "    \"nickname\": \"Stéphane\",\n"
        "    \"timestamp\": \"2021-03-27 09:32:27\"\n"
        "}\n"
    );
    QTest::newRow("Stéphane") << jsonFileName3 << jsonData3 << "Stéphane" << 21 << "Plérin" << 5 << MP3FileName3 << 2021 << 3 << 27 << 9 << 32 << 27;
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
        JsonFile* o1 = new JsonFile;
        o1->setNickName(nickname)
                .setAge(age)
                .setCity(city)
                .setEvaluation(evaluation)
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
        JsonFile* o1 = new JsonFile;
        o1->setNickName(nickname)
                .setAge(age)
                .setCity(city)
                .setEvaluation(evaluation)
                .setTimeStamp(QDateTime(QDate(year, month, day), QTime(hour, minute, second)));
        JsonFile o2;
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

    JsonFile o1;
    o1.setNickName("Maxime")
            .setAge(33)
            .setCity("Plérin")
            .setEvaluation(5)
            .setTimeStamp(QDateTime(QDate(2019, 8, 31), QTime(12, 13, 0)));
    JsonFile o2;
    o1 = o2;

    QVERIFY_EXCEPTION_THROWN(o2.getNickname(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getAge(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getCity(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getEvaluation(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getMP3FileName(), QString);
    QVERIFY_EXCEPTION_THROWN(o2.getTimeStamp(), QString);
}


void testJsonFile::testAutoAssignment_data() {
    commonData();
}

void testJsonFile::testAutoAssignment() {
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

    JsonFile o1;
    o1.setNickName(nickname)
            .setAge(age)
            .setCity(city)
            .setEvaluation(evaluation)
            .setTimeStamp(QDateTime(QDate(year, month, day), QTime(hour, minute, second)));
    o1 = o1; // auto assignment
    VERIFY_JSONFILE_PROPERTIES(o1, nickname, age, city, evaluation, MP3FileName, year, month, day, hour, minute, second);
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

        JsonFile jsonFile = JsonFile().load(databaseDirectory.absoluteFilePath(jsonFileName));
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

    JsonFile jsonFile;
    jsonFile.setNickName(nickname);
    jsonFile.setAge(age);
    jsonFile.setCity(city);
    jsonFile.setEvaluation(evaluation);
    jsonFile.setTimeStamp(timeStamp);

    jsonFile.save(databaseDirectory.absoluteFilePath(jsonFileName));

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

    JsonFile o1 = JsonFile()
            .setNickName(nickname)
            .setAge(age)
            .setCity(city)
            .setEvaluation(evaluation)
            .setTimeStamp(QDateTime(QDate(year, month, day), QTime(hour, minute, second)));

    VERIFY_JSONFILE_PROPERTIES(o1, nickname, age, city, evaluation, MP3FileName, year, month, day, hour, minute, second);
}

void testJsonFile::testFileDoesntExist() {
    JsonFile jsonFile;
    QVERIFY_EXCEPTION_THROWN(jsonFile.load(databaseDirectory.absoluteFilePath("inexistentFile.json")), QString);
}


void testJsonFile::testGettersSetters_data() {
    commonData();
}

template <typename T>
void testGettersSettersHelperFunction(JsonFile& jsonFile, const QString methodName, T (JsonFile::*getter)(void) const, JsonFile& (JsonFile::*setter)(const T&), const T value) {
    QVERIFY_EXCEPTION_THROWN((jsonFile.*getter)(), QString);
    (jsonFile.*setter)(value);
    try {
        T returnValue = (jsonFile.*getter)();
        QVERIFY2(returnValue == value, (methodName + "getter didn't return the correct value.").toStdString().c_str());
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

    JsonFile jsonFile;

    testGettersSettersHelperFunction<QString>(jsonFile, "nickname", &JsonFile::getNickname, &JsonFile::setNickName, nickname);
    testGettersSettersHelperFunction<int>(jsonFile, "age", &JsonFile::getAge, &JsonFile::setAge, age);
    testGettersSettersHelperFunction<QString>(jsonFile, "city", &JsonFile::getCity, &JsonFile::setCity, city);
    testGettersSettersHelperFunction<int>(jsonFile, "evaluation", &JsonFile::getEvaluation, &JsonFile::setEvaluation, evaluation);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getMP3FileName(), QString);   // there is no setter for getMP3FileName, only a getter. MP3FileName is derived from the timeStamp.
    testGettersSettersHelperFunction<QDateTime>(jsonFile, "timeStamp", &JsonFile::getTimeStamp, &JsonFile::setTimeStamp, timeStamp);
    auto returnedMP3FileName = jsonFile.getMP3FileName();
    QVERIFY2(returnedMP3FileName == MP3FileName, ("JsonFile::getMP3FileName : expected \"" + MP3FileName + "\", got \"" + returnedMP3FileName + "\".").toStdString().c_str());
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

    QString absoluteJsonFilePath = databaseDirectory.absoluteFilePath(jsonFileName);

    JsonFile jsonFile;
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(absoluteJsonFilePath), QString);
    jsonFile.setNickName(nickname);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(absoluteJsonFilePath), QString);
    jsonFile.setAge(age);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(absoluteJsonFilePath), QString);
    jsonFile.setCity(city);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(absoluteJsonFilePath), QString);
    jsonFile.setEvaluation(evaluation);
    QVERIFY_EXCEPTION_THROWN(jsonFile.save(absoluteJsonFilePath), QString);
    jsonFile.setTimeStamp(timeStamp);

    try {
        jsonFile.save(databaseDirectory.absoluteFilePath(jsonFileName));
    } catch (QString s) {
        QFAIL("When every parameter is set, no exception should be thrown when saving.");
    }

}

void testJsonFile::invalidCommonData() {
    QTest::addColumn<QString>("jsonData");

    auto jsonData1 = QString(
        "{\"nickname\": \"Maxime\","
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"timestamp\": \"2019-08-17 16:33:00\""
        "}"
    );

    QTest::newRow("Maxime") << jsonData1;  // age is missing

    auto jsonData2 = QString(
        "{\"nickname\": \"J.J. Brun's\","
        "\"age\": 20,"
        "\"timestamp\": \"2020-01-15 14:45:04\""
        "}"
    );

    QTest::newRow("J.J. Brun's") << jsonData2;  // city and evaluation are missing

    auto jsonData3 = QString(
        "{\"age\": 21,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 5,"
        "\"timestamp\": \"2021-03-27 09:32:27\""
        "}"
    );

    QTest::newRow("Stéphane") << jsonData3;  // nickname is missing



    // ******* ****************************************Invalid types

    auto jsonData4 = QString(
        "{\"nickname\": \"Maxime\","
        "\"age\": \"33\","
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"timestamp\": \"2019-08-17_16:33:00\""
        "}"
    );
    QTest::newRow("Maxime 2") << jsonData4; // age is a string instead of an int

    auto jsonData5 = QString(
        "{\"nickname\": \"J.J. Brun's\","
        "\"age\": 20,"
        "\"city\": 22190,"
        "\"evaluation\": 4,"
        "\"timestamp\": \"2020-01-15 14:45:04\""
        "}"
    );
    QTest::newRow("J.J. Brun's 2") << jsonData5; // city is an int instead of a string

    auto jsonData6 = QString(
        "{\"nickname\": \"Stéphane\","
        "\"age\": 21,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 4.5,"
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

void testJsonFile::testInvalidFile_data() {
    invalidCommonData();
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

    QVERIFY_EXCEPTION_THROWN(JsonFile jsonFile = JsonFile().load(databaseDirectory.absoluteFilePath(jsonFileName)), QString);

}

void testJsonFile::testGetterAfterFailedLoad() {
    JsonFile jsonFile;
    jsonFile.setNickName("Maxime");
    jsonFile.setAge(33);
    jsonFile.setCity("Plérin");
    jsonFile.setEvaluation(3);
    jsonFile.setTimeStamp(QDateTime(QDate(2019, 8, 17), QTime(16, 33, 0)));
    try {
        jsonFile.load(databaseDirectory.absoluteFilePath("inexistantFile.json"));
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

void testJsonFile::testGetterAfterFailedLoad2_data() {
    invalidCommonData();
}

void testJsonFile::testGetterAfterFailedLoad2() {
    QFETCH(QString, jsonData);

    auto jsonFileName = "test.json";

    QFile file(databaseDirectory.absoluteFilePath(jsonFileName));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        QFAIL("Couldn't write to json file.");
    QTextStream out(&file);
    out << jsonData;
    file.close();

    JsonFile jsonFile;
    jsonFile.setNickName("Monique");
    jsonFile.setAge(62);
    jsonFile.setCity("Plérin");
    jsonFile.setEvaluation(5);
    jsonFile.setTimeStamp(QDateTime(QDate(2019, 9, 7), QTime(16, 42, 35)));
    try {
        jsonFile.load(databaseDirectory.absoluteFilePath(jsonFileName));
    } catch (QString s) {
        // do nothing, an exception should be thrown and it's normal
    }
    QVERIFY_EXCEPTION_THROWN(jsonFile.getNickname(), QString);          // A failed load should clear every field previously set
    QVERIFY_EXCEPTION_THROWN(jsonFile.getAge(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getCity(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getEvaluation(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getMP3FileName(), QString);
    QVERIFY_EXCEPTION_THROWN(jsonFile.getTimeStamp(), QString);
}

QTEST_APPLESS_MAIN(testJsonFile)

#include "tst_testjsonfile.moc"
