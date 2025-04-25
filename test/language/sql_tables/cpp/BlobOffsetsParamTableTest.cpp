#include <cstdio>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SqliteFinalizer.h"
#include "zserio/StringConvertUtil.h"

namespace sql_tables
{
namespace blob_offsets_param_table
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class BlobOffsetsParamTableTest : public ::testing::Test
{
public:
    BlobOffsetsParamTableTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new sql_tables::TestDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~BlobOffsetsParamTableTest() override
    {
        delete m_database;
    }

    BlobOffsetsParamTableTest(const BlobOffsetsParamTableTest&) = delete;
    BlobOffsetsParamTableTest& operator=(const BlobOffsetsParamTableTest&) = delete;

    BlobOffsetsParamTableTest(BlobOffsetsParamTableTest&&) = delete;
    BlobOffsetsParamTableTest& operator=(BlobOffsetsParamTableTest&&) = delete;

protected:
    static void fillRow(BlobOffsetsParamTable::Row& row, uint32_t blobId, const StringType& name)
    {
        row.blobId = blobId;
        row.name = name;

        const uint32_t array_size = 1 + blobId;
        row.offsetsHolder = OffsetsHolder(VectorType<zserio::UInt32>(array_size));

        row.blob = ParameterizedBlob();
        ParameterizedBlob& parameterizedBlob = *row.blob;
        auto& array = parameterizedBlob.array;
        for (uint32_t i = 0; i < array_size; ++i)
        {
            array.push_back(i);
        }
    }

    static void fillRows(VectorType<BlobOffsetsParamTable::Row>& rows)
    {
        rows.clear();
        rows.resize(NUM_BLOB_OFFSETS_PARAM_TABLE_ROWS);
        for (uint32_t blobId = 0; blobId < NUM_BLOB_OFFSETS_PARAM_TABLE_ROWS; ++blobId)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(blobId);
            fillRow(rows[blobId], blobId, name);
        }
    }

    static void checkRow(const BlobOffsetsParamTable::Row& row1, const BlobOffsetsParamTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);
        ASSERT_EQ(row1.name, row2.name);
        ASSERT_EQ(row1.offsetsHolder, row2.offsetsHolder);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkRows(const VectorType<BlobOffsetsParamTable::Row>& rows1,
            const VectorType<BlobOffsetsParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr const char* DB_FILE_NAME = "language/sql_tables/blob_offsets_param_table_test.sqlite";

    static constexpr uint32_t NUM_BLOB_OFFSETS_PARAM_TABLE_ROWS = 20;

    sql_tables::TestDb* m_database;
};

TEST_F(BlobOffsetsParamTableTest, deleteTable)
{
    const std::string_view checkTableName = "blobOffsetsParamTable";
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    BlobOffsetsParamTable& testTable = m_database->getBlobOffsetsParamTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(BlobOffsetsParamTableTest, readWithoutCondition)
{
    BlobOffsetsParamTable& testTable = m_database->getBlobOffsetsParamTable();

    VectorType<BlobOffsetsParamTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    VectorType<BlobOffsetsParamTable::Row> readRows;
    BlobOffsetsParamTable::Reader reader = testTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(writtenRows, readRows);
}

TEST_F(BlobOffsetsParamTableTest, readWithCondition)
{
    BlobOffsetsParamTable& testTable = m_database->getBlobOffsetsParamTable();

    VectorType<BlobOffsetsParamTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<BlobOffsetsParamTable::Row> readRows;
    BlobOffsetsParamTable::Reader reader = testTable.createReader(condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(BlobOffsetsParamTableTest, update)
{
    BlobOffsetsParamTable& testTable = m_database->getBlobOffsetsParamTable();

    VectorType<BlobOffsetsParamTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(writtenRows);

    const uint64_t updateRowId = 3;
    BlobOffsetsParamTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "blobId=" + zserio::toString<AllocatorType>(updateRowId);
    testTable.update(updateRow, updateCondition);

    VectorType<BlobOffsetsParamTable::Row> readRows;
    BlobOffsetsParamTable::Reader reader = testTable.createReader(updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    ASSERT_EQ(1, readRows.size());

    checkRow(updateRow, readRows[0]);
}

TEST_F(BlobOffsetsParamTableTest, columnNames)
{
    ASSERT_EQ(4, BlobOffsetsParamTable::columnNames.size());
    ASSERT_EQ("blobId", BlobOffsetsParamTable::columnNames[0]);
    ASSERT_EQ("name", BlobOffsetsParamTable::columnNames[1]);
    ASSERT_EQ("offsetsHolder", BlobOffsetsParamTable::columnNames[2]);
    ASSERT_EQ("blob", BlobOffsetsParamTable::columnNames[3]);
}

} // namespace blob_offsets_param_table
} // namespace sql_tables
