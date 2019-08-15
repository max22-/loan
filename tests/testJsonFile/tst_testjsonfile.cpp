#include <QtTest>

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
    void test_case1();

};

testJsonFile::testJsonFile()
{

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

void testJsonFile::test_case1()
{
    JsonFile jsonFile("/tmp/test.json");
}

QTEST_APPLESS_MAIN(testJsonFile)

#include "tst_testjsonfile.moc"
