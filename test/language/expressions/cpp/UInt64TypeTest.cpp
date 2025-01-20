#include "expressions/uint64_type/UInt64TypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace uint64_type
{

TEST(UInt64TypeTest, bitSizeOfWithOptional)
{
    UInt64TypeExpression data;
    const uint32_t uint32Value = 8;
    const uint64_t uint64ValueWithOptional = 2;
    const bool boolValue = true;
    const uint8_t additionalValue = 0x03;
    data.uint32Value = uint32Value;
    data.uint64Value = uint64ValueWithOptional;
    data.boolValue = boolValue;
    data.additionalValue = additionalValue;

    zserio::View view(data);

    const size_t uint64TypeExpressionBitSizeWithOptional = 100;
    ASSERT_EQ(uint64TypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(UInt64TypeTest, bitSizeOfWithoutOptional)
{
    UInt64TypeExpression data;
    const uint32_t uint32Value = 8;
    const uint64_t uint64ValueWithoutOptional = 1;
    const bool boolValue = true;
    data.uint32Value = uint32Value;
    data.uint64Value = uint64ValueWithoutOptional;
    data.boolValue = boolValue;

    zserio::View view(data);

    const size_t uint64TypeExpressionBitSizeWithoutOptional = 97;
    ASSERT_EQ(uint64TypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace uint64_type
} // namespace expressions
