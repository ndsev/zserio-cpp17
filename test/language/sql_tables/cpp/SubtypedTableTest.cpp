#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace subtyped_table
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class SubtypedTableTest : public ::testing::Test
{
public:
    SubtypedTableTest()
    {
        const StringType dbFileName("language/sql_tables/subtyped_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~SubtypedTableTest() override
    {
        delete m_database;
    }

    SubtypedTableTest(const SubtypedTableTest&) = delete;
    SubtypedTableTest& operator=(const SubtypedTableTest&) = delete;

    SubtypedTableTest(SubtypedTableTest&&) = delete;
    SubtypedTableTest& operator=(SubtypedTableTest&&) = delete;

protected:
    sql_tables::TestDb* m_database;
};

TEST_F(SubtypedTableTest, testSubtypedTable)
{
    const StringType checkTableName("subtypedTable");
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    TestTable& studentsAsTestTable = m_database->getSubtypedTable();
    SubtypedTable& studentsAsSubtypedTable = m_database->getSubtypedTable();
    ASSERT_EQ(&studentsAsTestTable, &studentsAsSubtypedTable);
}

TEST_F(SubtypedTableTest, columnNames)
{
    ASSERT_EQ(1, SubtypedTable::columnNames.size());
    ASSERT_EQ("student", SubtypedTable::columnNames[0]);
}

} // namespace subtyped_table
} // namespace sql_tables
