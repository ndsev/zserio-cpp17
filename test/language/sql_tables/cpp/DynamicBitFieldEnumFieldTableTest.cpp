#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace dynamic_bit_field_enum_field_table
{

using allocator_type = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class DynamicBitFieldEnumFieldTableTest : public ::testing::Test
{
public:
    DynamicBitFieldEnumFieldTableTest()
    {
        const StringType dbFileName("language/sql_tables/dynamic_bit_field_enum_field_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~DynamicBitFieldEnumFieldTableTest() override
    {
        delete m_database;
    }

    DynamicBitFieldEnumFieldTableTest(const DynamicBitFieldEnumFieldTableTest&) = delete;
    DynamicBitFieldEnumFieldTableTest& operator=(const DynamicBitFieldEnumFieldTableTest&) = delete;

    DynamicBitFieldEnumFieldTableTest(DynamicBitFieldEnumFieldTableTest&&) = delete;
    DynamicBitFieldEnumFieldTableTest& operator=(DynamicBitFieldEnumFieldTableTest&&) = delete;

protected:
    static void fillRow(DynamicBitFieldEnumFieldTable::Row& row, size_t i)
    {
        row.id = static_cast<uint32_t>(i);
        row.enumField = (i % 3 == 0 ? TestEnum::ONE : i % 3 == 1 ? TestEnum::TWO : TestEnum::THREE);
    }

    static void fillRows(VectorType<DynamicBitFieldEnumFieldTable::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            DynamicBitFieldEnumFieldTable::Row row;
            fillRow(row, i);
            rows.push_back(row);
        }
    }

    static void checkRow(
            const DynamicBitFieldEnumFieldTable::Row& row1, const DynamicBitFieldEnumFieldTable::Row& row2)
    {
        ASSERT_EQ(row1.id, row2.id);
        ASSERT_EQ(row1.enumField, row2.enumField);
    }

    static void checkRows(const VectorType<DynamicBitFieldEnumFieldTable::Row>& rows1,
            const VectorType<DynamicBitFieldEnumFieldTable::Row>& rows2)
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

TEST_F(DynamicBitFieldEnumFieldTableTest, readWithoutCondition)
{
    DynamicBitFieldEnumFieldTable& table = m_database->getDynamicBitFieldEnumFieldTable();

    VectorType<DynamicBitFieldEnumFieldTable::Row> rows;
    fillRows(rows);
    table.write(rows);

    VectorType<DynamicBitFieldEnumFieldTable::Row> readRows;
    auto reader = table.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(rows, readRows);
}

TEST_F(DynamicBitFieldEnumFieldTableTest, columnNames)
{
    ASSERT_EQ(2, DynamicBitFieldEnumFieldTable::columnNames.size());
    ASSERT_EQ("id", DynamicBitFieldEnumFieldTable::columnNames[0]);
    ASSERT_EQ("enumField", DynamicBitFieldEnumFieldTable::columnNames[1]);
}

} // namespace dynamic_bit_field_enum_field_table
} // namespace sql_tables
