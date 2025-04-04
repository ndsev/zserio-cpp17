#include <cstdio>
#include <string>
#include <vector>

#include "explicit_parameters/ExplicitParametersDb.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace explicit_parameters
{
namespace explicit_same_as_field
{

using allocator_type = ExplicitParametersDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class ExplicitSameAsFieldTest : public ::testing::Test
{
public:
    ExplicitSameAsFieldTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new ExplicitParametersDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~ExplicitSameAsFieldTest() override
    {
        delete m_database;
    }

    ExplicitSameAsFieldTest(const ExplicitSameAsFieldTest&) = delete;
    ExplicitSameAsFieldTest& operator=(const ExplicitSameAsFieldTest&) = delete;

    ExplicitSameAsFieldTest(ExplicitSameAsFieldTest&&) = delete;
    ExplicitSameAsFieldTest& operator=(ExplicitSameAsFieldTest&&) = delete;

protected:
    class SameAsFieldTableParameterProvider : public SameAsFieldTable::IParameterProvider
    {
    public:
        zserio::UInt32 count(const zserio::View<SameAsFieldTable::Row>&) override
        {
            return SAME_AS_FIELD_TABLE_COUNT_EXPLICIT;
        }
    };

    void fillRow(SameAsFieldTable::Row& row, uint32_t id, const StringType& name)
    {
        row.id = id;
        row.name = name;
        row.count = SAME_AS_FIELD_TABLE_COUNT;

        TestBlob testBlob;
        auto& values = testBlob.values;
        for (uint32_t i = 0; i < SAME_AS_FIELD_TABLE_COUNT; ++i)
        {
            values.push_back(static_cast<uint8_t>(id));
        }
        row.blob = testBlob;

        TestBlob testBlobExplicit;
        auto& valuesExplicit = testBlobExplicit.values;
        for (uint32_t i = 0; i < SAME_AS_FIELD_TABLE_COUNT_EXPLICIT; ++i)
        {
            valuesExplicit.push_back(static_cast<uint8_t>(id + 1));
        }
        row.blobExplicit = testBlobExplicit;
    }

    void fillRows(VectorType<SameAsFieldTable::Row>& rows)
    {
        rows.clear();
        rows.reserve(NUM_SAME_AS_FIELD_TABLE_ROWS);
        for (uint32_t id = 0; id < NUM_SAME_AS_FIELD_TABLE_ROWS; ++id)
        {
            const StringType name = "Name" + zserio::toString<allocator_type>(id);
            SameAsFieldTable::Row row;
            fillRow(row, id, name);
            rows.push_back(row);
        }
    }

    static void checkRow(SameAsFieldTableParameterProvider& parameterProvider,
            const SameAsFieldTable::Row& row1, const SameAsFieldTable::Row& row2)
    {
        zserio::View rowView1(row1, parameterProvider);
        zserio::View rowView2(row2, parameterProvider);

        ASSERT_EQ(rowView1.id(), rowView2.id());
        ASSERT_EQ(rowView1.name(), rowView2.name());
        ASSERT_EQ(rowView1.count(), rowView2.count());
        ASSERT_EQ(rowView1.blob(), rowView2.blob());
        ASSERT_EQ(rowView1.blobExplicit(), rowView2.blobExplicit());
    }

    static void checkRows(SameAsFieldTableParameterProvider& parameterProvider,
            const VectorType<SameAsFieldTable::Row>& rows1, const VectorType<SameAsFieldTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(parameterProvider, rows1[i], rows2[i]);
        }
    }

    ExplicitParametersDb* m_database;

    static constexpr const char* const DB_FILE_NAME =
            "language/explicit_parameters/explicit_same_as_field_test.sqlite";

    static constexpr uint32_t NUM_SAME_AS_FIELD_TABLE_ROWS = 5;
    static constexpr uint32_t SAME_AS_FIELD_TABLE_COUNT = 10;
    static constexpr uint32_t SAME_AS_FIELD_TABLE_COUNT_EXPLICIT = 11;
};

TEST_F(ExplicitSameAsFieldTest, readWithoutCondition)
{
    SameAsFieldTable& sameAsFieldTable = m_database->getSameAsFieldTable();

    SameAsFieldTableParameterProvider parameterProvider;
    VectorType<SameAsFieldTable::Row> writtenRows;
    fillRows(writtenRows);
    sameAsFieldTable.write(parameterProvider, writtenRows);

    SameAsFieldTable::Reader reader = sameAsFieldTable.createReader(parameterProvider);

    VectorType<SameAsFieldTable::Row> readRows;
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(parameterProvider, writtenRows, readRows);
}

TEST_F(ExplicitSameAsFieldTest, readWithCondition)
{
    SameAsFieldTable& sameAsFieldTable = m_database->getSameAsFieldTable();

    SameAsFieldTableParameterProvider parameterProvider;
    VectorType<SameAsFieldTable::Row> writtenRows;
    fillRows(writtenRows);
    sameAsFieldTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    SameAsFieldTable::Reader reader = sameAsFieldTable.createReader(parameterProvider, condition);

    ASSERT_TRUE(reader.hasNext());
    SameAsFieldTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    const size_t expectedRowNum = 1;
    checkRow(parameterProvider, writtenRows[expectedRowNum], readRow);
}

TEST_F(ExplicitSameAsFieldTest, update)
{
    SameAsFieldTable& sameAsFieldTable = m_database->getSameAsFieldTable();

    SameAsFieldTableParameterProvider parameterProvider;
    VectorType<SameAsFieldTable::Row> writtenRows;
    fillRows(writtenRows);
    sameAsFieldTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    SameAsFieldTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "id=" + zserio::toString<allocator_type>(updateRowId);
    sameAsFieldTable.update(parameterProvider, updateRow, updateCondition);

    SameAsFieldTable::Reader reader = sameAsFieldTable.createReader(parameterProvider, updateCondition);
    ASSERT_TRUE(reader.hasNext());
    SameAsFieldTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());
    checkRow(parameterProvider, updateRow, readRow);
}

} // namespace explicit_same_as_field
} // namespace explicit_parameters
