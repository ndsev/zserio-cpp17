#include "gtest/gtest.h"
#include "offsets/offset_in_nested_union_in_template/OffsetInNestedUnionInTemplate.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace offset_in_nested_union_in_template
{

TEST(OffsetInNestedUnionInTemplateTest, writeRead)
{
    OffsetInNestedUnionInTemplate data;
    data.testUnion.emplace<TestUnion<NestedUnion<zserio::UInt32>>::Tag::nestedUnion>()
            .emplace<NestedUnion<zserio::UInt32>::Tag::offset>(100);
    test_utils::writeReadTest(data);
}

TEST(OffsetInNestedUnionInTemplateTest, hash)
{
    OffsetInNestedUnionInTemplate data;
    ASSERT_EQ(std::hash<OffsetInNestedUnionInTemplate>()(data), 34067);
}

} // namespace offset_in_nested_union_in_template
} // namespace offsets
