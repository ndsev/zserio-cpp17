#include "expressions/cast_uint8_to_uint64/CastUInt8ToUInt64Expression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace cast_uint8_to_uint64
{

TEST(CastUInt8ToUInt64Test, uint64ValueUsingUInt8Value)
{
    CastUInt8ToUInt64Expression data;
    const uint8_t uint8Value = 0xBA;
    data.uint8Value = uint8Value;
    data.useConstant = false;

    zserio::View view(data);

    const uint64_t expectedUInt64Value = static_cast<uint64_t>(uint8Value);
    ASSERT_EQ(expectedUInt64Value, view.uint64Value());
}

TEST(CastUInt8ToUInt64Test, uint64ValueUsingConstant)
{
    CastUInt8ToUInt64Expression data;
    const uint8_t uint8Value = 0xBA;
    data.uint8Value = uint8Value;
    data.useConstant = true;

    zserio::View view(data);

    const uint64_t expectedUInt64Value = static_cast<uint64_t>(1);
    ASSERT_EQ(expectedUInt64Value, view.uint64Value());
}

} // namespace cast_uint8_to_uint64
} // namespace expressions
