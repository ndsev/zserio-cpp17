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

} // namespace struct_templated_field
} // namespace templates
