#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_virtual_columns/simple_virtual_columns/SimpleVirtualColumnsDb.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SqliteFinalizer.h"

namespace sql_virtual_columns
{
namespace simple_virtual_columns
{

using AllocatorType = SimpleVirtualColumnsDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class SimpleVirtualColumnsTest : public ::testing::Test
{
public:
    SimpleVirtualColumnsTest() :
            m_tableName("simpleVirtualColumnsTable"),
            m_virtualColumnName("content")
    {
        const StringType dbFileName("language/sql_virtual_columns/simple_virtual_columns_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new SimpleVirtualColumnsDb(dbFileName);
        m_database->createSchema();
    }

    ~SimpleVirtualColumnsTest() override
    {
        delete m_database;
    }

    SimpleVirtualColumnsTest(const SimpleVirtualColumnsTest&) = delete;
    SimpleVirtualColumnsTest& operator=(const SimpleVirtualColumnsTest&) = delete;

    SimpleVirtualColumnsTest(SimpleVirtualColumnsTest&&) = delete;
    SimpleVirtualColumnsTest& operator=(SimpleVirtualColumnsTest&&) = delete;

protected:
    static void fillSimpleVirtualColumnsTableRow(SimpleVirtualColumnsTable::Row& row, const StringType& content)
    {
        row.content = content;
    }

    static void fillSimpleVirtualColumnsTableRows(VectorType<SimpleVirtualColumnsTable::Row>& rows)
    {
        rows.clear();
        for (int32_t id = 0; id < NUM_TABLE_ROWS; ++id)
        {
            const StringType content = "Content" + zserio::toString<AllocatorType>(id);
            SimpleVirtualColumnsTable::Row row;
            fillSimpleVirtualColumnsTableRow(row, content);
            rows.push_back(row);
        }
    }

    static void checkSimpleVirtualColumnsTableRow(
            const SimpleVirtualColumnsTable::Row& row1, const SimpleVirtualColumnsTable::Row& row2)
    {
        ASSERT_EQ(row1.content, row2.content);
    }

    static void checkSimpleVirtualColumnsTableRows(const VectorType<SimpleVirtualColumnsTable::Row>& rows1,
            const VectorType<SimpleVirtualColumnsTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkSimpleVirtualColumnsTableRow(rows1[i], rows2[i]);
        }
    }

    bool isTableInDb()
    {
        StringType sqlQuery =
                "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_tableName + "'";
        std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
                m_database->connection().prepareStatement(sqlQuery));

        int result = sqlite3_step(statement.get());
        if (result == SQLITE_DONE || result != SQLITE_ROW)
        {
            return false;
        }

        const unsigned char* readTableName = sqlite3_column_text(statement.get(), 0);
        return (readTableName != nullptr && m_tableName == reinterpret_cast<const char*>(readTableName));
    }

    bool isVirtualColumnInTable()
    {
        StringType sqlQuery = "PRAGMA table_info(" + m_tableName + ")";
        std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
                m_database->connection().prepareStatement(sqlQuery));

        bool isFound = false;
        while (!isFound)
        {
            int result = sqlite3_step(statement.get());
            if (result == SQLITE_DONE || result != SQLITE_ROW)
            {
                break;
            }

            const unsigned char* readColumnName = sqlite3_column_text(statement.get(), 1);
            if (readColumnName != nullptr &&
                    m_virtualColumnName == reinterpret_cast<const char*>(readColumnName))
            {
                isFound = true;
            }
        }

        return isFound;
    }

    static constexpr int32_t NUM_TABLE_ROWS = 5;

    StringType m_tableName;
    StringType m_virtualColumnName;
    SimpleVirtualColumnsDb* m_database;
};

TEST_F(SimpleVirtualColumnsTest, deleteTable)
{
    ASSERT_TRUE(isTableInDb());

    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();
    testTable.deleteTable();
    ASSERT_FALSE(isTableInDb());

    testTable.createTable();
    ASSERT_TRUE(isTableInDb());
}

TEST_F(SimpleVirtualColumnsTest, readWithoutCondition)
{
    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();

    VectorType<SimpleVirtualColumnsTable::Row> writtenRows;
    fillSimpleVirtualColumnsTableRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<SimpleVirtualColumnsTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkSimpleVirtualColumnsTableRows(writtenRows, readRows);
}

TEST_F(SimpleVirtualColumnsTest, readWithCondition)
{
    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();

    VectorType<SimpleVirtualColumnsTable::Row> writtenRows;
    fillSimpleVirtualColumnsTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "content='Content1'";
    VectorType<SimpleVirtualColumnsTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkSimpleVirtualColumnsTableRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(SimpleVirtualColumnsTest, update)
{
    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();

    VectorType<SimpleVirtualColumnsTable::Row> writtenRows;
    fillSimpleVirtualColumnsTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType updateContent = "UpdatedContent";
    SimpleVirtualColumnsTable::Row updateRow;
    fillSimpleVirtualColumnsTableRow(updateRow, updateContent);
    const StringType updateCondition = "content='Content3'";
    testTable.update(updateRow, updateCondition);

    VectorType<SimpleVirtualColumnsTable::Row> readRows;
    const StringType readCondition = "content='" + updateContent + "'";
    auto reader = testTable.createReader(readCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkSimpleVirtualColumnsTableRow(updateRow, readRows[0]);
}

TEST_F(SimpleVirtualColumnsTest, checkVirtualColumn)
{
    ASSERT_TRUE(isVirtualColumnInTable());
}

} // namespace simple_virtual_columns
} // namespace sql_virtual_columns
