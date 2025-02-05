#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_virtual_tables/fts5_virtual_table/Fts5TestDb.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SqliteFinalizer.h"

namespace sql_virtual_tables
{
namespace fts5_virtual_table
{

using AllocatorType = Fts5TestDb::AllocatorType;
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
    static void fillFts5VirtualTableRow(
            Fts5VirtualTable::Row& row, const StringType& title, const StringType& body)
    {
        row.title = title;
        row.body = body;
    }

    static void fillFts5VirtualTableRows(VectorType<Fts5VirtualTable::Row>& rows)
    {
        rows.clear();
        for (int32_t id = 0; id < NUM_VIRTUAL_TABLE_ROWS; ++id)
        {
            const StringType title = "Title" + zserio::toString<AllocatorType>(id);
            const StringType body = "Body" + zserio::toString<AllocatorType>(id);
            Fts5VirtualTable::Row row;
            fillFts5VirtualTableRow(row, title, body);
            rows.push_back(row);
        }
    }

    static void checkFts5VirtualTableRow(const Fts5VirtualTable::Row& row1, const Fts5VirtualTable::Row& row2)
    {
        ASSERT_EQ(row1.title, row2.title);
        ASSERT_EQ(row1.body, row2.body);
    }

    static void checkFts5VirtualTableRows(
            const VectorType<Fts5VirtualTable::Row>& rows1, const VectorType<Fts5VirtualTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkFts5VirtualTableRow(rows1[i], rows2[i]);
        }
    }

    bool isTableInDb()
    {
        StringType checkTableName = "fts5VirtualTable";
        StringType sqlQuery =
                "SELECT name FROM sqlite_master WHERE type='table' AND name='" + checkTableName + "'";
        std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
                m_database->connection().prepareStatement(sqlQuery));

        int result = sqlite3_step(statement.get());
        if (result == SQLITE_DONE || result != SQLITE_ROW)
        {
            return false;
        }

        const unsigned char* readTableName = sqlite3_column_text(statement.get(), 0);
        return (readTableName != nullptr && checkTableName == reinterpret_cast<const char*>(readTableName));
    }

    static constexpr int32_t NUM_VIRTUAL_TABLE_ROWS = 5;

    sql_virtual_tables::fts5_virtual_table::Fts5TestDb* m_database;
};

TEST_F(Fts5VirtualTableTest, deleteTable)
{
    ASSERT_TRUE(isTableInDb());

    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();
    testTable.deleteTable();
    ASSERT_FALSE(isTableInDb());

    testTable.createTable();
    ASSERT_TRUE(isTableInDb());
}

TEST_F(Fts5VirtualTableTest, readWithoutCondition)
{
    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();

    VectorType<Fts5VirtualTable::Row> writtenRows;
    fillFts5VirtualTableRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<Fts5VirtualTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkFts5VirtualTableRows(writtenRows, readRows);
}

TEST_F(Fts5VirtualTableTest, readWithCondition)
{
    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();

    VectorType<Fts5VirtualTable::Row> writtenRows;
    fillFts5VirtualTableRows(writtenRows);
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
    checkFts5VirtualTableRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(Fts5VirtualTableTest, update)
{
    Fts5VirtualTable& testTable = m_database->getFts5VirtualTable();

    VectorType<Fts5VirtualTable::Row> writtenRows;
    fillFts5VirtualTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType updateTitle = "Title3";
    Fts5VirtualTable::Row updateRow;
    fillFts5VirtualTableRow(updateRow, updateTitle, "UpdatedName");
    const StringType updateCondition = "title='" + updateTitle + "'";
    testTable.update(updateRow, updateCondition);

    VectorType<Fts5VirtualTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkFts5VirtualTableRow(updateRow, readRows[0]);
}

} // namespace fts5_virtual_table
} // namespace sql_virtual_tables
