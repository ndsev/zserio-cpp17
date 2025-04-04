#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_tables/TestDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"

namespace sql_tables
{
namespace complex_table
{

using allocator_type = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class ComplexTableTest : public ::testing::Test
{
public:
    ComplexTableTest()
    {
        const StringType dbFileName("language/sql_tables/complex_table_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_tables::TestDb(dbFileName);
        m_database->createSchema();
    }

    ~ComplexTableTest() override
    {
        delete m_database;
    }

    ComplexTableTest(const ComplexTableTest&) = delete;
    ComplexTableTest& operator=(const ComplexTableTest&) = delete;

    ComplexTableTest(ComplexTableTest&&) = delete;
    ComplexTableTest& operator=(ComplexTableTest&&) = delete;

protected:
    static void fillRowWithNullValues(ComplexTable::Row& row, uint64_t blobId)
    {
        row.blobId = blobId;
        row.age = std::numeric_limits<int64_t>::max();
        row.name.reset();
        row.isValid.reset();
        row.salary.reset();
        row.bonus.reset();
        row.value.reset();
        row.color.reset();
        row.blob.reset();
    }

    static void fillRowsWithNullValues(VectorType<ComplexTable::Row>& rows)
    {
        rows.clear();
        for (uint64_t blobId = 0; blobId < NUM_COMPLEX_TABLE_ROWS; ++blobId)
        {
            ComplexTable::Row row;
            fillRowWithNullValues(row, blobId);
            rows.push_back(row);
        }
    }

    static void fillRow(ComplexTable::Row& row, uint64_t blobId, const StringType& name)
    {
        row.blobId = blobId;
        row.age = std::numeric_limits<int64_t>::max();
        row.name = name;
        row.isValid = true;
        row.salary = 9.9F;
        row.bonus = 5.5;
        row.value = 0x34;
        row.color = TestEnum::RED;

        TestBlob testBlob;
        for (size_t i = 0; i < COMPLEX_TABLE_COUNT; ++i)
        {
            testBlob.values.push_back(static_cast<zserio::UInt3::ValueType>(blobId));
        }
        testBlob.offsetEnd = TEST_BLOB_OFFSET_END;
        testBlob.end = true;
        row.blob = testBlob;
    }

    static void fillRows(VectorType<ComplexTable::Row>& rows)
    {
        rows.clear();
        for (uint64_t blobId = 0; blobId < NUM_COMPLEX_TABLE_ROWS; ++blobId)
        {
            const StringType name = "Name" + zserio::toString<allocator_type>(blobId);
            ComplexTable::Row row;
            fillRow(row, blobId, name);
            rows.push_back(row);
        }
    }

    static void checkRow(const ComplexTable::Row& row1, const ComplexTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);

        ASSERT_EQ(row1.age, row2.age);
        ASSERT_EQ(row1.name, row2.name);
        ASSERT_EQ(row1.isValid, row2.isValid);
        ASSERT_EQ(row1.salary, row2.salary);
        ASSERT_EQ(row1.bonus, row2.bonus);
        ASSERT_EQ(row1.value, row2.value);
        ASSERT_EQ(row1.color, row2.color);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkRows(
            const VectorType<ComplexTable::Row>& rows1, const VectorType<ComplexTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(rows1[i], rows2[i]);
        }
    }

    static void checkRowWithNullValues(const ComplexTable::Row& row1, const ComplexTable::Row& row2)
    {
        ASSERT_EQ(row1.blobId, row2.blobId);
        ASSERT_EQ(row1.age, row2.age);
        ASSERT_EQ(row1.name, row2.name);
        ASSERT_EQ(row1.isValid, row2.isValid);
        ASSERT_EQ(row1.salary, row2.salary);
        ASSERT_EQ(row1.bonus, row2.bonus);
        ASSERT_EQ(row1.value, row2.value);
        ASSERT_EQ(row1.color, row2.color);
        ASSERT_EQ(row1.blob, row2.blob);
    }

    static void checkRowsWithNullValues(
            const VectorType<ComplexTable::Row>& rows1, const VectorType<ComplexTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRowWithNullValues(rows1[i], rows2[i]);
        }
    }

