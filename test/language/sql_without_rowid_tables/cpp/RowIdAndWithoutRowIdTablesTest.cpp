#include <cstdio>
#include <set>
#include <string_view>

#include "gtest/gtest.h"
#include "sql_without_rowid_tables/rowid_and_without_rowid_tables/RowIdAndWithoutRowIdDb.h"
#include "zserio/ValidationSqliteUtil.h"

namespace sql_without_rowid_tables
{
namespace rowid_and_without_rowid_tables
{

using allocator_type = RowIdAndWithoutRowIdDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T, typename COMPARE = std::less<T>>
using SetType = std::set<T, COMPARE, zserio::RebindAlloc<allocator_type, T>>;

class RowIdAndWithoutRowIdTablesTest : public ::testing::Test
{
public:
    RowIdAndWithoutRowIdTablesTest() :
            m_dbFileName("language/sql_without_rowid_tables/rowid_and_without_rowid_tables_test.sqlite"),
            m_withoutRowIdTableName("withoutRowIdTable")
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

    static constexpr std::string_view ORDINARY_ROW_ID_TABLE_NAME = "ordinaryRowIdTable";
    static constexpr std::string_view ROW_ID_COLUMN_NAME = "rowid";

    const StringType m_dbFileName;
    const StringType m_withoutRowIdTableName;
};

TEST_F(RowIdAndWithoutRowIdTablesTest, checkRowIdColumn)
{
    RowIdAndWithoutRowIdDb database(m_dbFileName);
    database.createSchema();
    ASSERT_FALSE(isColumnInTable(database, ROW_ID_COLUMN_NAME, m_withoutRowIdTableName));
    ASSERT_TRUE(isColumnInTable(database, ROW_ID_COLUMN_NAME, ORDINARY_ROW_ID_TABLE_NAME));
}

TEST_F(RowIdAndWithoutRowIdTablesTest, createOrdinaryRowIdTable)
{
    RowIdAndWithoutRowIdDb database(m_dbFileName);
    WithoutRowIdTable& testTable = database.getWithoutRowIdTable();
    testTable.createOrdinaryRowIdTable();
    ASSERT_TRUE(isColumnInTable(database, ROW_ID_COLUMN_NAME, m_withoutRowIdTableName));
}

TEST_F(RowIdAndWithoutRowIdTablesTest, checkWithoutRowIdTableNamesBlackList)
{
    RowIdAndWithoutRowIdDb database(m_dbFileName);
    SetType<StringType> withoutRowIdTableNamesBlackList;
    withoutRowIdTableNamesBlackList.insert(m_withoutRowIdTableName);
    database.createSchema(withoutRowIdTableNamesBlackList);
    ASSERT_TRUE(isColumnInTable(database, ROW_ID_COLUMN_NAME, m_withoutRowIdTableName));
    ASSERT_TRUE(isColumnInTable(database, ROW_ID_COLUMN_NAME, ORDINARY_ROW_ID_TABLE_NAME));
}

} // namespace rowid_and_without_rowid_tables

} // namespace sql_without_rowid_tables
