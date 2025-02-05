#include <cstdio>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace sql_tables
{
namespace blob_param_table
{

using AllocatorType = TestDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class BlobParamTableTest : public ::testing::Test
{
public:
    BlobParamTableTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new sql_tables::TestDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~BlobParamTableTest() override
    {
        delete m_database;
    }

    BlobParamTableTest(const BlobParamTableTest&) = delete;
    BlobParamTableTest& operator=(const BlobParamTableTest&) = delete;

    BlobParamTableTest(BlobParamTableTest&&) = delete;
    BlobParamTableTest& operator=(BlobParamTableTest&&) = delete;

protected:
    static void fillBlobParamTableRowWithNullValues(BlobParamTable::Row& row, uint32_t blobId)
    {
        row.blobId = blobId;
        row.name.reset();
        row.parameters.reset();
        row.blob.reset();
    }

    static void fillBlobParamTableRowsWithNullValues(VectorType<BlobParamTable::Row>& rows)
    {
        rows.clear();
        rows.resize(NUM_BLOB_PARAM_TABLE_ROWS);
        for (uint32_t blobId = 0; blobId < NUM_BLOB_PARAM_TABLE_ROWS; ++blobId)
        {
            fillBlobParamTableRowWithNullValues(rows[blobId], blobId);
        }
    }

    static void fillBlobParamTableRow(BlobParamTable::Row& row, uint32_t blobId, const StringType& name)
    {
        row.blobId = blobId;
        row.name = name;

        // count must be changing because we need to catch error (in the old C++ generator)
        const uint32_t count = 1 + blobId;
        Parameters parameters(count);
        row.parameters = parameters;

        ParameterizedBlob parameterizedBlob;
        VectorType<zserio::UInt32>& array = parameterizedBlob.array;
        for (uint32_t i = 0; i < count; ++i)
        {
            array.push_back(i);
        }
        row.blob = parameterizedBlob;
    }

    static void fillBlobParamTableRows(VectorType<BlobParamTable::Row>& rows)
    {
        rows.clear();
        rows.resize(NUM_BLOB_PARAM_TABLE_ROWS);
        for (uint32_t blobId = 0; blobId < NUM_BLOB_PARAM_TABLE_ROWS; ++blobId)
        {
            fillBlobParamTableRow(rows[blobId], blobId, "Name" + zserio::toString<AllocatorType>(blobId));
        }
    }

    static void checkBlobParamTableRow(const BlobParamTable::Row& row1, const BlobParamTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);
        ASSERT_EQ(row1.name, row2.name);
        ASSERT_EQ(row1.parameters, row2.parameters);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkBlobParamTableRows(
            const VectorType<BlobParamTable::Row>& rows1, const VectorType<BlobParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkBlobParamTableRow(rows1[i], rows2[i]);
        }
    }

    static const char* const DB_FILE_NAME;

    static const uint32_t NUM_BLOB_PARAM_TABLE_ROWS;

    sql_tables::TestDb* m_database;
};

const char* const BlobParamTableTest::DB_FILE_NAME = "language/sql_tables/blob_param_table_test.sqlite";

const uint32_t BlobParamTableTest::NUM_BLOB_PARAM_TABLE_ROWS = 20;

TEST_F(BlobParamTableTest, deleteTable)
{
    const StringType checkTableName = "blobParamTable";
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    BlobParamTable& testTable = m_database->getBlobParamTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(BlobParamTableTest, readWithoutCondition)
{
    BlobParamTable& testTable = m_database->getBlobParamTable();

    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<BlobParamTable::Row> readRows;
    BlobParamTable::Reader reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkBlobParamTableRows(writtenRows, readRows);
}

TEST_F(BlobParamTableTest, readWithoutConditionWithNullValues)
{
    BlobParamTable& testTable = m_database->getBlobParamTable();

    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRowsWithNullValues(writtenRows);
    testTable.write(writtenRows);

    VectorType<BlobParamTable::Row> readRows;
    BlobParamTable::Reader reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkBlobParamTableRows(writtenRows, readRows);
}

TEST_F(BlobParamTableTest, readWithCondition)
{
    BlobParamTable& testTable = m_database->getBlobParamTable();

    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<BlobParamTable::Row> readRows;
    BlobParamTable::Reader reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkBlobParamTableRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(BlobParamTableTest, update)
{
    BlobParamTable& testTable = m_database->getBlobParamTable();

    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRows(writtenRows);
    testTable.write(writtenRows);

    const uint64_t updateRowId = 3;
    BlobParamTable::Row updateRow;
    fillBlobParamTableRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "blobId=" + zserio::toString<AllocatorType>(updateRowId);
    testTable.update(updateRow, updateCondition);

    VectorType<BlobParamTable::Row> readRows;
    BlobParamTable::Reader reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkBlobParamTableRow(updateRow, readRows[0]);
}

} // namespace blob_param_table
} // namespace sql_tables
