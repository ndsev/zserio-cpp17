#include <cstdio>
#include <string>
#include <vector>

#include "explicit_parameters/ExplicitParametersDb.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace explicit_parameters
{
namespace explicit_bitmask_param
{

using AllocatorType = ExplicitParametersDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ExplicitBitmaskParamTest : public ::testing::Test
{
public:
    ExplicitBitmaskParamTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new ExplicitParametersDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~ExplicitBitmaskParamTest() override
    {
        delete m_database;
    }

    ExplicitBitmaskParamTest(const ExplicitBitmaskParamTest&) = delete;
    ExplicitBitmaskParamTest& operator=(const ExplicitBitmaskParamTest&) = delete;

    ExplicitBitmaskParamTest(ExplicitBitmaskParamTest&&) = delete;
    ExplicitBitmaskParamTest& operator=(ExplicitBitmaskParamTest&&) = delete;

protected:
    class BitmaskParamTableParameterProvider : public BitmaskParamTable::IParameterProvider
    {
    public:
        TestBitmask count1(const zserio::View<BitmaskParamTable::Row>&) override
        {
            return BITMASK_PARAM_TABLE_COUNT1;
        }

        TestBitmask count2(const zserio::View<BitmaskParamTable::Row>&) override
        {
            return BITMASK_PARAM_TABLE_COUNT2;
        }
    };

    void fillRow(BitmaskParamTable::Row& row, uint32_t id, const StringType& name)
    {
        row.id = id;
        row.name = name;

        TestBlob testBlob1;
        auto& values1 = testBlob1.values;
        for (uint32_t i = 0; i < BITMASK_PARAM_TABLE_COUNT1.getValue(); ++i)
        {
            values1.push_back(static_cast<uint8_t>(id));
        }
        row.blob1 = testBlob1;

        TestBlob testBlob2;
        auto& values2 = testBlob2.values;
        for (uint32_t i = 0; i < BITMASK_PARAM_TABLE_COUNT2.getValue(); ++i)
        {
            values2.push_back(static_cast<uint8_t>(id + 1));
        }
        row.blob2 = testBlob2;

        TestBlob testBlob3;
        auto& values3 = testBlob3.values;
        for (uint32_t i = 0; i < BITMASK_PARAM_TABLE_COUNT1.getValue(); ++i)
        {
            values3.push_back(static_cast<uint8_t>(id + 2));
        }
        row.blob3 = testBlob3;
    }

    void fillRows(VectorType<BitmaskParamTable::Row>& rows)
    {
        rows.clear();
        for (uint32_t id = 0; id < NUM_BITMASK_PARAM_TABLE_ROWS; ++id)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(id);
            BitmaskParamTable::Row row;
            fillRow(row, id, name);
            rows.push_back(row);
        }
    }

    static void checkRow(BitmaskParamTableParameterProvider& parameterProvider,
            const BitmaskParamTable::Row& row1, const BitmaskParamTable::Row& row2)
    {
        zserio::View rowView1(row1, parameterProvider);
        zserio::View rowView2(row2, parameterProvider);

        ASSERT_EQ(rowView1.id(), rowView2.id());
        ASSERT_EQ(rowView1.name(), rowView2.name());
        ASSERT_EQ(rowView1.blob1(), rowView2.blob1());
        ASSERT_EQ(rowView1.blob2(), rowView2.blob2());
        ASSERT_EQ(rowView1.blob3(), rowView2.blob3());

        // check reused explicit count parameter
        ASSERT_EQ(rowView2.blob1()->count(), rowView2.blob3()->count());
    }

    static void checkRows(BitmaskParamTableParameterProvider& parameterProvider,
            const VectorType<BitmaskParamTable::Row>& rows1, const VectorType<BitmaskParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(parameterProvider, rows1[i], rows2[i]);
        }
    }

    ExplicitParametersDb* m_database;

    static constexpr const char* DB_FILE_NAME =
            "language/explicit_parameters/explicit_bitmask_param_test.sqlite";

    static constexpr uint32_t NUM_BITMASK_PARAM_TABLE_ROWS = 5;
    static constexpr TestBitmask BITMASK_PARAM_TABLE_COUNT1 = TestBitmask::Values::TEN;
    static constexpr TestBitmask BITMASK_PARAM_TABLE_COUNT2 = TestBitmask::Values::ELEVEN;
};

TEST_F(ExplicitBitmaskParamTest, readWithoutCondition)
{
    BitmaskParamTable& bitmaskParamTable = m_database->getBitmaskParamTable();

    BitmaskParamTableParameterProvider parameterProvider;
    VectorType<BitmaskParamTable::Row> writtenRows;
    fillRows(writtenRows);
    bitmaskParamTable.write(parameterProvider, writtenRows);

    BitmaskParamTable::Reader reader = bitmaskParamTable.createReader(parameterProvider);

    VectorType<BitmaskParamTable::Row> readRows;
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(parameterProvider, writtenRows, readRows);
}

TEST_F(ExplicitBitmaskParamTest, readWithCondition)
{
    BitmaskParamTable& bitmaskParamTable = m_database->getBitmaskParamTable();

    BitmaskParamTableParameterProvider parameterProvider;
    VectorType<BitmaskParamTable::Row> writtenRows;
    fillRows(writtenRows);
    bitmaskParamTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    BitmaskParamTable::Reader reader = bitmaskParamTable.createReader(parameterProvider, condition);

    ASSERT_TRUE(reader.hasNext());
    BitmaskParamTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    const size_t expectedRowNum = 1;
    checkRow(parameterProvider, writtenRows[expectedRowNum], readRow);
}

TEST_F(ExplicitBitmaskParamTest, update)
{
    BitmaskParamTable& bitmaskParamTable = m_database->getBitmaskParamTable();

    BitmaskParamTableParameterProvider parameterProvider;
    VectorType<BitmaskParamTable::Row> writtenRows;
    fillRows(writtenRows);
    bitmaskParamTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    BitmaskParamTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "id=" + zserio::toString<AllocatorType>(updateRowId);
    bitmaskParamTable.update(parameterProvider, updateRow, updateCondition);

    BitmaskParamTable::Reader reader = bitmaskParamTable.createReader(parameterProvider, updateCondition);

    ASSERT_TRUE(reader.hasNext());
    BitmaskParamTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());
    checkRow(parameterProvider, updateRow, readRow);
}

} // namespace explicit_bitmask_param
} // namespace explicit_parameters
