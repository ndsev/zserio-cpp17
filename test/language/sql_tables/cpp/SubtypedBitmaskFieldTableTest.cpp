#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace subtyped_bitmask_field_table
{

using allocator_type = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class SubtypedBitmaskFieldTableTest : public ::testing::Test
{
public:
    SubtypedBitmaskFieldTableTest()
    {
        const StringType dbFileName("language/sql_tables/subtyped_bitmask_field_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~SubtypedBitmaskFieldTableTest() override
    {
        delete m_database;
    }

    SubtypedBitmaskFieldTableTest(const SubtypedBitmaskFieldTableTest&) = delete;
    SubtypedBitmaskFieldTableTest& operator=(const SubtypedBitmaskFieldTableTest&) = delete;

    SubtypedBitmaskFieldTableTest(SubtypedBitmaskFieldTableTest&&) = delete;
    SubtypedBitmaskFieldTableTest& operator=(SubtypedBitmaskFieldTableTest&&) = delete;

protected:
    static void fillRow(SubtypedBitmaskFieldTable::Row& row, size_t i)
    {
        row.id = static_cast<int32_t>(i);
        row.bitmaskField = (i % 3 == 0 ? TestBitmask::Values::ONE
                        : i % 3 == 1   ? TestBitmask::Values::TWO
                                       : TestBitmask::Values::THREE);
    }

    static void fillRows(VectorType<SubtypedBitmaskFieldTable::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            SubtypedBitmaskFieldTable::Row row;
            fillRow(row, i);
            rows.push_back(row);
        }
    }

    static void checkRow(const SubtypedBitmaskFieldTable::Row& row1, const SubtypedBitmaskFieldTable::Row& row2)
    {
        ASSERT_EQ(row1.id, row2.id);
        ASSERT_EQ(row1.bitmaskField, row2.bitmaskField);
    }

    static void checkRows(const VectorType<SubtypedBitmaskFieldTable::Row>& rows1,
            const VectorType<SubtypedBitmaskFieldTable::Row>& rows2)
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

TEST_F(SubtypedBitmaskFieldTableTest, readWithoutCondition)
{
    SubtypedBitmaskFieldTable& table = m_database->getSubtypedBitmaskFieldTable();

    VectorType<SubtypedBitmaskFieldTable::Row> rows;
    fillRows(rows);
    table.write(rows);

    VectorType<SubtypedBitmaskFieldTable::Row> readRows;
    auto reader = table.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(rows, readRows);
}

TEST_F(SubtypedBitmaskFieldTableTest, columnNames)
{
    ASSERT_EQ(2, SubtypedBitmaskFieldTable::columnNames.size());
    ASSERT_EQ("id", SubtypedBitmaskFieldTable::columnNames[0]);
    ASSERT_EQ("bitmaskField", SubtypedBitmaskFieldTable::columnNames[1]);
}

} // namespace subtyped_bitmask_field_table
} // namespace sql_tables
