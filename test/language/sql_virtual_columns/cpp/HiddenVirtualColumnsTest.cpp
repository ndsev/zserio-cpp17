#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_virtual_columns/hidden_virtual_columns/HiddenVirtualColumnsDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/ValidationSqliteUtil.h"

namespace sql_virtual_columns
{
namespace hidden_virtual_columns
{

using AllocatorType = HiddenVirtualColumnsDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class HiddenVirtualColumnsTest : public ::testing::Test
{
public:
    HiddenVirtualColumnsTest() :
            m_tableName("hiddenVirtualColumnsTable")
    {
        const StringType dbFileName("language/sql_virtual_columns/hidden_virtual_columns_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new HiddenVirtualColumnsDb(dbFileName);
        m_database->createSchema();
    }

    ~HiddenVirtualColumnsTest() override
    {
        delete m_database;
    }

    HiddenVirtualColumnsTest(const HiddenVirtualColumnsTest&) = delete;
    HiddenVirtualColumnsTest& operator=(const HiddenVirtualColumnsTest&) = delete;

    HiddenVirtualColumnsTest(HiddenVirtualColumnsTest&&) = delete;
    HiddenVirtualColumnsTest& operator=(HiddenVirtualColumnsTest&&) = delete;

protected:
    static void fillRow(HiddenVirtualColumnsTable::Row& row, int64_t docId, const StringType& searchTags)
    {
        row.docId = docId;
        const uint16_t languageCode = 1;
        row.languageCode = languageCode;
        row.searchTags = searchTags;
        const uint32_t frequency = 0xDEAD;
        row.frequency = frequency;
    }

    static void fillRows(VectorType<HiddenVirtualColumnsTable::Row>& rows)
    {
        rows.clear();
        for (int32_t id = 0; id < NUM_TABLE_ROWS; ++id)
        {
            const StringType searchTags = "Search Tags" + zserio::toString<AllocatorType>(id);
            HiddenVirtualColumnsTable::Row row;
            fillRow(row, id, searchTags);
            rows.push_back(row);
        }
    }

    static void checkRow(const HiddenVirtualColumnsTable::Row& row1, const HiddenVirtualColumnsTable::Row& row2)
    {
        ASSERT_EQ(row1.docId, row2.docId);
        ASSERT_EQ(row1.languageCode, row2.languageCode);
        ASSERT_EQ(row1.searchTags, row2.searchTags);
        ASSERT_EQ(row1.frequency, row2.frequency);
    }

    static void checkRows(const VectorType<HiddenVirtualColumnsTable::Row>& rows1,
            const VectorType<HiddenVirtualColumnsTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    bool isHiddenVirtualColumnInTable(const StringType& columnName)
    {
        return zserio::ValidationSqliteUtil<AllocatorType>::isColumnInTable(
                m_database->connection(), "", m_tableName, columnName, AllocatorType());
    }

    static constexpr int32_t NUM_TABLE_ROWS = 5;

    StringType m_tableName;
    HiddenVirtualColumnsDb* m_database;
};

TEST_F(HiddenVirtualColumnsTest, deleteTable)
{
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, m_tableName));

    HiddenVirtualColumnsTable& testTable = m_database->getHiddenVirtualColumnsTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, m_tableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, m_tableName));
}

TEST_F(HiddenVirtualColumnsTest, readWithoutCondition)
{
    HiddenVirtualColumnsTable& testTable = m_database->getHiddenVirtualColumnsTable();

    VectorType<HiddenVirtualColumnsTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<HiddenVirtualColumnsTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(writtenRows, readRows);
}

TEST_F(HiddenVirtualColumnsTest, readWithCondition)
{
    HiddenVirtualColumnsTable& testTable = m_database->getHiddenVirtualColumnsTable();

    VectorType<HiddenVirtualColumnsTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "searchTags='Search Tags1'";
    VectorType<HiddenVirtualColumnsTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(HiddenVirtualColumnsTest, update)
{
    HiddenVirtualColumnsTable& testTable = m_database->getHiddenVirtualColumnsTable();

    VectorType<HiddenVirtualColumnsTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const int64_t updateDocId = 1;
    HiddenVirtualColumnsTable::Row updateRow;
    fillRow(updateRow, updateDocId, "Updated Search Tags");
    const StringType updateCondition = "docId='" + zserio::toString<AllocatorType>(updateDocId) + "'";
    testTable.update(updateRow, updateCondition);

    VectorType<HiddenVirtualColumnsTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkRow(updateRow, readRows[0]);
}

TEST_F(HiddenVirtualColumnsTest, checkVirtualColumn)
{
    ASSERT_TRUE(isHiddenVirtualColumnInTable("docId"));
    ASSERT_TRUE(isHiddenVirtualColumnInTable("languageCode"));
}

} // namespace hidden_virtual_columns
} // namespace sql_virtual_columns
