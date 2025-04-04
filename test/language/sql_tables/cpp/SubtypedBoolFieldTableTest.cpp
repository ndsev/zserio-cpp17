#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace subtyped_bool_field_table
{

using allocator_type = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class SubtypedBoolFieldTableTest : public ::testing::Test
{
public:
    SubtypedBoolFieldTableTest()
    {
        const StringType dbFileName("language/sql_tables/subtyped_bool_field_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~SubtypedBoolFieldTableTest() override
    {
        delete m_database;
    }

    SubtypedBoolFieldTableTest(const SubtypedBoolFieldTableTest&) = delete;
    SubtypedBoolFieldTableTest& operator=(const SubtypedBoolFieldTableTest&) = delete;

    SubtypedBoolFieldTableTest(SubtypedBoolFieldTableTest&&) = delete;
    SubtypedBoolFieldTableTest& operator=(SubtypedBoolFieldTableTest&&) = delete;

protected:
    static void fillRow(SubtypedBoolFieldTable::Row& row, size_t i)
    {
        row.id = static_cast<int32_t>(i);
        row.boolField = (i % 2 == 0);
    }

    static void fillRows(VectorType<SubtypedBoolFieldTable::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            SubtypedBoolFieldTable::Row row;
            fillRow(row, i);
            rows.push_back(row);
        }
    }

    static void checkRow(const SubtypedBoolFieldTable::Row& row1, const SubtypedBoolFieldTable::Row& row2)
    {
        ASSERT_EQ(row1.id, row2.id);
        ASSERT_EQ(row1.boolField, row2.boolField);
    }

    static void checkRows(const VectorType<SubtypedBoolFieldTable::Row>& rows1,
            const VectorType<SubtypedBoolFieldTable::Row>& rows2)
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

TEST_F(SubtypedBoolFieldTableTest, readWithoutCondition)
{
    SubtypedBoolFieldTable& table = m_database->getSubtypedBoolFieldTable();

    VectorType<SubtypedBoolFieldTable::Row> rows;
    fillRows(rows);
    table.write(rows);

    VectorType<SubtypedBoolFieldTable::Row> readRows;
    auto reader = table.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(rows, readRows);
}

TEST_F(SubtypedBoolFieldTableTest, columnNames)
{
    ASSERT_EQ(2, SubtypedBoolFieldTable::columnNames.size());
    ASSERT_EQ("id", SubtypedBoolFieldTable::columnNames[0]);
    ASSERT_EQ("boolField", SubtypedBoolFieldTable::columnNames[1]);
}

} // namespace subtyped_bool_field_table
} // namespace sql_tables
