#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace subtyped_enum_field_table
{

using AllocatorType = TestDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class SubtypedEnumFieldTableTest : public ::testing::Test
{
public:
    SubtypedEnumFieldTableTest()
    {
        const StringType dbFileName("language/sql_tables/subtyped_enum_field_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~SubtypedEnumFieldTableTest() override
    {
        delete m_database;
    }

    SubtypedEnumFieldTableTest(const SubtypedEnumFieldTableTest&) = delete;
    SubtypedEnumFieldTableTest& operator=(const SubtypedEnumFieldTableTest&) = delete;

    SubtypedEnumFieldTableTest(SubtypedEnumFieldTableTest&&) = delete;
    SubtypedEnumFieldTableTest& operator=(SubtypedEnumFieldTableTest&&) = delete;

protected:
    static void fillRow(SubtypedEnumFieldTable::Row& row, size_t i)
    {
        row.id = static_cast<int32_t>(i);
        row.enumField = (i % 3 == 0 ? TestEnum::ONE : i % 3 == 1 ? TestEnum::TWO : TestEnum::THREE);
    }

    static void fillRows(VectorType<SubtypedEnumFieldTable::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            SubtypedEnumFieldTable::Row row;
            fillRow(row, i);
            rows.push_back(row);
        }
    }

    static void checkRow(const SubtypedEnumFieldTable::Row& row1, const SubtypedEnumFieldTable::Row& row2)
    {
        ASSERT_EQ(row1.id, row2.id);
        ASSERT_EQ(row1.enumField, row2.enumField);
    }

    static void checkRows(const VectorType<SubtypedEnumFieldTable::Row>& rows1,
            const VectorType<SubtypedEnumFieldTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr size_t NUM_ROWS = 5;

    sql_tables::TestDb* m_database;
};

TEST_F(SubtypedEnumFieldTableTest, readWithoutCondition)
{
    SubtypedEnumFieldTable& table = m_database->getSubtypedEnumFieldTable();

    VectorType<SubtypedEnumFieldTable::Row> rows;
    fillRows(rows);
    table.write(rows);

    VectorType<SubtypedEnumFieldTable::Row> readRows;
    auto reader = table.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(rows, readRows);
}

TEST_F(SubtypedEnumFieldTableTest, columnNames)
{
    ASSERT_EQ(2, SubtypedEnumFieldTable::columnNames.size());
    ASSERT_EQ("id", SubtypedEnumFieldTable::columnNames[0]);
    ASSERT_EQ("enumField", SubtypedEnumFieldTable::columnNames[1]);
}

} // namespace subtyped_enum_field_table
} // namespace sql_tables
