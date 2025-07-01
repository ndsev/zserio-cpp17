#include <cstdio>

#include "gtest/gtest.h"
#include "templates/sql_table_templated_field/SqlTableTemplatedFieldDb.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace sql_table_templated_field
{

using AllocatorType = SqlTableTemplatedFieldDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class SqlTableTemplatedFieldTest : public ::testing::Test
{
public:
    SqlTableTemplatedFieldTest()
    {
        std::remove(DB_FILE_NAME);
    }

    static constexpr const char* DB_FILE_NAME = "language/templates/sql_table_templated_field_test.sqlite";

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

TEST_F(SqlTableTemplatedFieldTest, readWrite)
{
    SqlTableTemplatedFieldDb sqlTableTemplatedFieldDb(DB_FILE_NAME);
    sqlTableTemplatedFieldDb.createSchema();

    TemplatedTable<zserio::UInt32>& uint32Table = sqlTableTemplatedFieldDb.getUint32Table();
    VectorType<TemplatedTable<zserio::UInt32>::Row> uint32TableRows;
    TemplatedTable<zserio::UInt32>::Row uint32Row1;
    uint32Row1.id = 0;
    uint32Row1.data = Data<zserio::UInt32>{42};
    uint32TableRows.push_back(uint32Row1);
    uint32Table.write(uint32TableRows);

    TemplatedTable<Union>& unionTable = sqlTableTemplatedFieldDb.getUnionTable();
    VectorType<TemplatedTable<Union>::Row> unionTableRows;
    TemplatedTable<Union>::Row unionRow1;
    unionRow1.id = 0;
    {
        Union unionField;
        unionField.emplace<Union::Tag::valueString>("string");
        unionRow1.data = Data<Union>{std::move(unionField)};
    }
    unionTableRows.push_back(unionRow1);
    unionTable.write(unionTableRows);

    SqlTableTemplatedFieldDb readSqlTableTemplatedFieldDb(DB_FILE_NAME);
    VectorType<TemplatedTable<zserio::UInt32>::Row> readUint32TableRows;
    auto readerUint32 = readSqlTableTemplatedFieldDb.getUint32Table().createReader();
    while (readerUint32.hasNext())
    {
        readerUint32.next(readUint32TableRows.emplace_back());
    }
    VectorType<TemplatedTable<Union>::Row> readUnionTableRows;
    auto readerUnion = readSqlTableTemplatedFieldDb.getUnionTable().createReader();
    while (readerUnion.hasNext())
    {
        readerUnion.next(readUnionTableRows.emplace_back());
    }

    assertEqualRows(uint32TableRows, readUint32TableRows);
    assertEqualRows(unionTableRows, readUnionTableRows);
}

} // namespace sql_table_templated_field
} // namespace templates
