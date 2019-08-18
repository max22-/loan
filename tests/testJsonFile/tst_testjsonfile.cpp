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
    void testSimpleSave();

private:
    QDir databaseDirectory;

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

void testJsonFile::testSimpleLoad_data() {
    QTest::addColumn<QString>("json");
    QTest::addColumn<QString>("nickname");
    QTest::addColumn<int>("age");
    QTest::addColumn<QString>("city");
    QTest::addColumn<int>("evaluation");
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("year");
    QTest::addColumn<int>("month");
    QTest::addColumn<int>("day");
    QTest::addColumn<int>("hour");
    QTest::addColumn<int>("minute");
    QTest::addColumn<int>("second");

    auto json1 = QString(
        "{\"nickname\": \"Maxime\","
        "\"age\": 33,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 3,"
        "\"filename\": \"2019-08-17 16:33:00.mp3\""
        "\"timestamp\": \"2019-08-17 16:33:00\""
        "}"
    );

    QTest::newRow("Maxime") << json1 << "Maxime" << 33 << "Plérin" << 3 << "2019-08-17 16:33:00.mp3" << 2019 << 8 << 17 << 16 << 33 << 0;

    auto json2 = QString(
        "{\"nickname\": \"J.J. Brun's\","
        "\"age\": 20,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 4,"
        "\"filename\": \"2020-01-15 14:45:04.mp3\""
        "\"timestamp\": \"2020-01-15 14:45:04\""
        "}"
    );

    QTest::newRow("J.J. Brun's") << json2 << "J.J. Brun's" << 20 << "Plérin" << 4 << "2020-01-15 14:45:04.mp3" << 2020 << 1 << 15 << 14 << 45 << 4;

    auto json3 = QString(
        "{\"nickname\": \"Stéphane\","
        "\"age\": 21,"
        "\"city\": \"Plérin\","
        "\"evaluation\": 5,"
        "\"filename\": \"2021-03-27 09:32:27.mp3\""
        "\"timestamp\": \"2021-03-27 09:32:27\""
        "}"
    );

    QTest::newRow("Stéphane") << json3 << "Stéphane" << 21 << "Plérin" << 5 << "2021-03-27 09:32:27.mp3" << 2021 << 3 << 27 << 9 << 32 << 27;

}

void testJsonFile::testSimpleLoad()
{

    JsonFile jsonFile("/tmp/test.json");
}

void testJsonFile::testSimpleSave()
{

}

QTEST_APPLESS_MAIN(testJsonFile)

#include "tst_testjsonfile.moc"
