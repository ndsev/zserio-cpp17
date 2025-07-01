#include "gtest/gtest.h"
#include "templates/instantiate_sql_table/U32Table.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SqliteConnection.h"

namespace templates
{
namespace instantiate_sql_table
{

using AllocatorType = U32Table::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

static const char* const SQLITE3_MEM_DB = ":memory:";

TEST(InstantiateSqlTableTest, instantiationOfU32Table)
{
    sqlite3* internalConnection = nullptr;
    int result = sqlite3_open(SQLITE3_MEM_DB, &internalConnection);
    ASSERT_EQ(SQLITE_OK, result);
    zserio::SqliteConnection connection(internalConnection, ::zserio::SqliteConnection::INTERNAL_CONNECTION);

    U32Table u32Table(connection, "u32Table");
    u32Table.createTable();

    U32Table::Row row;
    row.id = 13;
    row.info = "info";
    VectorType<U32Table::Row> rows{row};
    u32Table.write(rows);

    auto reader = u32Table.createReader();
    ASSERT_TRUE(reader.hasNext());
    U32Table::Row readRow;
    auto readRowView = reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    ASSERT_EQ(13, *readRowView.id());
    ASSERT_EQ(std::string("info"), *readRowView.info());
}

} // namespace instantiate_sql_table
} // namespace templates
