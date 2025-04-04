#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_virtual_columns/simple_virtual_columns/SimpleVirtualColumnsDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_virtual_columns
{
namespace simple_virtual_columns
{

using allocator_type = SimpleVirtualColumnsDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

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
    static void fillRow(SimpleVirtualColumnsTable::Row& row, const StringType& content)
    {
        row.content = content;
    }

    static void fillRows(VectorType<SimpleVirtualColumnsTable::Row>& rows)
    {
        rows.clear();
        for (int32_t id = 0; id < NUM_TABLE_ROWS; ++id)
        {
            const StringType content = "Content" + zserio::toString<allocator_type>(id);
            SimpleVirtualColumnsTable::Row row;
            fillRow(row, content);
            rows.push_back(row);
        }
    }

    static void checkRow(const SimpleVirtualColumnsTable::Row& row1, const SimpleVirtualColumnsTable::Row& row2)
    {
        ASSERT_EQ(row1.content, row2.content);
    }

    static void checkRows(const VectorType<SimpleVirtualColumnsTable::Row>& rows1,
            const VectorType<SimpleVirtualColumnsTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr int32_t NUM_TABLE_ROWS = 5;

    StringType m_tableName;
    StringType m_virtualColumnName;
    SimpleVirtualColumnsDb* m_database;
};

TEST_F(SimpleVirtualColumnsTest, deleteTable)
{
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, m_tableName));

    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, m_tableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, m_tableName));
}

TEST_F(SimpleVirtualColumnsTest, readWithoutCondition)
{
    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();

    VectorType<SimpleVirtualColumnsTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<SimpleVirtualColumnsTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(writtenRows, readRows);
}

TEST_F(SimpleVirtualColumnsTest, readWithCondition)
{
    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();

    VectorType<SimpleVirtualColumnsTable::Row> writtenRows;
    fillRows(writtenRows);
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
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(SimpleVirtualColumnsTest, update)
{
    SimpleVirtualColumnsTable& testTable = m_database->getSimpleVirtualColumnsTable();

    VectorType<SimpleVirtualColumnsTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType updateContent = "UpdatedContent";
    SimpleVirtualColumnsTable::Row updateRow;
    fillRow(updateRow, updateContent);
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

    checkRow(updateRow, readRows[0]);
}

TEST_F(SimpleVirtualColumnsTest, checkVirtualColumn)
{
    ASSERT_TRUE(test_utils::isVirtualColumnInTable(*m_database, m_tableName, m_virtualColumnName));
}

} // namespace simple_virtual_columns
} // namespace sql_virtual_columns
