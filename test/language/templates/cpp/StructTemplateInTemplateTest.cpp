#include "gtest/gtest.h"
#include "templates/struct_template_in_template/StructTemplateInTemplate.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace struct_template_in_template
{

using AllocatorType = StructTemplateInTemplate::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(StructTemplateInTemplateTest, writeRead)
{
    StructTemplateInTemplate data;
    data.uint32Field = Field<zserio::UInt32>{Compound<zserio::UInt32>{42}};
    data.stringField = Field<StringType>{Compound<StringType>{StringType{"string"}}};

    test_utils::writeReadTest(data);
}

} // namespace struct_template_in_template
} // namespace templates
