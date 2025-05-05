#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace const_param_table
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ConstParamTableTest : public ::testing::Test
{
public:
    ConstParamTableTest()
    {
        const StringType dbFileName("language/sql_tables/const_param_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~ConstParamTableTest() override
    {
        delete m_database;
    }

    ConstParamTableTest(const ConstParamTableTest&) = delete;
    ConstParamTableTest& operator=(const ConstParamTableTest&) = delete;

    ConstParamTableTest(ConstParamTableTest&&) = delete;
    ConstParamTableTest& operator=(ConstParamTableTest&&) = delete;

protected:
    static void fillRow(ConstParamTable::Row& row, uint32_t blobId, const StringType& name)
    {
        row.blobId = blobId;
        row.name = name;

        ParameterizedBlob parameterizedBlob;
        parameterizedBlob.value = PARAMETERIZED_BLOB_VALUE;
        row.blob = parameterizedBlob;
    }

    static void fillRows(VectorType<ConstParamTable::Row>& rows)
    {
        rows.clear();
        for (uint32_t blobId = 0; blobId < NUM_CONST_PARAM_TABLE_ROWS; ++blobId)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(blobId);
            ConstParamTable::Row row;
            fillRow(row, blobId, name);
            rows.push_back(row);
        }
    }

    static void checkRow(const ConstParamTable::Row& row1, const ConstParamTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);
        ASSERT_EQ(row1.name, row2.name);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkRows(
            const VectorType<ConstParamTable::Row>& rows1, const VectorType<ConstParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr uint32_t PARAMETERIZED_BLOB_VALUE = 0xABCD;
    static constexpr uint32_t NUM_CONST_PARAM_TABLE_ROWS = 5;

    sql_tables::TestDb* m_database;
};

TEST_F(ConstParamTableTest, deleteTable)
{
    const StringType checkTableName("constParamTable");
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    ConstParamTable& testTable = m_database->getConstParamTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(ConstParamTableTest, readWithoutCondition)
{
    ConstParamTable& testTable = m_database->getConstParamTable();

    VectorType<ConstParamTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<ConstParamTable::Row> readRows;
    auto reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(writtenRows, readRows);
}

TEST_F(ConstParamTableTest, readWithCondition)
{
    ConstParamTable& testTable = m_database->getConstParamTable();

    VectorType<ConstParamTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<ConstParamTable::Row> readRows;
    auto reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(ConstParamTableTest, update)
{
    ConstParamTable& testTable = m_database->getConstParamTable();

    VectorType<ConstParamTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const uint64_t updateRowId = 3;
    ConstParamTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "blobId=" + zserio::toString<AllocatorType>(updateRowId);
    testTable.update(updateRow, updateCondition);

    VectorType<ConstParamTable::Row> readRows;
    auto reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkRow(updateRow, readRows[0]);
}

TEST_F(ConstParamTableTest, columnNames)
{
    ASSERT_EQ(3, ConstParamTable::columnNames.size());
    ASSERT_EQ("blobId", ConstParamTable::columnNames[0]);
    ASSERT_EQ("name", ConstParamTable::columnNames[1]);
    ASSERT_EQ("blob", ConstParamTable::columnNames[2]);
}

} // namespace const_param_table
} // namespace sql_tables
