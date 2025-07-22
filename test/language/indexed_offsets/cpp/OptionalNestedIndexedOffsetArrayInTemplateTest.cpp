#include "gtest/gtest.h"
#include "indexed_offsets/optional_nested_indexed_offset_array_in_template/OptionalNestedIndexedOffsetArrayInTemplate.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace optional_nested_indexed_offset_array_in_template
{

TEST(OptionalNestedIndexedOffsetArrayInTemplateTest, writeReadTest)
{
    OptionalNestedIndexedOffsetArrayInTemplate data;
    test_utils::writeReadTest(data);
}

TEST(OptionalNestedIndexedOffsetArrayInTemplateTest, hashTest)
{
    OptionalNestedIndexedOffsetArrayInTemplate data;
    ASSERT_EQ(std::hash<OptionalNestedIndexedOffsetArrayInTemplate>()(data), static_cast<size_t>(63825));
}

} // namespace optional_nested_indexed_offset_array_in_template
} // namespace indexed_offsets
