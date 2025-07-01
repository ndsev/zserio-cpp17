#include "gtest/gtest.h"
#include "templates/instantiate_type_as_sql_table_field/Test32Table.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SqliteConnection.h"

namespace templates
{
namespace instantiate_type_as_sql_table_field
{

using AllocatorType = Test32Table::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

static const char* const SQLITE3_MEM_DB = ":memory:";

TEST(InstantiateSqlTableTest, instantiationOfTest32Table)
{
    sqlite3* internalConnection = nullptr;
    int result = sqlite3_open(SQLITE3_MEM_DB, &internalConnection);
    ASSERT_EQ(SQLITE_OK, result);
    zserio::SqliteConnection connection(internalConnection, ::zserio::SqliteConnection::INTERNAL_CONNECTION);

    Test32Table test32Table(connection, "test32Table");
    test32Table.createTable();

    Test32Table::Row row;
    row.id = 13;
    row.test = Test32{42};
    VectorType<Test32Table::Row> rows{row};
    test32Table.write(rows);

    auto reader = test32Table.createReader();
    ASSERT_TRUE(reader.hasNext());
    Test32Table::Row readRow;
    auto readRowView = reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    ASSERT_EQ(13, *readRowView.id());
    ASSERT_EQ(42, readRowView.test()->value());
}

} // namespace instantiate_type_as_sql_table_field
} // namespace templates