    class ComplexTableParameterProvider : public ComplexTable::IParameterProvider
    {
        zserio::UInt32 count(const ::zserio::View<ComplexTable::Row>&) override
        {
            return static_cast<zserio::UInt32::ValueType>(COMPLEX_TABLE_COUNT);
        }
    };

    static const size_t NUM_COMPLEX_TABLE_ROWS = 5;
    static const size_t COMPLEX_TABLE_COUNT = 10;
    static const uint32_t TEST_BLOB_OFFSET_END = 4 + (COMPLEX_TABLE_COUNT * 3 + 7) / 8;

    sql_tables::TestDb* m_database;
};

TEST_F(ComplexTableTest, deleteTable)
{
    const StringType checkTableName("complexTable");
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));

    ComplexTable& testTable = m_database->getComplexTable();
    testTable.deleteTable();
    ASSERT_FALSE(test_utils::isTableInDb(*m_database, checkTableName));

    testTable.createTable();
    ASSERT_TRUE(test_utils::isTableInDb(*m_database, checkTableName));
}

TEST_F(ComplexTableTest, readWithoutCondition)
{
    ComplexTable& testTable = m_database->getComplexTable();
    ComplexTableParameterProvider parameterProvider;

    VectorType<ComplexTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(parameterProvider, writtenRows);

    VectorType<ComplexTable::Row> readRows;
    auto reader = testTable.createReader(parameterProvider);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRows(writtenRows, readRows);
}

TEST_F(ComplexTableTest, readWithoutConditionWithNullValues)
{
    ComplexTable& testTable = m_database->getComplexTable();
    ComplexTableParameterProvider parameterProvider;

    VectorType<ComplexTable::Row> writtenRows;
    fillRowsWithNullValues(writtenRows);
    testTable.write(parameterProvider, writtenRows);

    VectorType<ComplexTable::Row> readRows;
    auto reader = testTable.createReader(parameterProvider);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    checkRowsWithNullValues(writtenRows, readRows);
}

TEST_F(ComplexTableTest, readWithCondition)
{
    ComplexTable& testTable = m_database->getComplexTable();
    ComplexTableParameterProvider parameterProvider;

    VectorType<ComplexTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(parameterProvider, writtenRows);

    const StringType condition = "name='Name1'";
    VectorType<ComplexTable::Row> readRows;
    auto reader = testTable.createReader(parameterProvider, condition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    ASSERT_EQ(1, readRows.size());

    const size_t expectedRowNum = 1;
    checkRow(writtenRows[expectedRowNum], readRows[0]);
}

TEST_F(ComplexTableTest, update)
{
    ComplexTable& testTable = m_database->getComplexTable();
    ComplexTableParameterProvider parameterProvider;

    VectorType<ComplexTable::Row> writtenRows;
    fillRows(writtenRows);
    testTable.write(parameterProvider, writtenRows);

    const uint64_t updateRowId = 3;
    ComplexTable::Row updateRow;
    fillRow(updateRow, updateRowId, "UpdatedName");
    const StringType updateCondition = "blobId=" + zserio::toString<allocator_type>(updateRowId);
    testTable.update(parameterProvider, updateRow, updateCondition);

    VectorType<ComplexTable::Row> readRows;
    auto reader = testTable.createReader(parameterProvider, updateCondition);
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    ASSERT_EQ(1, readRows.size());

    checkRow(updateRow, readRows[0]);
}

TEST_F(ComplexTableTest, columnNames)
{
    ASSERT_EQ(9, ComplexTable::columnNames.size());

    ASSERT_EQ("blobId", ComplexTable::columnNames[0]);
    ASSERT_EQ("age", ComplexTable::columnNames[1]);
    ASSERT_EQ("name", ComplexTable::columnNames[2]);
    ASSERT_EQ("isValid", ComplexTable::columnNames[3]);
    ASSERT_EQ("salary", ComplexTable::columnNames[4]);
    ASSERT_EQ("bonus", ComplexTable::columnNames[5]);
    ASSERT_EQ("value", ComplexTable::columnNames[6]);
    ASSERT_EQ("color", ComplexTable::columnNames[7]);
    ASSERT_EQ("blob", ComplexTable::columnNames[8]);
}

} // namespace complex_table
} // namespace sql_tables
