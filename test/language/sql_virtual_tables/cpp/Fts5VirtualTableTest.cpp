#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_virtual_tables/fts5_virtual_table/Fts5TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_virtual_tables
{
namespace fts5_virtual_table
{

using AllocatorType = Fts5TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class Fts5VirtualTableTest : public ::testing::Test
{
public:
    Fts5VirtualTableTest()
    {
        const StringType dbFileName("language/sql_virtual_tables/fts5_virtual_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_virtual_tables::fts5_virtual_table::Fts5TestDb(dbFileName);
        m_database->createSchema();
    }

    ~Fts5VirtualTableTest() override
    {
        delete m_database;
    }

    Fts5VirtualTableTest(const Fts5VirtualTableTest&) = delete;
    Fts5VirtualTableTest& operator=(const Fts5VirtualTableTest&) = delete;

    Fts5VirtualTableTest(Fts5VirtualTableTest&&) = delete;
    Fts5VirtualTableTest& operator=(Fts5VirtualTableTest&&) = delete;

protected:
    static void fillRow(Fts5VirtualTable::Row& row, const StringType& title, const StringType& body)
    {
        row.title = title;
        row.body = body;
    }

    static void fillRows(VectorType<Fts5VirtualTable::Row>& rows)
    {
        rows.clear();
        for (int32_t id = 0; id < NUM_VIRTUAL_TABLE_ROWS; ++id)
        {
            const StringType title = "Title" + zserio::toString<AllocatorType>(id);
            const StringType body = "Body" + zserio::toString<AllocatorType>(id);
            Fts5VirtualTable::Row row;
            fillRow(row, title, body);
            rows.push_back(row);
        }
    }

    static void checkRow(const Fts5VirtualTable::Row& row1, const Fts5VirtualTable::Row& row2)
    {
        ASSERT_EQ(row1.title, row2.title);
        ASSERT_EQ(row1.body, row2.body);
    }

    static void checkRows(
            const VectorType<Fts5VirtualTable::Row>& rows1, const VectorType<Fts5VirtualTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr int32_t NUM_VIRTUAL_TABLE_ROWS = 5;

    sql_virtual_tables::fts5_virtual_table::Fts5TestDb* m_database;
};

TEST_F(Fts5VirtualTableTest, deleteTable)
{
    const StringType checkTableName = "fts5VirtualTable";
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(Fts5VirtualTableTest, readWithoutCondition)
{
    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();

    VectorType<Fts5VirtualTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<Fts5VirtualTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(writtenRows, readRows);
}

TEST_F(Fts5VirtualTableTest, readWithCondition)
{
    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();

    VectorType<Fts5VirtualTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "body='Body1'";
    VectorType<Fts5VirtualTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(Fts5VirtualTableTest, update)
{
    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();

    VectorType<Fts5VirtualTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType updateTitle = "Title3";
    Fts5VirtualTable::Row updateRow;
    fillRow(updateRow, updateTitle, "UpdatedName");
    const StringType updateCondition = "title='" + updateTitle + "'";
    testTable.update(updateRow, updateCondition);

    VectorType<Fts5VirtualTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkRow(updateRow, readRows[0]);
}

} // namespace fts5_virtual_table
} // namespace sql_virtual_tables
