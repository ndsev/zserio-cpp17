#include <cstdio>
#include <string>
#include <vector>

#include "explicit_parameters/ExplicitParametersDb.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace explicit_parameters
{
namespace multiple_explicit_params
{

using AllocatorType = ExplicitParametersDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class MultipleExplicitParamsTest : public ::testing::Test
{
public:
    MultipleExplicitParamsTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new ExplicitParametersDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~MultipleExplicitParamsTest() override
    {
        delete m_database;
    }

    MultipleExplicitParamsTest(const MultipleExplicitParamsTest&) = delete;
    MultipleExplicitParamsTest& operator=(const MultipleExplicitParamsTest&) = delete;

    MultipleExplicitParamsTest(MultipleExplicitParamsTest&&) = delete;
    MultipleExplicitParamsTest& operator=(MultipleExplicitParamsTest&&) = delete;

protected:
    class MultipleParamsTableParameterProvider : public MultipleParamsTable::IParameterProvider
    {
    public:
        zserio::UInt32 count1(const zserio::View<MultipleParamsTable::Row>&) override
        {
            return MULTIPLE_PARAMS_COUNT1;
        }

        zserio::UInt32 count2(const zserio::View<MultipleParamsTable::Row>&) override
        {
            return MULTIPLE_PARAMS_COUNT2;
        }

        zserio::UInt32 count(const zserio::View<MultipleParamsTable::Row>&) override
        {
            return MULTIPLE_PARAMS_COUNT;
        }
    };

    void fillRow(MultipleParamsTable::Row& row, uint32_t id, const StringType& name)
    {
        row.id = id;
        row.name = name;

        TestBlob testBlob1;
        {
            auto& values8 = testBlob1.values8;
            auto& values16 = testBlob1.values16;
            for (uint32_t i = 0; i < MULTIPLE_PARAMS_COUNT1; ++i)
            {
                values8.push_back(static_cast<uint8_t>(id));
            }
            for (uint32_t i = 0; i < MULTIPLE_PARAMS_COUNT2; ++i)
            {
                values16.push_back(static_cast<uint16_t>(id));
            }
        }
        row.blob1 = testBlob1;

        TestBlob testBlob2;
        {
            auto& values8 = testBlob2.values8;
            auto& values16 = testBlob2.values16;
            for (uint32_t i = 0; i < MULTIPLE_PARAMS_COUNT; ++i)
            {
                values8.push_back(static_cast<uint8_t>(id + 1));
                values16.push_back(static_cast<uint16_t>(id + 1));
            }
        }
        row.blob2 = testBlob2;

        TestBlob testBlob3;
        {
            auto& values8 = testBlob3.values8;
            auto& values16 = testBlob3.values16;
            for (uint32_t i = 0; i < MULTIPLE_PARAMS_COUNT1; ++i)
            {
                values8.push_back(static_cast<uint8_t>(id + 2));
                values16.push_back(static_cast<uint16_t>(id + 2));
            }
        }
        row.blob3 = testBlob3;
    }

    void fillRows(VectorType<MultipleParamsTable::Row>& rows)
    {
        rows.clear();
        for (uint32_t id = 0; id < NUM_MULTIPLE_PARAMS_ROWS; ++id)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(id);
            MultipleParamsTable::Row row;
            fillRow(row, id, name);
            rows.push_back(row);
        }
    }

    static void checkRow(MultipleParamsTableParameterProvider& parameterProvider,
            const MultipleParamsTable::Row& row1, const MultipleParamsTable::Row& row2)
    {
        zserio::View rowView1(row1, parameterProvider);
        zserio::View rowView2(row2, parameterProvider);

        ASSERT_EQ(rowView1.id(), rowView2.id());
        ASSERT_EQ(rowView1.name(), rowView2.name());
        ASSERT_EQ(rowView1.blob1(), rowView2.blob1());
        ASSERT_EQ(rowView1.blob2(), rowView2.blob2());
        ASSERT_EQ(rowView1.blob3(), rowView2.blob3());

        // check reused explicit parameters
        ASSERT_EQ(rowView2.blob1()->count8(), rowView2.blob3()->count8());
        ASSERT_EQ(rowView2.blob1()->count8(), rowView2.blob3()->count16());
        ASSERT_EQ(rowView2.blob2()->count8(), rowView2.blob2()->count16());
    }

    static void checkRows(MultipleParamsTableParameterProvider& parameterProvider,
            const VectorType<MultipleParamsTable::Row>& rows1,
            const VectorType<MultipleParamsTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(parameterProvider, rows1[i], rows2[i]);
        }
    }

    ExplicitParametersDb* m_database;

    static const char* const DB_FILE_NAME;

    static const uint32_t NUM_MULTIPLE_PARAMS_ROWS;
    static const uint32_t MULTIPLE_PARAMS_COUNT1;
    static const uint32_t MULTIPLE_PARAMS_COUNT2;
    static const uint32_t MULTIPLE_PARAMS_COUNT;
};

const char* const MultipleExplicitParamsTest::DB_FILE_NAME =
        "language/explicit_parameters/multiple_explicit_param_test.sqlite";

const uint32_t MultipleExplicitParamsTest::NUM_MULTIPLE_PARAMS_ROWS = 5;
const uint32_t MultipleExplicitParamsTest::MULTIPLE_PARAMS_COUNT1 = 10;
const uint32_t MultipleExplicitParamsTest::MULTIPLE_PARAMS_COUNT2 = 11;
const uint32_t MultipleExplicitParamsTest::MULTIPLE_PARAMS_COUNT = 12;

TEST_F(MultipleExplicitParamsTest, readWithoutCondition)
{
    MultipleParamsTable& multipleParamsTable = m_database->getMultipleParamsTable();

    MultipleParamsTableParameterProvider parameterProvider;
    VectorType<MultipleParamsTable::Row> writtenRows;
    fillRows(writtenRows);
    multipleParamsTable.write(parameterProvider, writtenRows);

    MultipleParamsTable::Reader reader = multipleParamsTable.createReader(parameterProvider);

    VectorType<MultipleParamsTable::Row> readRows;
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(parameterProvider, writtenRows, readRows);
}

TEST_F(MultipleExplicitParamsTest, readWithCondition)
{
    MultipleParamsTable& multipleParamsTable = m_database->getMultipleParamsTable();

    MultipleParamsTableParameterProvider parameterProvider;
    VectorType<MultipleParamsTable::Row> writtenRows;
    fillRows(writtenRows);
    multipleParamsTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    MultipleParamsTable::Reader reader = multipleParamsTable.createReader(parameterProvider, condition);

    ASSERT_TRUE(reader.hasNext());
    MultipleParamsTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    const size_t expectedRowNum = 1;
    checkRow(parameterProvider, writtenRows[expectedRowNum], readRow);
}

TEST_F(MultipleExplicitParamsTest, update)
{
    MultipleParamsTable& multipleParamsTable = m_database->getMultipleParamsTable();

    MultipleParamsTableParameterProvider parameterProvider;
    VectorType<MultipleParamsTable::Row> writtenRows;
    fillRows(writtenRows);
    multipleParamsTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    MultipleParamsTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "id=" + zserio::toString<AllocatorType>(updateRowId);
    multipleParamsTable.update(parameterProvider, updateRow, updateCondition);

    MultipleParamsTable::Reader reader = multipleParamsTable.createReader(parameterProvider, updateCondition);
    ASSERT_TRUE(reader.hasNext());
    MultipleParamsTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());
    checkRow(parameterProvider, updateRow, readRow);
}

} // namespace multiple_explicit_params
} // namespace explicit_parameters
