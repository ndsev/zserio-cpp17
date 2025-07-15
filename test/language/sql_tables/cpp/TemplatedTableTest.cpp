#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "sql_tables/templated_sql_table/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace templated_sql_table
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class TemplatedTableTest : public ::testing::Test
{
public:
    TemplatedTableTest()
    {
        const StringType dbFileName("language/sql_tables/templated_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new TestDb(dbFileName);
        m_database->createSchema();
    }

    ~TemplatedTableTest() override
    {
        delete m_database;
    }

    TemplatedTableTest(const TemplatedTableTest&) = delete;
    TemplatedTableTest& operator=(const TemplatedTableTest&) = delete;

    TemplatedTableTest(TemplatedTableTest&&) = delete;
    TemplatedTableTest& operator=(TemplatedTableTest&&) = delete;

protected:
    template <typename T>
    static void fillRows(VectorType<typename TestTable<T>::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            typename TestTable<T>::Row row;
            row.id = static_cast<uint32_t>(i);
            if (i & 1)
                row.column = T();
            rows.push_back(std::move(row));
        }
    }
    template <typename T>
    static void checkRows(const VectorType<typename TestTable<T>::Row>& rows1,
            const VectorType<typename TestTable<T>::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            const auto& row1 = rows1[i];
            const auto& row2 = rows2[i];
            ASSERT_EQ(row1.id, row2.id);
            ASSERT_EQ(row1.column, row2.column);
        }
    }
    template <typename T>
    void readWriteTable(TestTable<T>& table)
    {
        VectorType<typename TestTable<T>::Row> rows;
        fillRows<T>(rows);
        table.write(rows);

        VectorType<typename TestTable<T>::Row> readRows;
        auto reader = table.createReader();
        while (reader.hasNext())
        {
            reader.next(readRows.emplace_back());
        }
        checkRows<T>(rows, readRows);
    }

    static constexpr size_t NUM_ROWS = 5;

    TestDb* m_database;
};

TEST_F(TemplatedTableTest, testTables)
{
    readWriteTable(m_database->getBitmaskTable());
    readWriteTable(m_database->getBlobTable());
    readWriteTable(m_database->getEnumTable());
    readWriteTable(m_database->getFloat16Table());
    readWriteTable(m_database->getInt8Table());
    readWriteTable(m_database->getStringTable());
    readWriteTable(m_database->getUint32Table());
    readWriteTable(m_database->getVarint16Table());
    readWriteTable(m_database->getVaruintTable());
}

} // namespace templated_sql_table
} // namespace sql_tables
