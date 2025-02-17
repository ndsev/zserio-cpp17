#include <cstdio>
#include <string>
#include <vector>

#include "explicit_parameters/ExplicitParametersDb.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace explicit_parameters
{
namespace multiple_with_same_name
{

using AllocatorType = ExplicitParametersDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class MultipleWithSameNameTest : public ::testing::Test
{
public:
    MultipleWithSameNameTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new ExplicitParametersDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~MultipleWithSameNameTest() override
    {
        delete m_database;
    }

    MultipleWithSameNameTest(const MultipleWithSameNameTest&) = delete;
    MultipleWithSameNameTest& operator=(const MultipleWithSameNameTest&) = delete;

    MultipleWithSameNameTest(MultipleWithSameNameTest&&) = delete;
    MultipleWithSameNameTest& operator=(MultipleWithSameNameTest&&) = delete;

protected:
    class MultipleParamsTableParameterProvider : public MultipleWithSameNameTable::IParameterProvider
    {
    public:
        zserio::UInt32 param1(const zserio::View<MultipleWithSameNameTable::Row>&) override
        {
            return PARAM1;
        }

        zserio::Float32 param2(const zserio::View<MultipleWithSameNameTable::Row>&) override
        {
            return PARAM2;
        }
    };

    void fillRow(MultipleWithSameNameTable::Row& row, uint32_t id, const StringType& name)
    {
        row.id = id;
        row.name = name;

        row.parameterized1 = Parameterized1(id * 10);
        row.parameterized2 = Parameterized2(static_cast<float>(id) * 1.5F);
    }

    void fillRows(VectorType<MultipleWithSameNameTable::Row>& rows)
    {
        rows.clear();
        for (uint32_t id = 0; id < NUM_ROWS; ++id)
        {
            const StringType name = "Name" + zserio::toString<AllocatorType>(id);
            MultipleWithSameNameTable::Row row;
            fillRow(row, id, name);
            rows.push_back(row);
        }
    }

    static void checkRow(MultipleParamsTableParameterProvider& parameterProvider,
            const MultipleWithSameNameTable::Row& row1, const MultipleWithSameNameTable::Row& row2)
    {
        zserio::View rowView1(row1, parameterProvider);
        zserio::View rowView2(row2, parameterProvider);

        ASSERT_EQ(rowView1.id(), rowView2.id());
        ASSERT_EQ(rowView1.name(), rowView2.name());
        ASSERT_EQ(rowView1.parameterized1(), rowView2.parameterized1());
        ASSERT_EQ(rowView1.parameterized2(), rowView2.parameterized2());
    }

    static void checkRows(MultipleParamsTableParameterProvider& parameterProvider,
            const VectorType<MultipleWithSameNameTable::Row>& rows1,
            const VectorType<MultipleWithSameNameTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(parameterProvider, rows1[i], rows2[i]);
        }
    }

    ExplicitParametersDb* m_database;

    static constexpr const char* DB_FILE_NAME =
            "language/explicit_parameters/multiple_with_same_name_test.sqlite";

    static constexpr uint32_t NUM_ROWS = 5;
    static constexpr uint32_t PARAM1 = 100;
    static constexpr float PARAM2 = 10.0F;
};

TEST_F(MultipleWithSameNameTest, readWithoutCondition)
{
    MultipleWithSameNameTable& multipleWithSameNameTable = m_database->getMultipleWithSameNameTable();

    MultipleParamsTableParameterProvider parameterProvider;
    VectorType<MultipleWithSameNameTable::Row> writtenRows;
    fillRows(writtenRows);
    multipleWithSameNameTable.write(parameterProvider, writtenRows);

    MultipleWithSameNameTable::Reader reader = multipleWithSameNameTable.createReader(parameterProvider);

    VectorType<MultipleWithSameNameTable::Row> readRows;
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }
    checkRows(parameterProvider, writtenRows, readRows);
}

TEST_F(MultipleWithSameNameTest, readWithCondition)
{
    MultipleWithSameNameTable& multipleWithSameNameTable = m_database->getMultipleWithSameNameTable();

    MultipleParamsTableParameterProvider parameterProvider;
    VectorType<MultipleWithSameNameTable::Row> writtenRows;
    fillRows(writtenRows);
    multipleWithSameNameTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    MultipleWithSameNameTable::Reader reader =
            multipleWithSameNameTable.createReader(parameterProvider, condition);

    ASSERT_TRUE(reader.hasNext());
    MultipleWithSameNameTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    const size_t expectedRowNum = 1;
    checkRow(parameterProvider, writtenRows[expectedRowNum], readRow);
}

TEST_F(MultipleWithSameNameTest, update)
{
    MultipleWithSameNameTable& multipleWithSameNameTable = m_database->getMultipleWithSameNameTable();

    MultipleParamsTableParameterProvider parameterProvider;
    VectorType<MultipleWithSameNameTable::Row> writtenRows;
    fillRows(writtenRows);
    multipleWithSameNameTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    MultipleWithSameNameTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "id=" + zserio::toString<AllocatorType>(updateRowId);
    multipleWithSameNameTable.update(parameterProvider, updateRow, updateCondition);

    MultipleWithSameNameTable::Reader reader =
            multipleWithSameNameTable.createReader(parameterProvider, updateCondition);
    ASSERT_TRUE(reader.hasNext());
    MultipleWithSameNameTable::Row readRow;
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());
    checkRow(parameterProvider, updateRow, readRow);
}

} // namespace multiple_with_same_name
} // namespace explicit_parameters
