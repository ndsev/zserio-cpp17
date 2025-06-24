#include "gtest/gtest.h"
#include "templates/subtype_template_argument/SubtypeTemplateArgument.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace subtype_template_argument
{

TEST(SubtypeTemplateArgumentTest, bitSizeOf)
{
    const Field<zserio::UInt32> field_uint32(10);
    const Field<Compound> field_compound(Compound(10));
    const SubtypeTemplateArgument data(
            field_uint32, field_uint32, field_uint32, field_compound, field_compound, field_compound);
    ASSERT_EQ(192, zserio::detail::bitSizeOf(zserio::View(data)));
}

TEST(SubtypeTemplateArgumentTest, writeRead)
{
    const Field<zserio::UInt32> field_uint32(10);
    const Field<Compound> field_compound(Compound(10));
    const SubtypeTemplateArgument data(
            field_uint32, field_uint32, field_uint32, field_compound, field_compound, field_compound);

    test_utils::writeReadTest(data);
}

} // namespace subtype_template_argument
} // namespace templates
