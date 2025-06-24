#include "gtest/gtest.h"
#include "templates/struct_full_name_template_argument/StructFullNameTemplateArgument.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace struct_full_name_template_argument
{

using AllocatorType = StructFullNameTemplateArgument::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(StructFullNameTemplateArgumentTest, writeRead)
{
    StructFullNameTemplateArgument data;
    data.structExternal = TemplatedStruct<import_storage::Storage>{import_storage::Storage{42}};
    data.structInternal = TemplatedStruct<Storage>{Storage{StringType{"string"}}};

    test_utils::writeReadTest(data);
}

} // namespace struct_full_name_template_argument
} // namespace templates
