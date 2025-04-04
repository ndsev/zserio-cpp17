#include <cstdio>
#include <string>
#include <vector>

#include "explicit_parameters/ExplicitParametersDb.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace explicit_parameters
{
namespace explicit_enum_param
{

using allocator_type = ExplicitParametersDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class ExplicitEnumParamTest : public ::testing::Test
{
public:
    ExplicitEnumParamTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new ExplicitParametersDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~ExplicitEnumParamTest() override
    {
        delete m_database;
    }

    ExplicitEnumParamTest(const ExplicitEnumParamTest&) = delete;
    ExplicitEnumParamTest& operator=(const ExplicitEnumParamTest&) = delete;

    ExplicitEnumParamTest(ExplicitEnumParamTest&&) = delete;
    ExplicitEnumParamTest& operator=(ExplicitEnumParamTest&&) = delete;

protected:
    class EnumParamTableParameterProvider : public EnumParamTable::IParameterProvider
    {
    public:
        TestEnum count1(const zserio::View<EnumParamTable::Row>&) override
        {
            return ENUM_PARAM_TABLE_COUNT1;
        }

        TestEnum count2(const zserio::View<EnumParamTable::Row>&) override
        {
            return ENUM_PARAM_TABLE_COUNT2;
        }
    };

    void fillRow(EnumParamTable::Row& row, uint32_t id, const StringType& name)
    {
        row.id = id;
        row.name = name;

        TestBlob testBlob1;
        auto& values1 = testBlob1.values;
        for (uint32_t i = 0; i < ::zserio::enumToValue(ENUM_PARAM_TABLE_COUNT1); ++i)
        {
            values1.push_back(static_cast<uint8_t>(id));
        }
        row.blob1 = testBlob1;

        TestBlob testBlob2;
        auto& values2 = testBlob2.values;
        for (uint32_t i = 0; i < ::zserio::enumToValue(ENUM_PARAM_TABLE_COUNT2); ++i)
        {
            values2.push_back(static_cast<uint8_t>(id + 1));
        }
        row.blob2 = testBlob2;

        TestBlob testBlob3;
        auto& values3 = testBlob3.values;
        for (uint32_t i = 0; i < ::zserio::enumToValue(ENUM_PARAM_TABLE_COUNT1); ++i)
        {
            values3.push_back(static_cast<uint8_t>(id + 2));
        }
        row.blob3 = testBlob3;
    }

    void fillRows(VectorType<EnumParamTable::Row>& rows)
    {
        rows.clear();
        for (uint32_t id = 0; id < NUM_ENUM_PARAM_TABLE_ROWS; ++id)
        {
            const StringType name = "Name" + zserio::toString<allocator_type>(id);
            EnumParamTable::Row row;
            fillRow(row, id, name);
            rows.push_back(row);
        }
    }

    static void checkRow(EnumParamTableParameterProvider& parameterProvider, const EnumParamTable::Row& row1,
            const EnumParamTable::Row& row2)
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

    static void checkRows(EnumParamTableParameterProvider& parameterProvider,
            const VectorType<EnumParamTable::Row>& rows1, const VectorType<EnumParamTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(parameterProvider, rows1[i], rows2[i]);
        }
    }

    ExplicitParametersDb* m_database;

    static constexpr const char* DB_FILE_NAME = "language/explicit_parameters/explicit_enum_param_test.sqlite";

    static constexpr uint32_t NUM_ENUM_PARAM_TABLE_ROWS = 5;
    static constexpr TestEnum ENUM_PARAM_TABLE_COUNT1 = TestEnum::TEN;
    static constexpr TestEnum ENUM_PARAM_TABLE_COUNT2 = TestEnum::ELEVEN;
};

TEST_F(ExplicitEnumParamTest, readWithoutCondition)
{
    EnumParamTable& enumParamTable = m_database->getEnumParamTable();

    EnumParamTableParameterProvider parameterProvider;
    VectorType<EnumParamTable::Row> writtenRows;
    fillRows(writtenRows);
    enumParamTable.write(parameterProvider, writtenRows);

    EnumParamTable::Reader reader = enumParamTable.createReader(parameterProvider);

    VectorType<EnumParamTable::Row> readRows;
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(parameterProvider, writtenRows, readRows);
}

TEST_F(ExplicitEnumParamTest, readWithCondition)
{
    EnumParamTable& enumParamTable = m_database->getEnumParamTable();

    EnumParamTableParameterProvider parameterProvider;
    VectorType<EnumParamTable::Row> writtenRows;
    fillRows(writtenRows);
    enumParamTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    EnumParamTable::Reader reader = enumParamTable.createReader(parameterProvider, condition);

    ASSERT_TRUE(reader.hasNext());
    EnumParamTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    const size_t expectedRowNum = 1;
    checkRow(parameterProvider, writtenRows[expectedRowNum], readRow);
}

TEST_F(ExplicitEnumParamTest, update)
{
    EnumParamTable& enumParamTable = m_database->getEnumParamTable();

    EnumParamTableParameterProvider parameterProvider;
    VectorType<EnumParamTable::Row> writtenRows;
    fillRows(writtenRows);
    enumParamTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    EnumParamTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "id=" + zserio::toString<allocator_type>(updateRowId);
    enumParamTable.update(parameterProvider, updateRow, updateCondition);

    EnumParamTable::Reader reader = enumParamTable.createReader(parameterProvider, updateCondition);

    ASSERT_TRUE(reader.hasNext());
    EnumParamTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());
    checkRow(parameterProvider, updateRow, readRow);
}

} // namespace explicit_enum_param
} // namespace explicit_parameters
