#include "expressions/cast_uint64_to_uint8/CastUInt64ToUInt8Expression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace cast_uint64_to_uint8
{

TEST(CastUInt64ToUInt8Test, uint8ValueUsingUInt64Value)
{
    CastUInt64ToUInt8Expression data;
    uint64_t uint64Value = 0xFFFFFFFFFFFFFFFEULL;
    data.uint64Value = uint64Value;
    data.useConstant = false;

    zserio::View view(data);

    const uint8_t expectedUInt8Value = static_cast<uint8_t>(uint64Value);
    ASSERT_EQ(expectedUInt8Value, view.uint8Value());
}

TEST(CastUInt64ToUInt8Test, uint8ValueUsingConstant)
{
    CastUInt64ToUInt8Expression data;
    const uint64_t uint64Value = 0xFFFFFFFFFFFFFFFEULL;
    data.uint64Value = uint64Value;
    data.useConstant = true;

    zserio::View view(data);

    const uint8_t expectedUInt8Value = 1;
    ASSERT_EQ(expectedUInt8Value, view.uint8Value());
}

} // namespace cast_uint64_to_uint8
} // namespace expressions
