#include <cstdio>
#include <set>
#include <string_view>

#include "gtest/gtest.h"
#include "sql_without_rowid_tables/simple_without_rowid_table/SimpleWithoutRowIdDb.h"
#include "zserio/ValidationSqliteUtil.h"

namespace sql_without_rowid_tables
{
namespace simple_without_rowid_table
{

using allocator_type = SimpleWithoutRowIdDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T, typename COMPARE = std::less<T>>
using SetType = std::set<T, COMPARE, zserio::RebindAlloc<allocator_type, T>>;

class SimpleWithoutRowIdTableTest : public ::testing::Test
{
public:
    SimpleWithoutRowIdTableTest() :
            m_dbFileName("language/sql_without_rowid_tables/simple_without_rowid_table_test.sqlite"),
            m_tableName("simpleWithoutRowIdTable")
    {
        std::remove(m_dbFileName.c_str());
    }

protected:
    bool isColumnInTable(
            zserio::ISqliteDatabase& database, std::string_view columnName, std::string_view tableName)
    {
        return zserio::ValidationSqliteUtil<allocator_type>::isColumnInTable(
                database.connection(), "", tableName, columnName, allocator_type());
    }

    static constexpr std::string_view ROW_ID_COLUMN_NAME = "rowid";

    const StringType m_dbFileName;
    const StringType m_tableName;
};

TEST_F(SimpleWithoutRowIdTableTest, checkRowIdColumn)
{
    SimpleWithoutRowIdDb database(m_dbFileName);
    database.createSchema();
    ASSERT_FALSE(isColumnInTable(database, ROW_ID_COLUMN_NAME, m_tableName));
}

TEST_F(SimpleWithoutRowIdTableTest, createOrdinaryRowIdTable)
{
    SimpleWithoutRowIdDb database(m_dbFileName);
    SimpleWithoutRowIdTable& testTable = database.getSimpleWithoutRowIdTable();
    testTable.createOrdinaryRowIdTable();
    ASSERT_TRUE(isColumnInTable(database, ROW_ID_COLUMN_NAME, m_tableName));
}

TEST_F(SimpleWithoutRowIdTableTest, checkWithoutRowIdTableNamesBlackList)
{
    SimpleWithoutRowIdDb database(m_dbFileName);
    SetType<StringType> withoutRowIdTableNamesBlackList;
    withoutRowIdTableNamesBlackList.insert(m_tableName);
    database.createSchema(withoutRowIdTableNamesBlackList);
    ASSERT_TRUE(isColumnInTable(database, ROW_ID_COLUMN_NAME, m_tableName));
}

} // namespace simple_without_rowid_table

} // namespace sql_without_rowid_tables
