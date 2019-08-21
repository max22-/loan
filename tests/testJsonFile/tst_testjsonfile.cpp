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
    void testSimpleLoad_data();
    void testSimpleLoad();
    void testSimpleSave_data();
    void testSimpleSave();

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
        "\"filename\": \"2019-08-17 16:33:00.mp3\""
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
        "\"filename\": \"2020-01-15 14:45:04.mp3\""
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
        "\"filename\": \"2021-03-27 09:32:27.mp3\""
        "\"timestamp\": \"2021-03-27 09:32:27\""
        "}"
    );

    QTest::newRow("Stéphane") << timeStamp3 + QString(".json") << jsonData3 << "Stéphane" << 21 << "Plérin" << 5 << timeStamp3 + QString(".mp3") << 2021 << 3 << 27 << 9 << 32 << 27;
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

    QFile file(databaseDirectory.absoluteFilePath(jsonFileName));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        QFAIL("Couldn't write to json file.");
    QTextStream out(&file);
    out << jsonData;
    file.close();

    JsonFile jsonFile = JsonFile(databaseDirectory.absoluteFilePath(jsonFileName)).load();
    QVERIFY2(jsonFile.getNickname().compare(nickname) == 0, "Nicknames don't match.");
    QVERIFY2(jsonFile.getAge() == age, "Ages don't match");
    QVERIFY2(jsonFile.getCity().compare(city) == 0, "Cities don't match.");
    QVERIFY2(jsonFile.getEvaluation() == evaluation, "Evaluations don't match.");
    QVERIFY2(jsonFile.getMP3FileName().compare(MP3FileName) == 0, "MP3 file names don't match.");
    QVERIFY2(jsonFile.getTimeStamp().date().year() == year, "Years don't match.");
    QVERIFY2(jsonFile.getTimeStamp().date().month() == month, "Months don't match.");
    QVERIFY2(jsonFile.getTimeStamp().date().day() == day, "Days don't match.");
    QVERIFY2(jsonFile.getTimeStamp().time().hour() == hour, "Hours don't match.");
    QVERIFY2(jsonFile.getTimeStamp().time().minute() == minute, "Minutes don't match.");
    QVERIFY2(jsonFile.getTimeStamp().time().second()== second, "Secondes don't match.");
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

    QVERIFY2(jsonData.compare(text) == 0, "Content of json document doesn't match the expected result.");
}

QTEST_APPLESS_MAIN(testJsonFile)

#include "tst_testjsonfile.moc"
