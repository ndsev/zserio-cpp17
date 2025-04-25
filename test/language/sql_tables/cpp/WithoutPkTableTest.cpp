#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace without_pk_table
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class WithoutPkTableTest : public ::testing::Test
{
public:
    WithoutPkTableTest()
    {
        const StringType dbFileName("language/sql_tables/without_pk_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~WithoutPkTableTest() override
    {
        delete m_database;
    }

    WithoutPkTableTest(const WithoutPkTableTest&) = delete;
    WithoutPkTableTest& operator=(const WithoutPkTableTest&) = delete;

    WithoutPkTableTest(WithoutPkTableTest&&) = delete;
    WithoutPkTableTest& operator=(WithoutPkTableTest&&) = delete;

protected:
    static void fillRow(WithoutPkTable::Row& row, int32_t identifier, const StringType& name)
    {
        row.identifier = identifier;
        row.name = name;
    }

    static void fillRows(VectorType<WithoutPkTable::Row>& rows)
    {
        rows.clear();
        for (int32_t identifier = 0; identifier < NUM_WITHOUT_PK_TABLE_ROWS; ++identifier)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(identifier);
            WithoutPkTable::Row row;
            fillRow(row, identifier, name);
            rows.push_back(row);
        }
    }

    static void checkRow(const WithoutPkTable::Row& row1, const WithoutPkTable::Row& row2)
    {
        ASSERT_EQ(row1.identifier, row2.identifier);
        ASSERT_EQ(row1.name, row2.name);
    }

    static void checkRows(
            const VectorType<WithoutPkTable::Row>& rows1, const VectorType<WithoutPkTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr int32_t NUM_WITHOUT_PK_TABLE_ROWS = 5;

    sql_tables::TestDb* m_database;
};

TEST_F(WithoutPkTableTest, deleteTable)
{
    const StringType checkTableName("withoutPkTable");
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    WithoutPkTable& testTable = m_database->getWithoutPkTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(WithoutPkTableTest, readWithoutCondition)
{
    WithoutPkTable& testTable = m_database->getWithoutPkTable();

    VectorType<WithoutPkTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<WithoutPkTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(writtenRows, readRows);
}

TEST_F(WithoutPkTableTest, readWithCondition)
{
    WithoutPkTable& testTable = m_database->getWithoutPkTable();

    VectorType<WithoutPkTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<WithoutPkTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(WithoutPkTableTest, update)
{
    WithoutPkTable& testTable = m_database->getWithoutPkTable();

    VectorType<WithoutPkTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const int32_t updateRowId = 3;
    WithoutPkTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "identifier=" + zserio::toString<AllocatorType>(updateRowId);
    testTable.update(updateRow, updateCondition);

    VectorType<WithoutPkTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkRow(updateRow, readRows[0]);
}

TEST_F(WithoutPkTableTest, columnNames)
{
    ASSERT_EQ(2, WithoutPkTable::columnNames.size());
    ASSERT_EQ("identifier", WithoutPkTable::columnNames[0]);
    ASSERT_EQ("name", WithoutPkTable::columnNames[1]);
}

} // namespace without_pk_table
} // namespace sql_tables
