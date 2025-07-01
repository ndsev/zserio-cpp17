#include <cstdio>

#include "gtest/gtest.h"
#include "templates/instantiate_type_as_sql_database_field/InstantiateTypeAsSqlDatabaseFieldDb.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace instantiate_type_as_sql_database_field
{

using AllocatorType = InstantiateTypeAsSqlDatabaseFieldDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class InstantiateTypeAsSqlDatabaseFieldTest : public ::testing::Test
{
public:
    InstantiateTypeAsSqlDatabaseFieldTest()
    {
        std::remove(DB_FILE_NAME);
    }

    static constexpr const char* DB_FILE_NAME =
            "language/templates/instantiate_type_as_sql_database_field_test.sqlite";

protected:
    template <typename T>
    void assertEqualRows(const VectorType<T>& rows1, const VectorType<T>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            assertEqualRow(rows1.at(i), rows2.at(i));
        }
    }

    template <typename T>
    void assertEqualRow(const T& row1, const T& row2)
    {
        zserio::View rowView1(row1);
        zserio::View rowView2(row2);
        ASSERT_TRUE(rowView1.id() == rowView2.id());
        ASSERT_TRUE(rowView1.data() == rowView2.data());
    }
};

TEST_F(InstantiateTypeAsSqlDatabaseFieldTest, readWrite)
{
    InstantiateTypeAsSqlDatabaseFieldDb instantiateTypeAsSqlDatabaseFieldDb(DB_FILE_NAME);
    instantiateTypeAsSqlDatabaseFieldDb.createSchema();

    StringTable& stringTable = instantiateTypeAsSqlDatabaseFieldDb.getStringTable();
    VectorType<StringTable::Row> stringTableRows;
    StringTable::Row stringRow1;
    stringRow1.id = 0;
    stringRow1.data = "test";
    stringTableRows.push_back(stringRow1);
    stringTable.write(stringTableRows);

    StringTable& otherStringTable = instantiateTypeAsSqlDatabaseFieldDb.getOtherStringTable();
    VectorType<StringTable::Row> otherStringTableRows;
    StringTable::Row otherStringRow1;
    otherStringRow1.id = 0;
    otherStringRow1.data = "other test";
    otherStringTableRows.push_back(otherStringRow1);
    otherStringTable.write(otherStringTableRows);

    InstantiateTypeAsSqlDatabaseFieldDb readInstantiateTypeAsSqlDatabaseFieldDb(DB_FILE_NAME);
    VectorType<StringTable::Row> readStringTableRows;
    auto readerString = readInstantiateTypeAsSqlDatabaseFieldDb.getStringTable().createReader();
    while (readerString.hasNext())
    {
        readerString.next(readStringTableRows.emplace_back());
    }
    VectorType<StringTable::Row> readOtherStringTableRows;
    auto readerOtherString = readInstantiateTypeAsSqlDatabaseFieldDb.getOtherStringTable().createReader();
    while (readerOtherString.hasNext())
    {
        readerOtherString.next(readOtherStringTableRows.emplace_back());
    }

    assertEqualRows(stringTableRows, readStringTableRows);
    assertEqualRows(otherStringTableRows, readOtherStringTableRows);
}

} // namespace instantiate_type_as_sql_database_field
} // namespace templates
