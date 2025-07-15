#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "sql_tables/templated_sql_table_param/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace templated_sql_table_param
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class TemplatedParamTableTest : public ::testing::Test
{
public:
    TemplatedParamTableTest()
    {
        const StringType dbFileName("language/sql_tables/templated_param_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new TestDb(dbFileName);
        m_database->createSchema();
    }

    ~TemplatedParamTableTest() override
    {
        delete m_database;
    }

    TemplatedParamTableTest(const TemplatedParamTableTest&) = delete;
    TemplatedParamTableTest& operator=(const TemplatedParamTableTest&) = delete;

    TemplatedParamTableTest(TemplatedParamTableTest&&) = delete;
    TemplatedParamTableTest& operator=(TemplatedParamTableTest&&) = delete;

protected:
    template <typename T>
    static void fillRows(VectorType<typename TestTable<T>::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            typename TestTable<T>::Row row;
            row.lenColumn = static_cast<uint32_t>(i + 1);
            row.id = static_cast<uint32_t>(i);
            row.parameterizedColumn.emplace(VectorType<StringType>(i + 1, "test"));
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
            ASSERT_EQ(row1.lenColumn, row2.lenColumn);
            ASSERT_EQ(row1.parameterizedColumn, row2.parameterizedColumn);
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

TEST_F(TemplatedParamTableTest, testTables)
{
    readWriteTable(m_database->getParameterizedTable());
}

} // namespace templated_sql_table_param
} // namespace sql_tables
