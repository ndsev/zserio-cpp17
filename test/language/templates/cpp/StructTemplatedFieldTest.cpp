#include "gtest/gtest.h"
#include "templates/struct_templated_field/StructTemplatedField.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace struct_templated_field
{

using AllocatorType = StructTemplatedField::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(StructTemplatedFieldTest, writeRead)
{
    StructTemplatedField data;
    data.uint32Field = Field<zserio::UInt32>{42};
    data.compoundField = Field<Compound>{Compound{42}};
    data.stringField = Field<StringType>{StringType{"string"}};

    test_utils::writeReadTest(data);
}

TEST(StructTemplatedFieldTest, methods)
{
    Field<StringType> data1;
    Field<StringType> data2;
    data2.value = "different";
    ASSERT_FALSE(data1 == data2);
    ASSERT_TRUE(data1 != data2);
    ASSERT_TRUE(data1 < data2);
    ASSERT_TRUE(data1 <= data2);
    ASSERT_FALSE(data1 > data2);
    ASSERT_FALSE(data1 >= data2);
    ASSERT_EQ(std::hash<decltype(data1)>()(data1), 23);

    zserio::View<Field<StringType>> vi1(data1);
    zserio::View<Field<StringType>> vi2(data2);
    ASSERT_FALSE(vi1 == vi2);
    ASSERT_TRUE(vi1 != vi2);
    ASSERT_TRUE(vi1 < vi2);
    ASSERT_TRUE(vi1 <= vi2);
    ASSERT_FALSE(vi1 > vi2);
    ASSERT_FALSE(vi1 >= vi2);

    ASSERT_EQ(&vi1.zserioData(), &data1);
    ASSERT_EQ(std::hash<decltype(vi1)>()(vi1), 23);
}

} // namespace struct_templated_field
} // namespace templates
