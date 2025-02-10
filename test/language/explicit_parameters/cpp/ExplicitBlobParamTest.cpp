#include <cstdio>
#include <string>
#include <vector>

#include "explicit_parameters/ExplicitParametersDb.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"
#include "zserio/Vector.h"

namespace explicit_parameters
{
namespace explicit_blob_param
{

using AllocatorType = ExplicitParametersDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ExplicitBlobParamTest : public ::testing::Test
{
public:
    ExplicitBlobParamTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new ExplicitParametersDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~ExplicitBlobParamTest() override
    {
        delete m_database;
    }

    ExplicitBlobParamTest(const ExplicitBlobParamTest&) = delete;
    ExplicitBlobParamTest& operator=(const ExplicitBlobParamTest&) = delete;

    ExplicitBlobParamTest(ExplicitBlobParamTest&&) = delete;
    ExplicitBlobParamTest& operator=(ExplicitBlobParamTest&&) = delete;

protected:
    class BlobParamTableParameterProvider : public BlobParamTable::IParameterProvider
    {
    public:
        BlobParamTableParameterProvider()
        {
            m_headerParam.count = BLOB_PARAM_TABLE_HEADER_COUNT;
            m_blob.count = BLOB_PARAM_TABLE_BLOB_COUNT;
        }

        zserio::View<Header> headerParam(const zserio::View<BlobParamTable::Row>&) override
        {
            return zserio::View(m_headerParam);
        }

        zserio::View<Header> blob(const zserio::View<BlobParamTable::Row>&) override
        {
            return zserio::View(m_blob);
        }

    private:
        Header m_headerParam;
        Header m_blob;
    };

    void fillBlobParamTableRow(BlobParamTable::Row& row, uint32_t id, const StringType& name)
    {
        row.id = id;
        row.name = name;

        TestBlob testBlob1;
        auto& values1 = testBlob1.values;
        for (uint32_t i = 0; i < BLOB_PARAM_TABLE_HEADER_COUNT; ++i)
        {
            values1.push_back(static_cast<uint8_t>(id));
        }
        row.blob1 = testBlob1;

        TestBlob testBlob2;
        auto& values2 = testBlob2.values;
        for (uint32_t i = 0; i < BLOB_PARAM_TABLE_BLOB_COUNT; ++i)
        {
            values2.push_back(static_cast<uint8_t>(id + 1));
        }
        row.blob2 = testBlob2;

        TestBlob testBlob3;
        auto& values3 = testBlob3.values;
        for (uint32_t i = 0; i < BLOB_PARAM_TABLE_HEADER_COUNT; ++i)
        {
            values3.push_back(static_cast<uint8_t>(id + 2));
        }
        row.blob3 = testBlob3;
    }

    void fillBlobParamTableRows(VectorType<BlobParamTable::Row>& rows)
    {
        rows.clear();
        rows.resize(NUM_BLOB_PARAM_TABLE_ROWS);
        for (uint32_t id = 0; id < NUM_BLOB_PARAM_TABLE_ROWS; ++id)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(id);
            fillBlobParamTableRow(rows[id], id, name);
        }
    }

    static void checkBlobParamTableRow(BlobParamTableParameterProvider& parameterProvider,
            const BlobParamTable::Row& row1, const BlobParamTable::Row& row2)
    {
        zserio::View rowView1(row1, parameterProvider);
        zserio::View rowView2(row2, parameterProvider);

        ASSERT_EQ(rowView1.id(), rowView2.id());
        ASSERT_EQ(rowView1.name(), rowView2.name());
        ASSERT_EQ(rowView1.blob1(), rowView2.blob1());
        ASSERT_EQ(rowView1.blob2(), rowView2.blob2());
        ASSERT_EQ(rowView1.blob3(), rowView2.blob3());

        // check reused explicit parameters
        ASSERT_EQ(rowView2.blob1()->blob().count(), rowView2.blob3()->blob().count());
        // check that even address of the reused explicit header parameter is the same!
        ASSERT_EQ(rowView2.blob1()->blob(), rowView2.blob3()->blob());
    }

    static void checkBlobParamTableRows(BlobParamTableParameterProvider& parameterProvider,
            const VectorType<BlobParamTable::Row>& rows1, const VectorType<BlobParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkBlobParamTableRow(parameterProvider, rows1[i], rows2[i]);
        }
    }

    ExplicitParametersDb* m_database;

    static constexpr const char* DB_FILE_NAME = "language/explicit_parameters/explicit_blob_param_test.sqlite";

    static const uint32_t NUM_BLOB_PARAM_TABLE_ROWS = 5;
    static const uint32_t BLOB_PARAM_TABLE_HEADER_COUNT = 10;
    static const uint32_t BLOB_PARAM_TABLE_BLOB_COUNT = 11;
};

TEST_F(ExplicitBlobParamTest, readWithoutCondition)
{
    BlobParamTable& blobParamTable = m_database->getBlobParamTable();

    BlobParamTableParameterProvider parameterProvider;
    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRows(writtenRows);
    blobParamTable.write(parameterProvider, writtenRows);

    BlobParamTable::Reader reader = blobParamTable.createReader(parameterProvider);

    VectorType<BlobParamTable::Row> readRows;
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkBlobParamTableRows(parameterProvider, writtenRows, readRows);
}

TEST_F(ExplicitBlobParamTest, readWithCondition)
{
    BlobParamTable& blobParamTable = m_database->getBlobParamTable();

    BlobParamTableParameterProvider parameterProvider;
    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRows(writtenRows);
    blobParamTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    BlobParamTable::Reader reader = blobParamTable.createReader(parameterProvider, condition);

    ASSERT_TRUE(reader.hasNext());
    BlobParamTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    const size_t expectedRowNum = 1;
    checkBlobParamTableRow(parameterProvider, writtenRows[expectedRowNum], readRow);
}

TEST_F(ExplicitBlobParamTest, update)
{
    BlobParamTable& blobParamTable = m_database->getBlobParamTable();

    BlobParamTableParameterProvider parameterProvider;
    VectorType<BlobParamTable::Row> writtenRows;
    fillBlobParamTableRows(writtenRows);
    blobParamTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    BlobParamTable::Row updateRow;
    fillBlobParamTableRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "id=" + zserio::toString<AllocatorType>(updateRowId);
    blobParamTable.update(parameterProvider, updateRow, updateCondition);

    BlobParamTable::Reader reader = blobParamTable.createReader(parameterProvider, updateCondition);
    ASSERT_TRUE(reader.hasNext());
    BlobParamTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());
    checkBlobParamTableRow(parameterProvider, updateRow, readRow);
}

} // namespace explicit_blob_param
} // namespace explicit_parameters
