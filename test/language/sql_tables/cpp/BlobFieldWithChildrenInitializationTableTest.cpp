#include <cstdio>
#include <fstream>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace blob_field_with_children_initialization_table
{

using allocator_type = TestDb::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class BlobFieldWithChildrenInitializationTableTest : public ::testing::Test
{
public:
    BlobFieldWithChildrenInitializationTableTest()
    {
        std::remove(DB_FILE_NAME);

        m_database = new sql_tables::TestDb(DB_FILE_NAME);
        m_database->createSchema();
    }

    ~BlobFieldWithChildrenInitializationTableTest() override
    {
        delete m_database;
    }

    BlobFieldWithChildrenInitializationTableTest(const BlobFieldWithChildrenInitializationTableTest&) = delete;
    BlobFieldWithChildrenInitializationTableTest& operator=(
            const BlobFieldWithChildrenInitializationTableTest&) = delete;

    BlobFieldWithChildrenInitializationTableTest(BlobFieldWithChildrenInitializationTableTest&&) = delete;
    BlobFieldWithChildrenInitializationTableTest& operator=(
            BlobFieldWithChildrenInitializationTableTest&&) = delete;

protected:
    static void fillRow(BlobFieldWithChildrenInitializationTable::Row& row, size_t index)
    {
        row.id = static_cast<uint32_t>(index);
        const uint32_t arrayLength = static_cast<uint32_t>(index);
        VectorType<zserio::UInt32> array;
        for (uint32_t i = 0; i < arrayLength; ++i)
        {
            array.push_back(i);
        }
        row.blob = BlobWithChildrenInitialization{arrayLength, ParameterizedArray{std::move(array)}};
    }

    static void fillRows(VectorType<BlobFieldWithChildrenInitializationTable::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            BlobFieldWithChildrenInitializationTable::Row row;
            fillRow(row, i);
            rows.push_back(row);
        }
    }

    static void checkRow(const BlobFieldWithChildrenInitializationTable::Row& row1,
            const BlobFieldWithChildrenInitializationTable::Row& row2)
    {
        ASSERT_EQ(row1.id, row2.id);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkRows(const VectorType<BlobFieldWithChildrenInitializationTable::Row>& rows1,
            const VectorType<BlobFieldWithChildrenInitializationTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static constexpr const char* DB_FILE_NAME =
            "language/sql_tables/blob_field_with_children_initialization_table_test.sqlite";
    static constexpr size_t NUM_ROWS = 5;

    sql_tables::TestDb* m_database;
};

TEST_F(BlobFieldWithChildrenInitializationTableTest, readWithoutCondition)
{
    BlobFieldWithChildrenInitializationTable& table = m_database->getBlobFieldWithChildrenInitializationTable();

    VectorType<BlobFieldWithChildrenInitializationTable::Row> rows;
    fillRows(rows);
    table.write(rows);

    VectorType<BlobFieldWithChildrenInitializationTable::Row> readRows;
    auto reader = table.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(rows, readRows);
}

TEST_F(BlobFieldWithChildrenInitializationTableTest, columnNames)
{
    ASSERT_EQ(2, BlobFieldWithChildrenInitializationTable::columnNames.size());
    ASSERT_EQ("id", BlobFieldWithChildrenInitializationTable::columnNames[0]);
    ASSERT_EQ("blob", BlobFieldWithChildrenInitializationTable::columnNames[1]);
}

} // namespace blob_field_with_children_initialization_table
} // namespace sql_tables
