#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace multiple_pk_table
{

using AllocatorType = TestDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class MultiplePkTableTest : public ::testing::Test
{
public:
    MultiplePkTableTest()
    {
        const StringType dbFileName("language/sql_tables/multiple_pk_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~MultiplePkTableTest() override
    {
        delete m_database;
    }

    MultiplePkTableTest(const MultiplePkTableTest&) = delete;
    MultiplePkTableTest& operator=(const MultiplePkTableTest&) = delete;

    MultiplePkTableTest(MultiplePkTableTest&&) = delete;
    MultiplePkTableTest& operator=(MultiplePkTableTest&&) = delete;

protected:
    static void fillMultiplePkTableRow(MultiplePkTable::Row& row, int32_t blobId, const StringType& name)
    {
        row.blobId = blobId;
        row.age = 10;
        row.name = name;
    }

    static void fillMultiplePkTableRows(VectorType<MultiplePkTable::Row>& rows)
    {
        rows.clear();
        for (int32_t blobId = 0; blobId < NUM_MULTIPLE_PK_TABLE_ROWS; ++blobId)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(blobId);
            MultiplePkTable::Row row;
            fillMultiplePkTableRow(row, blobId, name);
            rows.push_back(row);
        }
    }

    static void checkMultiplePkTableRow(const MultiplePkTable::Row& row1, const MultiplePkTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);
        ASSERT_EQ(row1.age, row2.age);
        ASSERT_EQ(row1.name, row2.name);
    }

    static void checkMultiplePkTableRows(
            const VectorType<MultiplePkTable::Row>& rows1, const VectorType<MultiplePkTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkMultiplePkTableRow(rows1[i], rows2[i]);
        }
    }

    static constexpr int32_t NUM_MULTIPLE_PK_TABLE_ROWS = 5;

    sql_tables::TestDb* m_database;
};

TEST_F(MultiplePkTableTest, deleteTable)
{
    const StringType checkTableName("multiplePkTable");
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    MultiplePkTable& testTable = m_database->getMultiplePkTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(MultiplePkTableTest, readWithoutCondition)
{
    MultiplePkTable& testTable = m_database->getMultiplePkTable();

    VectorType<MultiplePkTable::Row> writtenRows;
    fillMultiplePkTableRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<MultiplePkTable::Row> readRows;
    MultiplePkTable::Reader reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkMultiplePkTableRows(writtenRows, readRows);
}

TEST_F(MultiplePkTableTest, readWithCondition)
{
    MultiplePkTable& testTable = m_database->getMultiplePkTable();

    VectorType<MultiplePkTable::Row> writtenRows;
    fillMultiplePkTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<MultiplePkTable::Row> readRows;
    MultiplePkTable::Reader reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkMultiplePkTableRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(MultiplePkTableTest, update)
{
    MultiplePkTable& testTable = m_database->getMultiplePkTable();

    VectorType<MultiplePkTable::Row> writtenRows;
    fillMultiplePkTableRows(writtenRows);
    testTable.write(writtenRows);

    const int32_t updateRowId = 3;
    MultiplePkTable::Row updateRow;
    fillMultiplePkTableRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "blobId=" + zserio::toString<AllocatorType>(updateRowId);
    testTable.update(updateRow, updateCondition);

    VectorType<MultiplePkTable::Row> readRows;
    MultiplePkTable::Reader reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkMultiplePkTableRow(updateRow, readRows[0]);
}

TEST_F(MultiplePkTableTest, columnNames)
{
    ASSERT_EQ(3, MultiplePkTable::columnNames.size());
    ASSERT_EQ("blobId", MultiplePkTable::columnNames[0]);
    ASSERT_EQ("age", MultiplePkTable::columnNames[1]);
    ASSERT_EQ("name", MultiplePkTable::columnNames[2]);
}

} // namespace multiple_pk_table
} // namespace sql_tables
