#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_virtual_tables/fts3_virtual_table/Fts3TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_virtual_tables
{
namespace fts3_virtual_table
{

using AllocatorType = Fts3TestDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class Fts3VirtualTableTest : public ::testing::Test
{
public:
    Fts3VirtualTableTest()
    {
        const StringType dbFileName("language/sql_virtual_tables/fts3_virtual_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_virtual_tables::fts3_virtual_table::Fts3TestDb(dbFileName);
        m_database->createSchema();
    }

    ~Fts3VirtualTableTest() override
    {
        delete m_database;
    }

    Fts3VirtualTableTest(const Fts3VirtualTableTest&) = delete;
    Fts3VirtualTableTest& operator=(const Fts3VirtualTableTest&) = delete;

    Fts3VirtualTableTest(Fts3VirtualTableTest&&) = delete;
    Fts3VirtualTableTest& operator=(Fts3VirtualTableTest&&) = delete;

protected:
    static void fillFts3VirtualTableRow(
            Fts3VirtualTable::Row& row, const StringType& title, const StringType& body)
    {
        row.title = title;
        row.body = body;
    }

    static void fillFts3VirtualTableRows(VectorType<Fts3VirtualTable::Row>& rows)
    {
        rows.clear();
        for (int32_t id = 0; id < NUM_VIRTUAL_TABLE_ROWS; ++id)
        {
            const StringType title = "Title" + zserio::toString<AllocatorType>(id);
            const StringType body = "Body" + zserio::toString<AllocatorType>(id);
            Fts3VirtualTable::Row row;
            fillFts3VirtualTableRow(row, title, body);
            rows.push_back(row);
        }
    }

    static void checkFts3VirtualTableRow(const Fts3VirtualTable::Row& row1, const Fts3VirtualTable::Row& row2)
    {
        ASSERT_EQ(row1.title, row2.title);
        ASSERT_EQ(row1.body, row2.body);
    }

    static void checkFts3VirtualTableRows(
            const VectorType<Fts3VirtualTable::Row>& rows1, const VectorType<Fts3VirtualTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkFts3VirtualTableRow(rows1[i], rows2[i]);
        }
    }

    static constexpr int32_t NUM_VIRTUAL_TABLE_ROWS = 5;

    sql_virtual_tables::fts3_virtual_table::Fts3TestDb* m_database;
};

TEST_F(Fts3VirtualTableTest, deleteTable)
{
    const StringType checkTableName = "fts3VirtualTable";
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    Fts3VirtualTable& testTable = m_database->getFts3VirtualTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(Fts3VirtualTableTest, readWithoutCondition)
{
    Fts3VirtualTable& testTable = m_database->getFts3VirtualTable();

    VectorType<Fts3VirtualTable::Row> writtenRows;
    fillFts3VirtualTableRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<Fts3VirtualTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkFts3VirtualTableRows(writtenRows, readRows);
}

TEST_F(Fts3VirtualTableTest, readWithCondition)
{
    Fts3VirtualTable& testTable = m_database->getFts3VirtualTable();

    VectorType<Fts3VirtualTable::Row> writtenRows;
    fillFts3VirtualTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "body='Body1'";
    VectorType<Fts3VirtualTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkFts3VirtualTableRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(Fts3VirtualTableTest, update)
{
    Fts3VirtualTable& testTable = m_database->getFts3VirtualTable();

    VectorType<Fts3VirtualTable::Row> writtenRows;
    fillFts3VirtualTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType updateTitle = "Title3";
    Fts3VirtualTable::Row updateRow;
    fillFts3VirtualTableRow(updateRow, updateTitle, "UpdatedName");
    const StringType updateCondition = "title='" + updateTitle + "'";
    testTable.update(updateRow, updateCondition);

    VectorType<Fts3VirtualTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkFts3VirtualTableRow(updateRow, readRows[0]);
}

} // namespace fts3_virtual_table
} // namespace sql_virtual_tables
