#include <cstdio>
#include <limits>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace sql_tables
{
namespace column_param_table
{

using AllocatorType = TestDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ColumnParamTableTest : public ::testing::Test
{
public:
    ColumnParamTableTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new sql_tables::TestDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~ColumnParamTableTest() override
    {
        delete m_database;
    }

    ColumnParamTableTest(const ColumnParamTableTest&) = delete;
    ColumnParamTableTest& operator=(const ColumnParamTableTest&) = delete;

    ColumnParamTableTest(ColumnParamTableTest&&) = delete;
    ColumnParamTableTest& operator=(ColumnParamTableTest&&) = delete;

protected:
    static void fillColumnParamTableRow(ColumnParamTable::Row& row, uint32_t blobId, const StringType& name)
    {
        row.blobId = blobId;
        row.name = name;

        ParameterizedBlob parameterizedBlob;
        parameterizedBlob.value = PARAMETERIZED_BLOB_VALUE;
        row.blob = parameterizedBlob;
    }

    static void fillColumnParamTableRows(VectorType<ColumnParamTable::Row>& rows)
    {
        rows.clear();
        for (uint32_t blobId = 0; blobId < NUM_COLUMN_PARAM_TABLE_ROWS; ++blobId)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(blobId);
            ColumnParamTable::Row row;
            fillColumnParamTableRow(row, blobId, name);
            rows.push_back(row);
        }
    }

    static void checkColumnParamTableRow(const ColumnParamTable::Row& row1, const ColumnParamTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);
        ASSERT_EQ(row1.name, row2.name);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkColumnParamTableRows(
            const VectorType<ColumnParamTable::Row>& rows1, const VectorType<ColumnParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkColumnParamTableRow(rows1[i], rows2[i]);
        }
    }

    static constexpr const char* DB_FILE_NAME = "language/sql_tables/column_param_table_test.sqlite";

    static const uint32_t PARAMETERIZED_BLOB_VALUE = 0xABCD;
    static const uint32_t NUM_COLUMN_PARAM_TABLE_ROWS = 5;

    sql_tables::TestDb* m_database;
};

TEST_F(ColumnParamTableTest, deleteTable)
{
    std::string_view checkTableName = "columnParamTable";
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    ColumnParamTable& testTable = m_database->getColumnParamTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(ColumnParamTableTest, readWithoutCondition)
{
    ColumnParamTable& testTable = m_database->getColumnParamTable();

    VectorType<ColumnParamTable::Row> writtenRows;
    fillColumnParamTableRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<ColumnParamTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkColumnParamTableRows(writtenRows, readRows);
}

TEST_F(ColumnParamTableTest, readWithCondition)
{
    ColumnParamTable& testTable = m_database->getColumnParamTable();

    VectorType<ColumnParamTable::Row> writtenRows;
    fillColumnParamTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<ColumnParamTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkColumnParamTableRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(ColumnParamTableTest, update)
{
    ColumnParamTable& testTable = m_database->getColumnParamTable();

    VectorType<ColumnParamTable::Row> writtenRows;
    fillColumnParamTableRows(writtenRows);
    testTable.write(writtenRows);

    const uint64_t updateRowId = 3;
    ColumnParamTable::Row updateRow;
    fillColumnParamTableRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "blobId=" + zserio::toString<AllocatorType>(updateRowId);
    testTable.update(updateRow, updateCondition);

    VectorType<ColumnParamTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkColumnParamTableRow(updateRow, readRows[0]);
}

TEST_F(ColumnParamTableTest, columnNames)
{
    ASSERT_EQ(3, ColumnParamTable::columnNames.size());
    ASSERT_EQ("blobId", ColumnParamTable::columnNames[0]);
    ASSERT_EQ("name", ColumnParamTable::columnNames[1]);
    ASSERT_EQ("blob", ColumnParamTable::columnNames[2]);
}

} // namespace column_param_table
} // namespace sql_tables
